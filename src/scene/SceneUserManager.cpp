/**
 * \file	SceneUser.h
 * \version  	$Id: .cpp 37 2013-04-08 01:52:56Z  $
 * \author  	,
 * \date 	2013年04月07日 15时48分31秒 CST
 * \brief 	场景用户管理器定义
 *
 * 
 */

#include "SceneServer.h"
#include "SceneTaskManager.h"
#include "zMetaData.h"
#include "HMessages.h"
#include "CommonStruct.h"
#include "StringUtil.h"

#include "TimerMgr.h"
#include "zBILog.h"
#include "TopListSystem.h"
#include "GmManager.h"
#include "SceneUserManager.h"
#include "RedisMgr.h"

SceneUserManager::SceneUserManager()
{
}

SceneUserManager::~SceneUserManager()
{
	for(auto itr=m_mapUsers.begin(); itr!=m_mapUsers.end(); itr++)
	{
		SAFE_DELETE(itr->second);
	}
	m_mapUsers.clear();
}

bool SceneUserManager::addUser(SceneUser *user)
{   
	if(NULL == user) 
		return false;
	if(m_mapUsers.find(user->getCharid()) != m_mapUsers.end())
		return false;

	return m_mapUsers.insert(std::make_pair(user->getCharid(), user)).second;
}   


bool SceneUserManager::removeUser(SceneUser *user)
{
	if(NULL == user) 
		return false;
	auto it = m_mapUsers.find(user->getCharid());
	if(it == m_mapUsers.end())
	{
		return false;
	}
	m_mapUsers.erase(it);

	return true;
}

SceneUser* SceneUserManager::getUserByID(DWORD charid)
{
	auto itr=m_mapUsers.find(charid);
	if(itr == m_mapUsers.end())
		return NULL;

	return itr->second;
}

void SceneUserManager::update()
{
	static DWORD saveGroup = 0;
	
	saveGroup = (saveGroup+1)%SceneService::getMe().writeBackGroup;
	
	for(auto itr=m_mapUsers.begin(); itr!=m_mapUsers.end(); itr++)
	{
		SceneUser* u = itr->second;
		if(NULL == u) continue;
		if(u->_charid%SceneService::getMe().writeBackGroup == saveGroup)
			u->save();
	}	
}

void SceneUserManager::saveAll()
{
	Fir::logger->info("%s", __PRETTY_FUNCTION__);
	for(auto itr=m_mapUsers.begin(); itr!=m_mapUsers.end(); itr++)
	{
		SceneUser* u = itr->second;
		if(NULL == u) continue;
		u->save();
	}
}

void SceneUserManager::offlineAll()
{
	Fir::logger->info("%s", __PRETTY_FUNCTION__);
	for(auto itr=m_mapUsers.begin(); itr!=m_mapUsers.end(); itr++)
	{
		SceneUser* u = itr->second;
		if(NULL == u) continue;
		u->offline();
	}
}

void SceneUserManager::execAll(UserFunction func)
{
	for(auto itr=m_mapUsers.begin(); itr!=m_mapUsers.end(); itr++)
	{
		SceneUser* u = itr->second;
		if(NULL == u) continue;
		func(u);
	}
}

void SceneUserManager::delAll()
{
	for (auto it = m_mapUsers.begin(); it != m_mapUsers.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	m_mapUsers.clear();
}

void SceneUserManager::SendMsg2AllOnlinePlayer( Packet& p )
{
	SceneTaskManager::getMe().sendCmdToWorld(0,(const void*)&p[0],p.getPosition());
}

void SceneUserManager::init()
{
	TimerMgr::getMe().AddCircleTimerFromNow(5*60, std::bind(&SceneUserManager::print_online_roles, this));
}

void SceneUserManager::print_online_roles()
{
	typedef std::map<std::string, int>	PlatformCountMap;

	//统计次数的计数器
	static unsigned int countid = 0;
	PlatformCountMap  platformCountMap;

	for (auto iter = m_mapUsers.begin(); iter != m_mapUsers.end(); ++iter)
	{
		SceneUser* p = iter->second;
		if (NULL == p) continue;

		const std::string& rPlatform  = p->getPlatfrom();

		std::map<std::string, int>::iterator it = platformCountMap.find(rPlatform);
		if ( it != platformCountMap.end())
		{
			++ (it->second);
		}
		else
		{
			//找不到这个平台，插入一条新的
			platformCountMap.insert(std::pair<std::string, int>(rPlatform, 1));
		}
	}

	Fir::logger->info("[在线人数]，总人数 = %d", (int)m_mapUsers.size());
	
	//防止越界，随便用一个小一点的数
	countid++;
	if (countid > 1000000)
		countid = 0;
}
