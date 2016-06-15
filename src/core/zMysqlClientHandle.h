/**
 * \file
 * \version  $Id: zXMLParser.h 13 2013-03-20 02:35:18Z  $
 * \author  ,okyhc@263.sina.com
 * \date 2004年11月03日 14时15分45秒 CST
 * \brief zXMLParser器定义文件
 *
 * 
 */

#ifndef _ZMYSQL_CLIENT_HANDLE_H_
#define _ZMYSQL_CLIENT_HANDLE_H_

#include <pthread.h>
#include <unistd.h>
#include <zlib.h>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "zType.h"
#include "Fir.h"
#include "zMutex.h"
#include "zNoncopyable.h"
#include "zTime.h"
#include "zMisc.h"
#include "zMetaData.h"

using namespace Fir;

typedef unsigned int connHandleID;

enum handleState
{
	MYSQLCLIENT_HANDLE_INVALID  = 1,   //无效的
	MYSQLCLIENT_HANDLE_VALID    = 2,   //有效的
	MYSQLCLIENT_HANDLE_USED     = 3,   //被使用
};


struct UrlInfo
{
	const unsigned int hashcode;
	const std::string url;
	const bool supportTransactions;

	char host[MAX_HOSTSIZE];
	char user[MAX_USERSIZE];
	char passwd[MAX_PASSWORD];
	unsigned int port;
	char dbName[MAX_DBSIZE];

	UrlInfo()
		: hashcode(0),url(),supportTransactions(false) {};
	UrlInfo(const unsigned int hashcode,const std::string &url,const bool supportTransactions)
		: hashcode(hashcode),url(url),supportTransactions(supportTransactions)
		{
			parseMySQLURLString();
		}
	UrlInfo(const UrlInfo &ui)
		: hashcode(ui.hashcode),url(ui.url),supportTransactions(ui.supportTransactions)
		{
			parseMySQLURLString();
		}

	private:
	/*
	void parseMySQLURLString()
	{
		bzero(host,sizeof(host));
		bzero(user,sizeof(user));
		bzero(passwd,sizeof(passwd));
		port=3306;
		bzero(dbName,sizeof(dbName));

		std::string strhost,struser,strpasswd,strport,strdbname;
		std::string target = url;
		std::string::size_type pos = target.find("mysql://");
		target.replace(0,pos+strlen("mysql://"),"");
	}
	*/
	void parseMySQLURLString()
	{
		bzero(host,sizeof(host));
		bzero(user,sizeof(user));
		bzero(passwd,sizeof(passwd));
		port=3306;
		bzero(dbName,sizeof(dbName));

		char strPort[16] = "";
		int  j, k;
		size_t i;
		const char *connString = url.c_str();
		if (0 == strncmp(connString, "mysql://", strlen("mysql://")))
		{
			i = 0; j = 0; k = 0;
			for(i = strlen("mysql://"); i < strlen(connString) + 1; i++)
			{
				switch(j)
				{
					case 0:
						if (connString[i] == ':')
						{
							user[k] = '\0'; j++; k = 0;
						}
						else
							user[k++] = connString[i];
						break;
					case 1:
						if (connString[i] == '@')
						{
							passwd[k] = '\0'; j++; k = 0;
						}
						else
							passwd[k++] = connString[i];
						break;
					case 2:
						if (connString[i] == ':')
						{
							host[k] = '\0'; j++; k = 0;
						}
						else
							host[k++] = connString[i];
						break;
					case 3:
						if (connString[i] == '/')
						{
							strPort[k] = '\0'; j++; k = 0;
						}
						else
							strPort[k++] = connString[i];
						break;
					case 4:
						if (connString[i] == '\0')
						{
							dbName[k] = '\0'; j++; k = 0;
						}
						else
							dbName[k++] = connString[i];
						break;
					default:
						break;
				}
			}
		}
		port=atoi(strPort);
	}
};


typedef struct
{
	const char *name;	/**< 字段名字 */
	int type;			/**< FIR数据类型 */
	unsigned int size;	/**< 数据大小 */
} dbCol;

enum
{
	DB_BYTE,		/**< BYTE类型 1字节长度 */
	DB_CHAR,		/**< CHAR类型 1字节长度 */
	DB_WORD,		/**< WORD类型 2字节长度 */
	DB_DWORD,		/**< DWORD类型 4字节长度 */
	DB_QWORD,		/**< QWORD类型 8字节长度 */
	DB_STR,			/**< 字符串类型 */
	DB_BIN,			/**< 二进制数据类型 */
	DB_ZIP,			/**< zip压缩数据类型 */
	DB_BIN2,		/**< 扩展二进制数据类型 */
	DB_ZIP2			/**< 扩展zip压缩数据类型 */
};

struct st_mysql;
/**
 * \brief Mysql连接句柄类
 * 封装了大部分的mysql操作
 */
class MysqlClientHandle : private zNoncopyable
{
	/**
		 * \brief 数据库支持的数据类型
		 */
	private:

		const connHandleID id;

		const UrlInfo url;

		unsigned int getedCount;
		zTime lifeTime;
		st_mysql *mysql;

		bool initMysql();
		void finalHandle();

		static connHandleID HandleID_generator;

	public:
		handleState state; ///handle状态
		pthread_t getedThread;
		zTime useTime;
		std::string my_sql;

		MysqlClientHandle(const UrlInfo &url);
		~MysqlClientHandle();

		const connHandleID &getID() const
		{
			return id;
		}

		const unsigned int hashcode() const
		{
			return url.hashcode;
		}

		bool isSupportTransactions() const
		{
			return url.supportTransactions;
		}

		bool initHandle();
		bool setHandle();
		void unsetHandle();

		bool setTransactions(bool supportTransactions);
		int execSql(const char *sql,unsigned int sqllen);

		RecordSetPtr exeSelect(const char* tableName,Record* column, Record* where, Record* order);
		RecordSetPtr exeSelect(const std::string& strsql);
		RecordSetPtr exeSelect(FieldSet* table, Record* column, Record* where, Record* order = NULL, unsigned int limit=0,Record* groupby = NULL, Record* having = NULL);

		unsigned int exeInsert(const std::string& strsql);

		unsigned int exeUpdate(const std::string& strsql);

		char * escapeString(const char *src,char *dest,unsigned int size);
		std::string& escapeString(const std::string &src,std::string &dest);
};

#endif
