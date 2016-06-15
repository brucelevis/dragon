/**
 * \file
 * \version  $Id: RecordTask.cpp 64 2013-04-23 02:05:08Z  $
 * \author  ,@163.com
 * \date 2004年11月23日 10时20分01秒 CST
 * \brief 实现读档连接类
 *
 * 
 */
#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "RecordTask.h"
#include "Fir.h"
#include "zDBConnPool.h"
#include "zString.h"
#include "RecordCommand.h"
#include "SessionCommand.h"
#include "RecordServer.h"
#include "RecordUserManager.h"
#include "RecordUser.h"
#include "zMetaData.h"
#include <string>
#include <vector>
#include "RecordTaskManager.h"
#include "StringUtil.h"
#include "AccountMgr.h"
#include "TimeTick.h"
#include "zBILog.h"
#include "XmlConfig.h"
#include "RecordTopList.h"
#include "GmManager.h"
#include "proto/monitor_protocol.pb.h"

/**
 * \brief 验证登陆档案服务器的连接指令
 *
 * 如果验证不通过直接断开连接
 *
 * \param ptCmd 登陆指令
 * \return 验证是否成功
 */
bool RecordTask::verifyLogin(const CMD::RECORD::t_LoginRecord *ptCmd)
{
	using namespace CMD::RECORD;

	if (CMD_LOGIN == ptCmd->cmd
			&& PARA_LOGIN == ptCmd->para)
	{
		const CMD::SUPER::ServerEntry *entry = RecordService::getMe().getServerEntry(ptCmd->wdServerID);
		char strIP[32];
		strncpy(strIP, getIP(), sizeof(strIP)-1);
		if (entry
				&& ptCmd->wdServerType == entry->wdServerType
				&& 0 == strcmp(strIP, entry->pstrIP))
		{
			wdServerID = ptCmd->wdServerID;
			wdServerType = ptCmd->wdServerType;

			return true;
		}
	}

	return false;
}

/**
 * \brief 等待接受验证指令并进行验证
 *
 * 实现虚函数<code>zTCPTask::verifyConn</code>
 *
 * \return 验证是否成功，或者超时
 */
int RecordTask::verifyConn()
{
	int retcode = mSocket.recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//这里只是从缓冲取数据包，所以不会出错，没有数据直接返回
			return 0;
		else
		{
			using namespace CMD::RECORD;
			if (verifyLogin((t_LoginRecord *)pstrCmd))
			{
				Fir::logger->debug("客户端连接通过验证");
				return 1;
			}
			else
			{
				Fir::logger->error("客户端连接验证失败");
				return -1;
			}
		}
	}
	else
		return retcode;
}

/**
 * \brief 确认一个服务器连接的状态是可以回收的
 *
 * 当一个连接状态是可以回收的状态，那么意味着这个连接的整个生命周期结束，可以从内存中安全的删除了：）<br>
 * 实现了虚函数<code>zTCPTask::recycleConn</code>
 *
 * \return 是否可以回收
 */
int RecordTask::recycleConn()
{
	Fir::logger->debug("关闭服务器:%u", getID());
	//TODO 需要保证存档指令处理完成了
	//
	return 1;
}

bool RecordTask::uniqueAdd()
{
	return RecordTaskManager::getMe().uniqueAdd(this);
}

bool RecordTask::uniqueRemove()
{
	return RecordTaskManager::getMe().uniqueRemove(this);
}

