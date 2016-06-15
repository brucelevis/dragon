#include "RedisMgr.h"

#include <zlib.h>

#include "zMemDBPool.h"
#include "zSocket.h"
#include "SceneUserManager.h"
#include "CommonStruct.h"
#include "StringUtil.h"
#include "SceneServer.h"

// 礼品码是否已兑换
bool RedisMgr::isGiftCodeExchanged(const std::string& code)
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle) 
	{    
		Fir::logger->error("[isGiftCodeExchanged失败],找不到内存数据库，code=%s", code.c_str());
		return false;
	}    

	return handle->checkSet("gitcode_exchanged", 0, NULL, code.c_str());
}

// 添加已兑换礼品码
bool RedisMgr::addGiftCodeExchanged(const std::string& code, DWORD charid, DWORD time)
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle) 
	{    
		Fir::logger->error("[addGiftCodeExchanged]失败,找不到内存数据库，code=%s", code.c_str());
		return false;
	}  

	// 同步Redis
	if (!handle->setSet("gitcode_exchanged", 0, NULL, code.c_str()))
	{
		Fir::logger->error("[addGiftCodeExchanged]失败,同步内存数据库失败，code=%s",code.c_str());
		return false;
	}

	// 同步数据库
	std::ostringstream oss;
	oss << "insert into t_giftcode_exchanged(f_code,f_charid,f_time) values('";
	oss << code << "'," << charid << "," << time << ")";
	std::string strsql = oss.str();

	unsigned int recode = SceneService::dbConnPool->exeInsert(strsql);

	if ((unsigned int)-1==recode)
	{
		Fir::logger->error("[addGiftCodeExchanged]失败,插入数据库失败，strsql=%s", strsql.c_str());
		return false;
	}

	return true;
}