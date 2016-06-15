#include "zDBConnPool.h"

unsigned int defaultHashCode(const void *anyArg)
{
	return 0;
}

zDBConnPool::zDBConnPool(hashCodeFunc hashfunc)
{
	if(hashfunc!=NULL)
		hashCode=hashfunc;
	else
		hashCode=defaultHashCode;
}

zDBConnPool::~zDBConnPool()
{
	mlock.lock();
	if (!handles.empty())
	{
		for(handlesPool::iterator it = handles.begin(); it != handles.end(); ++it)
		{
			MysqlClientHandle *tempHandle=(*it).second;
			SAFE_DELETE(tempHandle);
		}
	}
	handles.clear();
	urls.clear();
	hashCode=defaultHashCode;
	mlock.unlock();
}

bool zDBConnPool::putURL(unsigned int hashcode,const char *url,bool supportTransactions)
{
	UrlInfo ui(hashcode,url,supportTransactions);
	MysqlClientHandle *handle= FIR_NEW MysqlClientHandle(ui);
	if (handle==NULL)
		return false;
	if(handle->initHandle())
	{
		mlock.lock();
		handles.insert(handlesPool::value_type(hashcode, handle));
		urls.insert(urlsPool::value_type(hashcode, ui));
		idmaps.insert(handlesIDMap::value_type(handle->getID(), handle));
		mlock.unlock();
		return true;
	}
	else
	{
		SAFE_DELETE(handle);
		return false;
	}
}

connHandleID zDBConnPool::getHandle(const void *hashData)
{
	unsigned int hashcode=0;
	if(hashData!=NULL)
		hashcode=hashCode(hashData);
	MysqlClientHandle* handle = getHandleByHashcode(hashcode);
	if(handle!=NULL)
		return handle->getID();
	else
		return (connHandleID)-1;
}

void zDBConnPool::putHandle(const connHandleID handleID)
{
	MysqlClientHandle* handle = getHandleByID(handleID);
	if(handle!=NULL)
	{
		handle->unsetHandle();
	}
}

int zDBConnPool::execSql(const char *sql,unsigned int sqllen)
{
	FunctionTimesV2 times(__FUNCTION__, (unsigned long) __builtin_return_address(0));

	int nResult = -1;

	connHandleID handleID = getHandle();
	if ((connHandleID)-1 == handleID)
		return nResult;

	MysqlClientHandle* handle = getHandleByID(handleID);
	if(handle!=NULL)
	{
		nResult =  handle->execSql(sql,sqllen);
	}
	
	putHandle(handleID);

	return nResult;
}

RecordSetPtr zDBConnPool::exeSelect(const char* tableName,Record* column, Record* where,Record* order)
{
	FunctionTimesV2 times(__FUNCTION__, (unsigned long) __builtin_return_address(0));

	RecordSetPtr pResult;

	connHandleID handleID = getHandle();
	if ((connHandleID)-1 == handleID)
		return pResult;

	MysqlClientHandle* handle = getHandleByID(handleID);
	if(handle!=NULL)
	{
		pResult = handle->exeSelect(tableName,column,where,order);
	}
	
	putHandle(handleID);

	return pResult;
}

RecordSetPtr zDBConnPool::exeSelect(const std::string& strsql)
{
	FunctionTimesV2 times(__FUNCTION__, (unsigned long) __builtin_return_address(0));

	RecordSetPtr pResult;

	connHandleID handleID = getHandle();
	if ((connHandleID)-1 == handleID)
		return pResult;

	MysqlClientHandle* handle = getHandleByID(handleID);
	if(handle!=NULL)
	{
		pResult =  handle->exeSelect(strsql);
	}
	putHandle(handleID);
	return pResult;
}

RecordSetPtr zDBConnPool::exeSelect(FieldSet* table, Record* column,Record* where,  Record* order, unsigned int limit, Record* groupby, Record* having)
{
	FunctionTimesV2 times(__FUNCTION__, (unsigned long) __builtin_return_address(0));

	RecordSetPtr pResult;

	connHandleID handleID = getHandle();
	if ((connHandleID)-1 == handleID)
		return pResult;

	MysqlClientHandle* handle = getHandleByID(handleID);

	if (handle!=NULL)
	{
		pResult = handle->exeSelect(table, column, where, order, limit, groupby, having);
	}
	putHandle(handleID);

	return pResult;
}

unsigned int zDBConnPool::exeInsert(const std::string& strsql)
{
	FunctionTimesV2 times(__FUNCTION__, (unsigned long) __builtin_return_address(0));

	int nResult = -1;

	connHandleID handleID = getHandle();
	if ((connHandleID)-1 == handleID)
		return nResult;

	MysqlClientHandle* handle = getHandleByID(handleID);

	if (handle!=NULL)
	{
		nResult =  handle->exeInsert(strsql);
	}
	
	putHandle(handleID);
	return nResult;
}

