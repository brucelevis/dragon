/**
 * \file
 * \version  $Id: FLClient.cpp 252 2013-05-30 04:02:01Z  $
 * \author  ,@163.com
 * \date 2005年03月12日 16时16分40秒 CST
 * \brief 定义登陆服务器客户端
 *
 * 
 */

#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "FLClient.h"
#include "SuperServer.h"
#include "FLCommand.h"
#include "SuperCommand.h"
#include "ServerManager.h"
#include "FLClientManager.h"
#include "TimeTick.h"

/**
 * \brief 临时编号分配器
 *
 */
WORD FLClient::tempidAllocator = 0;

/**
 * \brief 构造函数
 * \param ip 服务器地址
 * \param port 服务器端口
 */
FLClient::FLClient(
		const std::string &ip, 
		const unsigned short port) : zTCPClientTask(ip, port, true), tempid(++tempidAllocator), netType(NetType_near)
{
}

/**
 * \brief 析构函数
 *
 */
FLClient::~FLClient()
{
}

int FLClient::checkRebound()
{
	int retcode = pSocket->recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = pSocket->recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//这里只是从缓冲取数据包，所以不会出错，没有数据直接返回
			return 0;
		else
		{
			using namespace CMD::FL;

			t_LoginFL_OK *ptCmd = (t_LoginFL_OK *)pstrCmd;
			if (CMD_LOGIN == ptCmd->cmd
					&& PARA_LOGIN_OK == ptCmd->para)
			{
				Fir::logger->debug("登陆FLServer成功，收到区的编号：%u, %s, %u",
						ptCmd->gameZone,
						ptCmd->name,
						ptCmd->netType);
				netType = (ptCmd->netType == 0 ? NetType_near : NetType_far);
				SuperService::getMe().setZoneID(ptCmd->gameZone);
				SuperService::getMe().setZoneName(ptCmd->name);

				CMD::SUPER::t_ZoneID_Scene send;
				send.zone = SuperService::getMe().getZoneID();
				ServerManager::getMe().broadcastByType(SCENESSERVER, &send, sizeof(send));

				CMD::SUPER::t_ZoneID_Session send1;
				send1.zone =  SuperService::getMe().getZoneID();
				strncpy(send1.name,ptCmd->name,MAX_NAMESIZE);
				ServerManager::getMe().broadcastByType(SESSIONSERVER, &send1, sizeof(send1));

				CMD::SUPER::t_ZoneID_Gate send2;
				send2.zone = SuperService::getMe().getZoneID();
				ServerManager::getMe().broadcastByType(GATEWAYSERVER, &send2, sizeof(send2));
				
				CMD::SUPER::t_ZoneID_Record send3;
				send3.zone = SuperService::getMe().getZoneID();
				ServerManager::getMe().broadcastByType(RECORDSERVER, &send3, sizeof(send3));

				return 1;
			}
			else
			{
				Fir::logger->error("登陆FLServer失败");
				return -1;
			}
		}
	}
	else
		return retcode;
}

void FLClient::addToContainer()
{
	FLClientManager::getMe().add(this);
}

void FLClient::removeFromContainer()
{
	FLClientManager::getMe().remove(this);
}

bool FLClient::connect()
{
	if (!zTCPClientTask::connect())
		return false;

	using namespace CMD::FL;
	t_LoginFL cmd;
	strncpy(cmd.strIP, SuperService::getMe().getIP(), sizeof(cmd.strIP)-1);
	cmd.port = SuperService::getMe().getPort();
	return sendCmd(&cmd, sizeof(cmd));
}

bool FLClient::msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::FL;

#ifdef _COMMAND_TIME
	CommandTime func_time(SuperService::getMe().timeCmdLog, "FLClient", ptNullCmd->cmd, ptNullCmd->para);
#endif

	switch(ptNullCmd->cmd)
	{
		case CMD_GYLIST:
			if (msgParse_gyList(ptNullCmd, nCmdLen))
				return true;
			break;
		case CMD_SESSION:
			if (msgParse_session(ptNullCmd, nCmdLen))
				return true;
			break;
	}

	Fir::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool FLClient::msgParse_gyList(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::FL;

	switch(ptNullCmd->para)
	{
		case PARA_FL_RQGYLIST:
			{
				//t_RQGYList_FL *ptCmd = (t_RQGYList_FL *)ptNullCmd;
				CMD::SUPER::t_RQGYList_Gateway tCmd;

				return ServerManager::getMe().broadcastByType(GATEWAYSERVER, &tCmd, sizeof(tCmd));
			}
			break;
	}

	Fir::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool FLClient::msgParse_session(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::FL;
	using namespace CMD::SUPER;
	

	switch(ptNullCmd->para)
	{
		case PARA_ALLZONE2SUPER_FORWARD2GATEWAY:
			{
				t_FL2Super_Forward2Gateway* cmd = (t_FL2Super_Forward2Gateway*)ptNullCmd;

				BYTE buf[zSocket::MAX_DATASIZE];
				t_Super2Gate_FromFL *scmd=(t_Super2Gate_FromFL *)buf;
				constructInPlace(scmd);

				scmd->datasize = cmd->datasize;
				bcopy(cmd->data, scmd->data, cmd->datasize);
				ServerManager::getMe().broadcastByID(cmd->gateid,scmd, scmd->getSize());
				return true;
			}
			break;
		case PARA_FL2SUPER_FORWARD2SCENE:
			{
				t_FL2Super_Forward2Scene* cmd = (t_FL2Super_Forward2Scene*)ptNullCmd;

				BYTE buf[zSocket::MAX_DATASIZE];
				t_Super2Scene_FromFL *scmd=(t_Super2Scene_FromFL *)buf;
				constructInPlace(scmd);

				scmd->datasize = cmd->datasize;
				bcopy(cmd->data, scmd->data, cmd->datasize);
				ServerManager::getMe().broadcastByID(cmd->sceneid,scmd, scmd->getSize());
				return true;
			}
			break;
		case PARA_FL2SUPER_FORWARD2SESSION:
			{
				t_FL2Super_Forward2Session* cmd = (t_FL2Super_Forward2Session*)ptNullCmd;

				BYTE buf[zSocket::MAX_DATASIZE];
				t_Super2Session_FromFL *scmd=(t_Super2Session_FromFL *)buf;
				constructInPlace(scmd);

				Fir::logger->debug("%s,sssssssssssssssssssss,cmd->sessionid=%u", __PRETTY_FUNCTION__,cmd->sessionid);

				scmd->datasize = cmd->datasize;
				bcopy(cmd->data, scmd->data, cmd->datasize);
				ServerManager::getMe().broadcastByID(cmd->sessionid,scmd, scmd->getSize());
				return true;
			}
			break;

		case PARA_FL2SUPER_GM2SESSION:
			{
				t_FL2Super_GM2Session* cmd = (t_FL2Super_GM2Session*)ptNullCmd;

				BYTE buf[zSocket::MAX_DATASIZE];
				t_ReqGM_Super2Session *scmd = (t_ReqGM_Super2Session *)buf;
				constructInPlace(scmd);

				scmd->datasize = cmd->datasize;
				bcopy(cmd->data, scmd->data, cmd->datasize);
				ServerManager::getMe().broadcastByType(SESSIONSERVER, scmd, scmd->getSize());
				return true;
			}
		break;
	}

	Fir::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

