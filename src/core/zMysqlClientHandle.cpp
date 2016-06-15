/**
 * \file
 * \version  $Id: zMysqlDBConnPool.cpp 15 2013-03-22 10:31:07Z  $
 * \author  ,@163.com
 * \date 2005年01月27日 14时55分32秒 CST
 * \brief 实现Mysql连接池
 *
 * 
 */

#include "zMysqlClientHandle.h"
#include <mysql.h>

connHandleID MysqlClientHandle::HandleID_generator = 0;

/* ********************************* *
 * MysqlClientHandle类函数实现       *
 * ********************************* */
MysqlClientHandle::MysqlClientHandle(const UrlInfo &url)
	: id(++HandleID_generator), url(url), lifeTime(), useTime()
{
	state = MYSQLCLIENT_HANDLE_INVALID;
	getedCount = 0;
	getedThread = 0;
	mysql = NULL;
}

MysqlClientHandle::~MysqlClientHandle()
{
	finalHandle();
}
bool MysqlClientHandle::initMysql()
{
	if (mysql)
	{
		logger->info("initMysql():The mysql connect will been closed...");
		mysql_close(mysql);
		mysql=NULL;
	}
	mysql=mysql_init(NULL);
	if(mysql==NULL)
	{
		logger->error("initMysql():Initiate mysql error...");
		return false;
	}

	if(mysql_real_connect(mysql,url.host,url.user,url.passwd,url.dbName,url.port,NULL,CLIENT_COMPRESS|CLIENT_MULTI_STATEMENTS|CLIENT_INTERACTIVE)==NULL)
	{
		logger->error("initMysql():connect mysql://%s:%u/%s failed...",url.host,url.port,url.dbName);
		//logger->error("reason: %s",mysql_error(mysql));
		return false;
	}
	if (!setTransactions(url.supportTransactions))
	{
		return false;
	}
	mysql_query(mysql,"set names 'utf8'");
	logger->info("initMysql():connect mysql://%s:%u/%s successful...",url.host,url.port,url.dbName);
	state=MYSQLCLIENT_HANDLE_VALID;
	lifeTime.now();
	getedCount=0;
	return true;
}

bool MysqlClientHandle::initHandle()
{
	FunctionTime func_time(0,__PRETTY_FUNCTION__,"连接数据库" , 32);
	if(!initMysql())
	{
		finalHandle();
		return false;
	}
	return true;
}

void MysqlClientHandle::finalHandle()
{
	if (mysql)
	{
		logger->info("finalHandle():The mysql connect will been closed...");
		mysql_close(mysql);
		mysql=NULL;
	}
	state=MYSQLCLIENT_HANDLE_INVALID;
	getedCount=0;
	getedThread=(pthread_t)0;
	my_sql="";
}

bool MysqlClientHandle::setHandle()
{
	//无效连接,句柄被使用超过1800次或生成超过半小时,重连
	if(getedCount>18000 || lifeTime.elapse()>1800 || mysql_ping(mysql)!=0)
	{
		if(!initMysql())
		{
			finalHandle();
			return false;
		}
	}
	state=MYSQLCLIENT_HANDLE_USED;
	getedCount++;
	useTime.now();
	getedThread=pthread_self();
	return true;
}

void MysqlClientHandle::unsetHandle()
{
	state=MYSQLCLIENT_HANDLE_VALID;
	useTime.now();
	getedThread=0;
}

bool MysqlClientHandle::setTransactions(bool supportTransactions)
{
	if(supportTransactions)
		return (0 == execSql("SET AUTOCOMMIT=0",strlen("SET AUTOCOMMIT=0")));
	else
		return (0 == execSql("SET AUTOCOMMIT=1",strlen("SET AUTOCOMMIT=1")));
}

