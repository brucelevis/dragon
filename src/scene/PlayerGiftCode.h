/**
 * \file    PlayerGiftCode.h
 * \version $Id: PlayerGiftCode.h 43 2015-07-22 10:49:15Z  $
 * \autor   ,
 * \date    2015年10月20日 10时49分15秒 CST
 * \brief   玩家礼包码
 *
 */

#ifndef _PLAYER_GIFTCODE_H
#define _PLAYER_GIFTCODE_H

#include <map>
#include <set>

#include "zType.h"
#include "packet.h"
#include "proto/serialize.pb.h"

using namespace FIGHTING;

class SceneUser;
/**
 * \brief 玩家礼包码
 *
 */
class PlayerGiftCode
{
	// 领取记录
	struct Record
	{
		std::string code;	// 礼包码
		DWORD time;			// 领取时间
	};
	typedef std::map<std::string, Record> GiftCodeRecordMap;

	// 码组记录
	struct GroupRecord
	{
		int group;			// 礼包码组
		DWORD time;			// 领取时间
	};
	typedef std::map<int, GroupRecord> GiftCodeGroupRecordMap;

public:
	PlayerGiftCode(SceneUser* owner_);
	~PlayerGiftCode();

	// 保存
	void save(Serialize& binary);
	// 加载
	void load(const Serialize& binary);

	// 是否领取过礼包码
	bool isGiftCodeExchanged(const std::string& code) const;
	// 领取礼包码
	void addGiftCodeExchanged(const std::string& code);

	// 是否领取过礼包码组
	bool isGiftCodeGroupExchanged(const int& group) const;
	// 领取礼包码组
	void addGiftCodeGroupExchanged(const int& group);

private:
	// 序列化
	bool __Serialize(PbGiftCodeExchanged& pb);
	// 反序列化
	bool __UnSerialize(const PbGiftCodeExchanged& pb);

private:
	SceneUser* owner;

	// 已兑换礼包码
	GiftCodeRecordMap m_mapGiftCodeExchanged;
	// 已兑换礼包码组
	GiftCodeGroupRecordMap m_mapGiftCodeGroupExchanged;
};

#endif