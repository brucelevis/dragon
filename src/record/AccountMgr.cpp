#include "RecordServer.h"
#include "AccountMgr.h"
#include <hiredis/hiredis.h>
#include "zDBConnPool.h"
#include "zMetaData.h"

bool AccountMgr::init()
{
	if(!initAccounts())
		return false;

	return true;
}

bool AccountMgr::initAccounts()
{
	FieldSet* fields = RecordService::metaData->getFields("t_account");
	if(NULL == fields)
	{
		Fir::logger->error("找不到t_account的FieldSet");
		return false;
	}

	Record col,where;

	col.put("f_accountid");
	col.put("f_playerid1");
	col.put("f_playerid2");
	col.put("f_playerid3");
	col.put("f_playerid4");

	RecordSetPtr recordset = RecordService::dbConnPool->exeSelect(fields, &col, NULL);
	
	if(recordset != NULL) 
	{
		for(unsigned int i = 0; i<recordset->size(); i++)
		{   
			Record *rec = recordset->get(i);

			AccountItem item;
			item.accountid = rec->get("f_accountid");
			item.playerid1 = rec->get("f_playerid1");
			item.playerid2 = rec->get("f_playerid2");
			item.playerid3 = rec->get("f_playerid3");
			item.playerid4 = rec->get("f_playerid4");
			syncMemDbAccountItem(item);
		}

		Fir::logger->trace("初始化成功，一共读取账号: %u 个账号", recordset->size());
	}

	return true;
}


bool AccountMgr::syncMemDbAccountRoles(DWORD accountid)
{
	AccountItem item;
	if (!AccountMgr::getMemDbAccountItem(accountid, item))
	{   
		Fir::logger->error("[同步角色列表]失败，获取账号数据失败，账号ID = %u", accountid);
		return false;
	} 

	FieldSet* fields = RecordService::metaData->getFields("t_account");
	if(NULL == fields)
	{
		Fir::logger->error("[同步角色列表]失败，找不到t_account的FieldSet");
		return false;
	}

	Record col,where;

	col.put("f_accountid");
	col.put("f_playerid1");
	col.put("f_playerid2");
	col.put("f_playerid3");
	col.put("f_playerid4");

	std::ostringstream oss;
	oss << "f_accountid=" << accountid;
	where.put("f_accountid",oss.str());

	RecordSetPtr recordset = RecordService::dbConnPool->exeSelect(fields, &col, &where);

	if (NULL == recordset || recordset->empty())
	{ 
		Fir::logger->error("[同步角色列表]失败，读取角色列表失败");
		return false;
	}

	Record *rec = recordset->get(0);

	item.playerid1 = rec->get("f_playerid1");
	item.playerid2 = rec->get("f_playerid2");
	item.playerid3 = rec->get("f_playerid3");
	item.playerid4 = rec->get("f_playerid4");

	return syncMemDbAccountItem(item);
}

bool AccountMgr::getMemDbAccountItem(DWORD accid, AccountItem& item)
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle)
	{
		Fir::logger->error("[获取账号数据]，找不到内存数据库，账号ID = %u,",accid);
		return false;
	}

	if (handle->getBin("account_item",accid,"account_item",(char*)&item) == 0)
	{
		item.accountid = accid;
	}

	return true;
}

bool AccountMgr::syncMemDbAccountItem(const AccountItem& item)
{
    zMemDB* redishandle = zMemDBPool::getMe().getMemDBHandle();
    if (redishandle==NULL)
    {   
		Fir::logger->error("[同步账号数据],获取内存数据库失败，accountid=%u",item.accountid);
		return false;
    }   

    if (!redishandle->setBin("account_item", item.accountid, "account_item", (const char*)&item, sizeof(item)))
    {   
		Fir::logger->error("[同步账号数据],同步内存数据库account_item失败，accountid=%u",item.accountid);
		return false;
    }   

    return true;
}

