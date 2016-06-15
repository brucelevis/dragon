/**
 * \file	RecordUser.h
 * \version  	$Id: RecordUser.h 53 2013-04-17 05:03:41Z  $
 * \author  	, @ztgame.com 
 * \date 	2007年01月30日 06时36分10秒 CST
 * \brief 	档案对象定义，以支持存档Cache
 *
 * 
 */

#ifndef RECORDUSER_H
#define RECORDUSER_H

#include <map>
#include <vector>

#include "RecordCommand.h"
#include "zDBConnPool.h"
#include "zSocket.h"
#include "zMisc.h"


const dbCol record_charbase[] = { 
	{ "f_charid",     DB_DWORD,  sizeof(DWORD) },  
	{ "f_nickname",   DB_STR,    50 },
	{ "f_profession",    DB_DWORD,  sizeof(DWORD) },
	{ "f_experience",      DB_QWORD,  sizeof(QWORD) },
	{ "f_level",      DB_DWORD,  sizeof(DWORD) },
	{ "f_gold",     DB_DWORD,  sizeof(DWORD) },
	{ "f_diamond",     DB_DWORD,  sizeof(DWORD) },
	{ "f_energy",      DB_DWORD,  sizeof(DWORD) },
	{ "f_vip",      DB_DWORD,  sizeof(DWORD) },
	{ "f_vip_charge",    DB_DWORD,  sizeof(DWORD) },
	{ "f_current_weapon",     DB_DWORD,  sizeof(DWORD) },
	{ "f_current_suit",      DB_DWORD,  sizeof(DWORD) },
	{ "f_ever_battle_layer",DB_DWORD,   sizeof(DWORD) },
	{ "f_ever_battle_time",DB_DWORD,   sizeof(DWORD) },
	{ "f_createtime", DB_DWORD,   sizeof(DWORD) },  
	{ "f_onlinetime", DB_DWORD,   sizeof(DWORD) },  
	{ "f_offlinetime",DB_DWORD,   sizeof(DWORD) },
	{ "f_accountid",DB_DWORD,   sizeof(DWORD) },
	{ "f_platform",   DB_STR,    64 },
	{ "f_account",  DB_STR,    64 },
	{ "f_allbinary",  DB_BIN2,   0}, 
	{ NULL, 0, 0}
}; 

class RecordTask;

class RecordUser
{
	public:
		RecordUser();
		~RecordUser();

		DWORD charid;//玩家主角ID，由ID生成服务器生成，全服唯一
		char nickname[MAX_NAMESIZE+1]; //玩家主公姓名
		char platform[MAX_FLAT_LENGTH+1]; //渠道

		//从数据库读取角色完整档案到内存
		bool readCharBase();
		//同步角色基础信息到MemDB
		bool syncBaseMemDB();
		
		bool refreshSaveBase(const CMD::RECORD::t_WriteUser_SceneRecord *rev);
};

#endif

