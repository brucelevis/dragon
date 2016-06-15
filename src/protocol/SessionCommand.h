/**
 * \file
 * \author  ,@163.com
 * \date 2004年12月11日 14时36分43秒 CST
 * \brief 定义Session服务器相关指令
 *
 */


#ifndef _SessionCommand_h_
#define _SessionCommand_h_

#include "zNullCmd.h"
#include "common.h"

#pragma pack(1)


namespace CMD
{
	namespace SESSION
	{
		const BYTE CMD_LOGIN = 1;
		//const BYTE CMD_GATE = 3;
		//const BYTE CMD_FORWARD = 4;
		//const BYTE CMD_SCENE = 5;
		//const BYTE CMD_SESSION = 6;

		namespace ID
		{
			enum CMDID
			{
				session_cmd_min = 100,
				t_regUser_GateSession,			// gate    -> session reg   user
				t_unregUser_GateSession,		// gate    -> session unreg user
				t_enterUser_GateSession,		// gate    -> session enter   user

				t_regUser_SessionGate,			// session -> gate    reg
				t_unregUser_SessionGate,		// session -> gate    unreg user
				t_enterUser_Return_SessionGate,	// session -> gate	  enter   user

				t_regUser_SessionScene,			// session -> scene   reg   user
				t_unregUser_SessionScene,		// session -> scene   unreg user
				t_regUser_Return_SceneSession,	// scene   -> session reg   success
				t_unregUser_SceneSession,		// scene   -> session unreg user

				t_online_offline_SessionScene,	// session -> scene   online offline

				t_GmRequest_SessionScene,
				t_GmReply_SceneSession,
				t_RechargeNotify_SessionScene,

				t_forwardUser_GateSession,//在会话上，直接给玩家发送命令,经由网关转发
				t_Session_ForwardUser,//从网关转发用户命令到会话
				t_Session_ForwardWorld,//会话广播消息到所有玩家

				session_cmd_max = 65535,
			};
		}

		//////////////////////////////////////////////////////////////
		/// 登陆Session服务器指令,会话，场景，等sessionClient使用者，连接SessionServer后，发送连接验证消息
		//////////////////////////////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginSession : t_NullCmd
		{
			WORD wdServerID;
			WORD wdServerType;
			t_LoginSession()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN) {};
		};
		//////////////////////////////////////////////////////////////
		/// 登陆Session服务器指令
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		/// 网关服务器指令
		//////////////////////////////////////////////////////////////
		struct DECLARE_CMD(t_regUser_GateSession, SESSION::ID)
		{
			QWORD seqid;
			DWORD accid;
			BYTE regtype;	// 注册类型，0普通登录 1快速登录 2重连

			t_regUser_GateSession()
			{
				seqid = 0;
				accid = 0;
				regtype = 0;
			};
		};

		struct DECLARE_CMD(t_regUser_SessionGate,SESSION::ID)
		{
			QWORD seqid;
			DWORD accid;
			BYTE regtype;
			BYTE retcode;	// 返回代码，0成功 1失败

			t_regUser_SessionGate()
			{
				seqid = 0;
				accid = 0;
				regtype = -1;
				retcode = 0;
			};
		};

		struct DECLARE_CMD(t_unregUser_GateSession, SESSION::ID)
		{
			QWORD seqid;
			DWORD accid;

			t_unregUser_GateSession()
			{
				seqid = 0;
				accid = 0;
			};
		};

		struct DECLARE_CMD(t_unregUser_SessionGate, SESSION::ID)
		{
			QWORD seqid;
			BYTE unregtype;

			t_unregUser_SessionGate()
			{
				seqid = 0;
				unregtype = 0;
			};
		};

		struct DECLARE_CMD(t_enterUser_GateSession, SESSION::ID)
		{
			QWORD seqid;
			DWORD accid;
			DWORD charid;

			t_enterUser_GateSession()
			{
				seqid = 0;
				accid = 0;
				charid = 0;
			};
		};

		struct DECLARE_CMD(t_enterUser_Return_SessionGate, SESSION::ID)
		{
			QWORD seqid;
			DWORD accid;
			DWORD charid;
			BYTE retcode;	// 返回代码，0成功 1失败

