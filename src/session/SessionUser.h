#ifndef _SESSION_USER_H_
#define _SESSION_USER_H_

#include "SessionTask.h"
#include "zRWLock.h"
#include <unordered_map>


/**
 * \brief 定义玩家信息
 *
 */
class SessionUser 
{

	friend class SessionUserManager;

	public:

		/**
		 * \brief 在线状态
		 *
		 */
		enum OnlineState
		{
			state_none,				/**< 无 */
			state_online,			/**< 在线状态 */
			state_offline,			/**< 离线状态 */
		};
		/**
		 * \brief 会话状态
		 *
		 */
		enum SessionState
		{
			none,				/**< 无 */
			select,				/**< 角色选择状态 */
			enter,				/**< 进入游戏状态 */
			play,				/**< 游戏状态 */
		};

		/**
		 * \brief 注销方式
		 *
		 */
		enum UnregMethod
		{
			unreg_no,								/**< 未知 */
			unreg_active,							/**< 主动注销 */
			unreg_passive_gate,						/**< 被动注销，网关发起 */
			unreg_passive_scene,					/**< 被动注销，场景发起 */
		};

		/**
		 * \brief 构造函数
		 *
		 * \param gateid 网关ID
		 * \param seqid 网关序号
		 * \param accid 账号ID
		 */
		SessionUser(DWORD gateid, QWORD seqid, DWORD accid)
			: _gateid(gateid)
			, _seqid(seqid)
			, _accid(accid)
			, _state(SessionState::none)
			, _state_online(OnlineState::state_none)
		{
			_sceneid = 0;
			_charid = 0;
		}
	
		/**
		 * \brief 析构函数
		 *
		*/
		~SessionUser() {};

		/**
		 * \brief 注册到网关
		 *
		*/
		bool regToGate(BYTE regtype);
		/**
		 * \brief 注册到场景
		 *
		*/
		bool regToScene(SessionTask* scene, DWORD charid, BYTE regtype = 0);
		/**
		 * \brief 注册到场景失败
		 *
		*/
		void onRegToSceneError(DWORD charid, BYTE retcode);

		/**
		 * \brief 重连
		 *
		*/
		bool reconn();

		/**
		 * \brief 获取网关序号
		 */
		QWORD getSeqID() const
		{
			return _seqid;
		}

		/**
		 * \brief 设置网关序号
		 */
		void setSeqID(QWORD seqid)
		{
			_seqid = seqid;
		}
		
		/**
		 * \brief 网关ID
		 */
		DWORD getGateID() const
		{
			return _gateid;
		}

		/**
		 * \brief 场景ID
		 */
		DWORD getSceneID() const
		{
			return _sceneid;
		}
		/**
		 * \brief 设置场景ID
		 */
		void setSceneID(DWORD sceneid)
		{
			_sceneid = sceneid;
		}

		/**
		 * \brief 获取账号ID
		 */
		DWORD getACCID() const
		{
			return _accid;
		}

		/**
		 * \brief 角色ID
		 */
		DWORD getCharID() const
		{
			return _charid;
		}
		/**
		 * \brief 设置角色ID
		 */
		void setCharID(DWORD charid)
		{
			_charid = charid;
		}

		/**
		 * \brief 发消息到网关
		 */
		bool sendCmdToGate(const void *pstrCmd, const int nCmdLen);
		/**
		 * \brief 发消息到场景
		 */
		bool sendCmdToScene(const void *pstrCmd, const int nCmdLen);

		/**
		 * \brief 是否角色选择状态
		 */
		bool isSelectState()
		{
			return (_state == SessionState::select);
		}
		/**
		 * \brief 是否进入游戏状态
		 */
		bool isEnterState()
		{
			return (_state == SessionState::enter);
		}
		/**
		 * \brief 是否游戏状态
		 */
		bool isPlayState()
		{
			return (_state == SessionState::play);
		}

		/**
		 * \brief 设置状态
		 */
		void setState(SessionState state) 
		{ 
			_state = state; 
		}

		/**
		 * \brief 上线
		 */
		void online();
		/**
		 * \brief 下线
		 */
		void offline();
		/**
		 * \brief 是否在线
		 */
		bool isOnline()
		{
			return (_state_online == OnlineState::state_online);
		}
		/**
		 * \brief 获取离线时间
		 */
		const zRTime& getOfflineTime()
		{
			return _time_offline;
		}

	private:
			
		/**
		 * \brief 从网关注销
		 *
		*/
		void unregFromGate(const UnregMethod method, const BYTE unregtype = 0);
		/**
		 * \brief 从场景注销
		 *
		*/
		void unregFromScene(const UnregMethod method, const BYTE unregtype = 0);

		/**
		 * \brief 网关在线
		 */
		void online_gate();
		/**
		* \brief 网关下线
		*/
		void offline_gate();

		/**
		 * \brief 场景在线
		 */
		void online_scene();
		/**
		 * \brief 场景下线
		 */
		void offline_scene();

		/**
		 * \brief 通知场景在线\离线
		 */
		void noticeToSceneOnlineOffline(BYTE online_offline);

	private:

		/**
		 * \brief 网关连接
		 */
		DWORD _gateid;
		/**
		 * \brief 所在网关序号
		 */
		QWORD _seqid;

		/**
		 * \brief 场景服ID
		 */
		DWORD _sceneid;

		/**
		 * \brief 账号ID
		 */
		DWORD _accid;
		/**
		 * \brief 角色ID
		 */
		DWORD _charid;

		/**
		 * \brief 玩家状态
		 */
		BYTE _state;
		/**
		 * \brief 玩家在线状态
		 */
		BYTE _state_online;

		/**
		 * \brief 上线时间
		 */
		zRTime _time_online;
		/**
		 * \brief 下线时间
		 */
		zRTime _time_offline;
};

#endif