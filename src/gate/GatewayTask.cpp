/**
* \file
* \version  $Id: GatewayTask.cpp 67 2013-04-23 09:44:20Z  $
* \author  ,@163.com
* \date 2004年11月23日 10时20分01秒 CST
* \brief 定义登陆连接任务
*
*/


#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <unordered_map>
#include <stdlib.h>
#include <iconv.h>

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "GatewayTask.h"
#include "GatewayServer.h"
#include "SessionClient.h"
#include "RecordClient.h"
#include "SceneCommand.h"
#include "SceneClient.h"
#include "GatewayTaskManager.h"
#include "TimeTick.h"
#include "WordFilter.h"
#include "EncDec/EncDec.h"
#include "HMessages.h"
#include "StringUtil.h"
#include "zBILog.h"
#include "hellmd5.h"

using namespace FIGHTING;

static QWORD getNextSeq()
{
	static QWORD s_seq = 0;
	return ++s_seq;
}

BYTE getRegType(WORD msgId)
{
	switch (msgId)
	{
	case ID_Ci2Gs_PlayerLoginCommon:
		return 0;
		break;
	case ID_Ci2Gs_PlayerLoginQuick:
		return 1;
		break;
	case ID_Ci2As_ReConnectAgentServer:
		return 2;
		break;
	default:
		return -1;
	}
}

WORD getLoginRetMsg(BYTE regtype)
{
	switch (regtype)
	{
	case 0:
		return ID_Ls2Ci_PlayerLoginCommon_Reply;
	case 1:
		return ID_Ls2Ci_PlayerLoginQuick_Reply;
	case 2:
		return ID_Gs2Ci_ReConnectAgentServer_Reply;
	default:
		return -1;
	}
}

bool getMemDbAccountItem(DWORD accid, AccountItem& item)
{
	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle)
	{
		Fir::logger->error("[获取账号数据]，找不到内存数据库，账号ID = %u,",accid);
		return false;
	}

	if (handle->getBin("account_item",accid,"account_item",(char*)&item) == 0)
	{
		item.accountid = accid;
	}

	return true;
}

bool syncMemDbAccountItem(const AccountItem& item)
{
	zMemDB* redishandle = zMemDBPool::getMe().getMemDBHandle();
	if (redishandle==NULL)
	{   
		Fir::logger->error("[同步账号数据]，获取内存数据库失败，账号ID = %u",item.accountid);
		return false;
	}   

	if (!redishandle->setBin("account_item", item.accountid, "account_item", (const char*)&item, sizeof(item)))
	{   
		Fir::logger->error("[同步账号数据],同步内存数据库account_item失败，账号ID = %u", item.accountid);
		return false;
	}   

	return true;
}

std::string genToken32(UINT32 accid, UINT32 tokenTime, UINT32 tokenRand, std::string platform)
{
	std::string str = StringUtil::Format("%010d%08X%06X%s%s", accid, tokenTime, tokenRand, KEY_STR.c_str(),platform.c_str());
	HellMd5  md5(str);
	return  md5.md5();
}

bool verifyToken(UINT32 accid, std::string token, UINT32 tokenTime, UINT32 tokenRand,std::string platform)
{
	// 验证token时效性
	DWORD timeoutTime = GatewayTaskManager::getMe().GetTokenTime();
	DWORD now = GatewayTimeTick::currentTime.sec();
	if (now > tokenTime && now - tokenTime > timeoutTime)
	{
		Fir::logger->info("[登陆Token验证]失败，token验证超时，账号ID = %u, 当前时间 = %u, Token时间 = %u, 超时时间 = %u", accid, now, tokenTime, timeoutTime);
		return false;
	}		
	// 验证token
	std::string temp = genToken32(accid, tokenTime, tokenRand, platform);
	if (temp.compare(token.c_str()) != 0)
	{		
		Fir::logger->warn("[登陆Token验证]失败，token已被篡改, 账号ID = %u, 验证Token = %s, 计算Token = %s", accid, token.c_str(), temp.c_str());
		return false;
	}

	return true;
}

