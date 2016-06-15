/**
 * \file
 * \version  $Id: GateUserManager.cpp 67 2013-04-23 09:44:20Z  $
 * \author  ,@163.com
 * \date 2005年04月01日 11时56分57秒 CST
 * \brief 实现网关用户管理类
 */

#include "GateUserManager.h"
#include "GatewayServer.h"
#include "SceneClient.h"
#include "GatewayTask.h"

GateUserManager::GateUserManager()
{
}

GateUserManager::~GateUserManager()
{
}

GateUser * GateUserManager::getUserCharid( DWORD charid)
{
	GateUser *ret = NULL;

	charid_lock.rdlock();
	auto itrFind = m_mapUsers.find(charid);
	if(itrFind != m_mapUsers.end())
	{
		ret = itrFind->second;
	}
	charid_lock.unlock();

	return ret;
}

DWORD GateUserManager::getUserCount()
{
	plat_account_rwlock.rdlock();
	DWORD count = account_map.size();
	plat_account_rwlock.unlock();

	return count;
}

DWORD GateUserManager::getRoleCount()
{
	charid_lock.rdlock();
	DWORD count = m_mapUsers.size();
	charid_lock.unlock();

	return count;
}

bool GateUserManager::addUserCharid(GateUser *user)
{
	bool ret = false;

	charid_lock.wrlock();

	if (m_mapUsers.find(user->getCharID()) == m_mapUsers.end())
	{
		ret = m_mapUsers.insert(std::make_pair(user->getCharID(), user)).second;
	}
	else
	{
		ret = false;
	}

	charid_lock.unlock();

	return ret;
}

//bool GateUserManager::addUserCharidForce(GateUser *user)
//{
//	bool ret = true;
//	GateUser* tmp=NULL;
//	charid_lock.wrlock();
//
//	tmp = getUserAccount(user->_accountid);
//	if(tmp)
//	{
//		//ret = m_mapUsers.insert(std::make_pair(user->getcharid(),user)).second;
//		m_mapUsers[user->getcharid()] = user;
//	}
//
//	charid_lock.unlock();
//	if(tmp==NULL)
//		Fir::logger->error("[异常]:%s,%u",__PRETTY_FUNCTION__,((GateUser*)user)->getcharid());
//
//	return ret;
//}


void GateUserManager::removeUserCharid(DWORD charid)
{
	charid_lock.wrlock();
	m_mapUsers.erase(charid);
	charid_lock.unlock();
}

GateUser* GateUserManager::getUserAccount(DWORD accountid)
{
	GateUser* ret = NULL;
	plat_account_rwlock.rdlock();
	auto it = account_map.find(accountid);
	if (it != account_map.end())
		ret = (GateUser*)it->second;
	plat_account_rwlock.unlock();
	return ret;
}

bool GateUserManager::addUserAccount(GateUser* user)
{
	if (!user)
		return false;
	bool ret = false;
	plat_account_rwlock.wrlock();
	if (account_map.find(user->getACCID()) == account_map.end())
	{
		ret = account_map.insert(std::make_pair(user->getACCID(),user)).second;
	}
	plat_account_rwlock.unlock();
	return ret;
}

//bool GateUserManager::addUserAccountForce(GateUser* user)
//{
//	if (!user)
//		return false;
//	plat_account_rwlock.wrlock();
//	account_map[user->_accountid] = user;
//	plat_account_rwlock.unlock();
//	return true;
//}

void GateUserManager::removeUserAccount(DWORD accountid)
{
	plat_account_rwlock.wrlock();
	account_map.erase(accountid);
	plat_account_rwlock.unlock();
}

void GateUserManager::sendCmdToWorld(DWORD except_charid, const void *pstrCmd, const unsigned int nCmdLen)
{                   
	charid_lock.rdlock();                       
	for(auto iter = m_mapUsers.begin();iter != m_mapUsers.end() ; ++iter)
	{   
		GateUser* u = iter->second;
		if (NULL == u)
		{
			continue;
		}

		if (u->getCharID() == except_charid)
		{
			continue;
		}

		if (u->isTerminateWait() || u->isTerminate())
		{
			continue;
		}

		u->sendCmd(pstrCmd,nCmdLen);
	} 
	charid_lock.unlock();                       
}       


void GateUserManager::execAll(UserFunction func)
{   
	charid_lock.rdlock();
	std::map<DWORD, GateUser*> temp = m_mapUsers;
	charid_lock.unlock();

	for(auto itr = temp.begin();itr != temp.end(); ++itr)
	{   
		func(itr->second);
	}
}                                   
