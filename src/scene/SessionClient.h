/**
 * \file
 * \version  $Id: SessionClient.h 37 2013-04-08 01:52:56Z  $
 * \author  , 
 * \date 2013年04月05日 13时58分55秒 CST
 * \brief 定义Session服务器连接客户端
 *
 */

#ifndef _SessionClient_h_
#define _SessionClient_h_

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "SessionCommand.h"
#include "zMisc.h"
#include "zSocket.h"
#include "MessageQueue.h"

class SceneUser;

class SessionClient;

class SessionClient : public zTCPBufferClient , public MessageQueue<>
{

	public:

		/**
		* \brief 构造函数
		* \param  name 名称
		* \param  ip   地址
		* \param  port 端口
		*/
		SessionClient(
				const std::string &name, 
				const std::string &ip, 
				const unsigned short port)
			: zTCPBufferClient(name, ip, port) {};

		bool connectToSessionServer();
		void run();

		bool msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool cmdMsgParse(const CMD::t_NullCmd *, const unsigned int);
};

/// 声明
extern SessionClient *sessionClient;

#endif

