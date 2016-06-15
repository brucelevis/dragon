#include "SessionUser.h"
#include "SessionTaskManager.h"
#include "TimeTick.h"
#include "SessionUserManager.h"

/**
* \brief 注册到网关
*
*/
bool SessionUser::regToGate(BYTE regtype)
{
	CMD::SESSION::t_regUser_SessionGate send;
	send.seqid = _seqid;
	send.accid = _accid;
	send.regtype = regtype;
	send.retcode = 0;

	sendCmdToGate(&send,sizeof(send));

	Fir::logger->trace("[到网关注册]，网关ID[%u]，网关序号[%llu]，账号ID[%u]，注册类型[%u]", getGateID(), getSeqID(), getACCID(), (DWORD)regtype);

	return true;
}

/**
* \brief 注册到场景
*
*/
bool SessionUser::regToScene(SessionTask* scene, DWORD charid, BYTE regtype)
{
	setSceneID(scene->getID());
	setCharID(charid);

	CMD::SESSION::t_regUser_SessionScene send;
	send.accid = getACCID();
	send.charid = getCharID();
	send.gateid = getGateID();
	send.reg_type = regtype;

	sendCmdToScene(&send, sizeof(send));
	
	if (regtype == 0)
	{
		// 进入游戏状态
		setState(SessionState::enter);
	}

	Fir::logger->trace("[到场景注册]，场景ID[%u]，账号ID[%u]，角色ID[%u]，注册类型[%d]", getSceneID(), getACCID(), getCharID(), int(regtype));

	return true;
}

/**
* \brief 注册到场景失败
*
*/
void SessionUser::onRegToSceneError(DWORD charid, BYTE retcode)
{
	CMD::SESSION::t_enterUser_Return_SessionGate send;
	send.seqid = getSeqID();
	send.accid = getACCID();
	send.charid = charid;
	send.retcode = retcode;

	sendCmdToGate(&send, sizeof(send));

	// 恢复状态
	setState(SessionUser::SessionState::select);

	Fir::logger->trace("[到场景注册]失败，账号ID[%u]，角色ID[%u]", getACCID(), getCharID());
}

/**
* \brief 从网关注销
*
*/
void SessionUser::unregFromGate(const UnregMethod method, const BYTE unregtype/* = 0*/)
{
	if (getGateID() <= 0)
		return;

	CMD::SESSION::t_unregUser_SessionGate send;
	send.seqid = getSeqID();
	send.unregtype = unregtype;

	sendCmdToGate(&send,sizeof(send));

	offline_gate();

	Fir::logger->trace("[从网关注销]，网关ID[%u]，网关序号[%llu]，账号ID[%u]", getGateID(), getSeqID(), getACCID());
}

/**
* \brief 从场景注销
*
*/
void SessionUser::unregFromScene(const UnregMethod method, const BYTE unregtype/* = 0*/)
{
	if (getSceneID() <= 0)
		return;

	if (method != UnregMethod::unreg_passive_scene)
	{
		CMD::SESSION::t_unregUser_SessionScene send;
		send.charid = getCharID();
		send.accid = getACCID();
		send.unreg_type = unregtype;

		sendCmdToScene(&send, sizeof(send));
	}
	if (unregtype == 0)
	{
		offline_scene();
	}

	Fir::logger->trace("[从场景注销]，场景ID[%u]，账号ID[%u]，角色ID[%u]，注销类型[%d]", getSceneID(), getACCID(), getCharID(), (int)unregtype);
}

/**
* \brief 重连
*
*/
bool SessionUser::reconn()
{
	return regToGate(2);
}

/**
* \brief 发消息到网关
*/
bool SessionUser::sendCmdToGate(const void *pstrCmd, const int nCmdLen)
{
	return SessionTaskManager::getMe().broadcastByID(getGateID(), pstrCmd, nCmdLen);
}

/**
* \brief 发消息到场景
*/
bool SessionUser::sendCmdToScene(const void *pstrCmd, const int nCmdLen)
{
	return SessionTaskManager::getMe().broadcastByID(getSceneID(), pstrCmd, nCmdLen);
}

/**
* \brief 网关在线
*/
void SessionUser::online_gate()
{
	SessionTask* gate = SessionTaskManager::getMe().getTaskByID(getGateID());
	if(gate) 
	{
		gate->total_online++;
	}
	// 进入角色选择状态
	setState(SessionState::select);
}

/**
* \brief 网关下线
*/
void SessionUser::offline_gate()
{
	SessionTask* gate = SessionTaskManager::getMe().getTaskByID(getGateID());
	if(gate) 
	{
		gate->total_online--;
	}
	// 进入空闲状态
	setState(SessionState::none);
}

/**
* \brief 场景在线
*/
void SessionUser::online_scene()
{
	SessionTask* scene = SessionTaskManager::getMe().getTaskByID(getSceneID());
	if(scene) 
	{
		scene->total_online++;
	}
	// 进入游戏状态
	setState(SessionState::play);
}

/**
* \brief 场景下线
*/
void SessionUser::offline_scene()
{
	SessionTask* scene = SessionTaskManager::getMe().getTaskByID(getSceneID());
	if(scene) 
	{
		scene->total_online--;
	}
	// 进入角色选择状态
	setState(SessionState::select);
}

/**
* \brief 上线
*/
void SessionUser::online()
{
	if (_state_online == OnlineState::state_online)
		return;
	_state_online = OnlineState::state_online;
	// 记录上线线时间
	_time_online = SessionTimeTick::currentTime;

	// 通知场景
	if (isPlayState() || isEnterState())
	{
		noticeToSceneOnlineOffline(1);
	}

	Fir::logger->trace("[玩家上线]，账号ID[%u]", getACCID());
}

/**
* \brief 下线
*/
void SessionUser::offline()
{
	if (_state_online == OnlineState::state_offline)
		return;
	_state_online = OnlineState::state_offline;
	// 记录下线时间
	_time_offline = SessionTimeTick::currentTime;

	// 通知场景
	if (isPlayState() || isEnterState())
	{
		noticeToSceneOnlineOffline(0);
	}

	Fir::logger->trace("[玩家下线]，账号ID[%u]", getACCID());
}

/**
* \brief 通知场景在线\离线
*/
void SessionUser::noticeToSceneOnlineOffline(BYTE online_offline)
{
	CMD::SESSION::t_online_offline_SessionScene send;
	send.charid = getCharID();
	send.accid = getACCID();
	send.online_offline = online_offline;

	sendCmdToScene(&send, sizeof(send));
}