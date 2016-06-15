/**
 * \file
 * \version  $Id: RecordClient.cpp 64 2013-04-23 02:05:08Z  $
 * \author  ,@163.com
 * \date 2005年04月01日 11时57分23秒 CST
 * \brief 实现网关服务器到档案服务器的连接客户端
 */

#include "RecordClient.h"
#include "RecordCommand.h"
#include "GatewayServer.h"
#include "Fir.h"
#include "GateUserManager.h"
#include "SuperCommand.h"
#include "GatewayServer.h"
#include "SceneClient.h"
#include "TimeTick.h"
#include "GatewayTaskManager.h"
#include "GatewayTask.h"
#include "TimeTick.h"

RecordClient *recordClient = NULL;

/**
 * \brief 创建到档案服务器的连接
 *
 * \return 连接是否成功
 */
bool RecordClient::connectToRecordServer()
{
	//Fir::logger->debug(__PRETTY_FUNCTION__);

	if (!connect())
	{
		Fir::logger->error("连接档案服务器失败");
		return false;
	}

	using namespace CMD::RECORD;
	t_LoginRecord tCmd;
	tCmd.wdServerID = GatewayService::getMe().getServerID();
	tCmd.wdServerType = GatewayService::getMe().getServerType();

	return sendCmd(&tCmd, sizeof(tCmd));
}

/**
 * \brief 线程回调函数
 *
 */
void RecordClient::run()
{
	zTCPBufferClient::run();

	//与档案服务器的连接断开，关闭服务器
	GatewayService::getMe().Terminate();
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
	CommandTime func_time(GatewayService::getMe().timeCmdLog, "RecordClient", ptNullCmd->cmd, ptNullCmd->para);
#endif

	if(ptNullCmd->cmd==CMD::RECORD::CMD_GATE)
	{
		switch(ptNullCmd->para)
		{
			case PARA_GATE_CREATECHAR_RETURN:
				{
					t_CreateChar_Return_GateRecord* cmd = (t_CreateChar_Return_GateRecord*)ptNullCmd;
					GateUser* u = GateUserManager::getMe().getUserAccount(cmd->accountid);
	
					if (!u || !u->gatewaytask)
					{
						Fir::logger->debug("[创建角色], accountid=%u,charid=%u, 角色已下线", cmd->accountid,cmd->charid);
						return true;
					}

					if (u->isTerminateWait() || u->isTerminate() || !u->isCreateState())
					{
						Fir::logger->debug("[创建角色], accountid=%u,charid=%u, 角色已不在创建状态", cmd->accountid,cmd->charid);
						return true;
					}

					u->gatewaytask->onCreateResult(cmd->retcode, cmd->charid);
					
					return true;
				}
				break;
			case PARA_GATE_DELCHAR_RETURN:
				{
					t_DelChar_Return_GateRecord* cmd = (t_DelChar_Return_GateRecord*)ptNullCmd;
					GateUser* u = GateUserManager::getMe().getUserAccount(cmd->accountid);

					if (!u || !u->gatewaytask)
					{
						Fir::logger->debug("[删除角色], accountid=%u,charid=%u, 账号已不在", cmd->accountid,cmd->charid);
						return true;
					}

					u->gatewaytask->onDelResult(cmd->retcode,cmd->charid);

					return true;
				}
				break;
			default:
				break;
		}
	}

	Fir::logger->info("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}
