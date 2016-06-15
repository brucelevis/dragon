#ifndef _AllZoneCommand_h
#define _AllZoneCommand_h

#include "zType.h"
#include "zNullCmd.h"
#include "common.h"

#pragma pack(1)
namespace CMD
{
	namespace ALLZONE 
	{
		const BYTE CMD_LOGIN = 1;//登陆服务器指令

		namespace ID
		{
			enum
			{
				allzone_cmd_min = 1000,

				// 大区指令
				t_Super2AllZone_FromScene = 1001,
				t_AllZone2Super_Forward2Scene = 1002,
				t_Super2AllZone_FromGateway = 1003,
				t_AllZone2Super_Forward2Gateway = 1004,
				t_Super2AllZone_FromSession = 1005,
				t_AllZone2Super_Forward2Session = 1006,
				
				allzone_cmd_max = 65535,
			};
		}

		///////////////////////////////////////
		///登入服务器指令
		//////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginAllZone : t_NullCmd
		{
			GameZone_t zoneID;
			t_LoginAllZone() : t_NullCmd(CMD_LOGIN, PARA_LOGIN),zoneID() 
			{
			}
		};
		const BYTE PARA_LOGIN_OK = 2;
		struct t_LoginCmd_OK : t_NullCmd
		{
			t_LoginCmd_OK()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN_OK) {};
		};

		struct DECLARE_CMD(t_AllZone2Super_Forward2Scene, ALLZONE::ID)
		{
			t_AllZone2Super_Forward2Scene()
			{
				sceneid = 0;
				datasize = 0;
			};
			DWORD sceneid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		struct DECLARE_CMD(t_Super2AllZone_FromScene, ALLZONE::ID)
		{
			t_Super2AllZone_FromScene()
			{
				sceneid = 0;
				datasize = 0;
			};
			DWORD sceneid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		struct DECLARE_CMD(t_AllZone2Super_Forward2Gateway, ALLZONE::ID)
		{
			t_AllZone2Super_Forward2Gateway()
			{
				gateid = 0;
				datasize = 0;
			};
			DWORD gateid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		struct DECLARE_CMD(t_Super2AllZone_FromGateway, ALLZONE::ID)
		{
			t_Super2AllZone_FromGateway()
			{
				gateid = 0;
				datasize = 0;
			};
			DWORD gateid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		struct DECLARE_CMD(t_AllZone2Super_Forward2Session, ALLZONE::ID)
		{
			t_AllZone2Super_Forward2Session()
			{
				sessionid = 0;
				datasize = 0;
			};
			DWORD sessionid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		struct DECLARE_CMD(t_Super2AllZone_FromSession, ALLZONE::ID)
		{
			t_Super2AllZone_FromSession()
			{
				sessionid = 0;
				datasize = 0;
			};
			DWORD sessionid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};
	};
};
#pragma pack()

#endif
