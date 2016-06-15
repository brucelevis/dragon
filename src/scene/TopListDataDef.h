////////////////////////////////////////////////////////////////////////////////////////
// CopyRight(C),
// FileName:	TopListDataDef.h
// Author:		李梦松
// Version:
// Date:		2015-11-11
// Description:	
// History:		[1] 2015-11-11 19:33 创建文件
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TOPLIST_DATA_DEF_H_
#define _TOPLIST_DATA_DEF_H_

#define CALLBACK_TOPLIST_PARAM1(func, ponit) std::bind(func, ponit, std::placeholders::_1)
#define CALLBACK_TOPLIST_PARAM2(func, ponit) std::bind(func, ponit, std::placeholders::_1, std::placeholders::_2)

// 排行榜类型
enum TOPLIST_TYPE
{
	TOPLIST_TYPE_USER_GOLD					= 1,	// 玩家金币排行榜
	TOPLIST_TYPE_USER_LEVEL					= 2,	// 玩家等级排行榜
	TOPLIST_TYPE_USER_FIGHTPOWER			= 3,	// 玩家战力排行榜
	TOPLIST_TYPE_USER_PVP					= 4,	// 玩家PVP排行榜
	TOPLIST_TYPE_USER_EVERBATTLE			= 5,	// 生死门排行榜
	TOPLIST_TYPE_USER_BOSS					= 6,	// 世界BOSS排行榜
	TOPLOST_TYPE_TURN_SCORE                 = 7,    // 转盘积分排行榜
	TOPLIST_TYPE_USER_GODWEAPON				= 8,	// 神兵排行榜

	TOPLIST_TYPE_GUILD_LEVEL				= 101,	// 帮派等级排行榜
	TOPLIST_TYPE_REALTIME_PVP               = 201,  //实时对战排行榜

};

// 排行榜数据结构
union UIN_TOPLIST_DATA_ST
{
	// TOPLIST_TYPE_USER_GOLD		玩家金币排行榜
	// TOPLIST_TYPE_USER_LEVEL		玩家等级排行榜
	// TOPLIST_TYPE_USER_FIGHTPOWER 玩家战力排行榜
	struct ST_PLAYER_RANK_DATA
	{
		unsigned int		PlayerID;
		char				name[MAX_NAMESIZE];
		short				profession;
		int					experience;
		short				level;
		int					gold;
		int					fight_power;
		int					GoldRank;
		int					LevelRank;
		int					FightRank;
		int                 turn_score;
		int                 ScoreRank;
	}m_stPlayerRankData;

	// 普通排行榜
	struct ST_PLAYER_RANK_DATA_NORMAL
	{
		unsigned int		PlayerID;				// 角色ID
		char				name[MAX_NAMESIZE];		// 昵称
		short				level;					// 等级
		int					profession;				// 职业
		int					rank;					// 排名
		int					value;					// 值
	}m_stPlayerRankDataNormal, m_stPlayerRankDataBoss;

	// 竞技场排行榜
	struct ST_PLAYER_RANK_PVP_DATA
	{
		unsigned int		PlayerID;				// 角色ID
		char				name[MAX_NAMESIZE];		// 昵称
		short				level;					// 等级
		int					profession;				// 职业
		int					suit;					// 套装
		int					fight_power;			// 战力
		int					rank;					// 排名
	}m_stPlayerPvpRankData;


	struct ST_GUILD_LEVEL_RANK_DATA
	{
		unsigned int		unGuildId;
		char				szName[MAX_NAMESIZE];
		unsigned int		unAmount;
		int					nRank;
		unsigned int		unLevel;
		unsigned int		unFightPower;
	}m_stGuildLevelRankData;

	//实时对战排行榜
	struct ST_REALTIME_PVP_RANK_DATA
	{
		unsigned int		PlayerID;
		char				name[MAX_NAMESIZE];		// 昵称
		short				level;					// 等级
		int					profession;				// 职业
		int					fight_power;			// 战力
		int					rank;					// 排名
	}m_stRealTimePVPRankData;

	// 生死门排行榜
	struct ST_PLAYER_RANK_EVERBATTLE_DATA
	{
		unsigned int		PlayerID;				// 角色ID
		char				name[MAX_NAMESIZE];		// 昵称
		short				level;					// 等级
		int					profession;				// 职业
		int					rank;					// 排名
		int					layer;					// 层数
	}m_stPlayerEverBattleRankData;
	// 神兵排行榜
	struct ST_PLAYER_RANK_GODWERAPON_DATA
	{
		unsigned int		PlayerID;				// 角色ID
		char				name[MAX_NAMESIZE];		// 昵称
		short				level;					// 等级
		int					profession;				// 职业
		int					rank;					// 排名
		int					weaponid;				// 神兵id
		int					fightpower;				// 战力
	}m_stPlayerGodWeaponRankData;

	// 转盘积分排行榜
	struct ST_PLAYER_RANK_TURN_SCORE
	{
		unsigned int		PlayerID;				// 角色ID
		char				name[MAX_NAMESIZE];		// 昵称
		int					profession;				// 职业
		int					turn_score;			    // 战力
		int					scoreRank;			    // 排名
	}m_stPlayerTurnScoreRankData;
};

// 获取排行榜单条数据的回调参数定义
struct ST_CALLBACK_TOPLIST_GETDATA_PARAM
{
	int nTopListType;		// 排行榜TYPE
	unsigned int unKey;		// 当前这条数据的KEY
	int nRank;				// 当前这条数据的RANK（由0开始）
	unsigned int unValue1;	// 排序规则数值1
	unsigned int unValue2;	// 排序规则数值2
	unsigned int unValue3;	// 排序规则数值3
};
// 获取排行榜单条数据的回调定义
typedef std::function<bool (const ST_CALLBACK_TOPLIST_GETDATA_PARAM& stParam, UIN_TOPLIST_DATA_ST& stData)> FUNC_TOPLIST_GETDATA;

// 判定能否更新排行榜的回掉参数定义
struct ST_CALLBACK_TOPLIST_CANUPDATE_PARAM
{
	int nTopListType;		// 排行榜TYPE
	unsigned int unKey;		// 当前这条数据的KEY
	unsigned int unValue1;	// 排序规则数值1
	unsigned int unValue2;	// 排序规则数值2
	unsigned int unValue3;	// 排序规则数值3
};
// 判定能否更新排行榜的回掉定义
typedef std::function<bool (const ST_CALLBACK_TOPLIST_CANUPDATE_PARAM& stParam)> FUNC_TOPLIST_CANUPDATE;

#endif
