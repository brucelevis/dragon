/**
 * \file
 * \version  $Id: SessionCmdHandler.h 42 2013-04-10 07:33:59Z  $
 * \author   ,
 * \date 2013年03月27日 12时14分48秒 CST
 * \brief 出来来自session的命令
 *
 */

#ifndef _SESSION_CMD_HANDLER_H_
#define _SESSION_CMD_HANDLER_H_

#include <string.h>
#include "Fir.h"
#include "SceneServer.h"
#include "SessionCommand.h"
#include "SessionClient.h"

// 处理来自session的命令
class SessionCmdHanlder : public Singleton<SessionCmdHanlder>
{
	friend class Singleton<SessionCmdHanlder>;
	public:
		SessionCmdHanlder()
		{

		}

		bool handleCmd(SessionClient* task, const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		bool userreg(SessionClient* task, const CMD::SESSION::t_regUser_SessionScene* cmd, DWORD nCmdLen);
		bool user_unreg(SessionClient* task, const CMD::SESSION::t_unregUser_SessionScene* cmd, DWORD nCmdLen);
		bool user_online_offline(SessionClient* task, const CMD::SESSION::t_online_offline_SessionScene* cmd, DWORD nCmdLen);

		bool req_gm(SessionClient* task, const CMD::SESSION::t_GmRequest_SessionScene* cmd,const DWORD len);

	private:
		void ret_userreg(SessionClient* task, const CMD::SESSION::t_regUser_SessionScene* cmd, BYTE retcode);
};

#endif
