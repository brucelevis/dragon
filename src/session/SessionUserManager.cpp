#include "SessionUserManager.h"
#include "SessionUser.h"

#define OFFLINE_TIMEOUT	(5 * 60 * 1000)		// 离线超时时间


/**
* \brief 析构函数
*
*/
SessionUserManager::~SessionUserManager()
{
	zRWLock_scope_wrlock lock(rwlock);

	SessionUserHashmap_iterator itr = _mapAccUsers.begin();
	for ( ; itr != _mapAccUsers.end(); ++itr)
	{
		SAFE_DELETE(itr->second);
	}

	_mapAccUsers.clear();
	_mapCharUsers.clear();
	_mapUsersToUnreg.clear();
}

/**
* \brief 更新
*
*/
void SessionUserManager::update(const zRTime& current)
{
	unregAllUserToUnregTimeout(current);
}

/**
* \brief 通过账号获取玩家
*
*/
SessionUser* SessionUserManager::getUserByACCID(DWORD accid)
{
	SessionUser *user = NULL;
	zRWLock_scope_rdlock scope_rdlock(rwlock);

	SessionUserHashmap_const_iterator it = _mapAccUsers.find(accid);
	if (it != _mapAccUsers.end())
	{
		user = it->second;
	}

	return user;
}

/**
* \brief 通过角色获取玩家
*
*/
SessionUser* SessionUserManager::getUserByCharID(DWORD charid)
{
	SessionUser *user = NULL;
	zRWLock_scope_rdlock scope_rdlock(rwlock);

	SessionUserHashmap_const_iterator it = _mapCharUsers.find(charid);
	if (it != _mapCharUsers.end())
	{
		user = it->second;
	}

	return user;
}

/**
* \brief 添加网关玩家
*
*/
bool SessionUserManager::addGateUser(SessionUser* user)
{
	zRWLock_scope_wrlock lock(rwlock);

	SessionUserHashmap_iterator itr = _mapAccUsers.find(user->getACCID());
	if( itr != _mapAccUsers.end())
		return false;

	_mapAccUsers.insert(std::make_pair(user->getACCID(), user));	
	return true;
}
/**
* \brief 删除网关玩家
*
*/
bool SessionUserManager::removeGateUser(SessionUser* user)
{
	zRWLock_scope_wrlock lock(rwlock);

	SessionUserHashmap_iterator itr = _mapAccUsers.find(user->getACCID());
	if( itr == _mapAccUsers.end())
		return false;

	_mapAccUsers.erase(itr);
	return true;
}

/**
* \brief 添加场景玩家
*
*/
bool SessionUserManager::addSceneUser(SessionUser* user)
{
	zRWLock_scope_wrlock lock(rwlock);

	SessionUserHashmap_iterator itr = _mapCharUsers.find(user->getCharID());
	if( itr != _mapCharUsers.end())
		return false;

	_mapCharUsers.insert(std::make_pair(user->getCharID(), user));	
	return true;
}

/**
* \brief 删除场景玩家
*
*/
bool SessionUserManager::removeSceneUser(SessionUser* user)
{
	zRWLock_scope_wrlock lock(rwlock);

	SessionUserHashmap_iterator itr = _mapCharUsers.find(user->getCharID());
	if( itr == _mapCharUsers.end())
		return false;

	_mapCharUsers.erase(itr);
	return true;
}

/**
* \brief 添加待删除玩家
*
*/
bool SessionUserManager::addUserToUnreg(SessionUser* user)
{
	zRWLock_scope_wrlock lock(rwlock);

	SessionUserHashmap_iterator itr = _mapUsersToUnreg.find(user->getACCID());
	if( itr != _mapUsersToUnreg.end())
		return false;

	_mapUsersToUnreg.insert(std::make_pair(user->getACCID(), user));

	Fir::logger->debug("添加待删除玩家，账号ID = %u", user->getACCID());
	return true;
}
/**
* \brief 删除待删除玩家
*
*/
bool SessionUserManager::removeUserToUnreg(SessionUser* user)
{
	zRWLock_scope_wrlock lock(rwlock);

	SessionUserHashmap_iterator itr = _mapUsersToUnreg.find(user->getACCID());
	if( itr == _mapUsersToUnreg.end())
		return false;

	_mapUsersToUnreg.erase(itr);

	Fir::logger->debug("删除待删除玩家，账号ID = %u", user->getACCID());
	return true;
}

