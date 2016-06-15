/**
 * \file
 * \version  $Id: GateUser.cpp 67 2013-04-23 09:44:20Z  $
 * \author  ,@163.com
 * \date 2005年04月01日 11时56分41秒 CST
 * \brief 实现网关用户类
 */

#include <stdlib.h>
#include "GateUser.h"
#include "GateUserManager.h"
#include "RecordCommand.h"
#include "RecordClient.h"
#include "GatewayTask.h"
#include "SessionClient.h"
#include "SceneClient.h"
#include "GatewayServer.h"
#include "TimeTick.h"
#include "SceneClientManager.h"
#include "HMessages.h"

extern bool getMemDbAccountItem(DWORD accid, AccountItem& item);

GateUser::GateUser(DWORD accid, GatewayTask* task)
{
	_accid = accid;
	_charid=0;	
	_sceneid = 0;

	initState();

	gatewaytask=task;

	Fir::logger->debug("[构造玩家]，账号ID = %u, 连接序号 = %llu", getACCID(), gatewaytask->getID());
}

GateUser::~GateUser()
{
	Fir::logger->debug("[销毁玩家]，账号ID = %u, 连接序号 = %llu", getACCID(), gatewaytask->getID());
	gatewaytask = NULL;
}

/**
 * \brief 设置游戏状态
 *
 *
 */
void GateUser::playState(SceneClient *s , DWORD scene_tempid)
{
	systemstate = SYSTEM_STATE_PLAY;
}

/**
 * \brief 发送数据给客户端
 *
 *
 * \param pstrCMD: 发送的指令
 * \param nCmdLen: 指令长度
 * \return 发送是否成功
 */
bool GateUser::sendCmd(const void *pstrCmd, const unsigned int nCmdLen,const bool hasPacked)
{
	if (!hasPacked)
	{
		if(gatewaytask)
		{
			return gatewaytask->sendCmd(pstrCmd,nCmdLen);
		}
	}else
	{
		if(gatewaytask)
		{
			return gatewaytask->sendCmdNoPack(pstrCmd,nCmdLen);
		}
	}
	return false;
}

/**
 * \brief 网关注销一个用户
 *
 */
void GateUser::unreg(BYTE unregtype)
{
	if (isWaitUnregState())
	{
		return;
	}
	onQuitGame();
	GateUserManager::getMe().removeUserAccount(getACCID());

	waitUnregState();

	GatewayService::getMe().notifyLoginServer();
	// 通知退出
	notifyLogout(unregtype);

	Fir::logger->trace("[网关注销玩家]，账号ID = %u，注销类型 = %d", getACCID(), (int)unregtype);
}

/**
 * \brief 网关注册一个用户
 *
 */
bool GateUser::reg(GatewayTask* task)
{
	setACCID(task->getACCID());

	if (!GateUserManager::getMe().addUserAccount(this))
	{
		Fir::logger->trace("[网关注册玩家]失败，加入容器失败，账号ID = %u", getACCID());
		return false;
	}

	lock();
	{
		gatewaytask = task;
		gatewaytask->_user = this;
	}
	unlock();

	createState();
	GatewayService::getMe().notifyLoginServer();

	Fir::logger->trace("[网关注册玩家]，账号ID = %u", getACCID());

	return true;
}

/**
 * \brief 重连
 *
 */
bool GateUser::recon(GatewayTask* task)
{
	zMutex_scope_lock lock(mlock);

	GatewayTask* taskOld = gatewaytask;
	if (taskOld)
	{
		taskOld->_user = NULL;
		taskOld->logoutOK();

		if (!taskOld->isTerminate() && !taskOld->isTerminateWait())
		{
			taskOld->TerminateWait();
		}
	}
	gatewaytask = task;
	gatewaytask->_user = this;

	return true;
}

