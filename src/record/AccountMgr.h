#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <set>
#include "zSingleton.h"
#include "zMisc.h"
#include "CharBase.h"

class AccountMgr : public Fir::Singleton<AccountMgr>
{
	public:

		AccountMgr() 
		{
		};

		~AccountMgr()
		{
		}

		bool init();
		bool initAccounts();
		
		/**
		 * \brief 同步账号数据-角色列表
		 *
		 */
		static bool syncMemDbAccountRoles(DWORD accountid);
		/**
		 * \brief 获取账号数据
		 *
		 */
		static bool getMemDbAccountItem(DWORD accid, AccountItem& item);
		/**
		 * \brief 同步账号数据
		 *
		 */
		static bool syncMemDbAccountItem(const AccountItem& item);
};

#endif