int MysqlClientHandle::execSql(const char *sql,unsigned int sqllen)
{
	FunctionTime func_time(100000,__PRETTY_FUNCTION__,sql , 32); 
	if(sql==NULL || sqllen==0 || mysql==NULL)
	{
		logger->error("invalid mysql handle or sql statement.");
		return -1;
	}
#ifdef _DEBUG_WCX
	Fir::logger->debug("execSql: %s", sql);
#endif
#ifdef _ZJW_DEBUG
	Fir::logger->debug("execSql: %s", sql);
	my_sql=sql;
#endif
	my_sql=sql;

#ifdef _OP_TOOL_VERSION
	                    //运维属性察看工具
	std::string tempOfstr(sql);
						//取消对数据库的一切修改行为
	for(unsigned int i=0;i<tempOfstr.length();i++)
		tempOfstr[i]=toupper(tempOfstr[i]);
	if(tempOfstr.find("INSERT")!=std::string::npos)return 1;
	if(tempOfstr.find("UPDATE")!=std::string::npos)return 1;
	if(tempOfstr.find("DELETE")!=std::string::npos)return 1;
#endif
	int ret=mysql_real_query(mysql,sql,sqllen);

	if(ret)
	{
		logger->error("%s", mysql_error(mysql));
		logger->error("%s", sql);
	}
	return ret;
}

RecordSetPtr MysqlClientHandle::exeSelect(const std::string& strsql)
{
	RecordSetPtr pResult(new RecordSet);

	if(0 != execSql(strsql.c_str(), strsql.size()))
	{
		logger->error("%s", mysql_error(mysql));
		return pResult;
	}

	MYSQL_RES *result = mysql_store_result(mysql);
	if(result==NULL)
	{
		logger->error("%s", mysql_error(mysql));
		return pResult;
	}
	unsigned int retCount = mysql_num_rows(result);
	if(retCount==0)
	{
		mysql_free_result(result);
		return pResult;
	}

	//ret_set = FIR_NEW RecordSet();
	MYSQL_ROW row;

	unsigned int num_field = mysql_num_fields(result);
	MYSQL_FIELD* mysql_fields = mysql_fetch_fields(result);

	while ((row = mysql_fetch_row(result)))
	{
		Record* rec = FIR_NEW Record();
		unsigned long *lengths= mysql_fetch_lengths(result);
		if (rec)
		{
			for (unsigned int i=0; i<num_field; i++)
			{
				if (row[i] != NULL)
				{
					if (FIELD_TYPE_BLOB==mysql_fields[i].type)
						rec->put(mysql_fields[i].name, row[i],lengths[i]);
					else if (FIELD_TYPE_MEDIUM_BLOB==mysql_fields[i].type)
						rec->put(mysql_fields[i].name, row[i],lengths[i]);
					else
						rec->put(mysql_fields[i].name, row[i]);
				}
			}
		}
		else
		{
			continue;
		}
		pResult->put(rec);
	}

	mysql_free_result(result);
	return pResult;
}

