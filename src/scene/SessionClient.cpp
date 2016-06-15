/**
 * \file
 * \version  $Id: SessionClient.cpp 36 2013-04-07 11:42:48Z  $
 * \author  ,
 * \date 2013年03月31日 10时20分55秒 CST
 * \brief 定义Session服务器连接客户端
 *
 */

#include <unistd.h>
#include <iostream>
#include "zTCPClient.h"
#include "SessionCommand.h"
#include "SessionClient.h"
#include "SceneServer.h"
#include "Fir.h"
#include "SceneTaskManager.h"
#include "RecordClient.h"
#include "StringUtil.h"
#include "TimeTick.h"
#include "SessionCmdHandler.h"


SessionClient *sessionClient = NULL;

/**
 * \brief 重载zThread中的纯虚函数，是线程的主回调函数，用于处理接收到的指令
 *
 */
void SessionClient::run()
{
	zTCPBufferClient::run();

	//与Session之间的连接断开，需要关闭服务器
	SceneService::getMe().Terminate();
	while(!SceneService::getMe().isSequeueTerminate())
	{
		zThread::msleep(10);
	}

}


/**
 * \brief  处理会话发送过来的消息
 * \param  
 * \return  
 */
bool SessionClient::msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}


bool SessionClient::cmdMsgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
#ifdef _COMMAND_TIME
	CommandTime func_time(SceneService::getMe().timeCmdLog, "SessionClient", ptNullCmd->cmd, ptNullCmd->para);
#endif

	if(SessionCmdHanlder::getMe().handleCmd(this,ptNullCmd,nCmdLen))
	{
		return true;
	}
	
	Fir::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief  登录会话服务器
 * \return  true 登录消息发送成功， false 无法发送消息
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
	tCmd.wdServerID = SceneService::getMe().getServerID();
	tCmd.wdServerType = SceneService::getMe().getServerType();

	return sendCmd(&tCmd, sizeof(tCmd));
}
	
