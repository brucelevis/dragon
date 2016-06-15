#ifndef _REDIS_MGR_H
#define _REDIS_MGR_H

#include <map>
#include <unordered_map>
#include <functional>

#include "zSingleton.h"
#include "common.h"



class RedisMgr : public Singleton<RedisMgr>
{
	friend class Singleton<RedisMgr>;
public:
	virtual ~RedisMgr(){}

	// 礼品码是否已兑换
	static bool isGiftCodeExchanged(const std::string& code);
	// 添加已兑换礼品码
	static bool addGiftCodeExchanged(const std::string& code, DWORD charid, DWORD time);

private:
	RedisMgr(){}
	
};

#endif
