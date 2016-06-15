/**
 * \file
 * \version  $Id: SceneClient.cpp 65 2013-04-23 09:34:49Z  $
 * \author  ,@163.com
 * \date 2004年11月05日 13时58分55秒 CST
 * \brief 定义场景服务器连接客户端
 *
 */

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "SceneCommand.h"
#include "RecordCommand.h"
#include "SceneClient.h"
#include "SessionCommand.h"
#include "SessionClient.h"
#include "GatewayServer.h"
#include "GateUserManager.h"
#include "Fir.h"
#include "GatewayTask.h"
#include "RecordClient.h"
#include <time.h>
#include "GatewayTask.h"
#include "RecordClient.h"
#include "SceneClientManager.h"
#include "TimeTick.h"

#if 0
SceneClientManager *SceneClientManager::instance = NULL;
#endif

/**
 * \brief 创建到场景服务器的连接
 *
 * \return 连接是否成功
 */
bool SceneClient::connectToSceneServer()
{
	//Fir::logger->debug(__PRETTY_FUNCTION__);

	if (!zTCPClientTask::connect())
	{
		Fir::logger->error("连接场景服务器失败");
		return false;
	}

	CMD::SUPER::t_restart_ServerEntry_NotifyOther notify; 
	notify.srcID=GatewayService::getMe().getServerID();
	notify.dstID=this->getServerID();
	GatewayService::getMe().sendCmdToSuperServer(&notify, sizeof(notify));
	using namespace CMD::SCENE;
	t_LoginScene tCmd;
	tCmd.wdServerID = GatewayService::getMe().getServerID();
	tCmd.wdServerType = GatewayService::getMe().getServerType();

	return sendCmd(&tCmd, sizeof(tCmd));
}

int SceneClient::checkRebound()
{
	return 1;
}
void SceneClient::addToContainer()
{
	SceneClientManager::getMe().add(this);
}

void SceneClient::removeFromContainer()
{
	SceneClientManager::getMe().remove(this);
}
bool SceneClient::connect()
{
	return connectToSceneServer();
}

/**
 * \brief 解析来自场景服务器的指令
 *
 * \param ptNullCmd 待处理的指令
 * \param nCmdLen 待处理的指令长度
 * \return 解析是否成功
 */
bool SceneClient::msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}

bool SceneClient::cmdMsgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::SCENE;

#ifdef _COMMAND_TIME
	CommandTime func_time(GatewayService::getMe().timeCmdLog, "SceneClient", ptNullCmd->cmd, ptNullCmd->para);
#endif

	if (ptNullCmd->_id == CMD::SCENE::ID::t_User_FromScene)
	{
		CMD::SCENE::t_User_FromScene* rev = (CMD::SCENE::t_User_FromScene*)ptNullCmd;
		GateUser* u = GateUserManager::getMe().getUserCharid(rev->charid);

		if (u && !u->isTerminateWait() && !u->isTerminate())
		{
			u->sendCmd(rev->data, rev->size);
		}

		return true;
	}
	else if (ptNullCmd->_id == CMD::SCENE::ID::t_Forward2World_FromScene)
	{
		CMD::SCENE::t_Forward2World_FromScene* rev = (CMD::SCENE::t_Forward2World_FromScene*)ptNullCmd;
		GateUserManager::getMe().sendCmdToWorld(rev->except_charid,rev->data,rev->size);
		return true;
	}
	else if(ptNullCmd->_id == CMD::SCENE::ID::t_Refresh_LoginScene)
	{
		CMD::SCENE::t_Refresh_LoginScene* cmd = (CMD::SCENE::t_Refresh_LoginScene*)ptNullCmd;
		GateUser *pUser=GateUserManager::getMe().getUserAccount(cmd->accid);

		if (!pUser) 
		{
			Fir::logger->warn("[进入场景]失败，账号已下线，账号ID = %u，角色ID = %u", cmd->accid, cmd->charid);
			return true;
		}

		if (pUser->isTerminateWait() || pUser->isTerminate())
		{
			Fir::logger->warn("[进入场景]失败，网关用户不在等待状态，账号ID = %u，角色ID = %u", cmd->accid, cmd->charid);
			return true;
		}

		if (!pUser->isWaitPlayState())
		{
			Fir::logger->warn("[进入场景]失败，不在等待游戏状态，账号ID = %u，角色ID = %u", cmd->accid, cmd->charid);
			return true;
		}

		pUser->onEnterGameSuccess(this, cmd->charid);

		return true;
	}
	else if (ptNullCmd->_id == CMD::SCENE::ID::t_StartOKSceneGate)
	{
		this->isStartOK = true;
#ifdef _DEBUG_WCX
		Fir::logger->debug("场景网关连接启动完成:%u",this->getServerID());
#endif
		return true;
	}
	
	Fir::logger->info("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

