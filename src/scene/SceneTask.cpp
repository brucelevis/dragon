#include "SceneTask.h"
#include "SceneCommand.h"
#include "SceneServer.h"
#include "SceneTaskManager.h"
#include "SceneUserManager.h"
#include "SceneUser.h"
#include "zMetaData.h"
#include "TimeTick.h"
#include "StringUtil.h"
/**
 * \brief 验证登陆档案服务器的连接指令
 *
 * 如果验证不通过直接断开连接
 *
 * \param ptCmd 登陆指令
 * \return 验证是否成功
 */
bool SceneTask::verifyLogin(const CMD::SCENE::t_LoginScene *ptCmd)
{
	using namespace CMD::SCENE;

	if (CMD_LOGIN == ptCmd->cmd
			&& PARA_LOGIN == ptCmd->para)
	{
		const CMD::SUPER::ServerEntry *entry = SceneService::getMe().getServerEntry(ptCmd->wdServerID);
		char strIP[32];
		strncpy(strIP, getIP(), 31);
		if (entry
				&& ptCmd->wdServerType == entry->wdServerType
				&& 0 == strcmp(strIP, entry->pstrIP))
		{
			wdServerID = ptCmd->wdServerID;
			wdServerType = ptCmd->wdServerType;
			return true;
		}
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
int SceneTask::verifyConn()
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
			using namespace CMD::SCENE;
			if (verifyLogin((t_LoginScene *)pstrCmd))
			{
				Fir::logger->debug("客户端连接通过验证");
				veriry_ok=true;
				return 1;
			}
			else
			{
				Fir::logger->error("客户端连接验证失败");
				return -1;
			}
		}
	}
	else
		return retcode;
}

bool SceneTask::checkRecycle()
{
	if(recycle_state == 0)
	{
		return false;
	}
	if(recycle_state == 1)
	{
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
int SceneTask::recycleConn()
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

		::getMe().removeUserByGatewayID(wdServerID);
	return 1;
}
// */

bool SceneTask::uniqueAdd()
{
	return SceneTaskManager::getMe().uniqueAdd(this);
}

bool SceneTask::uniqueRemove()
{
	return SceneTaskManager::getMe().uniqueRemove(this);
}

void SceneTask::addToContainer()
{
	//场景启动完成
	CMD::SCENE::t_StartOKSceneGate send;
	this->sendCmd(&send, sizeof(send));
}

/**
 * \brief 解析来自各个服务器连接的指令
 *
 * \param ptNullCmd 待处理的指令
 * \param nCmdLen 指令长度
 * \return 处理是否成功
 */
bool SceneTask::msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}


bool SceneTask::user_cmdparse(const CMD::t_NullCmd* ptNullCmd, const unsigned int nCmdLen)
{
	CMD::SCENE::t_Scene_ForwardScene* rev = (CMD::SCENE::t_Scene_ForwardScene*)ptNullCmd;
	const CMD::t_NullCmd* cmd = (const CMD::t_NullCmd*)(rev->data);

#ifdef _COMMAND_TIME
	std::string strMsg = StringUtil::Format("解析消息：%u", cmd->_id);
	FunctionTime func_time(SceneService::getMe().timeCmdLog, "[SceneTask]", strMsg.c_str() , 32); 
#endif

	unsigned int cmdlen = rev->size;
	SceneService::cmd_record[0] = cmd->_id;
	SceneUser* u = SceneUserManager::getMe().getUserByID(rev->charid);
	if (!u) 
	{
		Fir::logger->debug("[消息处理]失败:用户不存在，账号ID = %u，角色ID = %u，消息ID = %u", rev->accid, rev->charid, cmd->_id);
		return false;
	}
#ifdef _CMD_DEBUG
	PrintDataRecv(cmd, cmdlen);
#endif
	Packet p;
	p.resize(cmdlen);
	memcpy(&p[0], (const char*)cmd, cmdlen);
	p.SeekSet(sizeof(CMD::t_NullCmd));
	
	//****************************
	//****************************
    return true;
}

bool SceneTask::cmdMsgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{//该函数处理来自网关的消息
	if (ptNullCmd->_id == CMD::SCENE::ID::t_Scene_ForwardScene)
	{//如果是转发用户消息，先把消息解开后，再派发
		if(user_cmdparse(ptNullCmd, nCmdLen))
		{
			return true;
		}
		return true;
	}
	Fir::logger->error("%s(%u, %u)", __PRETTY_FUNCTION__, ptNullCmd->_id, nCmdLen);
	return false;
}

bool SceneTask::parseGateMsg(const CMD::t_NullCmd * cmd, const unsigned int len)
{
	return false;
}

bool SceneTask::parseForwardMsg(const CMD::t_NullCmd * cmd, const unsigned int len)
{
	return false;
}

/**
 * \brief 发送命令给用户
 *
 * \param id 用户id
 * \param pstrCmd 命令指令
 * \param nCmdLen 命令长度
 */
bool SceneTask::sendCmdToUser(DWORD id, const void *pstrCmd, const unsigned int nCmdLen)
{
#ifdef _CMD_DEBUG
	Fir::logger->trace("[消息发送]: 角色ID = %u", id);
	PrintDataSend(pstrCmd, nCmdLen);
#endif
	BYTE buf[zSocket::MAX_DATASIZE];
	CMD::SCENE::t_User_FromScene *scmd=(CMD::SCENE::t_User_FromScene *)buf;
	constructInPlace(scmd);

	scmd->charid = id;
	scmd->size = nCmdLen;
	bcopy(pstrCmd, scmd->data, nCmdLen);
	return sendCmd(scmd, sizeof(CMD::SCENE::t_User_FromScene)+nCmdLen);
}

bool SceneTask::sendCmdToWorld(DWORD except_charid, const void *pstrCmd, const unsigned int nCmdLen)
{
#ifdef _CMD_DEBUG
	Fir::logger->trace("[消息发送]: 世界消息");
	PrintDataSend(pstrCmd, nCmdLen);
#endif
	BYTE buf[zSocket::MAX_DATASIZE];
	CMD::SCENE::t_Forward2World_FromScene *scmd=(CMD::SCENE::t_Forward2World_FromScene *)buf;
	constructInPlace(scmd);

	scmd->except_charid = except_charid;
	scmd->size = nCmdLen;
	bcopy(pstrCmd, scmd->data, nCmdLen);
	return sendCmd(scmd, sizeof(CMD::SCENE::t_Forward2World_FromScene)+nCmdLen);
}

