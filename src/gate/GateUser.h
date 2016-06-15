/**
 * \file
 * \version  $Id: GateUser.h 65 2013-04-23 09:34:49Z  $
 * \author  ,@163.com
 * \date 2005年04月01日 11时54分48秒 CST
 * \brief 定义网关用户类
 */

#ifndef _GATEUSER_H_
#define _GATEUSER_H_

#include <set>
#include "RecordCommand.h"
#include "Fir.h"
#include "zRWLock.h"
#include "zNullCmd.h"
#include "packet.h"

class GateUserManager;
class GatewayTask;
class SceneClient;
/**
 * \brief 网关用户
 *
 */
class GateUser
{
	friend class GatewayTask;
	friend class RecordClient;

	public:

		GateUser(DWORD accountid, GatewayTask* thistask);
		~GateUser();
		
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
		* \brief 设置该玩家账号ID
		*
		*/
		void setACCID(DWORD accid)
		{
			_accid = accid;
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
		DWORD getCharID()
		{
			return _charid;
		}

		/**
		 * \brief 获得场景服ID
		 *
		 * \return 场景服ID
		 */
		DWORD getSceneID()
		{
			return _sceneid;
		}

		/**
		 * \brief 发消息到玩家
		 */
		bool sendCmdToUser(Packet& p);
		/**
		 * \brief 发消息到场景
		 */
		bool sendCmdToScene(const void *pstrCmd, const int nCmdLen);

		/**
		 * \brief 请求进入游戏
		 *
		 */
		bool reqEnterGame(DWORD charid);
		/**
		 * \brief 进入游戏成功
		 *
		 */
		void onEnterGameSuccess(SceneClient* scene, DWORD charid);
		/**
		 * \brief 进入游戏失败
		 *
		 */
		void onEnterGameFailed(DWORD charid, BYTE retcode);
		/**
		 * \brief 退出游戏
		 *
		 */
		void onQuitGame();

		void lock()
		{   
			mlock.lock();
		}   

		void unlock()
		{   
			mlock.unlock();
		}

	public:

		// 游戏状态
		enum Systemstate
		{
			SYSTEM_STATE_INITING,		/// 初始状态
			SYSTEM_STATE_CREATING,		/// 创建角色状态
			SYSTEM_STATE_PLAY,			/// 游戏状态
			SYSTEM_WAIT_STATE_PLAY,		/// 等待游戏状态
			SYSTEM_WAIT_STATE_UNREG		/// 等待退出角色流程
		};
		void initState(){systemstate = SYSTEM_STATE_INITING;}
		bool isInitState() const {return SYSTEM_STATE_INITING == systemstate;}
		void createState(){systemstate = SYSTEM_STATE_CREATING;}
		bool isCreateState() const{return SYSTEM_STATE_CREATING == systemstate;}
		void playState(SceneClient *s=NULL , DWORD scene_tempid=0);
		bool isPlayState() const{return SYSTEM_STATE_PLAY == systemstate;}
		void waitPlayState(){systemstate = SYSTEM_WAIT_STATE_PLAY;}
		bool isWaitPlayState() const{return SYSTEM_WAIT_STATE_PLAY == systemstate;}
		void waitUnregState(){systemstate = SYSTEM_WAIT_STATE_UNREG;}
		bool isWaitUnregState() const{return (SYSTEM_WAIT_STATE_UNREG == systemstate);}
		int getState(){return systemstate;}


		void TerminateWait();
		bool isTerminateWait();
		bool isTerminate();
	
		bool reg(GatewayTask* task);
		void unreg(BYTE unregtype);
		bool recon(GatewayTask* task);

		bool sendCmd(const void *pstrCmd, const unsigned int nCmdLen,const bool hasPacked=false);
		void final();

		public:

		GatewayTask* getGatewayTask() { return gatewaytask; }

	private:
		/**
		 * \brief 通知退出
		 *
		 */
		void notifyLogout(BYTE unregtype);

	private:

		zMutex mlock;

		DWORD _accid;
		DWORD _charid;

		GatewayTask *gatewaytask;
		DWORD _sceneid;

		volatile Systemstate systemstate;
};
#endif