bool verifyTokenReconn(UINT32 accid, std::string token)
{
	AccountItem item;
	if (!getMemDbAccountItem(accid, item))
	{
		Fir::logger->info("[重连Token验证]失败，获取账号数据失败，账号ID = %u", accid);
		return false;
	}
	if (token.compare(item.token) != 0)
	{
		Fir::logger->warn("[重连Token验证]，token验证不匹配，账号ID = %u, 验证Token = %s, 存储Token = %s", accid, token.c_str(), item.token);
		return false;
	}
	// 时效性由账号生存时间控制

	return true;
}

void updateTokenTime(UINT32 accid)
{
	AccountItem item;
	if (!getMemDbAccountItem(accid, item))
	{                                                
		Fir::logger->error("[更新Token时间]失败，获取账号数据失败，账号ID = %u", accid);
		return;
	}

	item.tokentime = GatewayTimeTick::currentTime.sec();
	syncMemDbAccountItem(item);

	Fir::logger->trace("[更新Token时间]，账号ID = %u，Token = %s, Token时间 = %u", accid, item.token, item.tokentime);
}

void updateToken(UINT32 accid, std::string token)
{
	AccountItem item;
	if (!getMemDbAccountItem(accid, item))
	{                                                
		Fir::logger->error("[更新Token]失败，获取账号数据失败，账号ID = %u", accid);
		return;
	}

	strncpy(item.token, token.c_str(), MAX_TOKENSIZE);
	item.tokentime = GatewayTimeTick::currentTime.sec();
	syncMemDbAccountItem(item);

	Fir::logger->trace("[更新Token]，账号ID = %u，Token = %s, Token时间 = %u", accid, item.token, item.tokentime);
}


static void copyChooseRoleData(choose_role_data& dest, const CharBase& src)
{
	dest.player_id = src.charid;
	std::string nickname(src.nickname);
	dest.player_name = nickname;
	dest.profession = src.profession;
	dest.level = src.level;
	dest.weapon = src.current_weapon;
	dest.suit = src.current_suit;
}

/**
 * \brief 检查新角色名字是否合法
 * \param name 角色名字
 * \return 是否合法
 */
