#ifndef _MatchCommand_h
#define _MatchCommand_h

#include "zType.h"
#include "zNullCmd.h"
#include "common.h"

#pragma pack(1)
namespace CMD
{
	namespace MATCH
	{
		namespace ID
		{
			enum
			{
				t_match_cmd_min = 2000,

				t_Super2Match_Login = 532,
				t_Match2Super_Login_Reply = 2002,
				t_Match2Super_ForwardScene = 2640,

				t_Match_cmd_max = 65535,
			};
		}

		///////////////////////////////////////
		///登入服务器指令
		//////////////////////////////////////
		struct DECLARE_CMD(t_Super2Match_Login, MATCH::ID)
		{
			t_Super2Match_Login()
			{
			};
			short zoneID;
		};

		struct DECLARE_CMD(t_Match2Super_Login_Reply, MATCH::ID)
		{
			t_Match2Super_Login_Reply()
			{
			};
			
		};

		struct DECLARE_CMD(t_Match2Super_ForwardScene, MATCH::ID)
		{
			t_Match2Super_ForwardScene()
			{
			}

			char data[0];

		};


	};
};
#pragma pack()

#endif