			t_enterUser_Return_SessionGate()
			{
				seqid = 0;
				accid = 0;
				charid = 0;
				retcode = 0;
			};
		};


		///直接转发到用户的指令
		struct DECLARE_CMD(t_forwardUser_GateSession, SESSION::ID)
		{
			DWORD id;
			DWORD tempid;
			char name[MAX_NAMESIZE];
			DWORD cmd_len;
			BYTE cmd[0];
			t_forwardUser_GateSession()
			{
				id = 0;
				tempid = 0;
				bzero(name, MAX_NAMESIZE);
			}
		};

		//////////////////////////////////////////////////////////////
		/// 网关服务器指令结束
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		/// 会话服务器转发指令
		//////////////////////////////////////////////////////////////
		/// 网关到Session
		struct DECLARE_CMD(t_Session_ForwardUser, SESSION::ID)
		{
			DWORD dwID;
			WORD  size;
			BYTE  data[0];
			t_Session_ForwardUser()
			{};
		};

		/// Session到全世界
		struct DECLARE_CMD(t_Session_ForwardWorld, SESSION::ID)
		{
			WORD  size;
			BYTE  worldChat;
			BYTE  data[0];
			t_Session_ForwardWorld()
			{
				worldChat = 0;
				size = 0;
			}
		};

		//////////////////////////////////////////////////////////////
		/// 会话服务器转发指令
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// 场景服务器指令
		//////////////////////////////////////////////////////////////
		struct DECLARE_CMD(t_regUser_SessionScene, SESSION::ID)
		{
			DWORD accid;
			DWORD charid;
			DWORD gateid;
			DWORD reg_type; //0 普通注册 1gm注册
			t_regUser_SessionScene()
			{
				accid = 0;
				charid = 0;
				gateid = 0;
				reg_type = 0;
			};
		};

		struct DECLARE_CMD(t_unregUser_SessionScene, SESSION::ID)
		{
			DWORD accid;
			DWORD charid;
			DWORD unreg_type; //0 普通 1gm

			t_unregUser_SessionScene()
			{
				accid = 0;
				charid = 0;
				unreg_type = 0;
			};
		};

		struct DECLARE_CMD(t_regUser_Return_SceneSession, SESSION::ID)
		{
			DWORD accid;
			DWORD charid;
			BYTE retcode;	// 返回代码，0成功 1账号被封停 2未知错误

			 t_regUser_Return_SceneSession()
			 {
			 	 accid = 0;
				 charid = 0;
				 retcode = 0;
			 }
		};

		struct DECLARE_CMD(t_unregUser_SceneSession, SESSION::ID)
		{
			DWORD accid;
			SDWORD type;

			t_unregUser_SceneSession()
			{
				accid = 0;
			};
		};

		struct DECLARE_CMD(t_online_offline_SessionScene, SESSION::ID)
		{
			DWORD accid;
			DWORD charid;
			BYTE online_offline; //0：离线 1：在线

			t_online_offline_SessionScene()
			{
				accid = 0;
				charid = 0;
				online_offline = 0;
			};
		};

		struct DECLARE_CMD(t_GmRequest_SessionScene,SESSION::ID)
		{
			t_GmRequest_SessionScene()
			{
				datasize = 0;
			}
			DWORD datasize;
			char data[0];
			DWORD getSize() { return sizeof(*this) + datasize; }
		};

		struct DECLARE_CMD(t_GmReply_SceneSession,SESSION::ID)
		{
			t_GmReply_SceneSession()
			{
				datasize = 0;
			}
			DWORD datasize;
			char data[0];
			DWORD getSize() { return sizeof(*this) + datasize; }
		};

		struct DECLARE_CMD(t_RechargeNotify_SessionScene,SESSION::ID)
		{
			t_RechargeNotify_SessionScene()
			{
				datasize = 0;
			}
			DWORD datasize;
			char data[0];
			DWORD getSize() { return sizeof(*this) + datasize; }
		};
	};
};
#pragma pack()
#endif

