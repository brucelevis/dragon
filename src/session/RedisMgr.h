#ifndef _REDIS_MGR_H
#define _REDIS_MGR_H

#include "zSingleton.h"
#include "CharBase.h"
#include "proto/serialize.pb.h"

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

	bool get_charbase(DWORD charid, CharBase& charbase); 

	bool get_binary(DWORD charid, Serialize& binary);

	bool get_binary(DWORD charid, const char* input_buf, DWORD input_size, Serialize& binary);

	bool get_puppet(DWORD charid, stPuppet& puppet);
	
	DWORD get_charidbyName(const std::string nickname);

	DWORD get_charid(const DWORD charid);
private:
	RedisMgr(){}
};

#endif
