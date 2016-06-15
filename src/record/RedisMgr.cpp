#include "RedisMgr.h"

#include <zlib.h>

#include "zMemDBPool.h"
#include "zSocket.h"
#include "RecordServer.h"

// 初始化
bool RedisMgr::init()
{
	// 礼品兑换码
	if (!initGiftCodeExchange())
		return false;

	return true;
}

bool RedisMgr::get_charbase(QWORD charid,CharBase& charbase)
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle) 
	{    
		Fir::logger->error("[获得charbase失败],找不到内存数据库，charid=%llu",charid);
		return false;
	}    

	if (handle->getBin("charbase",charid,"charbase",(char*)&charbase) == 0)
	{
		Fir::logger->error("[获得charbase失败]，charid=%llu",charid);
		return false;
	}

	return true;
}

bool RedisMgr::get_binary(QWORD charid, Serialize& binary)
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle) 
	{    
		Fir::logger->error("[获得charbase失败],找不到内存数据库，charid=%llu到",charid);
		return false;
	}  
	
	char input_buf[zSocket::MAX_DATASIZE]={0};
	DWORD input_size = handle->getBin("charbase", charid, "allbinary", (char*)input_buf);
	if(input_size == 0)
		return true;

	return get_binary(charid,input_buf,input_size,binary);
}

bool RedisMgr::get_binary(QWORD charid,const char* input_buf, DWORD input_size,  Serialize& binary)
{
	unsigned char output_buf[MAX_UZLIB_CHAR];
	bzero(output_buf, sizeof(output_buf));
	uLongf output_size = MAX_UZLIB_CHAR;

	int retcode = uncompress(output_buf, &output_size , (Bytef *)input_buf, input_size);
	switch(retcode)
	{   
		case Z_OK:
			Fir::logger->debug("解压缩档案成功(charid=%llu), size = %u, usize = %lu", charid, input_size, output_size);
			break;
		case Z_MEM_ERROR:
		case Z_BUF_ERROR:
		case Z_DATA_ERROR:
			{   
				Fir::logger->error("解压档案失败(charid=%llu), size = %u, uszie = %lu, 错误码 = %d",charid, input_size, output_size, retcode);
				return false;
			}   
			break;
		default:
			{   
				Fir::logger->error("解压档案未知错误(charid=%llu))", charid);
				return false;
			}   
			break;
	}   

	if(!binary.ParseFromArray(output_buf, output_size))
	{   
		Fir::logger->error("解压档案解析失败(charid=%llu))", charid);
		return false;
	}   

	return true;
}

bool RedisMgr::get_puppet(QWORD charid, stPuppet& puppet)
{
	if(!get_charbase(charid,puppet.charbase))
		return false;

	if(!get_binary(charid,puppet.binary))
		return false;

	return true;
}

bool RedisMgr::getCharid(const std::string& nickname, DWORD& charid)
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle) 
	{    
		Fir::logger->error("[getCharid失败],找不到内存数据库，nickname=%s",nickname.c_str());
		return false;
	}    

	charid = handle->getInt("rolebaseinfo",nickname.c_str(),"charid");
	
	return true;
}

// 初始化礼品码兑换
bool RedisMgr::initGiftCodeExchange()
{
	FieldSet* fields = RecordService::metaData->getFields("t_giftcode_exchanged");
	if(NULL == fields)
	{
		Fir::logger->error("找不到t_giftcode_exchange的FieldSet");
		return false;
	}

	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle) 
	{    
		Fir::logger->error("[initGiftCodeExchange]失败,找不到内存数据库");
		return false;
	}    

	Record col,where;

	col.put("f_code");

	RecordSetPtr recordset = RecordService::dbConnPool->exeSelect(fields, &col, NULL);

	if(recordset != NULL) 
	{
		for(unsigned int i = 0; i<recordset->size(); i++)
		{   
			Record *rec = recordset->get(i);

			std::string code = rec->get("f_code").getString();

			if (!handle->setSet("gitcode_exchanged", 0, NULL, code.c_str()))
			{
				Fir::logger->error("[initGiftCodeExchange]失败,同步内存数据库失败，code=%s",code.c_str());
				return false;
			}
		}

		Fir::logger->trace("[礼品码兑换]初始化成功，一共读取: %u 个已兑换礼品码", recordset->size());
	}

	return true;
}


bool RedisMgr::get_fund_binary(QWORD fundid,Serialize& binary)
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle) 
	{    
		Fir::logger->error("[获得fundinfo失败],找不到内存数据库，fundid=%llu到",fundid);
		return false;
	}  

	char input_buf[zSocket::MAX_DATASIZE]={0};
	DWORD input_size = handle->getBin("t_fund_info", fundid, "f_allbinary", (char*)input_buf);
	if(input_size == 0)
		return true;

	return get_binary(fundid,input_buf,input_size,binary);
}

bool RedisMgr::get_binaryEx(const char* table, QWORD id, const char* col, Serialize& binary)
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle) 
	{    
		Fir::logger->error("[获得%s失败],找不到内存数据库，id=%llu到",table,id);
		return false;
	}  
	char input_buf[zSocket::MAX_DATASIZE]={0};
	DWORD input_size = handle->getBin(table, id, col, (char*)input_buf);
	if(input_size == 0)
		return true;
	return get_binary(id,input_buf,input_size,binary);
}