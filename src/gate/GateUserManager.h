/**
 * \file
 * \version  $Id: GateUserManager.h 53 2013-04-17 05:03:41Z  $
 * \author  ,
 * \date 2013年04月11日 09时55分24秒 CST
 * \brief 定义网关用户管理类
 */

#ifndef _GATEUSERMANAGER_H_
#define _GATEUSERMANAGER_H_
#include "GateUser.h"
#include <unordered_map>
#include <functional>

class GateUserManager : public Singleton<GateUserManager>
{
	friend class Singleton<GateUserManager>;
	private:
		GateUserManager();
		~GateUserManager();
	public:
		// charid map
		GateUser * getUserCharid(DWORD charid);
		bool addUserCharid(GateUser *user);
		//bool addUserCharidForce(GateUser *user);
		void removeUserCharid(DWORD charid);

		DWORD getUserCount();
		DWORD getRoleCount();

		typedef std::function<void (GateUser*)> UserFunction;
        void execAll(UserFunction func);
		
		//plat_account map
		GateUser * getUserAccount(DWORD accountid);
		bool addUserAccount(GateUser* user);
		//bool addUserAccountForce(GateUser* user);
		void removeUserAccount(DWORD accountid);

		void sendCmdToWorld(DWORD except_charid,const void *pstrCmd, const unsigned int nCmdLen);
	private:
		std::map<DWORD, GateUser*> m_mapUsers; // 所有gateuser
		zRWLock charid_lock;
	
		std::map<DWORD, GateUser*> account_map;	//所有帐号登陆的玩家
		zRWLock plat_account_rwlock;
};
#endif
