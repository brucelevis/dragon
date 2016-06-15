/**
 * \file	SceneUser.h
 * \version  	$Id: SceneUser.h 67 2013-04-23 09:44:20Z  $
 * \author  	,
 * \date 	2013年04月07日 15时48分31秒 CST
 * \brief 	场景用户定义
 *
 * 
 */

#ifndef _SCENEUSER_H_
#define _SCENEUSER_H_

#include <map>
#include <list>
#include <set>
#include <vector>
#include <bitset>

#include "Fir.h"
#include "SceneTask.h"
#include "SessionCommand.h"
#include "CharBase.h"
#include "zTime.h"
#include "RecordCommand.h"
#include "zCmdBuffer.h"
#include "common.h"
#include "zMacro.h"

#include "CommonStruct.h"
#include "packet.h"
#include "zGameEventDictionary.h"
#include "PlayerGiftCode.h"
#include "PlayerVars.h"

#include "proto/serialize.pb.h"


class SceneUser 
{
	public:
		friend class SceneUserManager;
	
		std::string _account;
		DWORD _charid;

		DWORD   dwLastSaveTime; //上次存档时间
		DWORD	dwOnlineTime;	//在线时长
		
		SceneUser(DWORD charid);

		virtual ~SceneUser();

		// 注册
		bool reg(DWORD gateid, BYTE regtype);
		// 注销
		bool unreg();

		// 获取注册类型
		BYTE getRegType() const { return _regtype; }

		// 注册到网关
		bool regToGate();

		void setupBinaryArchive(const Serialize& binary);
		DWORD saveBinaryArchive(unsigned char *out , const int maxsize);
	
		void SendErrorMessage(WORD MsgId, WORD ErrId);
		void SendMessage2Ci(Packet& p);
	
		template<typename T1> 
		void SendMessage2Ci(WORD msgId, T1& p1)
		{
			Packet p;

			p << msgId;
			p << p1;

			SendMessage2Ci(p);
		}

		bool save();
		bool saveToMemoryDB(bool& bNeedSave);

		bool saveCharbaseToMemeryDB(bool& bNeedSave);
		bool saveBinaryToMemoryDB(bool& bNeedSave);

		bool sendCmdToMe(const void *pstrCmd, const int nCmdLen);
		bool sendCmdToGateway(const void *pstrCmd, const int nCmdLen);

		bool online();
		void offline();
		void onlineInit();
		// 刷新玩家排行
		void updateUserTopList();
		// 退出玩家排行
		void quitUserTopList();

		bool KickOut(LOGOUT_TYPE kick_type = LOGOUT_KICK);
	private:
		// 网关ID
		DWORD _gateid;
		// 注册类型
		BYTE _regtype;

		unsigned char lastSaveBinaryMD5[MD5_LENGTH];
		unsigned char lastSaveCharBaseMD5[MD5_LENGTH];

	public:
		PlayerVars m_player_vars;	// 任务变量
		PlayerGiftCode m_player_giftcode;

	public:
		// 同步今日信息
		void SyncDailyInfo();
		// 刷新今日信息
		void RefreshDailyInfo(bool isMonthChanged, bool isWeekChanged);
		// 触发每日刷新事件
		void OnDailyRefreshEvent();
		
		// 获得角色id
		DWORD getCharid() const;
		// 获取昵称
		std::string getNickname() const;

		// 获取账号id
		DWORD getAccountid() const;
		// 获得平台
		std::string getPlatfrom() const;

	public:
		bool loadFromDb();
};

#endif
