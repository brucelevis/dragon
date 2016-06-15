/**
 * \file
 * \version  $Id: SessionClient.cpp 42 2013-04-10 07:33:59Z  $
 * \author  ,@163.com
 * \date 2004年11月05日 13时58分55秒 CST
 * \brief 定义Session服务器连接客户端
 *
 */

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "SessionCommand.h"
#include "SessionClient.h"
#include "GatewayServer.h"
#include "SceneClient.h"
#include "GateUserManager.h"
#include "Fir.h"
#include "RecordClient.h"
#include "zSocket.h"
#include "TimeTick.h"
#include "GatewayTask.h"
#include "GatewayTaskManager.h"

/**
 * \brief Session服务器连接客户端
 *
 * 一个区中只有一个Session服务器，所以这里只需要保留一个指针，不需要连接管理器之类的东东
 *
 */
SessionClient *sessionClient = NULL;

/**
 * \brief 建立到Session服务器的连接
 *
 * \return 连接是否成功
 */
bool SessionClient::connectToSessionServer()
{
	if (!connect())
	{
		Fir::logger->error("连接Session服务器失败");
		return false;
	}

	using namespace CMD::SESSION;
	t_LoginSession tCmd;
	tCmd.wdServerID = GatewayService::getMe().getServerID();
	tCmd.wdServerType = GatewayService::getMe().getServerType();

	return sendCmd(&tCmd, sizeof(tCmd));
}

/**
 * \brief 重载zThread中的纯虚函数，是线程的主回调函数，用于处理接收到的指令
 *
 */
void SessionClient::run()
{
	zTCPBufferClient::run();

	//与Session之间的连接断开，需要关闭服务器
	GatewayService::getMe().Terminate();
	while(!GatewayService::getMe().isSequeueTerminate())
	{
		zThread::msleep(10);
	}
	// */
}

/**
 * \brief 解析来自Session服务器的所有指令
 *
 * \param ptNullCmd 待解析的指令
 * \param nCmdLen 待解析的指令长度
 * \return 解析是否成功
 */
bool SessionClient::msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}

bool SessionClient::cmdMsgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{

	using namespace CMD::SESSION;

#ifdef _COMMAND_TIME
	CommandTime func_time(GatewayService::getMe().timeCmdLog, "SessionClient", ptNullCmd->cmd, ptNullCmd->para);
#endif

	if (ptNullCmd->_id == CMD::SESSION::ID::t_regUser_SessionGate)
	{
		t_regUser_SessionGate *cmd=(t_regUser_SessionGate *)ptNullCmd;
		GatewayTask* gatewaytask = GatewayTaskManager::getMe().getGatewayTask(cmd->seqid);

		if (!gatewaytask)
		{
			Fir::logger->warn("[会话注册]失败, 账号已下线, 网关序号 = %llu，账号ID = %u", cmd->seqid, cmd->accid);
			return true;
		}

		if (gatewaytask->isTerminateWait() || gatewaytask->isTerminate())
		{
			Fir::logger->warn("[会话注册]失败, 已不在登录状态, 网关序号 = %llu，账号ID = %u", cmd->seqid, cmd->accid);
			return true;
		}
		gatewaytask->onRegToSession(cmd->retcode, cmd->accid, cmd->regtype);

		return true;
	}
	else if (ptNullCmd->_id == CMD::SESSION::ID::t_unregUser_SessionGate)
	{
		t_unregUser_SessionGate *cmd=(t_unregUser_SessionGate *)ptNullCmd;
		GatewayTask* gatewaytask = GatewayTaskManager::getMe().getGatewayTask(cmd->seqid);

		if (!gatewaytask)
		{
			Fir::logger->warn("[会话注销]失败, 账号已下线, 网关序号 = %llu", cmd->seqid);
			return true;
		}

		gatewaytask->onUnRegFromSession(cmd->unregtype);

		return true;
	}
	else if (ptNullCmd->_id == CMD::SESSION::ID::t_enterUser_Return_SessionGate)
	{
		t_enterUser_Return_SessionGate *cmd=(t_enterUser_Return_SessionGate *)ptNullCmd;
		GateUser *u=(GateUser *)GateUserManager::getMe().getUserAccount(cmd->accid);

		if (!u || !u->getGatewayTask())
		{
			Fir::logger->warn("[会话游戏失败]，账号已下线，账号ID = %u，角色ID = %u", cmd->accid, cmd->charid);
			return true;
		}
		if (!u || u->isTerminateWait() || u->isTerminate())
		{
			Fir::logger->warn("[会话游戏失败]，账号已下线，账号ID = %u，角色ID = %u", cmd->accid, cmd->charid);
			return true;
		}

		if (u->getGatewayTask()->getID() != cmd->seqid)
		{
			Fir::logger->warn("[会话游戏失败]，网关序号不匹配，网关序号 = %llu, 账号ID = %u，角色ID = %u", cmd->seqid, cmd->accid, cmd->charid);
			return true;
		}

		if (cmd->retcode == 0)
		{
		}
		else
		{
			u->onEnterGameFailed(cmd->charid, cmd->retcode);
		}
	}
	else if (ptNullCmd->_id == CMD::SESSION::ID::t_Session_ForwardUser)
	{
		t_Session_ForwardUser *rev=(t_Session_ForwardUser *)ptNullCmd;
		GateUser *pUser=(GateUser *)GateUserManager::getMe().getUserCharid(rev->dwID);
		if (pUser)
			pUser->sendCmd(rev->data,rev->size);
		return true;
	}
	else if (ptNullCmd->_id == CMD::SESSION::ID::t_Session_ForwardWorld)
	{
		//t_Session_ForwardWorld *rev=(t_Session_ForwardWorld *)ptNullCmd;
		//GateUserManager::getMe().sendCmdToWorld(rev->data,rev->size);
		return true;
	}

	Fir::logger->info("%s(%u, %u, %u), _id:%d", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen, ptNullCmd->_id);
	return false;
}