RecordSetPtr MysqlClientHandle::exeSelect(const char* tableName,Record* column, Record* where, Record* order)
{
	using namespace std;

	RecordSetPtr pRecord;

	bool first=true;
	RecordSet* ret_set = NULL;
	ostringstream query_string;
	if(tableName==NULL || mysql==NULL)
	{
		logger->error("null pointer error. ---- %s",__PRETTY_FUNCTION__);
		return pRecord;
	}
	MYSQL_RES* field_res = NULL;
	field_res = mysql_list_fields(mysql, tableName, NULL);
	if (field_res == NULL)
	{
		logger->error("null pointer error. ---- %s",__PRETTY_FUNCTION__);
		return pRecord;
	}

	unsigned int num_field = mysql_num_fields(field_res);
	MYSQL_FIELD* mysql_fields = NULL;
	mysql_fields = mysql_fetch_fields(field_res);
	mysql_free_result(field_res);

	query_string << "SELECT ";

	if (column)
	{
		const char* fieldvalue = NULL;
		if (column->size() > 0)
		{

			if (column->find("*"))
			{
				first=false;

				if ((fieldvalue = (*column)["*"]) == NULL
						|| strlen((*column)["*"])==0)
				{
					query_string << "*";
				}
				else
				{
					query_string << fieldvalue;
				}

			}

			for (unsigned int i=0; i<num_field; i++)
			{
				if (!column->find(mysql_fields[i].name))
				{
					continue;
				}

				if (first)
					first=false;
				else
				{
					query_string <<  " , ";
				}

				if ((fieldvalue = (*column)[mysql_fields[i].name]) == NULL
						|| strlen((*column)[mysql_fields[i].name])==0)
				{
					query_string << mysql_fields[i].name;
				}
				else
				{
					query_string << fieldvalue;
				}
			}
		}
		else
		{
			query_string << " * ";
		}
	}
	else
	{
		query_string << " * ";
	}
	query_string << "  FROM " << " `" << tableName << "` ";

	first = true;

	if (where)
	{
		for (unsigned int i=0; i<num_field; i++)
		{
			const char *fieldvalue = NULL;

			if ((fieldvalue = (const char*)((*where)[mysql_fields[i].name])) == NULL)
			if (fieldvalue == NULL)
			{
				continue;
			}

			if (first)
			{
				query_string << " WHERE ";
				first=false;
			}
			else
			{
				query_string <<  " AND ";
			}

			//	query_string << field->name;

			switch (mysql_fields[i].type)
			{
				case FIELD_TYPE_STRING:
				case FIELD_TYPE_VAR_STRING:
				case FIELD_TYPE_DATE:
				case FIELD_TYPE_TIME:
				case FIELD_TYPE_DATETIME:
				case FIELD_TYPE_YEAR:
				case FIELD_TYPE_BLOB:
				case FIELD_TYPE_MEDIUM_BLOB:
					{
						//char strData[strlen(fieldvalue) * 2 + 1];
						//mysql_real_escape_string(mysql, strData, fieldvalue, strlen(fieldvalue));
						query_string << fieldvalue;
					}

					break;
				default:
					query_string << fieldvalue;
					break;
			}

		}
	}
	first = true;
	if (order)
	{
		for (unsigned int i=0; i<num_field; i++)
		{
			const char *fieldvalue = NULL;

			if (!order->find(mysql_fields[i].name))
			{
				continue;
			}

			if (first)
			{
				query_string << " ORDER BY  ";
				first=false;
			}
			else
			{
				query_string <<  ",  ";
			}

			if ((fieldvalue = (*order)[mysql_fields[i].name]) == NULL
					|| strlen((*order)[mysql_fields[i].name])==0)
			{
				query_string << mysql_fields[i].name;
			}
			else
			{
				query_string << mysql_fields[i].name << " " << fieldvalue;
			}

		}

	}

	if(0 == execSql(query_string.str().c_str(), query_string.str().size()))
	{
	}
	else
	{
		logger->error("%s", mysql_error(mysql));
		return pRecord;
	}


	unsigned int retCount=0;
	MYSQL_RES *result=NULL;

	{
		result=mysql_store_result(mysql);
		if(result==NULL)
		{
			logger->error("%s", mysql_error(mysql));
			return pRecord;
		}
		retCount =mysql_num_rows(result);

		if(retCount==0)
		{
			mysql_free_result(result);
			return pRecord;
		}
	}

	MYSQL_ROW row;
	num_field = mysql_num_fields(result);
//	MYSQL_FIELD* mysql_fields = NULL;
//	mysql_fields = mysql_fetch_fields(result);

	if (mysql_num_rows(result) > 0)
	{
		ret_set = FIR_NEW RecordSet();

		if (ret_set)
		{

			while ((row = mysql_fetch_row(result)))
			{
				Record* rec = FIR_NEW Record();
				unsigned long *lengths= mysql_fetch_lengths(result);
				if (rec)
				{

					for (unsigned int i=0; i<num_field; i++)
					{
						if (row[i] != NULL)
						{
							if (FIELD_TYPE_BLOB==mysql_fields[i].type)
								rec->put(mysql_fields[i].name, row[i],lengths[i]);
							else if (FIELD_TYPE_MEDIUM_BLOB==mysql_fields[i].type)
								rec->put(mysql_fields[i].name, row[i],lengths[i]);
							else
								rec->put(mysql_fields[i].name, row[i]);
#ifdef _ZJW_DEBUG							
//							Fir::logger->debug("%s:%s", mysql_fields[i].name, row[i]);
#endif							
						}
					}
				}
				else
				{
					continue;
				}
				ret_set->put(rec);
			}
		}
	}

	mysql_free_result(result);

	if (ret_set && ret_set->size() ==0 )
	{
		SAFE_DELETE(ret_set);
	}

	pRecord.reset(ret_set);
	return pRecord;
}