/**
* \brief 注销所有待注销超时玩家
*
*/
void SessionUserManager::unregAllUserToUnregTimeout(const zRTime& current)
{
	SessionUser *u = NULL;
	std::vector<SessionUser* > vecUserToUnreg;

	rwlock.rdlock();
	SessionUserHashmap_iterator itr = _mapUsersToUnreg.begin();
	for ( ; itr != _mapUsersToUnreg.end(); )
	{
		u = itr->second;
		if (!u)
		{
			Fir::logger->warn("%s,出现空指针玩家", __PRETTY_FUNCTION__);
			
			itr = _mapUsersToUnreg.erase(itr);
			continue;
		}
		else if (u->getOfflineTime().elapse(current) > OFFLINE_TIMEOUT)
		{
			vecUserToUnreg.push_back(u);
		}
		++ itr;
	}
	rwlock.unlock();

	for (auto itD = vecUserToUnreg.begin(); itD != vecUserToUnreg.end(); ++itD)
	{
		unregUser(*itD, SessionUser::UnregMethod::unreg_passive_gate);
	}
}

/**
* \brief 注册网关玩家
*
*/
bool SessionUserManager::regGateUser(SessionTask* task, const CMD::SESSION::t_regUser_GateSession* cmd)
{
	Fir::logger->trace("[注册网关玩家]，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);

	SessionUser* u = FIR_NEW SessionUser(task->getID(), cmd->seqid, cmd->accid);
	if (NULL == u)
	{
		Fir::logger->error("%s,分配SessionUser内存错误,accid=%u", __PRETTY_FUNCTION__, cmd->accid);
		return false;
	}

	if (addGateUser(u))
	{
		u->regToGate(cmd->regtype);
		u->online_gate();
		u->online();

		return true;
	}
	else
	{
		SAFE_DELETE(u);
		Fir::logger->trace("[注册网关玩家]失败，网关ID = %u，网关序号 = %llu，账号ID = %u", task->getID(), cmd->seqid, cmd->accid);

		return false;
	}
}

/**
* \brief 注销玩家
*
*/ 
bool SessionUserManager::unregUser(SessionUser* u, const SessionUser::UnregMethod method/* = SessionUser::unreg_no*/, const BYTE unregtype/* = 0*/)
{
	Fir::logger->trace("[注销玩家]，账号ID = %u",u->getACCID());

	// 从待注销队列删除
	removeUserToUnreg(u);

	// 注销场景玩家
	unregSceneUser(u, method);
	// 注销网关玩家
	unregGateUser(u, method, unregtype);

	SAFE_DELETE(u);

	return true;
}

/**
* \brief 等待注销玩家
*
*/ 
bool SessionUserManager::unregUserWait(SessionUser* u)
{
	if (!u->isOnline())
	{
		Fir::logger->warn("[等待账号注销]失败，账号未在线，账号ID = %u", u->getACCID());
		return false;
	}
	if (!addUserToUnreg(u))
	{
		Fir::logger->error("[等待账号注销]失败，加入容器失败，账号ID = %u", u->getACCID());
		return false;
	}
	u->offline();

	return true;
}

/**
* \brief 注销网关玩家
*
*/
bool SessionUserManager::unregGateUser(SessionUser* u, const SessionUser::UnregMethod method, const BYTE unregtype)
{
	if (u->getACCID() > 0 && removeGateUser(u))
	{
		u->unregFromGate(method, unregtype);
	}
	else
	{
		Fir::logger->error("[注销网关玩家]失败，退出容器失败，账号ID = %u", u->getACCID());
		return false;
	}

	Fir::logger->trace("[注销网关玩家]，账号ID = %u", u->getACCID());
	return true;
}

/**
* \brief 注销场景玩家
*
*/
bool SessionUserManager::unregSceneUser(SessionUser* u, const SessionUser::UnregMethod method)
{
	if (!u->isPlayState() && !u->isEnterState())
		return false;

	if (u->isPlayState() && !removeSceneUser(u))
	{
		Fir::logger->warn("[注销场景玩家]失败，退出容器失败，账号ID = %u，角色ID = %u", u->getACCID(), u->getCharID());
		return false;
	}
	u->unregFromScene(method);

	Fir::logger->trace("[注销场景玩家]，账号ID = %u，角色ID = %u", u->getACCID(), u->getCharID());
	return true;
}

/**
* \brief 重连网关玩家
*
*/
bool SessionUserManager::reconnGateUser(SessionUser* u, const CMD::SESSION::t_regUser_GateSession* cmd)
{
	Fir::logger->trace("[重连网关玩家]，网关ID = %u，网关序号 = %llu，账号ID = %u", u->getGateID(), u->getSeqID(), u->getACCID());

	// 从待注销队列删除
	removeUserToUnreg(u);

	u->setSeqID(cmd->seqid);
	u->reconn();
	u->online();

	return true;
}


/**
* \brief 注册场景玩家
*
*/
bool SessionUserManager::regSceneUser(SessionUser* u, SessionTask* scene)
{
	if (addSceneUser(u))
	{
		u->online_scene();

		return true;
	}
	else
	{
		Fir::logger->error("[注册场景玩家]失败，场景ID = %d，账号ID = %u，角色ID = %u", u->getSceneID(), u->getACCID(), u->getCharID());
		return false;
	}
}

/**
* \brief 注册GM玩家
*
*/
SessionUser* SessionUserManager::regGMUser(DWORD accid, DWORD charid, SessionTask* scene)
{
	SessionUser* u = FIR_NEW SessionUser(0, 0, accid);
	if (NULL == u)
	{
		Fir::logger->error("%s,分配SessionUser内存错误,accid=%u", __PRETTY_FUNCTION__, accid);
		return NULL;
	}

	u->regToScene(scene, charid, 1);

	return u;
}

/**
* \brief 注销GM玩家
*
*/
void SessionUserManager::unregGMUser(SessionUser* user)
{
	if (user)
	{
		user->unregFromScene(SessionUser::UnregMethod::unreg_active, 1);
	}

	SAFE_DELETE(user);
}

/**
* \brief 获取登录账号数量
*
*/
DWORD SessionUserManager::getUserNum()
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);

	return _mapAccUsers.size();
}

