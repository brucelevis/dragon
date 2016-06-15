#ifndef _SESSIONUSERMANAGER_H_
#define _SESSIONUSERMANAGER_H_

#include <functional>
#include <unordered_map>
#include "zSingleton.h"
#include "SessionCommand.h"
#include "zRWLock.h"
#include "SessionUser.h"


class SessionTask;
/**
 * \brief 定义玩家信息管理器
 *
 */
class SessionUserManager : public Singleton<SessionUserManager>
{

		friend class Singleton<SessionUserManager>;

	private:

		/**
		 * \brief 构造函数
		 *
		 */
		SessionUserManager()
		{
		}
	
		/**
		 * \brief 析构函数
		 *
		*/
		~SessionUserManager();

	public:

		/**
		 * \brief 更新
		 *
		*/
		void update(const zRTime& current);

		/**
		 * \brief 通过账号获取玩家
		 *
		*/
		SessionUser* getUserByACCID(DWORD accid);
		/**
		 * \brief 通过角色获取玩家
		 *
		*/
		SessionUser* getUserByCharID(DWORD charid);
		
		/**
		 * \brief 注册网关玩家
		 *
		*/
		bool regGateUser(SessionTask* task, const CMD::SESSION::t_regUser_GateSession* cmd);
		/**
		 * \brief 注册场景玩家
		 *
		*/
		bool regSceneUser(SessionUser* u, SessionTask* scene);

		/**
		 * \brief 注销玩家
		 *
		*/ 
		bool unregUser(SessionUser* u, const SessionUser::UnregMethod method = SessionUser::unreg_no, const BYTE unregtype = 0);
		
		/**
		 * \brief 等待注销玩家
		 *
		*/ 
		bool unregUserWait(SessionUser* u);

		/**
		 * \brief 重连网关玩家
		 *
		*/
		bool reconnGateUser(SessionUser* u, const CMD::SESSION::t_regUser_GateSession* cmd);

		/**
		 * \brief 获取登录账号数量
		 *
		*/
		DWORD getUserNum();
		/**
		 * \brief 获取登录角色数量
		 *
		*/
		DWORD getCharNum();
		/**
		 * \brief 获取离线数量
		 *
		*/
		DWORD getOfflineNum();

		/**
		 * \brief 删除场景玩家
		 *
		*/
		void removeUserBySceneID(DWORD sceneid);
		/**
		 * \brief 删除网关玩家
		 *
		*/
		void removeUserByGateID(DWORD gateid);
		
		/**
		 * \brief 添加待删除玩家
		 *
		*/
		bool addUserToUnreg(SessionUser* user);
	
		/**
		 * \brief 删除待删除玩家
		 *
		*/
		bool removeUserToUnreg(SessionUser* user);

		/**
		 * \brief 注册GM玩家
		 *
		*/
		SessionUser* regGMUser(DWORD accid, DWORD charid, SessionTask* scene);
		/**
		 * \brief 注销GM玩家
		 *
		*/
		void unregGMUser(SessionUser* user);

	private:
		/**
		 * \brief 添加网关玩家
		 *
		*/
		bool addGateUser(SessionUser* user);
		
		/**
		 * \brief 删除网关玩家
		 *
		*/
		bool removeGateUser(SessionUser* user);

		/**
		 * \brief 添加场景玩家
		 *
		*/
		bool addSceneUser(SessionUser* user);
		
		/**
		 * \brief 删除场景玩家
		 *
		*/
		bool removeSceneUser(SessionUser* user);

		/**
		 * \brief 注销所有待注销超时玩家
		 *
		*/
		void unregAllUserToUnregTimeout(const zRTime& current);
		
		/**
		 * \brief 注销网关玩家
		 *
		*/
		bool unregGateUser(SessionUser* u, const SessionUser::UnregMethod method, const BYTE unregtype);
		/**
		 * \brief 注销场景玩家
		 *
		*/
		bool unregSceneUser(SessionUser* u, const SessionUser::UnregMethod method);

		/**
		 * \brief 定义容器类型
		 *
		 */
		typedef std::unordered_map<DWORD, SessionUser *> SessionUserHashmap;
		/**
		 * \brief 定义容器迭代器类型
		 *
		 */
		typedef SessionUserHashmap::iterator SessionUserHashmap_iterator;
		/**
		 * \brief 定义容器常量迭代器类型
		 *
		 */
		typedef SessionUserHashmap::const_iterator SessionUserHashmap_const_iterator;
		/**
		 * \brief 定义容器键值对类型
		 *
		 */
		typedef SessionUserHashmap::value_type SessionUserHashmap_pair;
		/**
		 * \brief 容器访问互斥变量
		 *
		 */
		zRWLock rwlock;
		
		/**
		 * \brief 声明一个容器，存放所有网关玩家（账号主键）
		 *
		 */
		SessionUserHashmap _mapAccUsers;
		/**
		 * \brief 声明一个容器，存放所有网关玩家（角色主键）
		 *
		 */
		SessionUserHashmap _mapCharUsers;

		/**
		 * \brief 声明一个容器，存放所有待注销玩家（账号主键）
		 *
		 */
		SessionUserHashmap _mapUsersToUnreg;

};
#endif