RecordSetPtr MysqlClientHandle::exeSelect(FieldSet* table, Record* column, Record* where, Record* order,
		unsigned int limit,	
		Record* groupby, Record* having)
{
	using namespace std;

	RecordSetPtr pRecord;

	RecordSet* ret_set = NULL;
	bool first=true;
	ostringstream query_string;
	
	if (table==NULL || mysql==NULL)
	{
		logger->error("null pointer error. ---- %s",__PRETTY_FUNCTION__);
		return pRecord;
	}

	unsigned int num_field = table->size();

	query_string << "SELECT ";

	if (column)
	{
		const char* fieldvalue = NULL;
		if (column->size() > 0)
		{

			if (column->find("*"))
			{
				first=false;

				if ((fieldvalue = (*column)["*"]) == NULL
						|| strlen((*column)["*"])==0)
				{
					query_string << "*";
				}
				else
				{
					query_string << fieldvalue;
				}

			}

			for (unsigned int i=0; i<num_field; i++)
			{

				Field* field = NULL;
				field = table->getField(i);

				if (!column->find(field->name))
				{
					continue;
				}

				if (first)
					first=false;
				else
				{
					query_string <<  " , ";
				}

				if ((fieldvalue = (*column)[field->name]) == NULL
						|| strlen((*column)[field->name])==0)
				{
					query_string << field->name;
				}
				else
				{
					query_string << fieldvalue;
				}
			}
		}
		else
		{
			query_string << " * ";
		}
	}
	else
	{
		query_string << " * ";
	}

	query_string << "  FROM " << " `" << table->getTableName() << "` ";

	first = true;

	if (where)
	{
		for (unsigned int i=0; i<num_field; i++)
		{
			const char *fieldvalue = NULL;
			Field* field = NULL;
			field = table->getField(i);

			if ((fieldvalue = (const char*)((*where)[field->name])) == NULL)
			if (fieldvalue == NULL)
			{
				continue;
			}

			if (first)
			{
				query_string << " WHERE ";
				first=false;
			}
			else
			{
				query_string <<  " AND ";
			}

			//	query_string << field->name;

			switch (field->type)
			{
				case FIELD_TYPE_STRING:
				case FIELD_TYPE_VAR_STRING:
				case FIELD_TYPE_DATE:
				case FIELD_TYPE_TIME:
				case FIELD_TYPE_DATETIME:
				case FIELD_TYPE_YEAR:
				case FIELD_TYPE_BLOB:
				case FIELD_TYPE_MEDIUM_BLOB:
					{
						//char strData[strlen(fieldvalue) * 2 + 1];
						//mysql_real_escape_string(mysql, strData, fieldvalue, strlen(fieldvalue));
						query_string << fieldvalue;
					}

					break;
				default:
					query_string << fieldvalue;
					break;
			}

		}
	}

	first = true;

	if (groupby)
	{
		for (unsigned int i=0; i<num_field; i++)
		{
			const char *fieldvalue = NULL;
			Field* field = NULL;
			field = table->getField(i);

			if (!groupby->find(field->name))
			{
				continue;
			}

			if (first)
			{
				query_string << " GROUP BY  ";
				first=false;
			}
			else
			{
				query_string <<  ",  ";
			}

			if ((fieldvalue = (*groupby)[field->name]) == NULL
					|| strlen((*groupby)[field->name])==0)
			{
				query_string << field->name;
			}
			else
			{
				query_string << field->name << " " << fieldvalue;
			}

		}

	}


	first = true;

	if (having)
	{
		for (unsigned int i=0; i<num_field; i++)
		{
			const char *fieldvalue = NULL;
			Field* field = NULL;
			field = table->getField(i);

			if ((fieldvalue = (*having)[field->name]) == NULL)
			{
				continue;
			}

			if (first)
			{
				query_string << " HAVING ";
				first=false;
			}
			else
			{
				query_string <<  " AND ";
			}

			//	query_string << field->name;

			switch (field->type)
			{
				case FIELD_TYPE_STRING:
				case FIELD_TYPE_VAR_STRING:
				case FIELD_TYPE_DATE:
				case FIELD_TYPE_TIME:
				case FIELD_TYPE_DATETIME:
				case FIELD_TYPE_YEAR:
				case FIELD_TYPE_BLOB:
				case FIELD_TYPE_MEDIUM_BLOB:
					{
						//char strData[strlen(fieldvalue) * 2 + 1];
						//mysql_real_escape_string(mysql, strData, fieldvalue, strlen(fieldvalue));
						query_string << fieldvalue;
					}

					break;
				default:
					query_string << fieldvalue;
					break;
			}

		}
	}
	first = true;
	if (order)
	{
		for (unsigned int i=0; i<num_field; i++)
		{
			const char *fieldvalue = NULL;
			Field* field = NULL;
			field = table->getField(i);

			if (!order->find(field->name))
			{
				continue;
			}

			if (first)
			{
				query_string << " ORDER BY  ";
				first=false;
			}
			else
			{
				query_string <<  ",  ";
			}

			if ((fieldvalue = (*order)[field->name]) == NULL
					|| strlen((*order)[field->name])==0)
			{
				query_string << field->name;
			}
			else
			{
				query_string << field->name << " " << fieldvalue;
			}

		}

	}

	if (limit)
	{
		query_string << " LIMIT " << limit;
	}

#ifdef _ZJW_DEBUG
//	logger->debug("sql:%s", query_string.str().c_str());
#endif 	

	if(0 == execSql(query_string.str().c_str(), query_string.str().size()))
	{
	}
	else
	{
		logger->error("%s", mysql_error(mysql));
		return pRecord;
	}


	unsigned int retCount=0;
	MYSQL_RES *result=NULL;

	{
		result=mysql_store_result(mysql);
		if(result==NULL)
		{
			logger->error("%s", mysql_error(mysql));
			return pRecord;
		}
		retCount =mysql_num_rows(result);

		if(retCount==0)
		{
			mysql_free_result(result);
			return pRecord;
		}
	}

	MYSQL_ROW row;
	num_field = mysql_num_fields(result);
	MYSQL_FIELD* mysql_fields = NULL;
	mysql_fields = mysql_fetch_fields(result);

	if (mysql_num_rows(result) > 0)
	{
		ret_set = FIR_NEW RecordSet();

		if (ret_set)
		{

			while ((row = mysql_fetch_row(result)))
			{
				Record* rec = FIR_NEW Record();
				unsigned long *lengths= mysql_fetch_lengths(result);
				if (rec)
				{

					for (unsigned int i=0; i<num_field; i++)
					{
						if (row[i] != NULL)
						{
							Field* field = NULL;
							field = table->getField(i);
							if (FIELD_TYPE_BLOB==field->type)
								rec->put(mysql_fields[i].name, row[i],lengths[i]);
							else if (FIELD_TYPE_MEDIUM_BLOB==field->type)
								rec->put(mysql_fields[i].name, row[i],lengths[i]);
							else
								rec->put(mysql_fields[i].name, row[i]);
#ifdef _ZJW_DEBUG							
//							Fir::logger->debug("%s:%s", mysql_fields[i].name, row[i]);
#endif							
						}
					}
				}
				else
				{
					continue;
				}
				rec->fields = table;
				ret_set->put(rec);
			}
		}
	}

	mysql_free_result(result);

	if (ret_set && ret_set->size() ==0 )
	{
		SAFE_DELETE(ret_set);
	}
	
	pRecord.reset(ret_set);
	return pRecord;
}