/**
* \brief 获取登录角色数量
*
*/
DWORD SessionUserManager::getCharNum()
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);

	return _mapCharUsers.size();
}

/**
* \brief 获取离线数量
*
*/
DWORD SessionUserManager::getOfflineNum()
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);

	return _mapUsersToUnreg.size();
}

/**
* \brief 删除场景玩家
*
*/
void SessionUserManager::removeUserBySceneID(DWORD sceneid)
{
	SessionUser* u = NULL;
	std::vector<SessionUser* > vecUserToRemove;

	rwlock.rdlock();
	SessionUserHashmap_iterator itr = _mapCharUsers.begin();
	for ( ; itr != _mapCharUsers.end(); ++itr)
	{
		u = itr->second;

		if (u && u->getSceneID() == sceneid)
		{
			vecUserToRemove.push_back(u);
		}
	}
	rwlock.unlock();

	Fir::logger->trace("[删除场景玩家]，场景ID = %d", sceneid);
	for (auto itD = vecUserToRemove.begin(); itD != vecUserToRemove.end(); ++itD)
	{
		unregUser(*itD, SessionUser::UnregMethod::unreg_passive_scene, (BYTE)LOGOUT_CLOSE);
	}
}

/**
* \brief 删除网关玩家
*
*/
void SessionUserManager::removeUserByGateID(DWORD gateid)
{
	SessionUser* u = NULL;
	std::vector<SessionUser* > vecUserToRemove;

	rwlock.rdlock();
	SessionUserHashmap_iterator itr = _mapAccUsers.begin();
	for ( ; itr != _mapAccUsers.end(); ++itr)
	{
		u = itr->second;

		if (u && u->getGateID() == gateid)
		{
			vecUserToRemove.push_back(u);
		}
	}
	rwlock.unlock();

	Fir::logger->trace("[删除网关玩家]，网关ID = %d", gateid);
	for (auto itD = vecUserToRemove.begin(); itD != vecUserToRemove.end(); ++itD)
	{
		unregUser(*itD, SessionUser::UnregMethod::unreg_passive_gate);
	}
}
