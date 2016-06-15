/**
 * \file: RecordUser.cpp
 * \version  $Id: RecordUser.cpp 64 2013-04-23 02:05:08Z  $
 * \author  , @ztgame.com 
 * \date 2007年01月30日 06时37分12秒 CST
 * \brief 档案对象
 *
 * 
 */


#include "Fir.h"
#include "zDBConnPool.h"
#include "RecordServer.h"
#include "RecordUser.h"
#include "RecordTask.h"
#include "zMemDB.h"
#include "StringUtil.h"

RecordUser::RecordUser()
{
	charid = 0;
	bzero(nickname,sizeof(nickname));
	bzero(platform,sizeof(platform));

}

RecordUser::~RecordUser()
{
}

bool RecordUser::readCharBase()
{
	using namespace CMD::RECORD;

	std::string strsql = StringUtil::Format("select f_charid,f_nickname,f_profession,f_experience,f_level,f_gold,f_diamond,f_energy,f_vip,f_vip_charge,"
		"f_current_weapon,f_current_suit,f_ever_battle_layer,f_ever_battle_time,f_createtime,f_onlinetime,f_offlinetime,f_accountid,f_platform,f_account,f_allbinary "
		"from t_charbase where f_charid=%u",this->charid);

	struct ReadData
	{
		ReadData()
		{
			role_size = 0;
			bzero(role, sizeof(role));
		}
		CharBase charbase;
		DWORD role_size;//角色档案数据大小
		unsigned char role[zSocket::MAX_DATASIZE];//角色档案数据,二进制数据
		
	}__attribute__ ((packed)) read_data;

	RecordSetPtr pResult = RecordService::dbConnPool->exeSelect(strsql);

	if (NULL==pResult || pResult->empty())
	{
		Fir::logger->error("[角色读写]:0,charid=%u,读取档案失败，没有找到记录",this->charid);
		return false;
	}

	Record *rec = pResult->get(0);

	read_data.charbase.charid = rec->get("f_charid");				// 角色编号,charid,全区唯一编号
	std::string nickname = rec->get("f_nickname").getString();
	strncpy(read_data.charbase.nickname,nickname.c_str(),MAX_NAMESIZE);// 角色名称
	read_data.charbase.profession = rec->get("f_profession");
	read_data.charbase.experience = rec->get("f_experience");
	read_data.charbase.level = rec->get("f_level");
	read_data.charbase.gold = rec->get("f_gold");
	read_data.charbase.diamond = rec->get("f_diamond");
	read_data.charbase.energy = rec->get("f_energy");
	read_data.charbase.vip = rec->get("f_vip");
	read_data.charbase.vip_charge = rec->get("f_vip_charge");
	read_data.charbase.current_weapon = rec->get("f_current_weapon");
	read_data.charbase.current_suit = rec->get("f_current_suit");
	read_data.charbase.ever_battle_layer = rec->get("f_ever_battle_layer");
	read_data.charbase.ever_battle_time = rec->get("f_ever_battle_time");
	read_data.charbase.createtime = rec->get("f_createtime");                      //角色创建时间
	read_data.charbase.onlinetime = rec->get("f_onlinetime");						//在线时间
	read_data.charbase.offlinetime = rec->get("f_offlinetime");					//离线时间
	read_data.charbase.accountid = rec->get("f_accountid");
	std::string platform = rec->get("f_platform").getString();
	strncpy(read_data.charbase.platform,platform.c_str(),MAX_FLAT_LENGTH);
	std::string account = rec->get("f_account").getString();
	strncpy(read_data.charbase.account,account.c_str(),MAX_ACCNAMESIZE);
	
	std::string strbinary = rec->get("f_allbinary").getBinString();

	read_data.role_size = strbinary.length();
	bcopy(strbinary.c_str(),read_data.role,strbinary.length());

	this->charid = read_data.charbase.charid;
	strncpy(this->nickname, read_data.charbase.nickname, MAX_NAMESIZE+1);
	strncpy(this->platform, read_data.charbase.platform,sizeof(this->platform)-1);
	zMemDB* redishandle = zMemDBPool::getMe().getMemDBHandle(read_data.charbase.charid);
	if (redishandle==NULL)
	{
		CharBase& tmp = read_data.charbase;
		Fir::logger->error("[读取角色],获取内存数据库失败，charid=%u,nickname=%s",tmp.charid,tmp.nickname);
		return false;
	}

	// 同步
	if (!redishandle->setBin("charbase", read_data.charbase.charid, "charbase", (const char*)&read_data.charbase, sizeof(read_data.charbase)))
	{
		CharBase& tmp = read_data.charbase;
		Fir::logger->error("[读取角色],同步内存数据库charbase失败,in readCharBase，charid=%u,nickname=%s",tmp.charid,tmp.nickname);
		return false;
	}

	if (!redishandle->setBin("charbase", read_data.charbase.charid, "allbinary", (const char*)read_data.role, read_data.role_size))
	{
		CharBase& tmp = read_data.charbase;
		Fir::logger->error("[读取角色],同步内存数据库allbinary失败，charid=%u,nickname=%s",tmp.charid,tmp.nickname);
		return false;
	}

	Fir::logger->trace("[角色读写],charid=%u,%s, 找到合格的角色记录", read_data.charbase.charid, read_data.charbase.nickname);
	Fir::logger->debug("读取档案服务器数据,压缩数据大小(size = %u)" , read_data.role_size);
	return true;
}