unsigned int MysqlClientHandle::exeInsert(const std::string& strsql)
{
#ifdef _DEBUG_WCX
	logger->debug("sql:%s", strsql.c_str());	
#endif	

	if (0 == execSql(strsql.c_str(), strsql.size()))
	{
		return (unsigned int)mysql_insert_id(mysql);
	}
	else
	{
		return (unsigned int)-1;
	}
}

unsigned int MysqlClientHandle::exeUpdate(const std::string& strsql)
{
#ifdef _DEBUG_WCX
	logger->debug("sql:%s", strsql.c_str());	
#endif

	if (0 == execSql(strsql.c_str(),strsql.length()))
	{
		return (unsigned int)mysql_affected_rows(mysql);
	}
	else
	{
		return (unsigned int)-1;
	}
}

char * MysqlClientHandle::escapeString(const char *src,char *dest,unsigned int size)
{
	if(src==NULL || dest==NULL || mysql==NULL) return NULL;
	char *end=dest;
	mysql_real_escape_string(mysql, end,src,size==0?strlen(src):size);
	return dest;
}

std::string& MysqlClientHandle::escapeString(const std::string &src,std::string &dest)
{
	if(mysql==NULL) return dest;
	char buff[2 * src.length() + 1];
	bzero(buff, sizeof(buff));
	mysql_real_escape_string(mysql, buff,src.c_str(),src.length());
	dest = buff;
	return dest;
}