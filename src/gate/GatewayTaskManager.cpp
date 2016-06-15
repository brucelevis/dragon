/**
 * \file
 * \version  $Id: GatewayTaskManager.cpp 24 2013-03-30 08:04:25Z  $
 * \author  ,@163.com
 * \date 2004年12月20日 17时28分53秒 CST
 * \brief 管理子连接的容器
 *
 * 
 */

#include "GatewayTask.h"
#include "GatewayTaskManager.h"
#include "Fir.h"

GatewayTaskManager::GatewayTaskManager()
{
	std::string tokentimestr = Fir::global["tokentime"];
	if (tokentimestr.compare("") == 0)
	{
		m_tokentime = 600;
	}
	else
	{
		m_tokentime = atoi(tokentimestr.c_str());
	}
}

GatewayTaskManager::~GatewayTaskManager()
{
}

bool GatewayTaskManager::uniqueAdd(GatewayTask *task)
{
	zRWLock_scope_wrlock lock(rwlock);

	auto itr = _mapTasks.find(task->getID());
	if( itr != _mapTasks.end())
		return false;

	_mapTasks.insert(std::make_pair(task->getID(),task));	

	return true;
}

bool GatewayTaskManager::uniqueRemove(GatewayTask *task)
{
	zRWLock_scope_wrlock lock(rwlock);

	_mapTasks.erase(task->getID());

	return true;
}

GatewayTask* GatewayTaskManager::getGatewayTask(QWORD seqid)
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);

	GatewayTaskHashmap_iterator it = _mapTasks.find(seqid);
	if (it != _mapTasks.end())
	{
		return it->second;
	}

	return NULL;
}

void GatewayTaskManager::execEvery()
{
	GatewayTask *task = NULL;
	zRWLock_scope_rdlock scope_rdlock(rwlock);

	for(GatewayTaskHashmap_iterator it = _mapTasks.begin(); it != _mapTasks.end(); it++)
	{
		task = it->second;
		if (task)
		{
			task->doCmd();
		}
	}
}

void GatewayTaskManager::unregEveryByClose()
{
	GatewayTask *task = NULL;
	zRWLock_scope_rdlock scope_rdlock(rwlock);

	for(GatewayTaskHashmap_iterator it = _mapTasks.begin(); it != _mapTasks.end(); it++)
	{
		task = it->second;
		if (task)
		{
			task->onUnRegFromSession((BYTE)LOGOUT_CLOSE);
		}
	}
}