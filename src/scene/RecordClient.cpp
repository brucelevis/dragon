/**
 * \file
 * \version  $Id: RecordClient.cpp 37 2013-04-08 01:52:56Z  $
 * \author  ,
 * \date 2013年04月05日 13时58分55秒 CST
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
#include "SceneServer.h"
#include "Fir.h"
#include "SceneTask.h"
#include "SessionClient.h"
#include "TimeTick.h"

///RecordClient的唯一实例
RecordClient *recordClient = NULL;
/**
 * \brief 解压角色需要保存的数据
 *
 * \param pUser 角色指针
 * \param data 输入：压缩的数据 / 输出：解压后的数据 
 * \param dataSize 输入 数据大小
 * \return 解压后的数据大小
 */
//bool uncompressSaveData(SceneUser *pUser , const unsigned char *data ,const DWORD dataSize, unsigned char * petData); 

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
	tCmd.wdServerID = SceneService::getMe().getServerID();
	tCmd.wdServerType = SceneService::getMe().getServerType();

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
	SceneService::getMe().Terminate();
	while (sessionClient)
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
    CommandTime func_time(SceneService::getMe().timeCmdLog, "RecordClient", ptNullCmd->cmd, ptNullCmd->para);
#endif
	if(ptNullCmd->cmd==CMD_SCENE)
	{//处理档案<->场景的消息

		
	}

	Fir::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);

	return false;
}

