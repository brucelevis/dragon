/**
 * \file
 * \version  $Id: GatewayTask.h 64 2013-04-23 02:05:08Z  $
 * \author  ,@163.com
 * \date 2004年11月23日 10时20分01秒 CST
 * \brief 定义网关连接任务
 *
 */

#ifndef _GatewayTask_h_
#define _GatewayTask_h_

#include "zTCPServer.h"
#include "zTCPTask.h"
#include "zService.h"
#include "zMisc.h"
#include "GateUserManager.h"
#include "zTime.h"
#include "GateUser.h"
#include "packet.h"
#include "MessageQueue.h"
#include "zCmdAnalysis.h"

enum
{
	TASK_STATE_NONE,
	TASK_STATE_LOGIN_WAIT,				// 登录等待
	TASK_STATE_LOGIN_OK,				// 登录成功
	TASK_STATE_LOGOUT_WAIT,				// 登出等待
	TASK_STATE_LOGOUT_OK,				// 登出成功
};

class GatewayTask : public zTCPTask , public MessageQueue<128>
{

	friend class GateUser;

	public:
		GatewayTask(zTCPTaskPool *pool, const int sock, const struct sockaddr_in *addr = NULL, const bool compress = false);
		~GatewayTask();

		int verifyConn();
		int recycleConn();
		void Terminate(const TerminateMethod method = terminate_passive);
		void addToContainer();
		void removeFromContainer();
		bool uniqueAdd();
		bool uniqueRemove();
		bool msgParse(const CMD::t_NullCmd *ptNull, const unsigned int nCmdLen);
		bool cmdMsgParse(const CMD::t_NullCmd *ptNull, const unsigned int nCmdLen);

		void SendMessage2Ci(Packet& p);
		void onCreateResult(DWORD retcode, DWORD charid);
		void onDelResult(DWORD retcode, DWORD charid);

		template<typename T1> 
		void SendMessage2Ci(WORD msgId, T1& p1) 
		{   
			Packet p;

			p << msgId;
			p << p1; 

			SendMessage2Ci(p);
		}   

		/**
		 * \brief 请求注册到Session
		 *
		 */
		void regToSession();
		/**
		 * \brief 请求从Session注销
		 *
		 */
		void unregFromSession();

		/**
		 * \brief 注册到Session
		 *
		 */
		void onRegToSession(BYTE retcode, DWORD accid, BYTE regtype);
		/**
		 * \brief 从Session注销
		 *
		 */
		void onUnRegFromSession(BYTE unregtype);

		/**
		 * \brief 设置该玩家账号ID
		 *
		 */
		void setACCID(DWORD accid)
		{
			_accid = accid;
		}
		/**
		 * \brief 获得该玩家账号ID
		 *
		 * \return 账号ID
		 */
		DWORD getACCID() const
		{
			return _accid;
		}

		/**
		 * \brief 设置该玩家角色ID
		 *
		 */
		void setCharID(DWORD charid)
		{
			_charid = charid;
		}
		/**
		 * \brief 获得该玩家角色ID
		 *
		 * \return 角色ID
		 */
		DWORD getCharID() const
		{
			return _charid;
		}

		/**
		 * \brief 设置登陆平台
		 *
		 */
		void setPlatform(std::string platform)
		{
			_platform = platform;
		}
		/**
		 * \brief 获得登陆平台
		 *
		 * \return 登陆平台
		 */
		std::string getPlatofrm() const
		{
			return _platform;
		}

		/**
		 * \brief 获得标识
		 *
		 * \return 标识
		 */
		QWORD getID() const
		{
			return _seqid;
		}

		/**
		 * \brief 获得登录类型
		 * 
		 */
		BYTE getLoginType() const
		{
			return _loginType;
		}
		/**
		 * \brief 获得登录时间
		 * 
		 */
		DWORD getLoginTime() const
		{
			return _loginTime;
		}

		void loginWait() { _curr_state = TASK_STATE_LOGIN_WAIT; }
		void loginOK() { _curr_state = TASK_STATE_LOGIN_OK; }
		void logoutWait() { _curr_state = TASK_STATE_LOGOUT_WAIT; }
		void logoutOK() { _curr_state = TASK_STATE_LOGOUT_OK; }

		bool isLoginWait() const { return _curr_state == TASK_STATE_LOGIN_WAIT; }
		bool isLoginOK() const { return _curr_state == TASK_STATE_LOGIN_OK; }
		bool isLogoutWait() const { return _curr_state == TASK_STATE_LOGOUT_WAIT; }
		bool isLogoutOK() const { return _curr_state == TASK_STATE_LOGOUT_OK; }

	private:
		/**
		 * \brief 验证检查负载指令指令
		 * 
		 * \param ptNullCmd 待验证的指令
		 * \return 验证指令是否成功
		 */
		bool verifyMaxLoad(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/**
		 * \brief 验证登陆网关指令
		 * 
		 * \param ptNullCmd 待验证的登陆指令
		 * \return 验证登陆指令是否成功
		 */
		bool verifyACCID(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		bool forwardScene(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool forwardSession(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool createRole(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool enterGame(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		bool delRole(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		/**
		 * \brief 对客户端发送过来的指令进行检测
		 *
		 * \param ptNullCmd 待检测的指令
		 * \param nCmldLen 指令长度
		 * \return 检测是否成功
		 */
		bool checkUserCmd(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		/**
		 * \brief 设置登录类型
		 * 
		 */
		void setLoginType(BYTE loginType)
		{
			_loginType = loginType;
		}
		/**
		 * \brief 设置登录时间
		 * 
		 */
		void setLoginTime(DWORD loginTime)
		{
			_loginTime = loginTime;
		}

		/**
		 * \brief 登录成功
		 *
		 */
		void onLoginSuccess(DWORD accid, BYTE regtype);
		/**
		 * \brief 登录失败
		 *
		 */
		void onLoginFailed(DWORD accid, BYTE regtype);

		/**
		 * \brief 快速登录成功
		 *
		 */
		void onQuickLoginSuccess();
		/**
		 * \brief 普通登录成功
		 *
		 */
		void onCommonLoginSuccess();
		/**
		 * \brief 重连成功
		 *
		 */
		void onRconnSuccess();

private:
	// 标识
	QWORD _seqid;
	// 该连接玩家
	GateUser* _user;

	// 账号标识
	DWORD _accid;
	// 角色标识
	DWORD _charid;

	// 平台标识
	std::string _platform;

	// 登录类型
	BYTE _loginType;
	// 登陆时间
	DWORD _loginTime;

	// 当前状态
	BYTE _curr_state;

	// 指令分析
	CmdAnalysis analysis_; 
};

#endif

