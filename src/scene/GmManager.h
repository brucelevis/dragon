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
	bool handle_gm_cmd(DWORD httpid, zProperties &paras);

	// 踢角色下线
	bool __KickRole(DWORD httpid, zProperties &paras);

private:
	GmManager(){}

public:
	typedef std::function<bool (DWORD httpid, zProperties &)> HANDLE_FUNC;
	typedef std::map<int, HANDLE_FUNC>      HANDLE_FUNC_MAP;
	HANDLE_FUNC_MAP                                 m_HandleFuncMap;  
	void __RegisterHandleFunc (int id, HANDLE_FUNC func);

};

#endif
