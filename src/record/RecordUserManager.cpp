/**
 * \file: RecordUserManager.cpp
 * \version  $Id: RecordUserManager.cpp 64 2013-04-23 02:05:08Z  $
 * \author  , @ztgame.com 
 * \date 2007年01月30日 08时15分25秒 CST
 * \brief 档案用户管理器实现
 *
 * 
 */

#include "RecordServer.h"
#include "RecordUserManager.h"
#include "RecordUser.h"
#include "zDBConnPool.h"
#include "zMetaData.h"
#include <hiredis/hiredis.h>


bool RecordUserM::init()
{
	FieldSet* charbase = RecordService::metaData->getFields("t_charbase");
	if(NULL == charbase)
	{
		Fir::logger->error("找不到t_charbase的FieldSet");
		return false;
	}

	Record col,where;

	col.put("f_charid");
	where.put("f_charid","f_charid>0");

	RecordSetPtr recordset = RecordService::dbConnPool->exeSelect(charbase, &col, &where);

	if(recordset != NULL) 
	{
		for(unsigned int i = 0; i<recordset->size(); i++)
		{   
			Record *rec = recordset->get(i);

			RecordUser* u = new RecordUser();
			u->charid = rec->get("f_charid");

			if (!u->readCharBase())
			{   
				Fir::logger->error("[角色读写],charid=%u,初始化档案失败",u->charid);
				SAFE_DELETE(u);
				return false;
			}

			if (RecordUserM::getMe().add(u))
			{
				//同步角色基础信息到内存缓存数据库
				u->syncBaseMemDB();
			}
			else
			{
				SAFE_DELETE(u);
				Fir::logger->error("[角色读写],charid=%u 添加档案失败，角色已存在", u->charid);
			}

			Fir::logger->info("[预加载测试],charid=%u,初始化档案成功",u->charid);
		}

		Fir::logger->trace("初始化成功，一共读取档案: %u 个角色", recordset->size());
	}

	return true;
}

bool RecordUserM::add(RecordUser* u)
{
	if (!u) return false;

	bool retval = false;

	mlock.lock();

	RecordUserHashmap_iterator it = userMap.find(u->charid);
	if (it==userMap.end())
	{
		userMap[u->charid] = u;
		m_mapNickNames[u->nickname] = u->charid;
		retval = true;
	}

	mlock.unlock();

	return retval;
}

RecordUserM::~RecordUserM()
{	
	for(auto itr=userMap.begin(); itr!=userMap.end(); itr++)
	{
		SAFE_DELETE(itr->second);
	}
	userMap.clear();
	m_mapNickNames.clear();
}

RecordUser* RecordUserM::getUserByCharid(DWORD charid)
{
	RecordUser* ret = NULL;

	mlock.lock();

	RecordUserHashmap_iterator it = userMap.find(charid);
	if (it!=userMap.end())
	{
		ret = it->second;
	}

	mlock.unlock();

	return ret;
}

DWORD RecordUserM::getCharidByNickname(const std::string& nickname)
{
	DWORD charid = 0;

	mlock.lock();
	auto itr = m_mapNickNames.find(nickname);
	if (itr != m_mapNickNames.end())
	{
		charid = itr->second;
	}
	mlock.unlock();

	return charid;
}

bool RecordUserM::existNickname(std::string nickname)
{
	bool ret = false;

	mlock.lock();

	auto itr = m_mapNickNames.find(nickname);
	if (itr!=m_mapNickNames.end())
	{   
		ret = true;
	} 

	mlock.unlock();

	return ret;
}

bool RecordUserM::existCharid(DWORD charid)
{
	bool ret = false;

	mlock.lock();

	RecordUserHashmap_iterator it = userMap.find(charid);
	if (it!=userMap.end())
	{
		ret = true;
	}

	mlock.unlock();

	return ret;
}
