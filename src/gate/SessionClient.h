/**
 * \file
 * \version  $Id: SessionClient.h 37 2013-04-08 01:52:56Z  $
 * \author  ,@163.com
 * \date 2004年11月05日 13时58分55秒 CST
 * \brief 定义Session服务器连接客户端
 *
 */

#ifndef _SessionClient_h_
#define _SessionClient_h_

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "SessionCommand.h"
#include "MessageQueue.h"

class SessionClient;

/**
 * \brief 定义Session服务器连接客户端类
 *
 */
class SessionClient : public zTCPBufferClient , public MessageQueue<>
{

	public:

		SessionClient(
				const std::string &name, 
				const std::string &ip, 
				const unsigned short port)
			: zTCPBufferClient(name, ip, port,false, 25000) 
		{
		};

		bool connectToSessionServer();
		void run();

		bool msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool cmdMsgParse(const CMD::t_NullCmd *, const unsigned int);
};

extern SessionClient *sessionClient;

#endif

