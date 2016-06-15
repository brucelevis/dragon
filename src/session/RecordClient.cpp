/**
 * \file
 * \version  $Id: RecordClient.cpp 29 2013-04-07 01:39:43Z  $
 * \author  ,@163.com
 * \date 2004年11月05日 13时58分55秒 CST
 * \brief 定义档案服务器连接客户端
 *
 * 负责与档案服务器交互，存取档案
 * 
 */

#include <unistd.h>
#include <iostream>

#include "zTCPClient.h"
#include "RecordCommand.h"
#include "RecordClient.h"
#include "SessionServer.h"
#include "Fir.h"
#include "SessionTask.h"
#include "TimeTick.h"
#include "zCmdBuffer.h"

///RecordClient的唯一实例
RecordClient *recordClient = NULL;

/**
 * \brief 创建到档案服务器的连接
 *
 * \return 连接是否成功
 */
bool RecordClient::connectToRecordServer()
{
	if (!connect())
	{
		Fir::logger->error("连接档案服务器失败");
		return false;
	}

	using namespace CMD::RECORD;
	t_LoginRecord tCmd;
	tCmd.wdServerID = SessionService::getMe().getServerID();
	tCmd.wdServerType = SessionService::getMe().getServerType();

	return sendCmd(&tCmd, sizeof(tCmd));
}

/**
 * \brief 重载zTCPBufferClient的run方法
 *
 *
 */
void RecordClient::run()
{
	zTCPBufferClient::run();

	//与档案服务器的连接断开，关闭服务器
	SessionService::getMe().Terminate();
	while (!SessionService::getMe().isSequeueTerminate())
	{
		zThread::msleep(10);
	}
}

/**
 * \brief 解析来自档案服务器的指令
 *
 * \param ptNullCmd 待解析的指令
 * \param nCmdLen 待解析的指令长度
 * \return 解析指令是否成功
 */
bool RecordClient::msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}
bool RecordClient::cmdMsgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::RECORD;

#ifdef _COMMAND_TIME
	CommandTime func_time(SessionService::getMe().timeCmdLog, "RecordClient", ptNullCmd->cmd, ptNullCmd->para);
#endif

	if(ptNullCmd->cmd==CMD::RECORD::CMD_SESSION)
	{//处理档案<->场景的消息

		switch(ptNullCmd->para)
		{   
		case PARA_RECORD_GM_REPLY:
			{
				const CMD::RECORD::t_GmReply_RecordSession* cmd = (const CMD::RECORD::t_GmReply_RecordSession*)ptNullCmd;
				this->retGmReply(cmd, nCmdLen);
			}
			break;
		default:
			break;
		}   
	}

	Fir::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);

	return false;
}

bool RecordClient::retGmReply(const CMD::RECORD::t_GmReply_RecordSession* cmd, DWORD cmd_size)
{
	if (!cmd)
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