bool RecordTask::msgParse_Session(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::RECORD;

	switch(ptNullCmd->para)
	{
		case PARA_SESSION_GM_REQUEST:
			{
				const CMD::RECORD::t_GmRequest_SessionRecord* cmd = (const CMD::RECORD::t_GmRequest_SessionRecord*)ptNullCmd;
				return this->reqGm(cmd, nCmdLen);
			}
			break;
		default:
			break;
	}

	Fir::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool RecordTask::msgParse_Super(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	return false;
}

//0成功 1失败 2账号已有角色 3角色名称重复
DWORD RecordTask::create_role_inner(DWORD accountid, const char* nickname, WORD profession, const char* platform, DWORD& charid)
{
	Fir::logger->info("%s,accountid=%u,nickname=%s,profession=%u", __PRETTY_FUNCTION__, accountid, nickname, (DWORD)profession);

	// 检查昵称是否已存在
	if (RecordUserM::getMe().existNickname(nickname))
	{
		return 3;
	}

	zRTime currentTime;
	DWORD energy = XmlConfig::getMe().getInitItem().energy;
	DWORD weapon =  XmlConfig::getMe().getInitItem().weapon;
	DWORD suit = XmlConfig::getMe().getInitItem().suit;

	std::string nicknameEscaped;
	RecordService::dbConnPool->escapeString(nickname, nicknameEscaped);

	std::string strsql = StringUtil::Format("call insert_player_role(%u, %u, '%s', %u, %u, %u, %u, %u ,'%s' );", RecordService::getMe().getZoneId(), accountid, nicknameEscaped.c_str(), (DWORD)profession, energy, weapon, suit, currentTime.sec(), platform);
	Fir::logger->info("%s,sql=%s",__PRETTY_FUNCTION__,strsql.c_str());
	
	RecordSetPtr pResult = RecordService::dbConnPool->exeSelect(strsql);
	if (NULL == pResult || pResult->empty())
	{
		Fir::logger->error("[创建角色], 结果集为空, sql=%s",strsql.c_str());
		return 1;
	}
	Record* rec = pResult->get(0);
	if (NULL == rec)
	{
		Fir::logger->error("[创建角色], 结果集为空, sql=%s",strsql.c_str());
		return 1;
	}
	charid = rec->get("retPlayerId");

	Fir::logger->trace("[创建角色], 账号ID = %u, 角色ID = %u", accountid, charid);

	RecordUser* u = FIR_NEW RecordUser();
	if (NULL == u)
	{
		Fir::logger->error("%s, 分配RecordUser内存错误, 角色ID = %u", __PRETTY_FUNCTION__, charid);
		return 1;
	}
	u->charid = charid;

	if (!u->readCharBase())
	{
		Fir::logger->error("[创建角色],读取档案失败");
		return 1;
	}

	if (!RecordUserM::getMe().add(u))
	{
		Fir::logger->error("[创建角色],添加用户管理器失败");
		SAFE_DELETE(u);
		return 1;
	}

	if (u->syncBaseMemDB())
	{
	}
	else
	{
		return 1;
	}

	AccountMgr::getMe().syncMemDbAccountRoles(accountid);

	return 0;
}

bool RecordTask::create_role(const CMD::RECORD::t_CreateChar_GateRecord* rev)
{
	if(!rev) return false;

	Fir::logger->info("[创建角色],accountid = %u, nickname = %s, profession = %u", rev->accountid, rev->nickname, rev->profession);

	CMD::RECORD::t_CreateChar_Return_GateRecord ret;
	ret.accountid = rev->accountid;
	ret.retcode = create_role_inner(rev->accountid, rev->nickname, rev->profession, rev->platform, ret.charid);

	sendCmd(&ret, sizeof(ret));

	return true;
}

bool RecordTask::del_role(const CMD::RECORD::t_DelChar_GateRecord* cmd)
{
	if (!cmd)
	{
		return false;
	}

	CMD::RECORD::t_DelChar_Return_GateRecord ret;
	ret.accountid = cmd->accountid;
	ret.charid = cmd->charid;
	ret.retcode = 1;

	AccountItem item;
	if (!AccountMgr::getMemDbAccountItem(cmd->accountid, item))
	{   
		Fir::logger->error("[删除角色]，获取账号数据失败，accountid=%u,charid=%u",cmd->accountid,cmd->charid);
		return false;
	}

	if (item.playerid1 == cmd->charid)
	{
		std::string strsql = StringUtil::Format("update t_account set f_playerid1=0 where f_accountid=%u",cmd->accountid);
		RecordService::dbConnPool->execSql(strsql.c_str(), strsql.length());
		item.playerid1 = 0;
	}
	else if (item.playerid2 == cmd->charid)
	{
		std::string strsql = StringUtil::Format("update t_account set f_playerid2=0 where f_accountid=%u",cmd->accountid);
		RecordService::dbConnPool->execSql(strsql.c_str(), strsql.length());
		item.playerid2 = 0;
	}
	else if (item.playerid3 == cmd->charid)
	{
		std::string strsql = StringUtil::Format("update t_account set f_playerid3=0 where f_accountid=%u",cmd->accountid);
		RecordService::dbConnPool->execSql(strsql.c_str(), strsql.length());
		item.playerid3 = 0;
	}
	else if (item.playerid4 == cmd->charid)
	{
		std::string strsql = StringUtil::Format("update t_account set f_playerid4=0 where f_accountid=%u",cmd->accountid);
		RecordService::dbConnPool->execSql(strsql.c_str(), strsql.length());
		item.playerid4 = 0;
	}
	else
	{
		Fir::logger->error("[删除角色],账号找不到角色，accountid=%u,charid=%u",cmd->accountid,cmd->charid);
		sendCmd(&ret, sizeof(ret));
		return false;
	}

    AccountMgr::getMe().syncMemDbAccountItem(item);
	ret.retcode = 0; // 成功
	sendCmd(&ret, sizeof(ret));
	return true;
}

void writeGlobalVar(const CMD::RECORD::t_WriteGlobalVar_SceneRecord* cmd)
{
	std::ostringstream oss;
	oss << "replace into t_globalvars(f_key, f_value) values(" << cmd->key << ", " << cmd->value << ")";
	std::string strsql = oss.str();

	unsigned int retcode = RecordService::dbConnPool->exeUpdate(strsql);
	if (((unsigned int)-1) == retcode)
	{
		Fir::logger->error("[全局变量]写入失败，键 = %llu，值 = %lld", cmd->key, cmd->value);
		return;
	}
	Fir::logger->trace("[全局变量]写入成功，键 = %llu，值 = %lld", cmd->key, cmd->value);
}

bool RecordTask::msgParse_Gateway(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
#ifdef _CMD_DEBUG
	PrintDataRecv(ptNullCmd, nCmdLen);
#endif
	using namespace CMD::RECORD;

	switch(ptNullCmd->para)
	{
		case PARA_GATE_CREATECHAR:
			{
				t_CreateChar_GateRecord* rev = (t_CreateChar_GateRecord*)ptNullCmd;
				create_role(rev);
				return true;
			}
			break;
		case PARA_GATE_DELCHAR:
			{
				t_DelChar_GateRecord* rev = (t_DelChar_GateRecord*)ptNullCmd;
				del_role(rev);
				return true;
			}
		default:
			break;
	}

	Fir::logger->info("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

bool RecordTask::msgParse_Scene(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if (NULL == ptNullCmd)
	{
		Fir::logger->error("[%s],NULL == ptNullCmd", __PRETTY_FUNCTION__);
		return false;
	}
	using namespace CMD::RECORD;
#ifdef _CMD_DEBUG
	PrintDataRecv(ptNullCmd, nCmdLen);
#endif
	switch(ptNullCmd->para)
	{
		case PARA_SCENE_USER_WRITE:
			{
				t_WriteUser_SceneRecord* rev = (t_WriteUser_SceneRecord*)ptNullCmd;
				RecordUser* u = RecordUserM::getMe().getUserByCharid(rev->charid);
				if (u)
				{
					if (u->refreshSaveBase(rev))
					{
					}
					else
					{
						Fir::logger->error("回写档案验证失败，不能回写档案,charid=%u", rev->charid);
					}
				}
				else
				{
					Fir::logger->error("回写档案验证失败，不能回写档案：charid=%u，玩家在RECORDUSERM管理器里找不到", rev->charid);
				}
			
				return true;
			}
			break;
		case PARA_SCENE_TOPLIST_WRITE:
			{
				t_WriteToplist_SceneRecord* rev = (t_WriteToplist_SceneRecord*)ptNullCmd;
				// 处理排行榜定时写库
				RecordTopList::getMe().saveDataToDB(rev);
				return true;
			}
			break;
		case PARA_SCENE_GLOBALVAR_WRITE:
			{
				t_WriteGlobalVar_SceneRecord* rev = (t_WriteGlobalVar_SceneRecord*)ptNullCmd;
				writeGlobalVar(rev);
			}
			break;			
		default:
			break;
	}

	Fir::logger->warn("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief 解析来自各个服务器连接的指令
 *
 * \param ptNullCmd 待处理的指令
 * \param nCmdLen 指令长度
 * \return 处理是否成功
 */
bool RecordTask::msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::RECORD;

#ifdef _COMMAND_TIME
	CommandTime func_time(RecordService::getMe().timeCmdLog, "RecordTask", ptNullCmd->cmd, ptNullCmd->para);
#endif

	switch(ptNullCmd->cmd)
	{
		case CMD_GATE:
			if (msgParse_Gateway(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		case CMD_SCENE:
			if (msgParse_Scene(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		case CMD_SESSION:
			if (msgParse_Session(ptNullCmd, nCmdLen))
			{
				return true;
			}
			break;
		default:
			break;
	}

	Fir::logger->info("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

// 请求gm信息
bool RecordTask::reqGm(const CMD::RECORD::t_GmRequest_SessionRecord* cmd, const DWORD len)
{
	if (!cmd)
		return false;

	PbGmRequest binary;
	binary.ParseFromArray(cmd->data, cmd->datasize);

	zProperties paras;
	for (int i=0; i<binary.pb_items_size(); i++)
	{
		const PbPropertyItem& pbitem = binary.pb_items(i);
		paras[pbitem.name()] = pbitem.value();
	}

	GmManager::getMe().handleCmd(this->getID(), binary.httpid(), paras);
	this->getID();
	return true;
}