/**
* \brief 请求进入游戏
*
*/
bool GateUser::reqEnterGame(DWORD charid)
{
	if (isWaitPlayState())
	{
		Fir::logger->warn("[请求进入游戏]失败，进入游戏中, 账号ID = %u，角色ID = %u", getACCID(), charid);
		return false;
	}
	if (isPlayState())
	{
		Fir::logger->warn("[请求进入游戏]失败，已在游戏中, 账号ID = %u，角色ID = %u", getACCID(), charid);
		return false;
	}

	AccountItem item;
	if (!getMemDbAccountItem(getACCID(), item))
	{
		Fir::logger->error("[请求进入游戏]失败，获取账号数据失败, 账号ID = %u，角色ID = %u", getACCID(), charid);
		return false;
	}

	if((item.playerid1 != charid) && (item.playerid2 != charid) && (item.playerid3 != charid) && (item.playerid4 != charid))
	{
		Fir::logger->warn("[请求进入游戏]失败，不存在角色, 账号ID = %u，角色ID = %u", getACCID(), charid);
		return false;
	}

	waitPlayState();

	CMD::SESSION::t_enterUser_GateSession send;
	send.seqid = gatewaytask->getID();
	send.accid = getACCID();
	send.charid = charid;

	sessionClient->sendCmd(&send,sizeof(send));

	Fir::logger->trace("[请求进入游戏]，账号ID = %u，角色ID = %u", getACCID(), charid);

	return true;
}

/**
* \brief 进入游戏失败
*
*/
void GateUser::onEnterGameSuccess(SceneClient* scene, DWORD charid)
{
	if (isPlayState())
	{
		Fir::logger->warn("[进入游戏]失败，已在游戏中, 账号ID = %u，角色ID = %u", getACCID(), getCharID());
		return;
	}
	setCharID(charid);

	if (!GateUserManager::getMe().addUserCharid(this))
	{
		Fir::logger->error("[进入游戏]失败，加入容器失败，账号ID = %u，角色ID = %u", getACCID(), getCharID());
		return;
	}
	playState();

	if(scene)
	{
		_sceneid = scene->getServerID();
	}

	Fir::logger->trace("[进入游戏]成功，账号ID = %u，角色ID = %u", getACCID(), getCharID());
}

/**
* \brief 进入游戏失败
*
*/
void GateUser::onEnterGameFailed(DWORD charid, BYTE retcode)
{
	WORD errorId = 2693;

	switch (retcode)
	{
	case 1:			// 未知错误
		break;
	case 2:			// 角色被封停
		errorId = 5668;
		break;
	default:
		break;
	}
	// 恢复状态
	createState();

	Packet p;
	p << WORD(ID_Gs2Ci_EnterGameFailed_Reply) << false << errorId;
	sendCmdToUser(p);

	Fir::logger->error("[进入游戏]失败，返回码 = %d，账号ID = %u，角色ID = %u", (int)retcode, getACCID(), charid);
}

/**
* \brief 退出游戏
*
*/
void GateUser::onQuitGame()
{
	if (!isPlayState())
	{
		return;
	}
	createState();

	GateUserManager::getMe().removeUserCharid(getCharID());

	Fir::logger->trace("[退出游戏]，账号ID = %u，角色ID = %u", getACCID(), getCharID());
}

/**
 * \brief 中断连接
 *
 */
void GateUser::TerminateWait()
{
	if(gatewaytask)
		gatewaytask->TerminateWait();
}

/**
 * \brief 是否处于等于中断状态
 *
 */
bool GateUser::isTerminateWait()
{
	if(gatewaytask)
		return gatewaytask->isTerminateWait();

	return true;
}

/**
 * \brief 是否已经中断
 *
 */
bool GateUser::isTerminate()
{
	if (gatewaytask)
		return gatewaytask->isTerminate();

	return true;
}

/**
* \brief 发消息到玩家
*/
bool GateUser::sendCmdToUser(Packet& p)
{
	if (gatewaytask)
	{
		gatewaytask->SendMessage2Ci(p);
		return true;
	}
	return false;
}
/**
* \brief 发消息到场景
*/
bool GateUser::sendCmdToScene(const void *pstrCmd, const int nCmdLen)
{
	return SceneClientManager::getMe().sendTo(getSceneID(), pstrCmd, nCmdLen);
}

/**
* \brief 通知退出
*
*/
void GateUser::notifyLogout(BYTE unregtype)
{
	Packet p;
	p << WORD(ID_Gs2Ci_PlayerLogout) << unregtype;

	sendCmdToUser(p);
}