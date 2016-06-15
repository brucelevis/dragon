#ifndef _RecordCommand_h_
#define _RecordCommand_h_

#include "zNullCmd.h"
#include "CharBase.h"
#include "common.h"

#pragma pack(1)

namespace CMD
{
	namespace RECORD
	{
		const BYTE CMD_LOGIN = 1;
		const BYTE CMD_GATE = 2;
		const BYTE CMD_SCENE = 3;
		const BYTE CMD_SESSION = 4;
		const BYTE CMD_SUPER = 5;

		namespace ID
		{
			enum
			{
				record_cmd_min = 100,
				record_cmd_max = 65535, 
			};
		};

		//////////////////////////////////////////////////////////////
		/// 登陆档案服务器指令
		//////////////////////////////////////////////////////////////
		const BYTE PARA_LOGIN = 1;
		struct t_LoginRecord : t_NullCmd
		{
			WORD wdServerID;
			WORD wdServerType;
			t_LoginRecord()
				: t_NullCmd(CMD_LOGIN, PARA_LOGIN) {};
		};
		//////////////////////////////////////////////////////////////
		/// 登陆档案服务器指令
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// 档案服务器和Session服务器交互的指令
		//////////////////////////////////////////////////////////////
		const BYTE PARA_SESSION_GM_REQUEST = 1;
		struct t_GmRequest_SessionRecord : public t_NullCmd
		{
			DWORD datasize;
			char data[0];
			t_GmRequest_SessionRecord() : t_NullCmd(CMD_SESSION, PARA_SESSION_GM_REQUEST)
			{
				datasize = 0;
			};
		};
		const BYTE PARA_RECORD_GM_REPLY = 1;
		struct t_GmReply_RecordSession : public t_NullCmd
		{
			DWORD datasize;
			char data[0];
			t_GmReply_RecordSession() : t_NullCmd(CMD_SESSION, PARA_RECORD_GM_REPLY)
			{
				datasize = 0;
			};
		};

		//////////////////////////////////////////////////////////////
		/// 档案服务器和Session服务器交互的指令
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// 档案服务器和网关交互的指令
		//////////////////////////////////////////////////////////////
		const BYTE PARA_GATE_CREATECHAR  = 1;
		struct t_CreateChar_GateRecord : public t_NullCmd
		{
			DWORD accountid; // 账号id
			char  nickname[MAX_NAMESIZE+1];		/// 角色名称
			WORD  profession;  // 职业
			char  platform[MAX_FLAT_LENGTH+1];//平台编号
			t_CreateChar_GateRecord() : t_NullCmd(CMD_GATE, PARA_GATE_CREATECHAR)
			{
				bzero(nickname, sizeof(nickname));
				bzero(platform, sizeof(platform));
				accountid = 0;
				profession = 0;
			};
		};

		const BYTE PARA_GATE_CREATECHAR_RETURN = 2;
		struct t_CreateChar_Return_GateRecord : public t_NullCmd
		{
			DWORD accountid;
			DWORD charid;
			BYTE retcode;	// 返回代码，0成功 1失败

			t_CreateChar_Return_GateRecord() : t_NullCmd(CMD_GATE, PARA_GATE_CREATECHAR_RETURN)
			{
				accountid = 0;
				charid = 0;
				retcode = 0;
			};
		};		

		const BYTE PARA_GATE_DELCHAR  = 3;
		struct t_DelChar_GateRecord : public t_NullCmd
		{
			DWORD accountid; // 账号id
			DWORD charid;
			t_DelChar_GateRecord() : t_NullCmd(CMD_GATE, PARA_GATE_DELCHAR)
			{
				accountid = 0;
				charid = 0;
			};
		};

		const BYTE PARA_GATE_DELCHAR_RETURN = 4;
		struct t_DelChar_Return_GateRecord : public t_NullCmd
		{
			DWORD accountid;
			DWORD charid;
			BYTE retcode;	// 返回代码，0成功 1失败

			t_DelChar_Return_GateRecord() : t_NullCmd(CMD_GATE, PARA_GATE_DELCHAR_RETURN)
			{
				accountid = 0;
				charid = 0;
				retcode = 0;
			};
		};		
		



		//////////////////////////////////////////////////////////////
		/// 档案服务器和网关交互的指令
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		/// 档案服务器和场景交互的指令
		//////////////////////////////////////////////////////////////

		// 保存角色数据
		const BYTE PARA_SCENE_USER_WRITE = 5;
		struct t_WriteUser_SceneRecord : t_NullCmd
		{
			DWORD charid;
			t_WriteUser_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_USER_WRITE)
			{
				charid = 0;
			}
		};