bool RecordUser::syncBaseMemDB()
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (handle==NULL) 
	{
		return false;
	}

	if(!handle->setInt("rolebaseinfo", this->nickname, "charid", charid)) return false;

	return true;
}

bool RecordUser::refreshSaveBase(const CMD::RECORD::t_WriteUser_SceneRecord *rev)
{
	if (NULL == rev)
	{
		Fir::logger->error("%s,指针为NULL",__PRETTY_FUNCTION__);
		return false;
	}

	zMemDB* redishandle = zMemDBPool::getMe().getMemDBHandle(rev->charid);
	if (redishandle == NULL)
	{
		Fir::logger->error("[读取角色],获取内存数据库失败，charid=%u", rev->charid);
		return false;
	}

	CharBase charbase;
	if (redishandle->getBin("charbase", charid, "charbase", (char*)&charbase) == 0)
	{
		Fir::logger->error("[读取角色]，获取charbase失败，charid=%u", rev->charid);
		return false;
	}

	char allbinary[zSocket::MAX_DATASIZE] = { 0 };
	DWORD binarysize = redishandle->getBin("charbase", charid, "allbinary", (char*)allbinary);
	if (binarysize == 0)
	{
		Fir::logger->error("[读取角色]，获取allbinary失败，charid=%u", rev->charid);
		return false;
	}

	std::string strbinary(allbinary, binarysize);
	std::string strbinary2;
	RecordService::dbConnPool->escapeString(strbinary, strbinary2);

	std::ostringstream oss;
	oss << "update t_charbase set ";
	oss << "f_charid = " << charbase.charid;
	oss << ",f_nickname = " << "\'" << charbase.nickname << "\'";
	oss << ",f_profession = " << charbase.profession;
	oss << ",f_experience = " << charbase.experience;
	oss << ",f_level = " << charbase.level;
	oss << ",f_gold = " << charbase.gold;
	oss << ",f_diamond = " << charbase.diamond;
	oss << ",f_energy = " << charbase.energy;
	oss << ",f_vip = " << charbase.vip;
	oss << ",f_vip_charge = " << charbase.vip_charge;
	oss << ",f_current_weapon = " << charbase.current_weapon;
	oss << ",f_current_suit = " << charbase.current_suit;
	oss << ",f_ever_battle_layer=" << charbase.ever_battle_layer;
	oss << ",f_ever_battle_time=" << charbase.ever_battle_time;
	oss << ",f_createtime = " << charbase.createtime;
	oss << ",f_onlinetime = " << charbase.onlinetime;
	oss << ",f_offlinetime = " << charbase.offlinetime;
	oss << ",f_accountid = " << charbase.accountid;
	oss << ",f_platform = " << "\'" << charbase.platform << "\'";
	oss << ",f_account = " << "\'" << charbase.account << "\'";
	oss << ",f_allbinary = " << "\'" << strbinary2 << "\'";
	oss << "where f_charid = " << charbase.charid;
	std::string strsql = oss.str();

	unsigned int retcode = RecordService::dbConnPool->exeUpdate(strsql);
	if (1 == retcode)
	{
		Fir::logger->trace("[角色读写],charid=%u,保存档案成功 retcode=%u", rev->charid, retcode);
	}
	else if (((unsigned int)-1) == retcode)
	{
		Fir::logger->error("[角色读写],charid=%u,保存档案失败 retcode=%u", rev->charid, retcode);
		return false;
	}

	return true;
}
