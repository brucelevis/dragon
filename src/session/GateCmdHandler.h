/**
 * \file
 * \version  $Id: GateCmdHandler.h 42 2013-04-10 07:33:59Z  $
 * \author   ,
 * \date 2013年03月27日 12时14分48秒 CST
 * \brief 出来来自gate的命令
 *
 */

#ifndef _GATE_CMD_HANDLER_H_
#define _GATE_CMD_HANDLER_H_

#include "Fir.h"
#include "SessionServer.h"
#include <string.h>
#include "SessionCommand.h"
#include "SessionTask.h"

// 处理来自gate的命令
class GateCmdHandler : public Singleton<GateCmdHandler>
{
	friend class Singleton<GateCmdHandler>;
	public:
		GateCmdHandler()
		{

		}

		bool handleCmd(SessionTask* task, const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		//处理网关注册用户命令
		bool user_reg(SessionTask*task, const CMD::SESSION::t_regUser_GateSession* cmd, DWORD cmd_size);
		//处理网关注销用户户命令
		bool user_unreg(SessionTask*task, const CMD::SESSION::t_unregUser_GateSession* cmd, DWORD cmd_size);
		//处理网关进入游戏命令
		bool user_enter(SessionTask*task, const CMD::SESSION::t_enterUser_GateSession* cmd, DWORD cmd_size);
};

#endif
