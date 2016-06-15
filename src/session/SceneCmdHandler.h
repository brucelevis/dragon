/**
 * \file
 * \version  $Id: SceneCmdHandler.h 42 2013-04-10 07:33:59Z  $
 * \author   ,
 * \date 2013年03月27日 12时14分48秒 CST
 * \brief 出来来自scene的命令
 *
 */

#ifndef _SCENE_CMD_HANDLER_H_
#define _SCENE_CMD_HANDLER_H_

#include <map>
#include "SessionCommand.h"
#include "SessionTask.h"
#include "Fir.h"
#include "SessionServer.h"
#include <string.h>

// 处理来自session的命令
class SceneCmdHandler : public Singleton<SceneCmdHandler>
{
	friend class Singleton<SceneCmdHandler>;
	public:
		SceneCmdHandler()
		{

		}

		bool handleCmd(SessionTask* task, const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		bool user_reg_reply(SessionTask*task, const CMD::SESSION::t_regUser_Return_SceneSession* cmd, DWORD cmd_size);
		bool ret_gm_reply(SessionTask* task, const CMD::SESSION::t_GmReply_SceneSession* cmd, DWORD cmd_size);
		bool unser_unreg(SessionTask* task, const CMD::SESSION::t_unregUser_SceneSession* cmd, DWORD cmd_size);
};

#endif
