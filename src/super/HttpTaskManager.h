
#ifndef _HttpTaskManager_h_
#define _HttpTaskManager_h_

#include <unordered_map>
#include "zType.h"
#include "zMisc.h"
#include "zRWLock.h"
#include "HttpTask.h"
#include "zSingleton.h"

#define CONNECT_TYPENUM     2 
#define TCP_TYPE            0
#define HTTP_TYPE           1
/**
 * \brief WEB 登陆连接管理容器
 *
 * 管理所有的登陆连接的容器，方便查找连接
 *
 */
class HttpTaskManager : public Singleton<HttpTaskManager>
{
	friend class Singleton<HttpTaskManager>;
	public:
		bool add(HttpTask *task);
		void remove(HttpTask *task);
		bool broadcast(const DWORD loginTempID, const void *pstrCmd, int nCmdLen);

		/**
		 * \brief 处理游戏服务器返回 
		 * \param loginTempID:HTTP请求的连接号
		 * \param retcode:SuperServer返回的错误码
		 * \param buf:返回信息字符串
		 */
		void verifyReturn(const DWORD loginTempID, const BYTE retcode, std::string buf = "");

		void verifyReturn(const DWORD loginTempID, std::string buf);

		HttpTaskManager(){};
		~HttpTaskManager() {};

	private:

	
		/**
		 * \brief 定义容器类型
		 *
		 */
		typedef std::unordered_map<DWORD, HttpTask *> HttpTaskHashmap;
		/**
		 * \brief 定义容器迭代器类型
		 *
		 */
		typedef HttpTaskHashmap::iterator HttpTaskHashmap_iterator;
		/**
		 * \brief 定义容器常量迭代器类型
		 *
		 */
		typedef HttpTaskHashmap::const_iterator HttpTaskHashmap_const_iterator;
		/**
		 * \brief 定义容器键值对类型
		 *
		 */
		typedef HttpTaskHashmap::value_type HttpTaskHashmap_pair;
		/**
		 * \brief 读写锁，保证原子访问容器
		 *
		 */
		zRWLock rwlock;
		/**
		 * \brief 子连接管理容器类型
		 *
		 */
		HttpTaskHashmap loginTaskSet;

};
#endif
