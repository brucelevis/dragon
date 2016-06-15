#ifndef _ZDBCONNPOOL_H_
#define _ZDBCONNPOOL_H_

#include "zType.h"
#include <string.h>
#include "zMysqlClientHandle.h"

typedef unsigned int(* hashCodeFunc)(const void *data);

typedef std::multimap<unsigned int,MysqlClientHandle *> handlesPool;
typedef std::map<unsigned int,UrlInfo> urlsPool;
typedef std::map<connHandleID,MysqlClientHandle *> handlesIDMap;

/**
 * \brief 数据链接池接口定义
 *
 * 本类提供了对数据库的简单的基本访问,比如UPDATE,INSERT,SELECT,DELETE,执行SQL语句等.
 *
 *  用户只需要定义要操作的数据库数据，即可访问。
 *
 * 如果选用不同数据库,必须实现这个接口,目前提供了Mysql的实现.
 */
class zDBConnPool
{
	public:

		/**
		 * \brief 新建立一个链接池的实例
		 *
		 * 本接口没有默认的构造函数，为了使用者无需关心底层数据库的差异。提供此接口。
		 * \param hashfunc 哈希函数指针，如果为NULL，接口实现者应该提供默认函数。
		 * \return 返回这个接口的一个实例，错误返回NULL。
		 */
		static zDBConnPool *newInstance(hashCodeFunc hashfunc);

		/**
		 * \brief 回收一个链接池的实例
		 *
		 * 本接口没有默认的析构函数，为了使用者无需关心底层数据库的差异。提供此接口。
		 * \param delThisClass 要回收的链接池实例。
		 */
		static void delInstance(zDBConnPool **delThisClass);

		zDBConnPool(hashCodeFunc hashfunc);
	
		/**
		 * \brief 接口析构虚函数
		 */
		~zDBConnPool();

		/**
		 * \brief 向连接池中添加数据库连接URL，并设置此连接是否支持事务
		 *
		 * \param hashcode 此连接所对应的哈希代码，使用者需要指定。
		 * \param url 数据库联接的url
		 * \param supportTransactions 此连接是否只是事务
		 * \return 成功返回true，否则返回false
		 */
		bool putURL(unsigned int hashcode,const char *url,bool supportTransactions);

		/**
		 * \brief 根据hashData得到连接Handle
		 * 
		 * \param hashData 链接池用此参数作为调用hashCodeFunc的参数，计算hashcode，用来得到相应的数据库联接。
		 * \return 数据库联接句柄,-1表示无效句柄
		 */
		connHandleID getHandle(const void *hashData=NULL);

		/**
		 * \brief 将Handle放回连接池
		 *
		 * 用户在使用完数据库联接句柄后，应该将其放回链接池，已备下次使用。
		 * \param handleID 放回链接池的链接句柄
		 */
		void putHandle(connHandleID handleID);

		/**
		 * \brief 执行Sql语句,返回db_real_query的返回结果
		 *
		 * 为了提供更灵活的数据库操作，提供了本函数
		 * \param handleID 操作的链接句柄
		 * \param sql 要执行的SQL语句
		 * \param sqllen SQL语句的长度
		 * \return 返回执行数据语句后的代码，根具体的数据库的返回值有关
		 */
		int execSql(const char *sql,unsigned int sqllen);

		/**
		 * \brief 执行SELECT SQL
		 *
		 *  使用方法可参见test/NewMySQLTest中的使用代码例程 
		 *
		 * \param handleID 操作的链接句柄
		 * \param table 要操作的表结构对象，通过MetaData.getFields取得
		 * \param column 要操作的数据字段描述，不指定时为返回所有字段"*" 
		 * \param where SQL的where描述,没有时用NULL
		 * \param order SQL的order描述,没有时用NULL，可不填写
		 *
		 * \return 返回结果集
		 */

		RecordSetPtr exeSelect(const char* tablename, Record* column,
				Record* where, Record* order = NULL);

		RecordSetPtr exeSelect(const std::string& strsql);
		/**
		 * \brief 执行SELECT SQL
		 *
		 *  使用方法可参见test/NewMySQLTest中的使用代码例程 
		 *
		 * \param handleID 操作的链接句柄
		 * \param table 要操作的表结构对象，通过MetaData.getFields取得
		 * \param column 要操作的数据字段描述，不指定时为返回所有字段"*" 
		 * \param where SQL的where描述,没有时用NULL
		 * \param order SQL的order描述,没有时用NULL，可不填写
		 * \param limit 返回结果的最大限制，为0时，为不限制，或可不填写
		 * \param groupby SQL中的GROUPBY子句描述，未有时，可不填写。也可填为NULL
		 * \param having SQL中的HAVING子句描述，未有时，可不填写，也可填为NULL
		 *
		 * \return 返回结果集
		 *
		 * \author zjw
		 */

		RecordSetPtr exeSelect(FieldSet* table, Record* column,
				Record* where, Record* order = NULL, 
				unsigned int limit=0,
				Record* groupby = NULL, Record* having = NULL);

		unsigned int exeInsert(const std::string& strsql);
		
		unsigned int exeUpdate(const std::string& strsql);

		/**
		 * \brief 转化字符串为有效的db字符串
		 * \param handleID 操作的链接句柄
		 * \param src 操作源数据
		 * \param dest 转换后字符串所存放的空间,为了程序的安全你应该为dest分配(size==0?strlen(src):size)*2+1的空间
		 * \param size 如果size>0,表示转化指定长度的字符串，用于二进制数据的转化，如果为0表示一般字符串的转户
		 * \return 失败返回NULL,成功返回dest
		 */
		char * escapeString(const char *src,char *dest,unsigned int size);

		std::string& escapeString(const std::string &src, std::string &dest);

		private:

#ifdef _ZJW_DEBUG
		static const unsigned int maxHandleBuf = 1;
#else		
		static const unsigned int maxHandleBuf = 1;
#endif		

		zMutex mlock;
		handlesPool handles;
		urlsPool urls;
		handlesIDMap idmaps;
		hashCodeFunc hashCode;

	private:

		MysqlClientHandle* getHandleByHashcode(unsigned int hashcode);
		MysqlClientHandle* getHandleByID(connHandleID handleID);
};

#endif
