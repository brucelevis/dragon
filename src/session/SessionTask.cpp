
#include "SessionTask.h"
#include "SessionCommand.h"
#include "SessionServer.h"
#include "SessionTaskManager.h"
#include "SessionUserManager.h"
#include "SessionUser.h"
#include "zMetaData.h"
#include "SceneCmdHandler.h"
#include "GateCmdHandler.h"

/**
 * \brief 验证登陆会话服务器的连接指令
 *
 * 如果验证不通过直接断开连接
 *
 * \param ptCmd 登陆指令
 * \return 验证是否成功
 */
bool SessionTask::verifyLogin(const CMD::SESSION::t_LoginSession *ptCmd)
{
	using namespace CMD::SESSION;

	if (CMD_LOGIN == ptCmd->cmd
			&& PARA_LOGIN == ptCmd->para)
	{
		const CMD::SUPER::ServerEntry *entry = SessionService::getMe().getServerEntry(ptCmd->wdServerID);
		if (entry
				&& ptCmd->wdServerType == entry->wdServerType
				&& 0 == strcmp(getIP(), entry->pstrIP))
		{
			wdServerID = ptCmd->wdServerID;
			wdServerType = ptCmd->wdServerType;
		}

		return true;
	}

	return false;
}


/**
 * \brief 等待接受验证指令并进行验证
 *
 * 实现虚函数<code>zTCPTask::verifyConn</code>
 *
 * \return 验证是否成功，或者超时
 */
int SessionTask::verifyConn()
{
	int retcode = mSocket.recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//这里只是从缓冲取数据包，所以不会出错，没有数据直接返回
			return 0;
		else
		{
			using namespace CMD::SESSION;
			if (verifyLogin((t_LoginSession *)pstrCmd))
			{
				Fir::logger->debug("客户端连接通过验证:%s(%u %u)", this->getIP(), ((t_LoginSession*)pstrCmd)->wdServerID,   ((t_LoginSession*)pstrCmd)->wdServerType);
				veriry_ok=true;
				return 1;
			}
			else
			{
				Fir::logger->error("客户端连接验证失败:%s(%u %u)", this->getIP(), ((t_LoginSession*)pstrCmd)->wdServerID,   ((t_LoginSession*)pstrCmd)->wdServerType);
				return -1;
			}
		}
	}
	else
		return retcode;
}

bool SessionTask::checkRecycle()
{
	if(recycle_state == 0)
	{
		return false;
	}
	if(recycle_state == 1)
	{
		if (this->getType() == SCENESSERVER)
		{
			Fir::logger->info("[注销场景服务器],0,0,0, %u:%u 成功", this->getType(), this->getID());
			SessionUserManager::getMe().removeUserBySceneID(this->getID());
		}
		else
		{
			Fir::logger->info("[注销网关服务器],0,0,0, %u:%u 成功", this->getType(), this->getID());
			SessionUserManager::getMe().removeUserByGateID(this->getID());
		}

		recycle_state=2;
		return true;
	}
	return true;
}
/**
 * \brief 确认一个服务器连接的状态是可以回收的
 *
 * 当一个连接状态是可以回收的状态，那么意味着这个连接的整个生命周期结束，可以从内存中安全的删除了：）<br>
 * 实现了虚函数<code>zTCPTask::recycleConn</code>
 *
 * \return 是否可以回收
 */
int SessionTask::recycleConn()
{
	if(veriry_ok)
	{
		switch(recycle_state)
		{
			case 0:
				{
					recycle_state=1;
					return 0;
				}
				break;
			case 1:
				{
					return 0;
				}
				break;
			case 2:
				{
					return 1;
				}
				break;
		}
	}
	return 1;
}
/*
   {

   SessionUserManager::getMe().removeUserByGatewayID(wdServerID);
   return 1;
   }
// */
void SessionTask::addToContainer()
{
	SessionTaskManager::getMe().addSessionTask(this);
}

void SessionTask::removeFromContainer()
{
	SessionTaskManager::getMe().removeSessionTask(this);
}

bool SessionTask::uniqueAdd()
{
	return SessionTaskManager::getMe().uniqueAdd(this);
}

bool SessionTask::uniqueRemove()
{
	return SessionTaskManager::getMe().uniqueRemove(this);
}

/**
 * \brief 解析来自各个服务器连接的指令
 *
 * \param ptNullCmd 待处理的指令
 * \param nCmdLen 指令长度
 * \return 处理是否成功
 */
bool SessionTask::msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}

bool SessionTask::cmdMsgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::SESSION;

#ifdef _COMMAND_TIME
	CommandTime func_time(SessionService::getMe().timeCmdLog, "SessionTask", ptNullCmd->cmd, ptNullCmd->para);
#endif
	
	if (GateCmdHandler::getMe().handleCmd(this,ptNullCmd,nCmdLen))
		return true;

	if (SceneCmdHandler::getMe().handleCmd(this,ptNullCmd,nCmdLen))
		return true;

	Fir::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief 发送命令给用户
 *
 * \param id 用户id
 * \param pstrCmd 命令指令
 * \param nCmdLen 命令长度
 */
bool SessionTask::sendCmdToUser(DWORD id, const void *pstrCmd, const unsigned int nCmdLen)
{

	using namespace CMD::SESSION;
	using namespace CMD;

	BYTE buf[zSocket::MAX_DATASIZE];
	t_Session_ForwardUser *scmd=(t_Session_ForwardUser *)buf;
	constructInPlace(scmd);

	scmd->dwID = id;
	scmd->size = nCmdLen;
	bcopy(pstrCmd, scmd->data, nCmdLen);
	return sendCmd(scmd, sizeof(t_Session_ForwardUser)+nCmdLen);
}


