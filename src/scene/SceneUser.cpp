#include "SceneUser.h"
#include "zMetaData.h"
#include <stdarg.h>
#include "SceneServer.h"
#include "zMetaData.h"
#include "TimeTick.h"
#include "SessionClient.h"
#include <zlib.h>
#include <bitset>
#include "RecordClient.h"
#include "firMD5.h"
#include <limits>
#include "RedisMgr.h"
#include "SceneUserManager.h"
#include "StringUtil.h"
#include "RedisMgr.h"
#include "HMessages.h"
#include "zGameEventDictionary.h"
#include "XmlConfig.h"
#include "zTime.h"
#include "SceneTaskManager.h"
#include "TopListSystem.h"

#include <json/json.h>
#include "proto/serialize.pb.h"

using namespace FIGHTING;

SceneUser::SceneUser(DWORD charid)
	: _charid(charid),
	m_player_giftcode(this),
	m_player_vars(this)
{
	bzero(lastSaveCharBaseMD5, MD5_LENGTH);
	bzero(lastSaveBinaryMD5, MD5_LENGTH);
	dwOnlineTime = 0;
}

SceneUser::~SceneUser()
{
}

/**
 * \brief 加载二进制数据
 *
 */
void SceneUser::setupBinaryArchive(const Serialize& binary)
{
	m_player_giftcode.load(binary);
	m_player_vars.load(binary.pb_player_vars());
}

/**
 * \brief 保存二进制数据
 *
 */
DWORD SceneUser::saveBinaryArchive(unsigned char *out , const int maxsize)
{
	Serialize binary;

	m_player_giftcode.save(binary);
	m_player_vars.save(*(binary.mutable_pb_player_vars()));

	binary.SerializeToArray(out,maxsize);
	return binary.ByteSize();
}


bool SceneUser::sendCmdToMe(const void *pstrCmd, const int nCmdLen)
{
	if ((DWORD)nCmdLen > zSocket::MAX_DATASIZE)
	{
		Fir::logger->error("发送消息过大:%d",nCmdLen);
		return false;
	}
	SceneTask *task = SceneTaskManager::getMe().getTaskByID(_gateid);
	if (!task)
	{
		Fir::logger->error("发送消息给玩家失败，未找到网关连接");
		return false;
	}

	return task->sendCmdToUser(getCharid(), pstrCmd, nCmdLen);
}

bool SceneUser::sendCmdToGateway(const void *pstrCmd, const int nCmdLen)
{
	if ((DWORD)nCmdLen > zSocket::MAX_DATASIZE)
	{    
		Fir::logger->error("发送消息过大:%d",nCmdLen);
		return false;
	} 
	return SceneTaskManager::getMe().broadcastByID(_gateid, pstrCmd, nCmdLen);
}

/**
 * \brief  保存角色数据到record服务器
 * \return 保存成功，返回TRUE,否则返回FALSE
 *
 */
