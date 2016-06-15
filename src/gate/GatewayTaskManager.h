/**
 * \file
 * \version  $Id: GatewayTaskManager.h 24 2013-03-30 08:04:25Z  $
 * \author  ,@163.com
 * \date 2004年12月20日 17时28分53秒 CST
 * \brief 管理子连接的容器
 *
 * 
 */


#ifndef _GatewayTaskManager_h_
#define _GatewayTaskManager_h_

#include <unordered_map>
#include "zRWLock.h"

class GatewayTask;

class GatewayTaskManager : public Singleton<GatewayTaskManager>
{

	friend class Singleton<GatewayTaskManager>;

	public:
		
		/**
		 * \brief 析构函数
		 *
		 */
		~GatewayTaskManager();

		bool uniqueAdd(GatewayTask *task);
		bool uniqueRemove(GatewayTask *task);
		DWORD GetTokenTime(){return m_tokentime;}

		/**
		 * \brief 处理所有连接消息
		 *
		 */
		void execEvery();
		/**
		 * \brief 关闭时注销所连接
		 *
		 */
		void unregEveryByClose();

		/**
		 * \brief 获取连接
		 *
		 */
		GatewayTask* getGatewayTask(QWORD seqid);
		
	private:

		/**
		 * \brief 构造函数
		 *
		 */
		GatewayTaskManager();
		/**
		 * \brief 定义容器类型
		 *
		 */
		typedef std::unordered_map<DWORD, GatewayTask *> GatewayTaskHashmap;
		/**
		 * \brief 定义容器的迭代器类型
		 *
		 */
		typedef GatewayTaskHashmap::iterator GatewayTaskHashmap_iterator;
		/**
		 * \brief 定义了容器的常量迭代器类型
		 *
		 */
		typedef GatewayTaskHashmap::const_iterator GatewayTaskHashmap_const_iterator;
		/**
		 * \brief 容器访问互斥变量
		 *
		 */
		zRWLock rwlock;
		/**
		 * \brief 声明一个容器，存放所有的子连接
		 *
		 */
		GatewayTaskHashmap _mapTasks;
		/**
		 * \brief Token时间
		 *
		 */
		DWORD m_tokentime;
};

#endif

