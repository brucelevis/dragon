#ifndef _REDIS_MGR_H
#define _REDIS_MGR_H

#include <map>
#include <unordered_map>
#include <functional>

#include "zSingleton.h"
#include "CharBase.h"
#include "proto/serialize.pb.h"
#include "common.h"


struct stPuppet
{
	CharBase charbase;
	Serialize binary;
};

class RedisMgr : public Singleton<RedisMgr>
{
	friend class Singleton<RedisMgr>;
public:
	virtual ~RedisMgr(){}

	// 初始化
	bool init();

	bool get_charbase(QWORD charid, CharBase& charbase); 

	bool get_binary(QWORD charid, Serialize& binary);

	bool get_binary(QWORD charid, const char* input_buf, DWORD input_size, Serialize& binary);

	bool get_puppet(QWORD charid, stPuppet& puppet);

	bool getCharid(const std::string& nickname, DWORD& charid);

	bool get_fund_binary(QWORD fundid,Serialize& binary);
	bool get_binaryEx(const char* table, QWORD id, const char* col, Serialize& binary);

private:
	// 初始化礼品码兑换
	bool initGiftCodeExchange();

private:
	RedisMgr(){}
};

#endif
