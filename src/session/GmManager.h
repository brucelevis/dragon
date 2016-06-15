#ifndef _GM_MGR_H
#define _GM_MGR_H

#include "zSingleton.h"
#include <set>
#include <list>
#include <memory>

class GmManager : public Singleton<GmManager>
{
	friend class Singleton<GmManager>;
public:
	virtual ~GmManager(){}

	bool init();

	// 处理gm命令
	bool handle_gm_cmd(DWORD httpid, zProperties &paras, const std::string& content);

	void send_query_rechage_super(DWORD http_id, std::string json);

	bool handle_query_recharge(DWORD http_id, const std::string& content);

	bool handle_recharge(const std::string& content);

	bool __RoleSearch(DWORD httpid, zProperties &paras);

private:
	GmManager(){}

	struct stFuzzyRoleItem
	{
		std::string charid;
		std::string account;
		std::string accountid;
		std::string platform;
		std::string nickname;
		std::string level;
		std::string vip;
		std::string createtime;
		std::string onlinetime;
		std::string offlinetime;
		stFuzzyRoleItem()
		{
		}
	};

	std::string  get_fuzzy_role_count(const std::string& nickname);
	std::vector<stFuzzyRoleItem> get_fuzzy_roles(UINT32 charid, const std::string& nickname, const std::string& offset, const std::string& limit);

	DWORD get62CharidInt(std::string inner_order_id);
	DWORD get62RechargeCountInt(std::string inner_order_id);
	std::string get62Charid(DWORD charid);
	std::string get62RechargeCount(DWORD num);

	// 发送消息给场景玩家（包括离线）
	bool sendCmdToSceneUser(DWORD charid, const void *cmd, const int cmdlen);
public:
	typedef std::function<bool (DWORD httpid, zProperties &)> HANDLE_FUNC;
	typedef std::map<int, HANDLE_FUNC>      HANDLE_FUNC_MAP;
	HANDLE_FUNC_MAP                                 m_HandleFuncMap;  
	void __RegisterHandleFunc (int id, HANDLE_FUNC func);

	void send_gm_error(DWORD httpid,std::string msg);
	void send_gm_error_new(DWORD httpid, std::string msg);
	
};

#endif
