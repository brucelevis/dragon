#ifndef _FLCommand_h_
#define _FLCommand_h_

#include "zType.h"
#include "zNullCmd.h"

#pragma pack(1)

namespace CMD
{
	namespace FL
	{
		const BYTE CMD_LOGIN = 1;
		const BYTE CMD_GYLIST = 2;
		const BYTE CMD_SESSION = 3;


		//////////////////////////////////////////////////////////////
		/// 登陆FL服务器指令
		//////////////////////////////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginFL : t_NullCmd
		{
			char strIP[MAX_IP_LENGTH];
			unsigned short port;

			t_LoginFL()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN) {};
		};

		const BYTE PARA_LOGIN_OK = 2;
		struct t_LoginFL_OK : t_NullCmd
		{
			GameZone_t gameZone;
			char name[MAX_NAMESIZE];
			BYTE netType;

			t_LoginFL_OK()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN_OK) 
			{
				bzero(name, sizeof(name));
			};
		};
		//////////////////////////////////////////////////////////////
		/// 登陆FL服务器指令
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// 定义网关信息相关指令
		//////////////////////////////////////////////////////////////
		const BYTE PARA_FL_GYLIST = 1;
		struct t_GYList_FL : t_NullCmd
		{
			WORD wdServerID;			/**< 服务器编号 */
			BYTE pstrIP[MAX_IP_LENGTH];	/**< 服务器地址 */
			WORD wdPort;				/**< 服务器端口 */
			WORD wdNumOnline;			/**< 网关在线人数 */
			int  state;					/**< 服务器状态 */
			DWORD zoneGameVersion;
			WORD wdNetType;				/**< 网关网络类型，0电信，1网通 */
			t_GYList_FL()
				: t_NullCmd(CMD_GYLIST, PARA_FL_GYLIST) {};
		};

		const BYTE PARA_FL_RQGYLIST = 2;
		struct t_RQGYList_FL : t_NullCmd
		{
			t_RQGYList_FL()
				: t_NullCmd(CMD_GYLIST, PARA_FL_RQGYLIST) {};
		};
		//////////////////////////////////////////////////////////////
		/// 定义网关信息相关指令
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// 定义与登陆session相关指令
		//////////////////////////////////////////////////////////////
		// 大区注册人数
		const BYTE PARA_SESSION_REG_ROLE_COUNT = 3;
		struct t_RegRoleCount_Session : t_NullCmd
		{
			t_RegRoleCount_Session()
				: t_NullCmd(CMD_SESSION,PARA_SESSION_REG_ROLE_COUNT)
			{
				role_count = 0;
			}
			DWORD role_count;
		};


		// 大区在线人数
		const BYTE PARA_SESSION_ONLINECOUNT = 4;
		struct t_OnlineCount_Session: t_NullCmd
		{    
			t_OnlineCount_Session(): t_NullCmd(CMD_SESSION,PARA_SESSION_ONLINECOUNT)
			{    
				onlinecount = 0;
			}
			DWORD onlinecount;
		};

		// 中转fl消息到gateway
		const BYTE PARA_ALLZONE2SUPER_FORWARD2GATEWAY = 5;
		struct t_FL2Super_Forward2Gateway : t_NullCmd
		{
			t_FL2Super_Forward2Gateway()
				: t_NullCmd(CMD_SESSION,PARA_ALLZONE2SUPER_FORWARD2GATEWAY)
			{
				gateid = 0;
				datasize = 0;
			}
			DWORD gateid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		// 中转gateway消息到fl
		const BYTE PARA_SUPER2FL_FROM_GATEWAY = 6;
		struct t_Super2FL_FromGateway: t_NullCmd
		{    
			t_Super2FL_FromGateway(): t_NullCmd(CMD_SESSION, PARA_SUPER2FL_FROM_GATEWAY)
			{    
				gateid = 0;
				datasize = 0;
			}    
			
			DWORD gateid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		// 中转fl消息到scene
		const BYTE PARA_FL2SUPER_FORWARD2SCENE = 7;
		struct t_FL2Super_Forward2Scene : t_NullCmd
		{
			t_FL2Super_Forward2Scene()
				: t_NullCmd(CMD_SESSION,PARA_FL2SUPER_FORWARD2SCENE)
			{
				sceneid = 0;
				datasize = 0;
			}
			DWORD sceneid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		// 中转scene消息到fl
		const BYTE PARA_SUPER2FL_FROM_SCENE = 8;
		struct t_Super2FL_FromScene: t_NullCmd
		{    
			t_Super2FL_FromScene(): t_NullCmd(CMD_SESSION, PARA_SUPER2FL_FROM_SCENE)
			{    
				sceneid = 0;
				datasize = 0;
			}    

			DWORD sceneid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		// 中转fl消息到session
		const BYTE PARA_FL2SUPER_FORWARD2SESSION = 9;
		struct t_FL2Super_Forward2Session : t_NullCmd
		{
			t_FL2Super_Forward2Session()
				: t_NullCmd(CMD_SESSION,PARA_FL2SUPER_FORWARD2SESSION)
			{
				sessionid = 0;
				datasize = 0;
			}
			DWORD sessionid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		// 中转session消息到fl
		const BYTE PARA_SUPER2FL_FROM_SESSION = 10;
		struct t_Super2FL_FromSession: t_NullCmd
		{    
			t_Super2FL_FromSession(): t_NullCmd(CMD_SESSION, PARA_SUPER2FL_FROM_SESSION)
			{    
				sessionid = 0;
				datasize = 0;
			}    

			DWORD sessionid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		// 中转fl上的Gm指令到session
		const BYTE PARA_FL2SUPER_GM2SESSION = 11;
		struct t_FL2Super_GM2Session : t_NullCmd
		{
			t_FL2Super_GM2Session()
				: t_NullCmd(CMD_SESSION, PARA_FL2SUPER_GM2SESSION)
			{
				datasize = 0;
			}
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize); }
		};

	};
};


#pragma pack()

#endif