		const BYTE PARA_SCENE_TOPLIST_WRITE = 6;
		struct t_WriteToplist_SceneRecord : t_NullCmd
		{
			SDWORD nTopListType;
			t_WriteToplist_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_TOPLIST_WRITE)
			{
				nTopListType = 0;
			}
		};

		const BYTE PARA_SCENE_UPDATEGUILD = 7;
		struct t_UpdateGuild_SceneRecord : t_NullCmd
		{
			DWORD		MasterCharId;	// 会长ID
			DWORD		GuildId;		// 公会ID
			DWORD		CreateTime;		// 创立时间
			DWORD		Exp;			// 公会经验
			DWORD		Level;			// 公会等级
			DWORD		FightPower;		// 公会战力
			char		Name[MAX_NAMESIZE+1];		// 公会名字	
			char		Notify[MAX_NOTIFY_LENGTH+1];	// 公会公告
			char		Note[MAX_NOTIFY_LENGTH+1];		// 公会宣言
 			DWORD		dataSize;		///存档二进制的大小
 			char        data[MAX_BUFFER_LENGTH+1];		///存档的二进制数据
			t_UpdateGuild_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_UPDATEGUILD)
			{
				MasterCharId = 0;
				GuildId = 0;
				CreateTime = 0;
				Exp = 0;
				Level = 0;
				FightPower = 0;
				dataSize = 0;
				bzero(Name , sizeof(Name));
				bzero(Notify , sizeof(Notify));
				bzero(Note , sizeof(Note));
				bzero(data , sizeof(data));
			}
		};

		// 添加角色离线数据
		const BYTE PARA_SCENE_ADD_OFFLINE = 8;
		struct t_AddOfflineInfo_SceneRecord : t_NullCmd
		{
			DWORD charid;
			int type;					// 数据类型
			DWORD dataSize;				// 存档二进制的大小
			char data[0];				// 存档的二进制数据
			t_AddOfflineInfo_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_ADD_OFFLINE)
			{
				charid = 0;
				type = 0;
				dataSize = 0;
			}
		};

		// 删除角色离线数据
		const BYTE PARA_SCENE_DEL_OFFLINE = 9;
		struct t_DelOfflineInfo_SceneRecord : t_NullCmd
		{
			DWORD charid;
			t_DelOfflineInfo_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_DEL_OFFLINE)
			{
				charid = 0;
			}
		};

		// 限时购买
		const BYTE PARA_SCENE_PURCHASE_WRITE = 10;
		struct t_WritePurchase_SceneRecord : t_NullCmd
		{
			int db_id;
			int buy_people_num;
			t_WritePurchase_SceneRecord() :t_NullCmd(CMD_SCENE, PARA_SCENE_PURCHASE_WRITE)
			{
				db_id = 0;
				buy_people_num = 0;
			}
		};

		// 删除购买数据
		const BYTE PARA_SCENE_DEL_PURCHASE = 11;
		struct t_DelPurchase_SceneRecord : t_NullCmd
		{
			t_DelPurchase_SceneRecord() :t_NullCmd(CMD_SCENE, PARA_SCENE_DEL_PURCHASE)
			{
			}
		};

		// 增加购买基金数据 
		const BYTE PARA_SCENE_ADD_FUND = 12;
		struct t_AddFund_SceneRecord : t_NullCmd
		{
			int fundId;
			DWORD dataSize;				// 存档二进制的大小
			char data[0];				// 存档的二进制数据
			t_AddFund_SceneRecord() :t_NullCmd(CMD_SCENE, PARA_SCENE_ADD_FUND)
			{
				fundId = 0;
				dataSize = 0;
			}

		};

		const BYTE PARA_SCENE_UPDATESTONE = 13;
		struct t_UpdateStone_SceneRecord : t_NullCmd
		{
			DWORD charId;
			DWORD StartTime;	//占矿时间
			DWORD StoneId;		//矿id
			DWORD StoneType;	//矿类型
			DWORD Money;		//赔偿金币
			DWORD Level;		//占矿人物等级
			t_UpdateStone_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_UPDATESTONE)
			{
				charId = 0;
				StartTime = 0;
				StoneId = 0;
				StoneType = 0;
				Money = 0;
				Level = 0;
			}
		};

		const BYTE PARA_SCENE_DELETESTONE = 14;
		struct t_DeleteStone_SceneRecord : t_NullCmd
		{
			DWORD charId;
			t_DeleteStone_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_DELETESTONE)
			{
				charId = 0;
			}
		};

		const BYTE PARA_SCENE_GLOBALVAR_WRITE = 15;
		struct t_WriteGlobalVar_SceneRecord : t_NullCmd
		{
			unsigned long long key;
			long long value;

			t_WriteGlobalVar_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_GLOBALVAR_WRITE)
			{
				key = 0;
				value = 0;
			}
		};

		const BYTE PARA_SCENE_GROUPBUYINGITEMINFO_WRITE = 16;
		struct t_WriteGroupBuyingItemInfo_SceneRecord : t_NullCmd
		{
			DWORD itemId;
			DWORD itemNum;
			DWORD itemBoughtPrice;
			DWORD itemBoughtCount;
			t_WriteGroupBuyingItemInfo_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_GROUPBUYINGITEMINFO_WRITE)
			{
				itemId = 0;
				itemNum = 0;
				itemBoughtPrice = 0;
				itemBoughtCount = 0;
			}  
		};

		const BYTE PARA_SCENE_GROUPBUYINGBOUGHTITEMINFO_WRITE = 17;
		struct t_WriteGroupBuyingBoughtItemInfo_SceneRecord : t_NullCmd
		{
			DWORD charId;
			DWORD itemId;
			DWORD itemNum;
			DWORD playerBoughtItemPrice;
			DWORD playerBoughtItemCount;
			DWORD playerTodayBoughtItemCount;
			t_WriteGroupBuyingBoughtItemInfo_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_GROUPBUYINGBOUGHTITEMINFO_WRITE)
			{
				charId = 0;
				itemId = 0;
				itemNum = 0;
				playerBoughtItemPrice = 0;
				playerBoughtItemCount = 0;
				playerTodayBoughtItemCount = 0;
			}
		};

		const BYTE PARA_SCENE_GROUPBUYING_DELETE= 18;
		struct t_DeleteGroupBuying_SceneRecord : t_NullCmd
		{
			t_DeleteGroupBuying_SceneRecord() : t_NullCmd(CMD_SCENE, PARA_SCENE_GROUPBUYING_DELETE)
			{
			}
		};

		//////////////////////////////////////////////////////////////
		/// 档案服务器和场景交互的指令
		//////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////
		/// 档案服务器和Super服务器交互的指令开始
		//////////////////////////////////////////////////////////////


	};

};

#pragma pack()

#endif

