////////////////////////////////////////////////////////////////////////////////////////
// CopyRight(C),
// FileName:	TopListDataDef.h
// Author:		������
// Version:
// Date:		2015-11-11
// Description:	
// History:		[1] 2015-11-11 19:33 �����ļ�
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TOPLIST_DATA_DEF_H_
#define _TOPLIST_DATA_DEF_H_

#define CALLBACK_TOPLIST_PARAM1(func, ponit) std::bind(func, ponit, std::placeholders::_1)
#define CALLBACK_TOPLIST_PARAM2(func, ponit) std::bind(func, ponit, std::placeholders::_1, std::placeholders::_2)

// ���а�����
enum TOPLIST_TYPE
{
	TOPLIST_TYPE_USER_GOLD					= 1,	// ��ҽ�����а�
	TOPLIST_TYPE_USER_LEVEL					= 2,	// ��ҵȼ����а�
	TOPLIST_TYPE_USER_FIGHTPOWER			= 3,	// ���ս�����а�
	TOPLIST_TYPE_USER_PVP					= 4,	// ���PVP���а�
	TOPLIST_TYPE_USER_EVERBATTLE			= 5,	// ���������а�
	TOPLIST_TYPE_USER_BOSS					= 6,	// ����BOSS���а�
	TOPLOST_TYPE_TURN_SCORE                 = 7,    // ת�̻������а�
	TOPLIST_TYPE_USER_GODWEAPON				= 8,	// ������а�

	TOPLIST_TYPE_GUILD_LEVEL				= 101,	// ���ɵȼ����а�
	TOPLIST_TYPE_REALTIME_PVP               = 201,  //ʵʱ��ս���а�

};

// ���а����ݽṹ
union UIN_TOPLIST_DATA_ST
{
	// TOPLIST_TYPE_USER_GOLD		��ҽ�����а�
	// TOPLIST_TYPE_USER_LEVEL		��ҵȼ����а�
	// TOPLIST_TYPE_USER_FIGHTPOWER ���ս�����а�
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

	// ��ͨ���а�
	struct ST_PLAYER_RANK_DATA_NORMAL
	{
		unsigned int		PlayerID;				// ��ɫID
		char				name[MAX_NAMESIZE];		// �ǳ�
		short				level;					// �ȼ�
		int					profession;				// ְҵ
		int					rank;					// ����
		int					value;					// ֵ
	}m_stPlayerRankDataNormal, m_stPlayerRankDataBoss;

	// ���������а�
	struct ST_PLAYER_RANK_PVP_DATA
	{
		unsigned int		PlayerID;				// ��ɫID
		char				name[MAX_NAMESIZE];		// �ǳ�
		short				level;					// �ȼ�
		int					profession;				// ְҵ
		int					suit;					// ��װ
		int					fight_power;			// ս��
		int					rank;					// ����
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

	//ʵʱ��ս���а�
	struct ST_REALTIME_PVP_RANK_DATA
	{
		unsigned int		PlayerID;
		char				name[MAX_NAMESIZE];		// �ǳ�
		short				level;					// �ȼ�
		int					profession;				// ְҵ
		int					fight_power;			// ս��
		int					rank;					// ����
	}m_stRealTimePVPRankData;

	// ���������а�
	struct ST_PLAYER_RANK_EVERBATTLE_DATA
	{
		unsigned int		PlayerID;				// ��ɫID
		char				name[MAX_NAMESIZE];		// �ǳ�
		short				level;					// �ȼ�
		int					profession;				// ְҵ
		int					rank;					// ����
		int					layer;					// ����
	}m_stPlayerEverBattleRankData;
	// ������а�
	struct ST_PLAYER_RANK_GODWERAPON_DATA
	{
		unsigned int		PlayerID;				// ��ɫID
		char				name[MAX_NAMESIZE];		// �ǳ�
		short				level;					// �ȼ�
		int					profession;				// ְҵ
		int					rank;					// ����
		int					weaponid;				// ���id
		int					fightpower;				// ս��
	}m_stPlayerGodWeaponRankData;

	// ת�̻������а�
	struct ST_PLAYER_RANK_TURN_SCORE
	{
		unsigned int		PlayerID;				// ��ɫID
		char				name[MAX_NAMESIZE];		// �ǳ�
		int					profession;				// ְҵ
		int					turn_score;			    // ս��
		int					scoreRank;			    // ����
	}m_stPlayerTurnScoreRankData;
};

// ��ȡ���а������ݵĻص���������
struct ST_CALLBACK_TOPLIST_GETDATA_PARAM
{
	int nTopListType;		// ���а�TYPE
	unsigned int unKey;		// ��ǰ�������ݵ�KEY
	int nRank;				// ��ǰ�������ݵ�RANK����0��ʼ��
	unsigned int unValue1;	// ���������ֵ1
	unsigned int unValue2;	// ���������ֵ2
	unsigned int unValue3;	// ���������ֵ3
};
// ��ȡ���а������ݵĻص�����
typedef std::function<bool (const ST_CALLBACK_TOPLIST_GETDATA_PARAM& stParam, UIN_TOPLIST_DATA_ST& stData)> FUNC_TOPLIST_GETDATA;

// �ж��ܷ�������а�Ļص���������
struct ST_CALLBACK_TOPLIST_CANUPDATE_PARAM
{
	int nTopListType;		// ���а�TYPE
	unsigned int unKey;		// ��ǰ�������ݵ�KEY
	unsigned int unValue1;	// ���������ֵ1
	unsigned int unValue2;	// ���������ֵ2
	unsigned int unValue3;	// ���������ֵ3
};
// �ж��ܷ�������а�Ļص�����
typedef std::function<bool (const ST_CALLBACK_TOPLIST_CANUPDATE_PARAM& stParam)> FUNC_TOPLIST_CANUPDATE;

#endif