bool checkRoleName(const char* name)
{
	bool ret = true;

	unsigned char* new_name = zMisc::stringConv((unsigned char*)name, "UTF-8", "GBK");
	if (!new_name) return false;

	size_t i = 0;
	while (new_name[i] != 0 && i < strlen((char*)new_name) && i < MAX_NAMESIZE)
	{
		BYTE a = new_name[i];
		if (a < 0x80)//asc码
		{
			if (!((a >= '0' && a <= '9') || (a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z'))) 
			{ 
				ret = false;
				break; 
			}
		}
		else//汉字
		{
			if (!(i < strlen((char*)new_name) - 1 && i < (MAX_NAMESIZE / 2 - 1))) 
			{
				ret = false;
				break; 
			}

			BYTE b = new_name[i+1];
			if (b == 0x7F || b < 0x40 || b > 0xFE)//尾字节在 40-FE 之间，剔除 xx7F 一条线
			{
				ret = false;
				break;
			}

			WORD val = (a<<8)|b;
			if (!((val >= 0xB0A1 && val <= 0xF7FE)))//GB 2312 汉字 6763 个
					//||(val>=0xB140 && val<=0xA0FE)//CJK汉字 6080 个
					//||(val>=0xAA40 && val<=0xFEA0)))//CJK 汉字和增补的汉字 8160 个
			{
				ret = false;
				break;
			}
			i++;
		}
		i++;
	}

	// 禁用字符过滤
	if (!WordFilter::getMe().doFilter((char*)name)) 
	{
		return false;
	}

	if (new_name) SAFE_DELETE_VEC(new_name);

	return ret;
}

GatewayTask::GatewayTask(zTCPTaskPool *pool,const int sock, const struct sockaddr_in *addr, const bool compress) 
	: zTCPTask(pool, sock, addr, compress, true, true)
	, _seqid(getNextSeq())
	, _curr_state(TASK_STATE_NONE)
	, analysis_("客户端指令分析", 10, 200, 200*64)
{
	_user = NULL;
	_accid = 0;
	_charid = 0;
	_loginType = 0;
	_loginTime = 0;
	_platform = "";

	//mSocket.setEncMethod(CEncrypt::ENCDEC_RC5);
	//mSocket.enc.set_key_rc5((const unsigned char *)Fir::global["rc5_key"].c_str(), 16, 12);
	//unsigned char key[16] = {28, 196, 25, 36, 193, 125, 86, 197, 35, 92, 194, 41, 31, 240, 37, 223};
	//unsigned char key[16] = { 0x2c,0xc5,0x29,0x25,0xd1,0x7c,0xa6,0xc6,0x33,0x5d,0xd2,0x2a,0x2f,0xf1,0x35,0xdf};
	//unsigned char key[16] = { 0x3f,0x79,0xd5,0xe2,0x4a,0x8c,0xb6,0xc1,0xaf,0x31,0x5e,0xc7,0xeb,0x9d,0x6e,0xcb};
	//mSocket.set_key_rc5((const unsigned char *)key, 16, 12);

	//设置zSocket的加密密钥
	//DES_cblock des_key = {'a','a','a','a','a','a','a','a'};
	//mSocket.setEncMethod(CEncrypt::ENCDEC_DES);
	//mSocket.set_key_des(&des_key);

	unsigned char key[] = {25, 64, 1, 45, 54, 45, 128, 19};
	mSocket.setEncMethod(CEncrypt::ENCDEC_RC4);
	mSocket.set_key_rc4((const unsigned char *)key, sizeof(key));
	
	// 开启指令分析
	analysis_.on();

	Fir::logger->debug("[构造连接]，连接序号 = %llu", getID());
}

/**
* \brief 析构函数
*
*/
GatewayTask::~GatewayTask()
{
	Fir::logger->debug("[销毁连接]，连接序号 = %llu", getID());
	SAFE_DELETE(_user);
}

/**
 * \brief 验证检查负载指令指令
 * 
 * \param ptNullCmd 待验证的指令
 * \return 验证指令是否成功
 */
bool GatewayTask::verifyMaxLoad(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	BYTE regType = getRegType(ptNullCmd->_id);

	// 重连不计算负载
	if (regType == 2 || !GatewayService::getMe().isReachFullLoad())
		return true;

	Packet p;
	p << (WORD)getLoginRetMsg(regType) << false << WORD(2692);
	SendMessage2Ci(p);

	return false;
}

/**
 * \brief 验证登陆网关指令
 * 
 * \param ptNullCmd 待验证的登陆指令
 * \return 验证登陆指令是否成功
 */
bool GatewayTask::verifyACCID(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	Packet p;
	p.resize(nCmdLen);
	memcpy(&p[0], (const char*)ptNullCmd, nCmdLen);
	p.SeekSet(sizeof(CMD::t_NullCmd));

	DWORD accid = 0;
	DWORD charid = 0;
	DWORD tokenTime = 0;
	DWORD tokenRand = 0;
	std::string token = "";
	std::string platform = "";
	p >> accid >> charid >> token >> tokenTime >> tokenRand >> platform; 

	BYTE regType = getRegType(ptNullCmd->_id);

	bool bVerified = false;
	do 
	{
		if (regType == -1)
			break;
		if (regType == 2)
		{
			if (!GateUserManager::getMe().getUserAccount(accid))
				break;
			// 重连验证
			if (!verifyTokenReconn(accid, token))
				break;
		}
		else
		{
			// 登陆验证
			if (!verifyToken(accid, token, tokenTime, tokenRand, platform))
			{
				//
				// 支持切换角色等重登功能
				//
				if (!GateUserManager::getMe().getUserAccount(accid))
					break;
				// 重连验证
				if (!verifyTokenReconn(accid, token))
					break;
			}
		}
		bVerified = true;
	} while (false);

	if (!bVerified)
	{
		Packet p;
		p << (WORD)getLoginRetMsg(regType) << false << WORD(17999);
		SendMessage2Ci(p);

		Fir::logger->info("[登录失败]，账号验证失败，账号ID = %u, 验证类型 = %d，消息 = %d", accid, (int)regType, ptNullCmd->_id);
		return false;
	}

	Fir::logger->trace("[账号验证]成功，网关序号 = %llu, 账号ID = %u，IP地址 = %s，端口 = %u，验证类型 = %d, Token = %s, 平台 = %s", getID(), accid, getIP(), getPort(), (int)regType, token.c_str(), platform.c_str());

	// 记录账号ID
	setACCID(accid);
	// 记录登陆平台
	setPlatform(platform);
	// 记录登录类型
	setLoginType(regType);
	// 更新Token时间
	updateToken(accid, token);

	return true;
}

/**
* \brief 连接确认
*
* \return 如果没有数据返回0,验证成功返回1,验证失败返回-1
*/
int GatewayTask::verifyConn()
{
	int retcode = mSocket.recvToBuf_NoPoll();
	if (retcode > 0)
	{
		unsigned char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));

		if (nCmdLen <= 0)
		{
#ifdef _DEBUG_WCX
			Fir::logger->debug("没有数据");
#endif
			//这里只是从缓冲取数据包，所以不会出错，没有数据直接返回
			return 0;
		}
		else if (nCmdLen < (int)sizeof(CMD::t_NullCmd))
		{
			Fir::logger->warn("服务器接收到错误大小的数据包，数据包大小 = %u", nCmdLen);
			return -1;
		}
		else
		{
			const CMD::t_NullCmd *ptNullCmd = (CMD::t_NullCmd *)pstrCmd;
			// 请求序号检查
			if (!checkReq(ptNullCmd->_req))
			{
				Fir::logger->warn("服务器接收消息请求序号错误，服务端序号 = %u，客户端序号 = %u", getReq(), ptNullCmd->_req);
				return -1;
			}
			if (verifyMaxLoad(ptNullCmd, nCmdLen) && verifyACCID(ptNullCmd, nCmdLen))
				return 1;
			else
				return -1;
		}
	}
	else
		return retcode;
}

int GatewayTask::recycleConn()
{
	if (isUnique() && !isLogoutOK())
		return 0;

	return 1;
}

/**
* \brief 需要主动断开客户端的连接
*
* \param method 连接断开方式
*/
void GatewayTask::Terminate(const TerminateMethod method)
{
	zTCPTask::Terminate(method);

	Fir::logger->trace("[断开连接] method = %d，网关序号 = %llu, 账号ID = %u，IP地址 = %s，端口= %u", (int)method, getID(), getACCID(), getIP(), getPort());
}

void GatewayTask::addToContainer()
{
	regToSession();
}

void GatewayTask::removeFromContainer()
{
	unregFromSession();
}

/**
* \brief 请求注册到Session
*
*/
void GatewayTask::regToSession()
{
	if (isLoginWait() || isLogoutOK())
	{
		return ;
	}
	loginWait();

	CMD::SESSION::t_regUser_GateSession send;
	send.seqid = _seqid;
	send.accid = _accid;
	send.regtype = getLoginType();

	sessionClient->sendCmd(&send,sizeof(send));

	Fir::logger->trace("[会话注册]申请，网关序号 = %llu, 账号ID = %u，登录类型 = %u", _seqid, _accid, getLoginType());
}

/**
* \brief 请求从Session注销
*
*/
void GatewayTask::unregFromSession()
{
	if (isLogoutWait() || isLogoutOK())
	{
		return ;
	}
	logoutWait();

	CMD::SESSION::t_unregUser_GateSession send;
	send.seqid = getID();
	send.accid= getACCID();

	sessionClient->sendCmd(&send,sizeof(send));

	Fir::logger->trace("[会话注销]请求，网关序号 = %llu, 账号ID = %u", getID(), getACCID());
}

/**
* \brief 注册到Session
*
*/
void GatewayTask::onRegToSession(BYTE retcode, DWORD accid, BYTE regtype)
{
	Fir::logger->trace("[会话注册]返回，网关序号 = %llu, 账号ID = %u，登录类型 = %u", getID(), accid, regtype);

	if (retcode == 0)
	{
		onLoginSuccess(accid, regtype);
	}
	else
	{
		onLoginFailed(accid, regtype);
	}
}

/**
* \brief 从Session注销
*
*/
void GatewayTask::onUnRegFromSession(BYTE unregtype)
{
	Fir::logger->trace("[会话注销]成功，网关序号 = %llu, 账号ID = %u，登录类型 = %u", _seqid, _accid, getLoginType());

	if (_user)
	{
		_user->unreg(unregtype);
	}

	logoutOK();

	if (!isTerminateWait() && !isTerminate())
	{
		TerminateWait();
	}
}

void GatewayTask::onLoginSuccess(DWORD accid, BYTE regtype)
{
	GateUser* u = NULL;

	if (regtype == 2) 
	{
		u = GateUserManager::getMe().getUserAccount(accid);
		if (NULL == u)
		{
			TerminateWait();

			Fir::logger->warn("[登录重连]失败，账号已不存在，账号ID = %u", accid);
			return;
		}
		u->recon(this);
		onRconnSuccess();

		Fir::logger->trace("[登录重连]成功，账号ID = %u", accid);
	}
	else
	{
		u = FIR_NEW GateUser(accid, this);
		if (NULL == u)
		{
			TerminateWait();

			Fir::logger->error("%s,分配GateUser内存错误,账号ID = %u", __PRETTY_FUNCTION__, accid);
			return;
		}
		u->reg(this);

		if (regtype == 0)
		{
			onCommonLoginSuccess();
			Fir::logger->trace("[普通登录]成功,账号ID = %u", accid);
		}
		else if (regtype == 1)
		{
			onQuickLoginSuccess();
			Fir::logger->trace("[快速登录]成功,账号ID = %u", accid);
		}
	}
	loginOK();

	// 记录登录时间
	setLoginTime(GatewayTimeTick::currentTime.sec());

	Fir::logger->trace("[登录成功]，网关序号 = %llu, 账号ID = %u，登录类型 = %u", _seqid, accid, regtype);
}

void GatewayTask::onLoginFailed(DWORD accid, BYTE regtype)
{
	Fir::logger->trace("[登录失败]，网关序号 = %llu, 账号ID = %u，登录类型 = %u", _seqid, _accid, getLoginType());

	Packet p;
	p << (WORD)getLoginRetMsg(regtype) << false << WORD(2693);
	SendMessage2Ci(p);

	logoutOK();
	TerminateWait();
}

void GatewayTask::onQuickLoginSuccess()
{
	AccountItem item;

	if (!getMemDbAccountItem(_accid, item))
	{
		Fir::logger->trace("[快速登录]失败，网关序号 = %llu, 账号ID = %u", _seqid, _accid);
		return;
	}

	std::set<DWORD> setCharids;
	setCharids.insert(item.playerid1);
	setCharids.insert(item.playerid2);
	setCharids.insert(item.playerid3);
	setCharids.insert(item.playerid4);
	setCharids.erase(0);

	Packet p;
	p << true;
	p << (DWORD)setCharids.size();
	for(auto it = setCharids.begin(); it != setCharids.end(); it++)
	{
		p << *it;
	}
	SendMessage2Ci(FIGHTING::ID_Ls2Ci_PlayerLoginQuick_Reply,p);
}

void GatewayTask::onCommonLoginSuccess()
{
	AccountItem item;

	if (!getMemDbAccountItem(_accid, item))
	{
		Fir::logger->trace("[普通登录]失败，网关序号 = %llu, 账号ID = %u", _seqid, _accid);
		return;
	}

	std::set<DWORD> setCharids;
	setCharids.insert(item.playerid1);
	setCharids.insert(item.playerid2);
	setCharids.insert(item.playerid3);
	setCharids.insert(item.playerid4);
	setCharids.erase(0);


	zMemDB* handle = zMemDBPool::getMe().getMemDBHandle();
	if (!handle)
	{
		Fir::logger->error("[登录],找不到内存数据库，accountid=%u,charid=%u",_accid,_charid);
		return;
	}

	std::vector<choose_role_data> vecRoles;
	for(auto itr=setCharids.begin(); itr!=setCharids.end(); itr++)
	{
		DWORD charid = *itr;
		CharBase charbase;

		if (handle->getBin("charbase",charid,"charbase",(char*)&charbase) == 0)
		{
			Fir::logger->error("[登录]，获取charbase失败，accountid=%u,charid=%u",_accid, charid);
			return;
		}
		choose_role_data target;
		copyChooseRoleData(target,charbase);
		vecRoles.push_back(target);
	}

	Packet p;
	p << true;
	p << (DWORD)vecRoles.size();
	for(DWORD i=0; i<vecRoles.size(); i++)
	{
		p << vecRoles[i];
	}
	SendMessage2Ci(FIGHTING::ID_Ls2Ci_PlayerLoginCommon_Reply,p);
}

void GatewayTask::onRconnSuccess()
{
	Packet p;
	p << true;
	SendMessage2Ci(FIGHTING::ID_Gs2Ci_ReConnectAgentServer_Reply,p);
}

bool GatewayTask::createRole(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(NULL == _user) return false;

	if(!_user->isCreateState())
	{	
		Fir::logger->warn("[创建角色]失败，不在创建状态，账号ID = %u", getACCID());
		return false;
	}

	AccountItem item;
	if (!getMemDbAccountItem(getACCID(), item))
	{
		Fir::logger->error("[创建角色]失败，获取账号数据失败，账号ID = %u", getACCID());
		return false;
	}

	std::set<DWORD> setCharids;
	setCharids.insert(item.playerid1);
	setCharids.insert(item.playerid2);
	setCharids.insert(item.playerid3);
	setCharids.insert(item.playerid4);
	setCharids.erase(0);

	if (setCharids.size() >= 4)
	{
		Fir::logger->warn("[创建角色]失败，角色已满，账号ID = %u", getACCID());
		return false;
	}

	Packet p;
	p.resize(nCmdLen);
	memcpy(&p[0], (const char*)ptNullCmd, nCmdLen);
	p.SeekSet(sizeof(CMD::t_NullCmd));

	std::string nickname;
	WORD profession = 0;
	p >> nickname >> profession;

	if (nickname.empty() || nickname.size() >= MAX_NAMESIZE)
	{
		Packet send;
		send << false << 2693;
		SendMessage2Ci(FIGHTING::ID_Ls2Ci_RequestCreateRole_Reply, send);

		Fir::logger->info("[创建角色]失败，无效的昵称，账号ID = %u，昵称 = %s", getACCID(), nickname.c_str());
		return false;
	}
	// 敏感字符过滤
	if (!checkRoleName(nickname.c_str()))
	{
		Packet send;
		send << false << 22625;
		SendMessage2Ci(FIGHTING::ID_Ls2Ci_RequestCreateRole_Reply, send);

		Fir::logger->info("[创建角色]失败，出现敏感字符，账号ID = %u，昵称 = %s", getACCID(), nickname.c_str());
		return false;
	}

	Fir::logger->trace("[创建角色]，账号ID = %u, 昵称 = %s, 职业 = %u",_accid,nickname.c_str(),profession);

	CMD::RECORD::t_CreateChar_GateRecord cmd;
	strncpy(cmd.nickname, nickname.c_str(), MAX_NAMESIZE);
	strncpy(cmd.platform, getPlatofrm().c_str(), MAX_FLAT_LENGTH);
	cmd.profession = profession;
	cmd.accountid = _accid;
	recordClient->sendCmd(&cmd,sizeof(cmd));
	return true;
}

void GatewayTask::onDelResult(DWORD retcode, DWORD charid)
{
	if (0 != retcode)
	{
		Packet p;
		bool IsSuccess = false;
		WORD errId = 202;   
		p << IsSuccess << errId; // 删除角色失败
		SendMessage2Ci(FIGHTING::ID_Ls2Ci_RequestRemoveRole_Reply,p);

		Fir::logger->info("[删除角色]失败，账号ID = %u, 角色ID = %u, retcode = %u", getACCID(), charid, retcode);

		return;
	}

	Packet p;
	bool IsSuccess = true;
	p << IsSuccess << charid;
	SendMessage2Ci(FIGHTING::ID_Ls2Ci_RequestRemoveRole_Reply,p);

	Fir::logger->info("[删除角色]成功，账号ID = %u, 角色ID = %u", getACCID(), charid);
}

void GatewayTask::onCreateResult(DWORD retcode, DWORD charid)
{
	Packet p;

	if (0 != retcode)
	{
		bool IsSuccess = false;
		WORD errId = 2645;    // this server is full
		if (retcode == 3)
		{
			errId = 2610;
		} 
		else if (retcode == 2)
		{
			errId = 203;
		}
		p << IsSuccess << errId; // 创建角色失败

		Fir::logger->info("[创建角色]失败，账号ID = %u, retcode = %u", getACCID(), retcode);
	}
	else
	{
		bool IsSuccess = true;
		p << IsSuccess << charid;

		Fir::logger->info("[创建角色]成功，账号ID = %u, 角色ID = %u", getACCID(), charid);
	}
	SendMessage2Ci(FIGHTING::ID_Ls2Ci_RequestCreateRole_Reply,p);
}

bool GatewayTask::delRole(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(NULL == _user) return false;

	if(!_user->isCreateState())
	{	
		Fir::logger->warn("[删除角色]失败，不在创建状态，账号ID = %u", getACCID());
		return false;
	}	

	Packet p;
	p.resize(nCmdLen);
	memcpy(&p[0], (const char*)ptNullCmd, nCmdLen);
	p.SeekSet(sizeof(CMD::t_NullCmd));

	DWORD delcharid = 0;
	p >> delcharid;

	if (0 == delcharid)
	{
		Fir::logger->info("[删除角色]失败，角色id为零，账号ID = %u", getACCID());
		return false;
	}

	AccountItem item;
	if (!getMemDbAccountItem(getACCID(), item))
	{
		Fir::logger->error("[删除角色]失败，获取账号数据失败，账号ID = %u", getACCID());
		return false;
	}

	if (!item.hasCharid(delcharid))
	{
		Fir::logger->info("[删除角色]失败，账号没有该角色，账号ID = %u，待删角色ID = %u", getACCID(), delcharid);
		return false;
	}

	Fir::logger->trace("[删除角色]，账号ID = %u，待删角色ID = %u", getACCID(), delcharid);

	CMD::RECORD::t_DelChar_GateRecord cmd;
	cmd.accountid = _accid;
	cmd.charid = delcharid;
	recordClient->sendCmd(&cmd,sizeof(cmd));
	return true;
}

bool GatewayTask::enterGame(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(NULL == _user) return false;

	if(!_user->isCreateState())
	{
		Fir::logger->warn("[进入游戏]失败，不在创建状态，账号ID = %u", getACCID());
		return false;
	}

	Packet p;
	p.resize(nCmdLen);
	memcpy(&p[0], (const char*)ptNullCmd, nCmdLen);
	p.SeekSet(sizeof(CMD::t_NullCmd));

	DWORD charid = 0;
	p >> charid;

	if (!_user->reqEnterGame(charid))
	{
		return false;
	}

	setCharID(charid);

	return true;
}

void GatewayTask::SendMessage2Ci(Packet& p)
{
	sendCmd((const void*)&p[0],p.getPosition());
}

bool GatewayTask::uniqueAdd()
{
	return GatewayTaskManager::getMe().uniqueAdd(this);
}

bool GatewayTask::uniqueRemove()
{
	return GatewayTaskManager::getMe().uniqueRemove(this);
}

bool GatewayTask::forwardScene(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{	
	if(NULL == _user) 
		return false;

	BUFFER_CMD(CMD::SCENE::t_Scene_ForwardScene, sendCmd, zSocket::MAX_DATASIZE);
	sendCmd->charid = _user->getCharID();
	sendCmd->size = nCmdLen;
	bcopy(ptNullCmd, sendCmd->data, nCmdLen);

	return _user->sendCmdToScene(sendCmd, sizeof(CMD::SCENE::t_Scene_ForwardScene) + nCmdLen);
}

bool GatewayTask::forwardSession(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if(NULL == _user) 
		return false;

	BUFFER_CMD(CMD::SESSION::t_Session_ForwardUser, sendCmd, zSocket::MAX_DATASIZE);
	sendCmd->dwID = _user->getCharID();
	sendCmd->size = nCmdLen;
	bcopy(ptNullCmd, sendCmd->data, nCmdLen);

	sessionClient->sendCmd(sendCmd, sizeof(CMD::SESSION::t_Session_ForwardUser) + nCmdLen);

	return true;
}

bool GatewayTask::checkUserCmd(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	// 指令分析
	if (!analysis_.add(ptNullCmd->_id, nCmdLen))
	{
		// return false;
	}

	return true;
}

/**
 * \brief 解析来自各个服务器连接的指令
 *
 * \param ptNullCmd 待处理的指令
 * \param nCmdLen 指令长度
 * \return 处理是否成功
 */
bool GatewayTask::msgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	if (isTerminateWait() || isTerminate())
		return false;
	// 指令检查
	if (!checkUserCmd(ptNullCmd, nCmdLen))
	{
		Fir::logger->warn("[指令检查]未通过，关闭连接，网关序号 = %llu, 账号ID = %u，IP地址 = %s，端口 = %u", getID(), getACCID(), getIP(), getPort());
		TerminateWait();
		return false;
	}

	return MessageQueue::msgParse(ptNullCmd , nCmdLen);
}

bool GatewayTask::cmdMsgParse(const CMD::t_NullCmd *ptNull, const unsigned int nCmdLen)
{
	if (isTerminateWait() || isTerminate())
		return false;

	// Fir::logger->trace("[消息接收],%d", ptNull->_id);

	if (ptNull->_id == FIGHTING::ID_Ci2Ls_RequestCreateRole)
	{
		// 创建角色
		return createRole(ptNull, nCmdLen);		
	}
	else if (ptNull->_id == FIGHTING::ID_Ci2Gs_EnterGame)
	{
		return enterGame(ptNull, nCmdLen);
	}
	else if (ptNull->_id == FIGHTING::ID_Ci2Ls_RequestRemoveRole)
	{
		return delRole(ptNull, nCmdLen);
	}
	else if (ptNull->_id >= FIGHTING::ID_Ci2Ls_MIN_SCENE_CMD_ID && ptNull->_id < FIGHTING::ID_Ci2Ls_MAX_SCENE_CMD_ID)
	{
		return forwardScene(ptNull, nCmdLen);
	}
	else
	{
		Fir::logger->warn("[消息解析]失败，未识别的消息，消息ID = %d", ptNull->_id);
	}

	return false;
}