/**
 * \file
 * \version  $Id: LoginCmdDispatcher.cpp 64 2013-04-23 02:05:08Z  $
 * \author   ,
 * \date 2013年03月27日 12时14分48秒 CST
 * \brief 定义用户登录相关命令处理文件，注册给dispatcher
 *
 */

#include "SessionCmdHandler.h"
#include "SceneUser.h"
#include "SceneUserManager.h"
#include "SceneTaskManager.h"
#include "GmManager.h"
#include "HMessages.h"
#include "zMemDB.h"
#include "GmManager.h"

#include "proto/monitor_protocol.pb.h"

// 处理来自session的命令
bool SessionCmdHanlder::handleCmd(SessionClient* task, const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if (NULL == task || NULL == ptNullCmd)
	{
		return false;
	}
	
	CMD::SESSION::ID::CMDID cmdid = (CMD::SESSION::ID::CMDID)ptNullCmd->_id;
	switch (cmdid)
	{
	case CMD::SESSION::ID::t_regUser_SessionScene: 
		{
			const CMD::SESSION::t_regUser_SessionScene* cmd = (const CMD::SESSION::t_regUser_SessionScene*)ptNullCmd;
			return userreg(task,cmd,nCmdLen);
		};
		break;
	case CMD::SESSION::ID::t_unregUser_SessionScene: 
		{
			const CMD::SESSION::t_unregUser_SessionScene* cmd = (const CMD::SESSION::t_unregUser_SessionScene*)ptNullCmd;
			return user_unreg(task,cmd,nCmdLen);
		};
		break;
	case CMD::SESSION::ID::t_online_offline_SessionScene: 
		{
			const CMD::SESSION::t_online_offline_SessionScene* cmd = (const CMD::SESSION::t_online_offline_SessionScene*)ptNullCmd;
			return user_online_offline(task,cmd,nCmdLen);
		};
		break;
	case CMD::SESSION::ID::t_GmRequest_SessionScene: 
		{
			const CMD::SESSION::t_GmRequest_SessionScene* cmd = (const CMD::SESSION::t_GmRequest_SessionScene*)ptNullCmd;
			return req_gm(task,cmd,nCmdLen);
		};
		break;
	default:
		{
		}
		break;
	}

	return false;
}

// 玩家登录
bool SessionCmdHanlder::userreg(SessionClient* task, const CMD::SESSION::t_regUser_SessionScene* cmd, DWORD nCmdLen)
{
	BYTE retcode = 0;

	SceneUser* u = SceneUserManager::getMe().getUserByID(cmd->charid);
	if (!u)
	{
		u = FIR_NEW SceneUser(cmd->charid);

		if (u)
		{
			if (u->reg(cmd->gateid, cmd->reg_type))
			{
				retcode = 0;
				Fir::logger->trace("[角色登录]成功，账号ID[%u]，角色ID[%u]，登录类型[%d]", cmd->accid, cmd->charid, (int)cmd->reg_type);
			}
			else
			{
				retcode = 1;
				Fir::logger->error("[角色登录]，角色注册失败，账号ID[%u]，角色ID[%u]，登录类型[%d]", cmd->accid, cmd->charid, (int)cmd->reg_type);
				SAFE_DELETE(u);
			}
		}
		else
		{
			retcode = 1;
			Fir::logger->error("[角色登录]，分配内存错误，账号ID[%u]，角色ID[%u，登录类型[%d]", cmd->accid, cmd->charid, (int)cmd->reg_type);
		}
	}
	else
	{
		retcode = 1;
		Fir::logger->error("[角色登录]，玩家已在游戏中，账号ID[%u]，角色ID[%u]，登录类型[%d]", cmd->accid, cmd->charid, (int)cmd->reg_type);
	}

	if (cmd->reg_type == 0)
	{
		ret_userreg(task, cmd, retcode);
	}

	return true;
}

// 玩家离线
bool SessionCmdHanlder::user_unreg(SessionClient* task, const CMD::SESSION::t_unregUser_SessionScene* cmd, DWORD nCmdLen)
{
	SceneUser* u = SceneUserManager::getMe().getUserByID(cmd->charid);
	if (!u) 
	{
		Fir::logger->error("[注销玩家]失败，角色不存在，角色ID[%u]", cmd->charid);
		return false;
	}

	if (u->getRegType() != cmd->unreg_type)
	{
		Fir::logger->error("[注销玩家]失败，注销类型不符，角色ID[%u]，注销类型[%d]", cmd->charid, (int)cmd->unreg_type);
		return false;
	}

	u->unreg();

	SAFE_DELETE(u);

	return true;
}

// 玩家在线\离线
bool SessionCmdHanlder::user_online_offline(SessionClient* task, const CMD::SESSION::t_online_offline_SessionScene* cmd, DWORD nCmdLen)
{
	SceneUser* u = SceneUserManager::getMe().getUserByID(cmd->charid);
	if (!u) 
	{
		Fir::logger->error("[玩家在线离线]通知失败，角色不存在，角色ID[%u]", cmd->charid);
		return false;
	}

	Fir::logger->trace("[玩家在线离线]通知，角色ID[%u]，[%s]", cmd->charid, cmd->online_offline == 1 ? "在线" : "离线");

	return true;
}

// 请求gm信息
bool SessionCmdHanlder::req_gm(SessionClient* task, const CMD::SESSION::t_GmRequest_SessionScene* cmd,const DWORD len)
{
	if (!task || !cmd)
		return false;

	PbGmRequest binary;
	binary.ParseFromArray(cmd->data,cmd->datasize);

	zProperties paras;
	for (int i=0; i<binary.pb_items_size(); i++)
	{
		const PbPropertyItem& pbitem = binary.pb_items(i);
		paras[pbitem.name()] = pbitem.value();
	}

	GmManager::getMe().handle_gm_cmd(binary.httpid(),paras);
	return true;
}

void SessionCmdHanlder::ret_userreg(SessionClient* task, const CMD::SESSION::t_regUser_SessionScene* cmd, BYTE retcode)
{
	CMD::SESSION::t_regUser_Return_SceneSession ret;
	ret.charid = cmd->charid;
	ret.accid = cmd->accid;
	ret.retcode = retcode;

	sessionClient->sendCmd(&ret, sizeof(ret));
}