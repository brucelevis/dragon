/**
 * \file
 * \version  $Id: LoginCmdDispatcher.cpp 64 2013-04-23 02:05:08Z  $
 * \author   ,
 * \date 2013年03月27日 12时14分48秒 CST
 * \brief 定义用户登录相关命令处理文件，注册给dispatcher
 *
 */

#include "SceneCmdHandler.h"
#include "SessionServer.h"
#include "zCmdBuffer.h"
#include "SessionUser.h"
#include "SessionUserManager.h"

// 处理来自scene的命令
bool SceneCmdHandler::handleCmd(SessionTask* task, const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if (NULL == task || NULL == ptNullCmd)
	{
		return false;
	}
	
	CMD::SESSION::ID::CMDID cmdid = (CMD::SESSION::ID::CMDID)ptNullCmd->_id;
	switch (cmdid)
	{
	case CMD::SESSION::ID::t_regUser_Return_SceneSession: 
		{
			const CMD::SESSION::t_regUser_Return_SceneSession* cmd = (const CMD::SESSION::t_regUser_Return_SceneSession*)ptNullCmd;
			user_reg_reply(task,cmd,nCmdLen);
		}
		break;
	case CMD::SESSION::ID::t_GmReply_SceneSession: 
		{
			const CMD::SESSION::t_GmReply_SceneSession* cmd = (const CMD::SESSION::t_GmReply_SceneSession*)ptNullCmd;
			ret_gm_reply(task,cmd,nCmdLen);
		}
		break;
	case CMD::SESSION::ID::t_unregUser_SceneSession:
		{
			const CMD::SESSION::t_unregUser_SceneSession* cmd = (const CMD::SESSION::t_unregUser_SceneSession*)ptNullCmd;
			unser_unreg(task,cmd,nCmdLen);
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	return true;
}

bool SceneCmdHandler::user_reg_reply(SessionTask*task, const CMD::SESSION::t_regUser_Return_SceneSession* cmd, DWORD cmd_size)
{
	SessionUser* u = SessionUserManager::getMe().getUserByACCID(cmd->accid);

	if (!u)
	{
		Fir::logger->warn("[进入游戏]失败，账号已下线，账号ID = %u，角色ID = %u", cmd->accid, cmd->charid);
		return true;
	}

	// 是否进入游戏状态
	if (!u->isEnterState())
	{
		Fir::logger->warn("[进入游戏]失败，不在进入游戏状态，账号ID = %u，角色ID = %u", cmd->accid, cmd->charid);
		return false;
	}

	if (cmd->retcode == 0)
	{
		if (SessionUserManager::getMe().regSceneUser(u, task))
		{
			Fir::logger->trace("[进入游戏]成功，账号ID = %u，角色ID = %u", cmd->accid, cmd->charid);
		}
		else
		{
			u->onRegToSceneError(cmd->charid, cmd->retcode);
			Fir::logger->error("[进入游戏]失败，注册场景玩家失败，账号ID = %u，角色ID = %u", cmd->accid, cmd->charid);

			return false;
		}
	}
	else
	{
		u->onRegToSceneError(cmd->charid, cmd->retcode);
		Fir::logger->error("[进入游戏]失败，注册到场景失败，账号ID = %u，角色ID = %u，返回码 = %d", cmd->accid, cmd->charid, (int)cmd->retcode);

		return false;
	}

	return true;
}

bool SceneCmdHandler::ret_gm_reply(SessionTask* task, const CMD::SESSION::t_GmReply_SceneSession* cmd, DWORD cmd_size)
{
	if (!task || !cmd)
		return false;

	Fir::logger->info("%s", __PRETTY_FUNCTION__);

	zCmdBufferAdapterT<CMD::SUPER::t_RetGM_Result_Session2Super> send;

	send->datasize = cmd->datasize;

	if (send.write(&cmd->data[0],send->datasize))
	{
		SessionService::getMe().sendCmdToSuperServer(send.data(),send.size());
	}

	return true;
}

bool SceneCmdHandler::unser_unreg(SessionTask* task, const CMD::SESSION::t_unregUser_SceneSession* cmd, DWORD cmd_size)
{
	if (!task || !cmd)
		return false;

	SessionUser* u = SessionUserManager::getMe().getUserByACCID(cmd->accid);
	if (!u)
	{
		Fir::logger->warn("[踢出账号]失败，账号不存在，账号ID = %u, 踢出类型 = %d", cmd->accid, cmd->type);
		return false;
	}

	if (!SessionUserManager::getMe().unregUser(u, SessionUser::UnregMethod::unreg_active, (BYTE)cmd->type))
	{
		Fir::logger->error("[踢出账号]失败，账号ID = %u, 踢出类型 = %d", cmd->accid, cmd->type);
		return false;
	}

	Fir::logger->trace("[踢出账号]，账号ID = %u, 踢出类型 = %d", cmd->accid, cmd->type);

	return true;
}