unsigned int zDBConnPool::exeUpdate(const std::string& strsql)
{
	FunctionTimesV2 times(__FUNCTION__, (unsigned long) __builtin_return_address(0));

	int nResult = -1;

	connHandleID handleID = getHandle();
	if ((connHandleID)-1 == handleID)
		return nResult;

	MysqlClientHandle* handle = getHandleByID(handleID);
	if(handle!=NULL)
	{
		nResult = handle->exeUpdate(strsql);
	}

	putHandle(handleID);
	return nResult;
}

char * zDBConnPool::escapeString(const char *src,char *dest,unsigned int size)
{
	char* result = dest;

	connHandleID handleID = getHandle();
	if ((connHandleID)-1 == handleID)
		return result;

	MysqlClientHandle* handle = getHandleByID(handleID);
	if(handle!=NULL)
	{
		result = handle->escapeString(src,dest,size);
	}
	
	putHandle(handleID);

	return result;
}

std::string& zDBConnPool::escapeString(const std::string &src,std::string &dest)
{
	std::string& result = dest;

	connHandleID handleID = getHandle();
	if ((connHandleID)-1 == handleID)
		return result;

	MysqlClientHandle* handle = getHandleByID(handleID);
	if(handle!=NULL)
	{
		result = handle->escapeString(src,dest);
	}
	
	putHandle(handleID);
	return result;
}

MysqlClientHandle* zDBConnPool::getHandleByHashcode(unsigned int hashcode)
{
	while(true)
	{
		MysqlClientHandle* invalidHandle=NULL;
		mlock.lock();
		std::pair<handlesPool::iterator,handlesPool::iterator> hps = handles.equal_range(hashcode);
		for(handlesPool::iterator it = hps.first; it != hps.second; ++it)
		{
			MysqlClientHandle* tempHandle=(*it).second;
			switch(tempHandle->state)
			{
			case MYSQLCLIENT_HANDLE_INVALID:
				//handle无效，如果没有找到可用的，需要初始化为可用
				if(invalidHandle==NULL)
					invalidHandle=tempHandle;
				break;
			case MYSQLCLIENT_HANDLE_VALID:
				//handle可用
				if(tempHandle->setHandle())
				{
					mlock.unlock();
					return tempHandle;
				}
				break;
			case MYSQLCLIENT_HANDLE_USED:
				//handle正在使用中
				if(tempHandle->useTime.elapse()>10)
				{
					//使用时间过长，是否程序存在问题
					logger->warnM("The handle(%u) timeout %lus by thread %lu",
						tempHandle->getID(),tempHandle->useTime.elapse(),tempHandle->getedThread);
					logger->warnM("The handle sql is : %s" , tempHandle->my_sql.c_str());
				}
				break;
			}
		}
		if(urls.find(hashcode)==urls.end() || urls[hashcode].url.size()==0)
		{
			mlock.unlock();
			return NULL;
		}
		if(invalidHandle!=NULL)
		{
			if(invalidHandle->initHandle())
			{
				if(invalidHandle->setHandle())
				{
					mlock.unlock();
					return invalidHandle;
				}
			}
		}
		else if(handles.count(hashcode) < maxHandleBuf)
		{
			MysqlClientHandle *handle= FIR_NEW MysqlClientHandle(urls[hashcode]);
			if (NULL==handle)
			{
				mlock.unlock();
				logger->fatal("not enough memory to allocate handle");
				return handle;
			}
			if(handle->initHandle())
			{
				handles.insert(handlesPool::value_type(hashcode, handle));
				idmaps.insert(handlesIDMap::value_type(handle->getID(), handle));

				if(handle->setHandle())
				{
					mlock.unlock();
					return handle;
				}
			}
		}
		mlock.unlock();

		logger->debug("usleep(10000) with getHandleByHashcode");
		usleep(10000);
	}
}

MysqlClientHandle* zDBConnPool::getHandleByID(connHandleID handleID)
{
	mlock.lock();
	if (!idmaps.empty())
	{
		handlesIDMap::iterator it = idmaps.find(handleID);
		if (it != idmaps.end())
		{
			mlock.unlock();
			return (*it).second;
		}
	}
	/*
	for(handlesPool::iterator it = handles.begin(); it != handles.end(); ++it)
	{
	MysqlClientHandle *tempHandle=(*it).second;
	if(tempHandle->getID()==handleID)
	{
	mlock.unlock();
	return tempHandle;
	}
	}*/
	mlock.unlock();
	return NULL;
}

/* ************************* *
* zDBConnPool类静态函数实现 *
* ************************* */
zDBConnPool *zDBConnPool::newInstance(hashCodeFunc hashfunc)
{
	return FIR_NEW zDBConnPool(hashfunc);
}

void zDBConnPool::delInstance(zDBConnPool **delThisClass)
{
	if(*delThisClass==NULL) return;
	SAFE_DELETE(*delThisClass);
}
