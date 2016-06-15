#ifndef _MonitorCommand_h
#define _MonitorCommand_h

#include "zType.h"
#include "zNullCmd.h"
#include "common.h"

#pragma pack(1)
namespace CMD
{
	namespace MONITOR 
	{
		namespace ID
		{
			enum
			{
				t_monitor_cmd_min = 1000,

				t_Super2Monitor_Login = 1001,
				t_Monitor2Super_Login_Reply = 1002,
				t_Monitor2Super_Forward2Scene = 1003,
				t_Monitor2Super_GmRequest = 1004,
				t_Super2Monitor_GmReply= 1005,
				t_monitor_cmd_max = 65535,
			};
		}

		///////////////////////////////////////
		///登入服务器指令
		//////////////////////////////////////
		struct DECLARE_CMD(t_Super2Monitor_Login, MONITOR::ID)
		{
			t_Super2Monitor_Login()
			{
			};
			GameZone_t zoneID;
		};

		struct DECLARE_CMD(t_Monitor2Super_Login_Reply, MONITOR::ID)
		{
			t_Monitor2Super_Login_Reply()
			{
			};
		};

		struct DECLARE_CMD(t_Monitor2Super_Forward2Scene, MONITOR::ID)
		{
			t_Monitor2Super_Forward2Scene()
			{
				sceneid = 0;
				datasize = 0;
			};
			DWORD sceneid;
			DWORD datasize;
			char data[0];
			DWORD getSize(){ return (sizeof(*this) + datasize);}
		};

		struct DECLARE_CMD(t_Monitor2Super_GmRequest,MONITOR::ID)
		{
			t_Monitor2Super_GmRequest()
			{
				datasize = 0;
			}
			DWORD datasize;
			char data[0];
			DWORD getSize() { return sizeof(*this) + datasize; }
		};

		struct DECLARE_CMD(t_Super2Monitor_GmReply,MONITOR::ID)
		{
			t_Super2Monitor_GmReply()
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
