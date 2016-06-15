#include "PlayerGiftCode.h"
#include "SceneUser.h"
#include "TimeTick.h"

PlayerGiftCode::PlayerGiftCode(SceneUser* owner_)
	: owner(owner_)
{
}

PlayerGiftCode::~PlayerGiftCode()
{
	owner = NULL;
	m_mapGiftCodeExchanged.clear();
	m_mapGiftCodeGroupExchanged.clear();
}

void PlayerGiftCode::save(Serialize& binary)
{
	// 序列化
	__Serialize(*binary.mutable_pb_giftcode_exchanged());
}

void PlayerGiftCode::load(const Serialize& binary)
{
	// 反序列化
	__UnSerialize(binary.pb_giftcode_exchanged());
}

// 是否领取过礼包码
bool PlayerGiftCode::isGiftCodeExchanged(const std::string& code) const
{
	auto iter = m_mapGiftCodeExchanged.find(code);
	if (iter != m_mapGiftCodeExchanged.end())
	{
		return true;
	}
	return false;
}

// 领取礼包码
void PlayerGiftCode::addGiftCodeExchanged(const std::string& code)
{
	Record record;
	record.code = code;
	record.time = SceneTimeTick::currentTime.sec();
	
	m_mapGiftCodeExchanged[code] = record;
}


// 是否领取过礼包码组
bool PlayerGiftCode::isGiftCodeGroupExchanged(const int& group) const
{
	auto iter = m_mapGiftCodeGroupExchanged.find(group);
	if (iter != m_mapGiftCodeGroupExchanged.end())
	{
		return true;
	}
	return false;
}

// 领取礼包码组
void PlayerGiftCode::addGiftCodeGroupExchanged(const int& group)
{
	GroupRecord record;
	record.group = group;
	record.time = SceneTimeTick::currentTime.sec();

	m_mapGiftCodeGroupExchanged[group] = record;
}

// 序列化
bool PlayerGiftCode::__Serialize(PbGiftCodeExchanged& pb)
{	
	for (auto iter = m_mapGiftCodeExchanged.begin(); iter != m_mapGiftCodeExchanged.end(); iter++)
	{
		const Record& rRecord = iter->second;

		PbGiftCodeExchanged::PbCodeRecord* pRecord = pb.add_coderecords();
		if (pRecord)
		{
			pRecord->set_code(rRecord.code);
			pRecord->set_time(rRecord.time);
		}
	}
	for (auto iter = m_mapGiftCodeGroupExchanged.begin(); iter != m_mapGiftCodeGroupExchanged.end(); iter++)
	{
		const GroupRecord& rRecord = iter->second;

		PbGiftCodeExchanged::PbGroupRecord* pRecord = pb.add_grouprecords();
		if (pRecord)
		{
			pRecord->set_group(rRecord.group);
			pRecord->set_time(rRecord.time);
		}
	}

	return true;
}

// 反序列化
bool PlayerGiftCode::__UnSerialize(const PbGiftCodeExchanged& pb)
{
	for (int i = 0; i < (int)pb.coderecords_size(); i++)
	{
		const PbGiftCodeExchanged::PbCodeRecord& rRecord = pb.coderecords(i);

		Record record;
		record.code = rRecord.code();
		record.time = rRecord.time();

		m_mapGiftCodeExchanged[record.code] = record;
	}

	for (int i = 0; i < (int)pb.grouprecords_size(); i++)
	{
		const PbGiftCodeExchanged::PbGroupRecord& rRecord = pb.grouprecords(i);

		GroupRecord record;
		record.group = rRecord.group();
		record.time = rRecord.time();

		m_mapGiftCodeGroupExchanged[record.group] = record;
	}

	return true;
}