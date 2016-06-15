#ifndef _SceneCommand_h_
#define _SceneCommand_h_

#include "zNullCmd.h"

#pragma pack(1)

namespace CMD
{

	namespace SCENE
	{
		const BYTE CMD_LOGIN = 1;

		namespace ID
		{
			enum
			{
				scene_cmd_min = 1,
				t_Refresh_LoginScene,
				t_Scene_ForwardScene, //gate to scene user cmd
				t_User_FromScene,  //scene to gate to user
				t_StartOKSceneGate,//scenetask ok
				t_Forward2World_FromScene,
				scene_cmd_max = 65535,
			};
		};

		const BYTE PARA_LOGIN = 1;
		struct t_LoginScene : t_NullCmd
		{
			WORD wdServerID;
			WORD wdServerType;
			t_LoginScene()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN) {};
		};

		struct DECLARE_CMD(t_Refresh_LoginScene, SCENE::ID)
		{
			DWORD accid;
			DWORD charid;

			t_Refresh_LoginScene()
			{
				accid = 0;
				charid = 0;
			};
		};

		struct DECLARE_CMD(t_Scene_ForwardScene, SCENE::ID)
		{
			DWORD charid;
			DWORD accid;
			DWORD size;
			char data[0];

			t_Scene_ForwardScene()
			{
				
			}
		};

		struct DECLARE_CMD(t_User_FromScene, SCENE::ID)
		{
			DWORD charid;
			DWORD size;
			char data[0];
			t_User_FromScene()
			{
				charid = 0;
				size = 0;
			}
		};

		struct DECLARE_CMD(t_StartOKSceneGate, SCENE::ID)
		{
			t_StartOKSceneGate()
			{
			}
		};

		struct DECLARE_CMD(t_Forward2World_FromScene, SCENE::ID)
		{
			t_Forward2World_FromScene()
			{
				except_charid = 0;
				size = 0;
			}
			DWORD except_charid;
			DWORD size;
			char data[0];
		};
	};
};

#pragma pack()

#endif
