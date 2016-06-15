
#ifndef _GM_MANAGER_H_
#define _GM_MANAGER_H_

#include "Fir.h"
#include "zSingleton.h"

class GmManager : public Fir::Singleton<GmManager>
{
protected:
	typedef std::function<bool (DWORD httpid, zProperties &)> HANDLE_FUNC;
	typedef std::map<int, HANDLE_FUNC>      HANDLE_FUNC_MAP; 

public:
	GmManager(){}
	virtual ~GmManager(){}
	bool handleCmd(WORD temp_server_id, DWORD httpid, zProperties &paras);
	bool init();
	void __RegisterHandleFunc (int id, HANDLE_FUNC func);

protected:
	HANDLE_FUNC_MAP handle_func_map_;
	WORD temp_server_id_;
};


#endif