bool SceneUser::save()
{
	Fir::logger->debug("[角色存档]，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());

	bool bNeedSave = false;
	// 存储到内存数据库
	if (!saveToMemoryDB(bNeedSave))
	{
		Fir::logger->debug("[角色存档]失败，存储到内存数据库失败，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());
		return false;
	}
	// 存储到数据库
	if (bNeedSave)
	{
		BUFFER_CMD(CMD::RECORD::t_WriteUser_SceneRecord, saveChar, zSocket::MAX_DATASIZE);
		saveChar->charid = _charid;
		recordClient->sendCmd(saveChar, sizeof(CMD::RECORD::t_WriteUser_SceneRecord));
	}

	return true;
}

bool SceneUser::saveToMemoryDB(bool& bNeedSave)
{
	if (!saveCharbaseToMemeryDB(bNeedSave))
		return false;
	if (!saveBinaryToMemoryDB(bNeedSave))
		return false;

	return true;
}

bool SceneUser::saveCharbaseToMemeryDB(bool& bNeedSave)
{
	CharBase charbase;

	unsigned char md5[MD5_LENGTH] = { 0 };
	// 检查数据是否改变
	if (SceneService::getMe().md5Verify)
	{
		MD5SPACE::MD5Data((const void *)(&charbase), sizeof(charbase), md5);
		if (memcmp((const void *)md5, (const void *)lastSaveCharBaseMD5, MD5_LENGTH) == 0)
		{
			Fir::logger->debug("[角色基础数据存档]未变更，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());
			return true;
		}
	}

	zMemDB* redishandle = zMemDBPool::getMe().getMemDBHandle(getCharid());
	if (redishandle == NULL)
	{
		Fir::logger->error("[角色基础数据存档]失败，连接内存数据库失败，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());
		return false;
	}
	if (!redishandle->setBin("charbase", getCharid(), "charbase", (const char*)&charbase, sizeof(charbase)))
	{
		Fir::logger->error("[角色基础数据存档]失败，同步内存数据库失败，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());
		return false;
	}
	bcopy(md5, lastSaveCharBaseMD5, MD5_LENGTH);
	bNeedSave = true;

	Fir::logger->debug("[角色基础数据存档]成功，角色ID = %u", getCharid());
	
	return true;
}

bool SceneUser::saveBinaryToMemoryDB(bool& bNeedSave)
{
	unsigned char buffer[MAX_UZLIB_CHAR] = { 0 };
	unsigned char buffer_compressed[zSocket::MAX_DATASIZE] = { 0 };

	// 得到二进制存档
	int binary_size = saveBinaryArchive(buffer, MAX_UZLIB_CHAR);
	//数据过大，内存越界
	if (binary_size >= MAX_UZLIB_CHAR)
	{
		Fir::logger->error("[角色二进制存档]失败，超过最大数值，角色ID = %u，昵称 = %s, 缓冲区大小 = %u，二进制数据大小 = %d", getCharid(), getNickname().c_str(), MAX_UZLIB_CHAR, binary_size);
		return false;
	}

	unsigned char md5[MD5_LENGTH] = { 0 };
	// 检查数据是否改变
	if (SceneService::getMe().md5Verify)
	{
		MD5SPACE::MD5Data((const void *)buffer, binary_size, md5);
		if (memcmp((const void *)md5, (const void *)lastSaveBinaryMD5, MD5_LENGTH) == 0)
		{
			Fir::logger->debug("[角色二进制存档]未变更，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());
			return true;
		}
	}

	uLongf binary_compressed_size = zSocket::MAX_DATASIZE;
	// 压缩数据
	int retcode = compress((Bytef *)buffer_compressed, &binary_compressed_size, (const Bytef *)buffer, (uLong)binary_size);
	switch (retcode)
	{
		case Z_OK:
		{
			Fir::logger->debug("[压缩档案数据]成功，角色ID = %u，昵称 = %s, 压缩前大小 = %d , 压缩后大小 = %d", getCharid(), getNickname().c_str(), binary_size, (int)binary_compressed_size);
		}
		break;
		case Z_MEM_ERROR:
		{
			Fir::logger->error("[压缩档案数据]失败，Z_MEM_ERROR，角色ID = %u，昵称 = %s, 压缩前大小 = %d , 压缩后大小 = %d", getCharid(), getNickname().c_str(), binary_size, (int)binary_compressed_size);
		} 
		return false;
		case Z_BUF_ERROR:
		{
			Fir::logger->error("[压缩档案数据]失败，Z_BUF_ERROR，角色ID = %u，昵称 = %s, 压缩前大小 = %d , 压缩后大小 = %d", getCharid(), getNickname().c_str(), binary_size, (int)binary_compressed_size);
		} 
		return false;
		default:
		{
			Fir::logger->error("[压缩档案数据]失败，未知原因，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());
		} 
		return false;
	}

	zMemDB* redishandle = zMemDBPool::getMe().getMemDBHandle(getCharid());
	if (redishandle == NULL)
	{
		Fir::logger->error("[角色二进制存档]失败，连接内存数据库失败，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());
		return false;
	}
	if (!redishandle->setBin("charbase", getCharid(), "allbinary", (const char*)buffer_compressed, binary_compressed_size))
	{
		Fir::logger->error("[角色二进制存档]失败，同步内存数据库失败，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());
		return false;
	}
	bcopy(md5, lastSaveBinaryMD5, MD5_LENGTH);
	bNeedSave = true;

	Fir::logger->debug("[角色二进制存档]成功，角色ID = %u，昵称 = %s", getCharid(), getNickname().c_str());

	return true;
}

void SceneUser::SendErrorMessage(WORD MsgId, WORD ErrId)
{   
	Packet pkt;
	bool IsSuccess = false;
	pkt << IsSuccess << ErrId;
	SendMessage2Ci(MsgId, pkt);
}   

bool SceneUser::reg(DWORD gateid, BYTE regtype)
{
	_gateid = gateid;
	_regtype = regtype;
	
	if (!loadFromDb())
	{
		Fir::logger->error("[注册玩家]失败，加载数据失败，角色ID[%u]", getCharid());
		return false;
	}
	if (!SceneUserManager::getMe().addUser(this))
	{
		Fir::logger->error("[注册玩家]失败，加入容器失败，角色ID[%u]", getCharid());
		return false;
	}
	// 普通注册
	if (_regtype == 0)
	{
		if (!regToGate())
		{
			Fir::logger->error("[注册玩家]失败，注册到网关失败，角色ID[%u]", getCharid());
			return false;
		}
		online();
	}

	Fir::logger->trace("[注册玩家]成功，角色ID[%u]，注册类型[%d]", getCharid(), int(regtype));
	return true;
}

bool SceneUser::unreg()
{
	// 普通注册
	if (_regtype == 0)
	{
		offline();
	}
	else
	{
		save();
	}

	if (!SceneUserManager::getMe().removeUser(this))
	{
		Fir::logger->error("[注销玩家]失败，退出容器失败，角色ID[%u]", getCharid());
		return false;
	}

	Fir::logger->trace("[注销玩家]成功，角色ID[%u]，注册类型[%d]", getCharid(), int(_regtype));
	return true;
}

// 注册到网关
bool SceneUser::regToGate()
{
	CMD::SCENE::t_Refresh_LoginScene ret_gate;
	ret_gate.accid = this->getAccountid();
	ret_gate.charid = this->_charid;

	return sendCmdToGateway(&ret_gate, sizeof(ret_gate));
}

bool SceneUser::loadFromDb()
{
	CharBase charbase;
	//if(!RedisMgr::getMe().get_charbase(_charid,charbase))
	//{
	//	Fir::logger->error("[登录]:%u,%u,%s 玩家注册场景，charbase数据异常",charbase.accountid,this->_charid,this->getNickname().c_str());
	//	return false;
	//}

	Serialize binary;

	//Fir::logger->info("[登录]:%u,%u,%s 玩家注册场景，loadFromDb,step1",this->accid,this->_charid,this->nickname);
	//if(!RedisMgr::getMe().get_binary(_charid,binary))
	//{
	//	Fir::logger->error("[登录]:%u,%u,%s 玩家注册场景，binary数据异常",charbase.accountid,this->_charid,this->getNickname().c_str());
	//	return false;
	//}
	//Fir::logger->info("[登录]:%u,%u,%s 玩家注册场景，loadFromDb,step2",this->accid,this->_charid,this->nickname);

	setupBinaryArchive(binary);
	return true;
}

void SceneUser::offline()
{
	Fir::logger->info("[退出]:玩家下线，账号ID = %u, 角色ID = %u", getAccountid(), getCharid());
	save();
}

bool SceneUser::online()
{
	this->onlineInit();

	Fir::logger->info("[登录]:玩家上线, 账号ID = %u，角色ID = %u", getAccountid(), getCharid());
	
	return true;
}

bool SceneUser::KickOut(LOGOUT_TYPE kick_type)
{
	CMD::SESSION::t_unregUser_SceneSession send;
	send.accid = getAccountid();
	send.type = kick_type;
	sessionClient->sendCmd(&send, sizeof(send));

	return true;
}

void SceneUser::onlineInit()
{
	// 触发每日刷新
	OnDailyRefreshEvent();
	// 同步每日信息
	SyncDailyInfo();
	// 同步任务变量
	m_player_vars.syncAll();

	// 更新排行榜
	updateUserTopList();
}

void SceneUser::updateUserTopList()
{
}


void SceneUser::quitUserTopList()
{
}

void  SceneUser::SendMessage2Ci(Packet& p)
{
	sendCmdToMe((const void*)&p[0],p.getPosition());
}

// 同步每日信息
void SceneUser::SyncDailyInfo()
{
	Packet pkt, FinalPkt;
	int iCount = 0;

	if (iCount > 0)
	{
		FinalPkt << iCount << pkt;
		SceneUserManager::getMe().SendMessage2Ci(_charid, ID_Gs2Ci_LobbySyncDailyInfo, FinalPkt);
	}
}

void SceneUser::RefreshDailyInfo(bool isMonthChanged, bool isWeekChanged)
{
    Packet pkt, FinalPkt;
    int iCount = 0;
	
	if (iCount > 0)
    {
		FinalPkt << iCount << pkt;
		SceneUserManager::getMe().SendMessage2Ci(_charid, ID_Gs2Ci_LobbyRefreshDailyInfo, FinalPkt);
    }
}

// 初始每日刷新事件
void SceneUser::OnDailyRefreshEvent()
{
	// 当前时间
	DWORD nowTime = SceneTimeTick::currentTime.sec();
	// 上次刷新时间
	DWORD lastRefresTime = /*(DWORD)cm.getCounter(LAST_TIME_DAILYREFRESH)*/0;

	Fir::logger->debug("[每日刷新]上次刷新时间：[%u]，当前时间[%u]", lastRefresTime, nowTime);
	// 是否换天
	if(nowTime <= lastRefresTime || zTimeTool::isSameDay(lastRefresTime, nowTime))
	{
		return;
	}
	// 更新时间
	//cm.setCounter(LAST_TIME_DAILYREFRESH, 0, nowTime);

	// 是否换月
	bool isMonthChanged = !zTimeTool::isSameMonth(lastRefresTime, nowTime);
	// 是否换周
	bool isWeekChanged = !zTimeTool::isSameWeek(lastRefresTime, nowTime);

	Fir::logger->debug("[每日刷新]，角色ID = %u", getCharid());
	// 刷新
	RefreshDailyInfo(isMonthChanged, isWeekChanged);
}

// 获得角色id
DWORD SceneUser::getCharid() const
{
	return _charid;
}

// 获取昵称
std::string SceneUser::getNickname() const
{
	return "";
}

// 获取账号id
DWORD SceneUser::getAccountid() const
{
	return 0;
}

// 获得平台
std::string SceneUser::getPlatfrom() const
{
	return "";
}