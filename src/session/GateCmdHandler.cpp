#include "GateCmdHandler.h"
#include "SessionUserManager.h"
#include "SessionTaskManager.h"
#include "SessionUser.h"
#include "TimeTick.h"

// 处理来自gate的命令
bool GateCmdHandler::handleCmd(SessionTask* task, const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if (NULL == task || NULL == ptNullCmd)
	{
		return false;
	}
	
	CMD::SESSION::ID::CMDID cmdid = (CMD::SESSION::ID::CMDID)ptNullCmd->_id;
	switch (cmdid)
	{
	case CMD::SESSION::ID::t_regUser_GateSession: 
		{
			const CMD::SESSION::t_regUser_GateSession* cmd = (const CMD::SESSION::t_regUser_GateSession*)ptNullCmd;
			user_reg(task,cmd,nCmdLen);
		}
		break;
	case CMD::SESSION::ID::t_unregUser_GateSession: 
		{
			const CMD::SESSION::t_unregUser_GateSession* cmd = (const CMD::SESSION::t_unregUser_GateSession*)ptNullCmd;
			user_unreg(task,cmd,nCmdLen);
		}
		break;
	case CMD::SESSION::ID::t_enterUser_GateSession: 
		{
			const CMD::SESSION::t_enterUser_GateSession* cmd = (const CMD::SESSION::t_enterUser_GateSession*)ptNullCmd;
			user_enter(task,cmd,nCmdLen);
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

bool GateCmdHandler::user_reg(SessionTask*task, const CMD::SESSION::t_regUser_GateSession* cmd, DWORD cmd_size)
{
	if (NULL == task || NULL == cmd)
	{
		Fir::logger->info("%s,参数错误", __PRETTY_FUNCTION__);
		return false;
	}

	SessionUser* u = SessionUserManager::getMe().getUserByACCID(cmd->accid);
	do 
	{
		if (cmd->regtype == 2)
		{
			if (!u)
			{
				Fir::logger->warn("[账号重连]失败，账号已不存在，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
				break;
			}
			if (u->getGateID() != task->getID())
			{
				Fir::logger->warn("[账号重连]失败，网关不匹配，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
				break;
			}
			// 重连
			if (SessionUserManager::getMe().reconnGateUser(u, cmd))
			{
				Fir::logger->trace("[账号重连]成功，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
				return true;
			}
			else
			{
				Fir::logger->warn("[账号重连]失败，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
				break;
			}
		}
		else
		{
			if (u)
			{
				// 踢掉旧连接
				if (SessionUserManager::getMe().unregUser(u, SessionUser::UnregMethod::unreg_active, (BYTE)LOGOUT_REPLACE))
				{
					Fir::logger->trace("[踢出账号]成功，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
				}
				else
				{
					Fir::logger->info("[踢出账号]失败，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
				}
			}
			// 构造新连接
			if (SessionUserManager::getMe().regGateUser(task, cmd))
			{
				Fir::logger->trace("[账号登陆]成功，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
				return true;
			}
			else
			{
				Fir::logger->error("[账号登陆]失败，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
				break;
			}
		}
	} while (false);

	// 登陆失败
	CMD::SESSION::t_regUser_SessionGate send;
	send.seqid = cmd->seqid;
	send.accid = cmd->accid;
	send.regtype = cmd->regtype;
	send.retcode = 1;

	task->sendCmd(&send,sizeof(send));

	return true;
}

bool GateCmdHandler::user_unreg(SessionTask*task, const CMD::SESSION::t_unregUser_GateSession* cmd, DWORD cmd_size)
{
	if (NULL == task || NULL == cmd)
	{
		Fir::logger->info("%s,参数错误", __PRETTY_FUNCTION__);
		return false;
	}

	SessionUser* u = SessionUserManager::getMe().getUserByACCID(cmd->accid);
	if (!u) 
	{
		Fir::logger->warn("[账号注销]失败，账号未登陆，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
		return true;
	}
	if (u->getGateID() != task->getID() || u->getSeqID() != cmd->seqid)
	{
		Fir::logger->warn("[账号注销]失败，网关序号不匹配，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
		return true;
	}

	if (SessionUserManager::getMe().unregUserWait(u))
	{
		Fir::logger->trace("[等待账号注销]，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
	}
	else
	{
		Fir::logger->error("[等待账号注销]失败，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);
	}

	return true;
}

bool GateCmdHandler::user_enter(SessionTask*task, const CMD::SESSION::t_enterUser_GateSession* cmd, DWORD cmd_size)
{
	if (NULL == task || NULL == cmd)
	{
		Fir::logger->info("%s,参数错误", __PRETTY_FUNCTION__);
		return false;
	}

	SessionUser* u = SessionUserManager::getMe().getUserByACCID(cmd->accid);

	if (u)
	{
		if (u->getGateID() == task->getID() && u->getSeqID() == cmd->seqid)
		{
			// 是否角色选择状态
			if (!u->isSelectState())
			{
				Fir::logger->warn("[进入游戏]失败，不在角色选择状态，网关ID = %u，网关序号 = %llu，账号ID = %u，角色ID = %u", task->getID(), cmd->seqid, cmd->accid, cmd->charid);
				return false;
			}

			//查找最小负载服务器
			SessionTask* scene = SessionTaskManager::getMe().getMinLoad();
			if (NULL == scene)
			{
				Fir::logger->error("[进入游戏]失败，查找最小负载场景失败，网关ID = %u，网关序号 = %llu，账号ID = %u，角色ID = %u", task->getID(), cmd->seqid, cmd->accid, cmd->charid);
				return false;
			}

			u->regToScene(scene, cmd->charid);
		}
		else
		{
			Fir::logger->warn("[进入游戏]失败，网关序号不匹配，网关ID = %u，网关序号 = %llu，账号ID = %u，角色ID = %u", task->getID(), cmd->seqid, cmd->accid, cmd->charid);
		}
	}
	else
	{
		Fir::logger->warn("[进入游戏]失败，账号未登陆，网关ID = %u，网关序号 = %llu，账号ID = %u，角色ID = %u", task->getID(), cmd->seqid, cmd->accid, cmd->charid);
		return false;
	}

	return true;
}
