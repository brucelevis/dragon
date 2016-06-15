//This file is auto generated. DO NOT MODIFY IT!

#ifndef _CommonStruct_h_
#define _CommonStruct_h_

#include <string>
#include "zType.h"

namespace FIGHTING
{

//Version control string!
const std::string VERSION_CONTROL_STR="1.0.53";
//Server type
enum SERVER_TYPE
{
	ST__CLIENT = 1,
	ST__AGENTSERVER,
	ST__LOBBYSERVER,
	ST__DBSERVER,
	ST__LOGINSERVER,
	ST__ROOMSERVER,
	ST__SORTSERVER,
	ST__WDOGSERVER,
	ST__WORLDSERVER,
	ST__SHOPSERVER,
	ST__LAUNCHER,
	ST__GAMEMANAGER,
	ST__RANKSERVER,
};

//Player State
enum Player_State
{
	PS_None = 0,
	PS_InLobby,
	PS_SendingFT,
	PS_RecvingFT,
	PS_WaitingPvp,
	PS_Fighting,
	PS_InShop,
	PS_Exercise,
	PS_FTOver,
	PS_Leave,
	PS_Email,
	PS_LookActive,
	PS_NoEnergy,
};

//Player position
enum PLAYER_POSITION
{
	PP_None = 0,
	PP_InLobby,
	PP_InLevel,
	PP_ACT_BossFight,
	PP_ACT_PVPRank,
	PP_ACT_GOLD,
	PP_ACT_EXP,
	PP_ACT_EverBattle,
	PP_ACT_Treasure,
	AT_ACT_Bounty,
	PP_GUIDE,
	AT_ACT_Hard,
	PP_FRIEND_PVP,
	PP_ACT_SP,
	PP_ACT_TEAM,
	PP_ACT_SOCIATY,
	PP_1V1_PVP,
};

//GAME position
enum GAME_POSITION
{
	GP_None = 0,
	GP_InLogin,
	GP_InChoosen,
	GP_InCreate,
	GP_InLobby,
	GP_InLevel,
};

//fight result
enum FIGHT_RESULT
{
	FR_None = 0,
	FR_Win,
	FR_Fail,
};

//money type
enum MONEY_TYPE
{
	MT_NONE = 0,
	MT_GOLD,
	MT_DIAMOND,
	MT_SPEXP,
	MT_FEAT,
	MT_ENERGY,
	MT_EXP,
};

//player type
enum PLAYER_TYPE
{
	PT_NONE = 0,
	PT_PLAYER,
	PT_PASSIVITY_NPC,
	PT_RESERVE_NPC,
	PT_INITIATIVE_NPC,
};

//mail type
enum MAIL_TYPE
{
	MAT_NONE = 0,
	MAT_COMMON,
	MAT_SYSTEM,
	MAT_SEND_GIFT,
};

//hyperlink type, same as hyperlink_desc table
enum HYPERLINK_TYPE
{
	HLT_NONE = 0,
	HLT_STORE,
	HLT_ACTIVITY,
	HLT_MAIL_GIFT = 10000,
};

//log type, for YUN YING
enum LOG_TYPE
{
	LOT_NONE = 0,
	LOT_MONEY_BUY,
	LOT_STORE_BUY,
	LOT_LOGIN,
	LOT_LOGOUT,
	LOT_USESKILL,
	LOT_ADDFRIEND,
	LOT_ENTERACTIVE,
	LOT_EXITACTIVE,
};

//Http client use, client status
enum HC_STATUS
{
	HC_NONE = 0,
	HC_FREE,
	HC_BUSY,
};

//client identifier purchase platform
enum PLATFORM_TYPE
{
	PLAT_NONE = 0,
	PLAT_APPSOTRE,
	PLAT_91,
	PLAT_PP,
	PLAT_YLT,
	PLAT_PGY,
	PLAT_TBT,
	PLAT_ANDROID,
};

//nat type
enum NAT_TYPE
{
	NT_NONE = 0,
	NT_FULL,
	NT_RESTRICT,
	NT_PORT,
	NT_SYMMETRIC,
};

//active type
enum ACTIVE_TYPE
{
	AT_NONE = 0,
	AT_BOSS_FIGHT,
	AT_PVP_RANK,
	AT_GOLD_LEVEL,
	AT_EXP_LEVEL,
	AT_EVER_BATTLE,
	AT_TREASURE_LEVEL,
	AT_BOSS_LEVEL,
	AT_HARD_LEVEL,
	AT_SPEXP_LEVEL,
};

//
enum CLASS_COLOR
{
	CC_NONEO = 0,
	CC_WHITE,
	CC_GREEN,
	CC_BLUE,
	CC_VIOLET,
	CC_ORANGE,
	CC_GOLD,
};

//
enum CLASS_ITEM
{
	CI_NONE = 0,
	CI_EQUIP,
	CI_MATERIAL,
	CI_GEM,
	CI_SKILL,
	CI_SYMBOL,
	CI_MONEY,
	CI_SNATCH_CHIP,
	CI_CHIP,
	CI_GOD_WEAPON,
};

//
enum CLASS2_EQUIP
{
	C2E_NONE = 0,
	C2E_HEAD,
	C2E_NECK,
	C2E_SHOULDER,
	C2E_WEAPON,
	C2E_BODY,
	C2E_WAIST,
	C2E_ARM,
	C2E_LEG,
	C2E_FOOT,
	C2E_WING,
	C2E_LFINGER,
	C2E_RFINGER,
	C2E_SUIT,
	C2E_MAX,
};

//
enum CLASS2_GEM
{
	C2G_NONE = 0,
	C2G_LIFE,
	C2G_DEFENSE,
	C2G_ATTACK,
	C2G_CRITICAL,
	C2G_BREAKE,
	C2G_INJURY,
	C2G_PENETRATE,
};

//
enum CLASS2_GOODS
{
	C2D_NONE = 0,
	C2D_HOT,
	C2D_GEM,
	C2D_GIFTBAG,
};

//
enum CLASS2_MATERIAL
{
	C2M_NONE = 0,
	C2M_CRYSTAL,
	C2M_EXCHANGE,
	C2M_BOX,
	C2M_CHIP,
	C2M_DRUG,
	C2M_GOLD,
	C2M_DIAMOND,
	C2M_EXP,
	C2M_EXPEND,
	C2M_BLOOD,
	C2M_SAODANGJUAN,
	C2M_TREASURE,
	C2M_SPEXP,
	C2M_SPEXPEND,
	C2M_ENHANCE,
	C2M_FEAT,
	C2M_TITLE,
	C2M_DRINKWINE,
	C2M_GODWEAPON,
	C2M_CHEST,
	C2M_SYMBOL,
	C2M_VIP,
	C2M_VIGOUR,
};

//
enum CLASS2_PLAYERATTRI
{
	C2P_NONE = 0,
	C2P_LIFE,
	C2P_DEFENSE,
	C2P_ATTACK,
	C2P_CRITICAL,
	C2P_BREAKE,
	C2P_INJURY,
	C2P_PENETRATE,
	C2P_PVPPOWER,
	C2P_PVPRESIST,
	C2P_CRITICALRESIST,
	C2P_BREAKRESIST,
	C2P_ENERGYADD,
	C2P_EXPADD,
	C2P_GOLDADD,
	C2P_MISSODDS,
	C2P_HITODDS,
	C2P_HURTDEEPEN,
	C2P_REMISSION,
	C2P_BREAKDEFENSE,
	C2P_RAGEBONUSES,
};

//
enum CLASS2_PROFESSION
{
	C2R_NONE = 0,
	C2R_SWORD,
	C2R_HAMMER,
	C2R_DOUBLESWORD,
	C2R_BOWMAN,
};

//
enum CLASS2_SKILL
{
	C2S_NONE = 0,
	C2S_ACTIVE,
	C2S_PASSIVE,
	C2S_UNIQUE,
};

//
enum CLASS2_SYMBOL
{
	C2Y_NONE = 0,
	C2Y_LIFE,
	C2Y_DEFENSE,
	C2Y_ATTACK,
	C2Y_CRITICAL,
	C2Y_BREAKE,
	C2Y_INJURY,
	C2Y_PENETRATE,
	C2Y_GOLD,
	C2Y_EXP,
	C2Y_CRITICALRESIST,
	C2Y_BREAKRESIST,
};

//
enum CHAPTER_TYPE
{
	CHAPTER_NONE = 0,
	CHAPTER_NORMAL,
	CHAPTER_ELITE,
};

//main_task_info box_get
enum CHAPTER_BOX
{
	CB_NONE = 0,
	CB_COPPER,
	CB_SILVER,
	CB_GOLD,
};

//this corresponding dynamic tables.
enum DATA_DIRTY
{
	DD_CURRENT_EPINLAY = 0,
	DD_CURRENT_EQUIP,
	DD_CURRENT_SKILL,
	DD_CURRENT_SYMBOL,
	DD_ITEMS_EQUIP,
	DD_ITEMS_GEM,
	DD_ITEMS_MATERIAL,
	DD_ITEMS_SKILL,
	DD_ITEMS_SYMBOL,
	DD_MAIN_TASK_INFO,
	DD_MAIN_TASK_TODAY_INFO,
	DD_PLAYER_ATTRIBUTES,
	DD_PLAYER_ENHANCE,
	DD_COMMON_TODAY_INFO,
	DD_PLAYER_MAIL,
	DD_WHEEL_LOTTERY,
	DD_COMMOM_SINGLE_USE,
	DD_LOGIN_TIMES,
	DD_PLAYER_FRIEND,
	DD_ACTIVE_BOSS_RANK,
	DD_FIGHT_ATTR,
	DD_PLAYER_ENERGY,
	DD_DAY_TASK,
	DD_ACTIVE_PVP_RANK,
	DD_ACTIVE_BATTLE_RANK,
	DD_PLAYER_TREASURE,
	DD_PLAYER_ACHIEVE,
	DD_GOD_WEAPON,
	DD_GOLD_LEVEL,
	DD_EXP_LEVEL,
	DD_BOSS_LEVEL,
	DD_SPEXP_LEVEL,
	DD_GENERAL_SHOP,
	DD_GAMBLE,
	DD_SNATCH,
	DD_SHOPGAME,
	DD_OPERATINGACTIVITY,
	DD_TEAMLEVEL,
	DD_GUILD,
	DD_GUILDWAR,
	DD_REALTIMEPVP,
	DD_BOSS_FIGHT,
	DD_GOLDSTONE,
	DD_CLIENT_VERSION,
	DD_GODSHOP,
	DD_DIRTY_COUNT,
};

//this synchronization load player data
enum DATA_SYNC_LOAD
{
	DSL_PLAYER_ITEM = 0,
	DSL_PLAYER_INFO,
	DSL_MAIN_TASK,
	DSL_GIVE_ENERGY,
	DSL_DAY_TASK,
	DSL_COUNT,
};

//chat channel
enum CHAT_CHANNEL
{
	CHC_NONE = 0,
	CHC_WORLD,
	CHC_UNION,
	CHC_PERSON,
	CHC_NOTICE,
	CHC_SHOW,
};

//use for loading data for server
enum LOADING_STATUS
{
	LS_NONE = 0,
	LS_LOADING,
	LS_OVER,
};

//gold and diamond
enum LOTTERY_TYPE
{
	LT_NONE = 0,
	LT_GOLD,
	LT_DIAMOND,
};

//gamble type
enum GAMBLE_TYPE
{
	GT_NONE = 0,
	GT_NORMAL,
	GT_SENIOR,
	GT_EXPERT,
};

//use for the message call back, all can use this
enum CALLBACK_TYPE
{
	CT_NONE = 0,
	CT_FRIEND_INFO,
	CT_PLAYER_INFO,
	CT_SELF_INFO,
	CT_FIGHT_ATTR,
	CT_BLACK_INFO,
	CT_PVP_RANK,
	CT_PVP_TOP100,
	CT_PVP_FILL_DATA,
	CT_PVP_READY_FIGHT,
	CT_ADD_FRIEND,
	CT_QUERY_FRIEND,
	CT_ADD_BLACK,
	CT_RECOMMEND_FRIEND,
	CT_PVP_CHALLENGE,
	CT_PVP_REFRESH_RANDOM,
};

//day task type
enum DAY_TASK_TYPE
{
	DTT_NONE = 0,
	DTT_EQUIP_ENHANCE,
	DTT_EQUIP_INLAY,
	DTT_PART_ENHANCE,
	DTT_SKILL_UPGRADE,
	DTT_LEVEL_BATTLE,
	DTT_GOLD_LOTTERY,
	DTT_DIAMOND_LOTTERY,
	DTT_BOSS_BATTLE,
	DTT_CHIP_FIGHT,
	DTT_STILL_BATTLE,
	DTT_PRESENT_ENERGY,
	DTT_DIAMOND_COST,
	DTT_PVP_BATTLE,
	DTT_BUY_GOLD,
	DTT_MONTH_CARD,
	DTT_EVER_CARD,
	DTT_SAODANG_JUAN,
	DTT_NOON_ENERGY,
	DTT_NIGHT_ENERGY,
	DTT_GODWEAPON_LEVELUP,
	DTT_MERIDIAN_LEVELUP,
	DTT_RANDOMSYMBOL_LEVELUP,
	DTT_PART_STARUP,
	DTT_WINE_START,
	DTT_EVER_BATTLE,
	DTT_EXP_LEVEL,
	DTT_GOLD_LEVEL,
	DTT_SP_LEVEL,
	DTT_HARD_LEVEL,
	DTT_GAMBLE,
	DTT_SAODANG_LEVEL,
	DTT_JINGYING_LEVEL,
	DTT_DAWN_ENERGY,
	DTT_GUILD_DONATE,
	DTT_GOLD_STONE,
	DTT_MAX,
};

//pvp rank challenge result
enum CHALLENGE_RESULT
{
	CR_NONE = 0,
	CR_FAIL,
	CR_WIN,
	CR_OTHER_FAIL,
};

//marquee type
enum MARQUEE_TYPE
{
	MTA_NONE = 0,
	MTA_TYPE1,
	MTA_TYPE2,
	MTA_TYPE3,
	MTA_TYPE4,
	MTA_TYPE5,
	MTA_TYPE6,
};

//rank type
enum RANK_TYPE
{
	RANK_NONE = 0,
	RANK_LEVEL,
	RANK_FIGHT_POWER,
	RANK_GODWEAPON,
	RANK_BOSS,
	RANK_TURN_SCORE,
};

//update rank type
enum UPDATE_RANK_TYPE
{
	UPDATE_RANK_NONE = 0,
	UPDATE_RANK_LEVEL,
	UPDATE_RANK_EXP,
	UPDATE_RANK_FIGHT_POWER,
	UPDATE_RANK_GOLD,
};

//player achieve type
enum PLAYER_ACHIEVE_TYPE
{
	PAT_NONE = 0,
	PAT_FIGHT_POWER,
	PAT_SP_LEVEL,
	PAT_HARD_LEVEL,
	PAT_ENDLESS_LAYERS,
	PAT_FEAT_VALUE,
	PAT_NORMAL_LEVEL,
	PAT_MAX,
};

//active operate type
enum ACTIVE_OPERATE_DESC
{
	AOD_NONE = 0,
	AOD_INTERSHOP,
	AOD_DISCOUNTSHOP,
};

//title type
enum TITLE_TYPE
{
	TT_NONE = 0,
	TT_FIGHTPOWER,
	TT_FIGHTRANKWEEK,
	TT_FIGHTRANKHOUR,
	TT_PVPRANK,
	TT_VIP,
	TT_BATTLE,
	TT_GOLDRANK,
	TT_LEVELRANK,
	TT_GUILDRANK,
	TT_GODWEAPONRANK,
};

//server status
enum SERVERSTATUS
{
	SS_NONE = 0,
	SS_NEW,
	SS_ADVICE,
	SS_HOT,
	SS_COUNT,
};

//zone state 
enum ZONESTATE
{
	ZS_NONE = 0,
	ZS_NORMAL,
	ZS_MAINTAIN,
	ZS_IDLE,
	ZS_BUSY,
	ZS_VERYBUSY,
	ZS_COUNT,
};

//Â¦PÂ¨B??u?Â«Â¬ 
enum SYNC_DATA_TYPE
{
	SDT_POSITION = 0,
	SDT_MOVE,
	SDT_SKILL,
	SDT_ATTACK,
	SDT_ROTATION,
};

//god weapon type
enum GOD_WEAPON_TYPE
{
	GWT_NONE = 0,
	GWT_EQUIP_COLOR,
	GWT_SP_STAR,
	GWT_LOGIN,
	GWT_EVER_LAYER,
	GWT_VIP,
	GWT_FIGHT_POWER,
	GWT_FIRSR_PAY,
	GWT_GOD_NUM,
	GWT_MAX,
};

//type can drop
enum DROP_TYPE
{
	DT_WEAPON = 1,
	DT_MATERIAL,
	DT_GOLD,
};

//cultivate type
enum CL_TYPE
{
	CL_NONE = 0,
	CL_PT = 1,
	CL_GJ = 2,
	CL_BJ = 3,
	CL_ZZ = 4,
};

//achieve task type
enum ACHIEVE_TASK_TYPE
{
	ATT_NONE = 0,
	ATT_PASS_LEVEL,
	ATT_STAR_LEVEL,
	ATT_SAODANG_LEVEL,
	ATT_HARD_LEVEL,
	ATT_PLAYER_LEVEL,
	ATT_FIGHT_POWER,
	ATT_GOD_NUM,
	ATT_GOD_LEVEL,
	ATT_MERDIAN_LEVEL,
	ATT_SYMBOL_NUM,
	ATT_SYMBOL_SUIT,
	ATT_SYMBOL_DIAMOND,
	ATT_SKILL_NUM,
	ATT_SKILL_LEVEL,
	ATT_EQUIP_LEVEL,
	ATT_EQUIP_STAR,
	ATT_GEM_LEVEL,
	ATT_GEM_NUM,
	ATT_PVP_RANK,
	ATT_BATTLE_LAYER,
	ATT_TREASURE,
	ATT_PLAYER_VIP,
	ATT_EQUIP_COLOR,
	ATT_MAX,
};

//Operating activity type
enum OPERATING_ACTIVITY_TYPE
{
	OAT_NONE = 0,
	OAT_FIRST_PAY,
	OAT_SINGLE_PAY,
	OAT_DAILY_PAY,
	OAT_DAYS_PAY,
	OAT_SINGLE_CONSUME,
	OAT_TOTAL_PAY,
	OAT_TOTAL_CONSUME,
	OAT_SUCEESIVE_PAY,
	OAT_SUCEESIVE_CONSUME,
	OAT_ONLINE,
	OAT_FUND,
	OAT_EARLY_LOGIN,
	OAT_SUCEESIVE_LOGIN,
	OAT_DOUBLE,
	OAT_ITEMEXCHANGE,
	OAT_EVENTS,
	OAT_HEALTHY,
	OAT_TURN_TABLE,
	OAT_GROUP_BUYING,
};

//type of recharge shop
enum SHOP_RECHARGE_TYPE
{
	SRT_NONE = 0,
	SRT_PAYMONEY,
	SRT_MONTHCARD,
	SRT_FUND,
};

//0:普通 1:服务器关闭 2:踢掉 3:其它地方登陆
enum LOGOUT_TYPE
{
	LOGOUT_COMMON = 0,
	LOGOUT_CLOSE = 1,
	LOGOUT_KICK = 2,
	LOGOUT_REPLACE = 3,
	LOGOUT_TIMEOUT = 4,
};

//重置类型
enum RESET_TYPE
{
	RT_NONE = 0,
	RT_DAY,
	RT_WEEK,
	RT_MONTH,
};

//组队副本类型
enum TEAM_LEVEL_TYPE
{
	TLT_NONE = 0,
	TLT_WHITE,
	TLT_GREEN,
	TLT_BLUE,
	TLT_VIOLET,
	TLT_ORANGE,
	TLT_MAX,
};

//公会职位
enum GUILD_OCCUPATION_TYPE
{
	GO_NONE = 0,
	GO_MASTER,
	GO_VICEMASTER,
	GO_ELITE,
	GO_COMMON,
};

//双倍活动类型
enum DOUBLE_TYPE
{
	DOUBLE_NONE = 0,
	INSTANCE_GOLD_DOUBLE = 1,
	INSTANCE_EXP_DOUBLE = 2,
	INSTANCE_CARD_DOUBLE = 3,
	GOLD_HAND_DOUBLE = 4 ,
};

//副本类型
enum INSTANCE_TYPE
{
	INSTANCE_NONE   = 0,
	INSTANCE_COMMON = 1,
	INSTANCE_ELITE= 2,
	INSTANCE_SP= 3,
	INSTANCE_EXP = 4 ,
	INSTANCE_GOLD = 5 ,
	INSTANCE_TEAM = 6 ,
	INSTANCE_HARD = 7 ,
};

//神兵道具位置
enum GODWEAPON_POSITION_TYPE
{
	GPT_NONE = 0,
	GPT_ONE,
	GPT_TWO,
	GPT_THREE,
	GPT_FOUR,
	GPT_MAX,
};

//转盘次数类型
enum TURN_TABLE_TIMES_TYPE
{
	TURN_NONE = 0,
	TURN_ONE,
	TURN_TEN,
};

//this is use for UDP, can not remove
class IP_PORT
{
public:
	IP_PORT();
	NAT_TYPE	m_natType;
	std::string		m_exterIp;
	unsigned short	m_exterPort;
	std::string		m_interIp;
	unsigned short	m_interPort;
};

//this is use for ip port
struct IPORT
{
	std::string		m_ip;
	unsigned short	m_port;
};

//attribute_base
struct attribute_base
{
	int	level;
	int	life;
	int	attack;
	int	defense;
	int	energy;
	int	vigour;
};

//
struct config_language
{
	int	db_id;
	std::string		info;
};

//
struct config_role_name
{
	int	db_id;
	std::string		name;
	int	sex;
};

//
struct config_mixed
{
	int	db_id;
	int	int_val;
	std::string		str_val;
	unsigned int	type;
	std::string		desc;
};

//
struct equip_attribute
{
	int	db_id;
	short	color;
	short	type;
	unsigned short	level_limit;
	bool	is_enhance;
	bool	is_upgrade;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	pvp_power;
	int	pvp_resist;
	int	critical_resist;
	int	break_resist;
	int	energy_add;
	int	exp_add;
	int	gold_add;
	short	suit_id;
	int	enhance_rate;
	int	quality;
	int	order;
	int	advance_id;
	int	tooltip;
	int	advance_1;
	int	advance_2;
};

//
struct equip_bowman
{
	int	db_id;
	int	name;
	int	icon_id;
	int	model_id;
	std::string		desc;
};

//
struct equip_double_sword
{
	int	db_id;
	int	name;
	int	icon_id;
	int	model_id;
	std::string		desc;
};

//
struct equip_enhance
{
	int	db_id;
	short	type;
	short	level;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	pvp_power;
	int	pvp_resist;
	int	critical_resist;
	int	break_resist;
	int	energy_add;
	int	exp_add;
	int	gold_add;
};

//
struct equip_grow
{
	int	db_id;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	pvp_power;
	int	pvp_resist;
	int	critical_resist;
	int	break_resist;
	short	limit;
};

//
struct equip_hammer
{
	int	db_id;
	int	name;
	int	icon_id;
	int	model_id;
	std::string		desc;
};

//
struct equip_sword
{
	int	db_id;
	int	name;
	int	icon_id;
	int	model_id;
	std::string		desc;
};

//
struct equip_upgrade
{
	int	db_id;
	unsigned short	material1;
	short	mat_size1;
	unsigned short	material2;
	short	mat_size2;
	unsigned short	material3;
	short	mat_size3;
	unsigned short	rare_material;
	short	rare_size;
	int	gold;
	int	trans_id;
};

//
struct item_gem
{
	int	db_id;
	int	name;
	short	type;
	short	level;
	int	icon_id;
	int	value;
	int	trans_id;
	int	price;
	int	tooltip;
	std::string		desc;
	int	color;
	int	order;
	int	quality;
};

//
struct item_gift_bag
{
	int	db_id;
	int	name;
	int	icon_id;
	int	limit;
	std::string		items;
	int	tooltip;
	std::string		desc;
};

//
struct item_goods
{
	int	db_id;
	int	item_id;
	int	item_size;
	bool	is_list;
	int	list_order;
	bool	is_hot;
	bool	is_new;
	int	gold;
	int	gem;
	short	money_type;
	short	level_limit;
	bool	is_day;
	bool	is_ever;
};

//
struct item_material
{
	int	db_id;
	int	name;
	short	type;
	short	level;
	std::string		data;
	int	trans_id;
	int	price;
	int	icon_id;
	int	tooltip;
	std::string		desc;
	int	quality;
	int	order;
	int	color;
	int	type_button;
};

//
struct item_snatch_chip
{
	int	db_id;
	int	name;
	short	type;
	unsigned short	level;
	std::string		data;
	unsigned short	trans_id;
	int	price;
	int	icon_id;
	int	tooltip;
	std::string		desc;
	int	quality;
	int	order;
	int	color;
	int	type_button;
};

//
struct item_money
{
	int	db_id;
	int	name;
	int	price;
	int	color;
	int	icon_id;
	int	tooltip;
	std::string		desc;
};

//
struct item_symbol
{
	int	db_id;
	int	name;
	short	color;
	unsigned short	attribute;
	int	base_exp;
	int	value;
	int	icon_id;
	int	trans_id;
	int	item_id;
	int	item_size;
	int	decompose_item;
	int	gold;
	short	max_level;
	std::string		desc;
};

//
struct item_symbol_limit
{
	short	pos1;
	short	pos2;
	short	pos3;
	short	pos4;
	short	pos5;
	short	pos6;
	short	pos7;
	short	pos8;
	short	pos9;
	short	pos10;
	short	pos11;
};

//
struct item_symbol_upgrade
{
	int	level;
	int	scale;
	int	exp1;
	int	exp2;
	int	exp3;
	int	exp4;
	int	exp5;
	int	exp6;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	critical_resist;
	int	break_resist;
};

//use for item symbol random
struct symbol_base
{
	short	db_id;
	short	level_limit;
	short	vip_limit;
	short	next_id;
	int	rate;
	int	gold;
	int	diamond;
	int	item_id;
	int	item_num;
	short	sp_type;
	int	sp_num;
	int	energy;
};

//
struct symbol_random
{
	symbol_base	symbol;
	std::string		com_rewards;
	std::string		sp_rewards;
	std::string		first_rewards;
	std::string		desc;
};

//
struct part_enhance
{
	int	level;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	critical_resist;
	int	break_resist;
	int	gold;
	int	com_crystal_id;
	int	com_crystal_num;
	int	per_crystal_id;
	int	per_crystal_num;
};

//
struct player_upgrade
{
	int	level;
	unsigned long	experience;
	int	gold;
	int	diamond;
	int	item_id;
	int	count;
	int	practice;
	int	treasure_id;
	int	bounty_exp;
	int	max_star;
	int	day_money;
	int	day_item;
	int	bounty_god;
	int	bounty_sp;
};

//
struct res_icon
{
	int	db_id;
	std::string		name;
	std::string		desc;
};

//
struct res_model
{
	int	db_id;
	std::string		name;
	std::string		desc;
};

//
struct res_sound
{
	int	db_id;
	std::string		name;
	std::string		desc;
};

//
struct skill_upgrade
{
	int	level;
	int	need_level;
	int	gold;
	int	add_attribute;
	int	add_attack;
	int	item_id;
	int	item_size;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	critical_resist;
	int	break_resist;
	int	fight_power;
	int	rank;
	int	main_id;
	float	break_defense;
};

//
struct skill_attribute
{
	int	db_id;
	short	type;
	short	attr_type;
};

//
struct skill_bowman
{
	int	db_id;
	int	name;
	int	icon_id;
	int	tooltip;
	int	model_id;
	std::string		desc;
};

//
struct skill_double_sword
{
	int	db_id;
	int	name;
	int	icon_id;
	int	tooltip;
	int	model_id;
	std::string		desc;
};

//
struct skill_hammer
{
	int	db_id;
	int	name;
	int	icon_id;
	int	tooltip;
	int	model_id;
	std::string		desc;
};

//
struct skill_sword
{
	int	db_id;
	int	name;
	int	icon_id;
	int	tooltip;
	int	model_id;
	std::string		desc;
};

//default_equip
struct default_equip
{
	short	db_id;
	unsigned short	gold;
	unsigned short	diamond;
	unsigned short	head;
	unsigned short	neck;
	unsigned short	shoulder;
	unsigned short	weapon;
	unsigned short	body;
	unsigned short	waist;
	unsigned short	arm;
	unsigned short	leg;
	unsigned short	foot;
	unsigned short	wing;
	unsigned short	left_finger;
	unsigned short	right_finger;
	unsigned short	active_skill;
	unsigned short	suit;
};

//
struct vip_privilege
{
	int	level;
	int	charge_num;
	short	saodang_num;
	short	energy_num;
	short	buy_gold_num;
	short	reset_level_num;
	short	gold_free_times;
	short	gold_level_num;
	short	exp_free_times;
	short	exp_level_num;
	short	present_num;
	short	recieve_num;
	short	arena_free_times;
	short	pvp_rank_num;
	int	pr_feat_num;
	short	eb_free_num;
	short	ever_battle_num;
	int	practice_num;
	int	mystical_shop_num;
	short	boss_level_num;
	short	gold_critical_rate;
	std::string		gold_critical_times;
	short	gold_free_num;
	short	mystical_shop_item_num;
	short	buy_treasure_times;
	short	treasure_times;
	short	treasure_rate;
	short	maxtreasure_rate;
	short	treasure_desc1;
	short	treasure_desc2;
	short	treasure_desc3;
	short	buy_hard_num;
	short	bounty_dis;
	short	bounty_yield;
	short	spexp_level_num;
	short	spexp_level_rate;
	short	vigour_num;
	int	guildbattle_times;
	int	reset_normallevel_num;
	short	cuilian_num;
	short	cuilian_high;
	short	fight_5;
	int	mystical_shop_reset_num;
	int	goldstone_free_times;
	int	goldstone_level_num;
	int	shop_goditem_num;
};

//
struct vip_times_price
{
	int	db_id;
	short	energy;
	short	gold;
	short	reset_level;
	short	pvp_rank_times;
	short	gold_level_times;
	short	exp_level_times;
	short	ever_battle_times;
	short	boss_level_times;
	short	mystical_shop_times;
	short	buy_treasure_times;
	short	spexp_level_times;
	short	vigour_times;
	int	reset_normallevel;
	int	goldstone_level_times;
};

//
struct wheel_lottery
{
	int	db_id;
	float	gold_rate;
	float	diamond_rate;
	short	gold_rand_rate;
	short	diamond_rand_rate;
};

//
struct gamble_form
{
	short	db_id;
	short	get_times;
	unsigned int	free_times;
	short	interval_time;
	int	first_bag;
	int	free_bag;
	int	every_day;
	int	money_type;
	int	price;
	unsigned int	sp_min_times;
	unsigned int	sp_max_times;
	int	sp_success_rate;
	int	sp_bag;
	int	bag_1;
	int	bag_1_rate;
	int	bag_2;
	int	bag_2_rate;
	int	bag_3;
	int	bag_3_rate;
	int	itemid;
	int	itemcount;
};

//
struct level_rewards
{
	short	db_id;
	short	level;
	int	item_id1;
	int	item_size1;
	int	item_id2;
	int	item_size2;
	int	item_id3;
	int	item_size3;
};

//
struct login_rewards
{
	int	item_id;
	int	item_size;
	short	vip_level;
};

//
struct pay_rewards
{
	int	level;
	int	item_id1;
	int	item_size1;
	int	item_id2;
	int	item_size2;
	int	item_id3;
	int	item_size3;
};

//boss active monster table
struct active_boss_monster
{
	short	db_id;
	short	type;
	std::string		name;
	int	level;
	int	life;
	int	defense;
	int	attack;
	int	breake;
	int	injury;
	int	penetrate;
	int	criticalresist;
	int	breakresist;
	int	attack_weight;
	std::string		desc;
};

//boss active rewards table 1
struct active_boss_rewards1
{
	int	db_id;
	int	item_id1;
	int	size1;
	int	item_id2;
	int	size2;
};

//boss active rewards table 2
struct active_boss_rewards2
{
	short	db_id;
	short	icon_id;
	int	target;
	short	type;
	int	value;
	int	item_id;
	int	item_size;
	short	tooltip;
};

//active describe table
struct active_desc
{
	short	db_id;
	std::string		name;
	short	show_name;
	unsigned int	start_time;
	unsigned int	end_time;
	unsigned int	start_time1;
	unsigned int	end_time1;
	short	level;
	short	free_times;
	short	charge;
	short	charge_add;
	short	charge_times;
	bool	is_open;
	int	user_data1;
	int	user_data2;
	std::string		rewards_table;
	short	icon_id;
	short	tooltip;
};

//PVP rank rewards table
struct active_pvp_rewards
{
	int	db_id;
	int	win_reward;
	int	defeat_reward;
	int	rank_reward;
	int	diamond_reward;
};

//PVP rank rewards table
struct active_pvp_trade
{
	int	db_id;
	int	feat;
	unsigned short	item_id;
	short	item_size;
};

//PVP rank robot table
struct active_pvp_robot
{
	int	db_id;
	int	name;
	int	rank;
	int	level;
	short	profession;
	int	head;
	int	head_enhance;
	int	head_star;
	int	weapon;
	int	weapon_enhance;
	int	weapon_star;
	int	body;
	int	body_enhance;
	int	body_star;
	int	arm;
	int	arm_enhance;
	int	arm_star;
	int	leg;
	int	leg_enhance;
	int	leg_star;
	int	foot;
	int	foot_enhance;
	int	foot_star;
	int	left_finger;
	int	right_finger;
	int	suit;
	int	fight_power;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	criticalresist;
	int	breakresist;
	std::string		skill;
};

//day task table
struct day_task
{
	short	db_id;
	int	name;
	short	level;
	int	value;
	short	order;
	short	icon_id;
	int	tooltip;
	int	reward_money;
	int	reward_moneynum;
	int	reward_item;
	int	reward_itemnum;
	int	param;
};

//pay money table
struct pay_money
{
	short	db_id;
	int	name;
	int	base_value;
	int	add_value1;
	int	add_value2;
	int	money;
	bool	is_limit;
	bool	is_recommend;
	short	order;
	short	icon_id;
	int	tooltip1;
	int	tooltip2;
};

//gold level table
struct gold_level
{
	short	db_id;
	short	level_limit;
	int	score;
	int	gold_rate;
	int	diamond_rate;
	int	level_id;
	int	tooltip;
	int	small;
	int	middle;
	int	big;
	int	icon_id;
	int	mo_total;
	std::string		killcount_reward;
	int	draw_card_id;
};

//active exp level table
struct active_exp_level
{
	short	db_id;
	short	level_limit;
	int	max_exp;
	int	level_id;
	int	icon_id;
	int	tooltip;
	int	max_health;
	int	mo_attack;
	short	add_attack1;
	short	add_attack2;
	int	add_exp;
	std::string		add_rate;
	int	mo_total;
	std::string		killcount_reward;
	int	draw_card_id;
};

//equip split table
struct equip_split
{
	short	db_id;
	int	mat_id1;
	int	mat_num1;
	int	mat_id2;
	int	mat_num2;
	int	mat_id3;
	int	mat_num3;
	int	mat_id4;
	int	mat_num4;
};

//exchange shop table
struct exchange_shop
{
	short	db_id;
	int	item_id;
	int	mat_id;
	int	mat_num;
	short	level;
	short	index;
};

//active battle rewards table
struct active_battle_rewards
{
	short	db_id;
	int	gift_id;
};

//active ever battle table
struct active_ever_battle
{
	short	db_id;
	int	com_gift;
	int	sp_gift;
	int	price;
	int	level_id;
	int	name;
	int	tooltip;
	int	desc;
};

//activeness rewards table
struct activeness_rewards
{
	int	db_id;
	int	value;
	int	gold;
	int	diamond;
	int	gift_id;
};

//continue login rewards table
struct continue_login_rewards
{
	int	db_id;
	int	gold;
	int	diamond;
	int	gift_id;
};

//only use for active treasure level
struct active_treasure_level
{
	int	db_id;
	unsigned int	item_id;
	int	size;
	int	max_size;
};

//only use for player meridian
struct player_meridian
{
	int	db_id;
	int	name;
	short	level;
	short	is_up;
	int	item_id;
	int	item_size;
	int	sp_exp;
	int	gold;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	critical_resist;
	int	penetrate;
	int	injury;
	int	break_resist;
	int	up_rate;
	int	tooltip;
};

//only use for bounty task
struct bounty_task
{
	int	db_id;
	short	type;
	int	name;
	int	gold;
	int	item_id;
	int	count;
	int	level_id;
	int	icon_id;
	int	tooltip;
};

//only use for bounty refresh
struct bounty_refresh
{
	int	db_id;
	int	reward_rate;
	int	up_rate;
	int	down_rate;
	int	normal_rate;
	short	up_star;
	short	down_star;
};

//only use for equip cultivate attr
struct equip_cultivate_attr
{
	int	db_id;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	critical_resist;
	int	break_resist;
	int	unlock_color;
};

//only use for equip cultivate max
struct equip_cultivate_max
{
	int	equip_color;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	critical_resist;
	int	break_resist;
};

//only use for equip cultivate type
struct equip_cultivate_type
{
	int	db_id;
	int	type;
	int	vip_level;
	int	attr_type;
	int	num_min;
	int	num_max;
	int	cost_item;
	int	cost_item_num;
	int	add_num;
	int	pro;
	int	times;
};

//only use for player achieve
struct player_achieve
{
	int	db_id;
	short	type;
	int	value;
	short	money_type;
	int	money;
	int	item_id1;
	int	count1;
	int	item_id2;
	int	count2;
	short	achieve_type;
	int	tooltip;
};

//only use for achieve rewards
struct achieve_rewards
{
	int	db_id;
	short	money_type;
	int	money;
	int	gift_id;
	int	tooltip;
};

//only use for part upgrade
struct part_upgrade
{
	int	db_id;
	int	min_exp;
	int	max_exp;
	int	rate;
	int	gold;
	int	item_id;
	int	count;
	int	add_rate;
	int	exp;
};

//only use for equip suit
struct equip_suit
{
	int	db_id;
	int	name;
	short	suit_id;
	short	num;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	critical_resist;
	int	break_resist;
	int	miss_odds;
	int	hit_odds;
	int	hurt_deepen;
	int	hurt_remission;
	int	break_defense;
	int	rage_bonuses;
	short	is_show;
};

//only use for drink wine
struct drink_wine
{
	int	db_id;
	int	gold;
	int	exp;
	int	sp_exp;
	int	energy;
	int	team_gold;
	int	team_exp;
	int	team_sp_exp;
	int	team_energy;
	int	captain_gold;
	int	captain_exp;
	int	captain_sp_exp;
	int	captain_energy;
	int	gold_item;
	int	exp_item;
	int	sp_exp_item;
	int	energy_item;
};

//db_id also level
struct class_level_rating
{
	int	db_id;
	std::string		name;
	int	icon_id;
	int	times;
};

//
struct ci_chapter_map_describe
{
	int	db_id;
	int	icon_id;
	int	name;
	int	total;
	int	level1;
	int	level2;
	int	level3;
	int	level4;
	int	level5;
	int	level6;
	int	level7;
	int	level8;
	int	level9;
	int	level10;
	int	route_id;
	int	gold_box;
	int	silver_box;
	int	copper_box;
	short	gold_times;
	short	silver_times;
	short	copper_times;
	int	sp_gold_box;
	int	sp_silver_box;
	int	sp_copper_box;
	short	sp_gold_times;
	short	sp_silver_times;
	short	sp_copper_times;
};

//
struct ci_level_object
{
	int	db_id;
	int	name;
	int	icon_id;
	int	pass_ref;
	int	com_level_id;
	int	sp_level_id;
	int	level_limit;
	int	com_gift_id;
	int	sp_gift_id;
	int	com_gold;
	int	sp_gold;
	int	com_exp;
	int	sp_exp;
	int	desc_id;
	int	cutscene_id;
	int	icon;
	std::string		event_table;
	int	drop_id;
	int	com_ad_fight;
	int	sp_ad_fight;
	int	com_extra_drop_id;
	int	sp_extra_drop_id;
	int	fbbox;
	int	sp_fbbox;
	int	fbbubble_desc;
	int	sp_fbbubble_desc;
};

//
struct task_gift_bag
{
	int	db_id;
	int	rate;
	int	draw_card_id;
	int	com_drop_id;
	int	sp_drop_id;
	int	drop_com_exp;
	int	drop_sp_exp;
	std::string		show_gift;
};

//
struct ci_auto_task
{
	int	db_id;
	int	type;
	int	name;
	int	cutscene_id;
	int	chapter_id;
	int	level_id;
	int	next_id;
};

//
struct sandbox_event
{
	int	db_id;
	short	type;
	short	child_type;
	int	add_val;
	bool	is_percent;
	int	keep_time;
	int	tooltip;
};

//ci_cut_scene table
struct cut_scene
{
	int	db_id;
	std::string		scene_name;
	std::string		dialog_table;
	std::string		object_table;
	int	gold;
	int	exp;
};

//cut_dialogue table
struct cut_dialogue
{
	int	db_id;
	int	object_id;
	std::string		info;
};

//cut_object table
struct cut_object
{
	int	db_id;
	int	icon_id;
	std::string		prefab;
	std::string		name;
	int	sound_id;
};

//current_equip
struct current_equip
{
	unsigned int	PlayerID;
	int	head;
	int	neck;
	int	shoulder;
	int	weapon;
	int	body;
	int	waist;
	int	arm;
	int	leg;
	int	foot;
	int	wing;
	int	left_finger;
	int	right_finger;
	int	suit;
};

//current_skill
struct current_skill
{
	int	PlayerID;
	int	active1;
	int	active2;
	int	active3;
	int	passive1;
	int	passive2;
	int	passive3;
	int	unique1;
};

//current equip inlay
struct current_epinlay
{
	unsigned int	PlayerID;
	std::string		head;
	std::string		neck;
	std::string		shoulder;
	std::string		weapon;
	std::string		body;
	std::string		waist;
	std::string		arm;
	std::string		leg;
	std::string		foot;
	std::string		wing;
	std::string		left_finger;
	std::string		right_finger;
};

//current_symbol
struct current_symbol
{
	unsigned int	PlayerID;
	std::string		pos1;
	std::string		pos2;
	std::string		pos3;
	std::string		pos4;
	std::string		pos5;
	std::string		pos6;
	std::string		pos7;
	std::string		pos8;
	std::string		pos9;
};

//team level object
struct obj_team_level
{
	int	index;
	int	value;
};

//use for mail money
struct mail_money
{
	short	type;
	int	value;
};

//achieve object
struct obj_achieve
{
	short	achieve_id;
	bool	is_get;
};

//use for bounty task
struct int_short
{
	int	intId;
	short	shortId;
};

//use for random symbol
struct random_symbol
{
	int	item_id;
	int	rate;
};

//now use for day task
class int_bool
{
public:
	int_bool();
	int	value;
	bool	is_get;
};

//now use for operating acvitity
class uint_bool
{
public:
	uint_bool();
	unsigned int	value;
	bool	is_get;
};

//use for recieve energy
struct recieve_energy
{
	unsigned int	player_id;
	unsigned int	recieve_time;
	bool	is_recieve;
};

//now use for friend id
struct int_uint
{
	int	intId;
	unsigned int	uintId;
};

//common item object, use for insert item and send to client
struct obj_common
{
	int	item_id;
	int	count;
};

//items_equip object
struct obj_equip
{
	int	item_id;
	int	count;
};

//items_gem object
struct obj_gem
{
	int	item_id;
	int	count;
};

//items_material object
struct obj_material
{
	int	item_id;
	int	count;
};

//items_piece object
struct obj_piece
{
	int	item_id;
	int	count;
};

//items_skill object
struct obj_skill
{
	int	skill_id;
	short	level;
};

//items_symbol
struct obj_symbol
{
	int	item_id;
	int	exp;
	short	level;
};

//equip inlay object
struct obj_epinlay
{
	short	order_id;
	int	gem_id;
};

//equip enhance object
struct obj_enhance
{
	int	attr_type;
	int	attr;
};

//main_task_info box_get object
struct obj_box_get
{
	short	chapter_id;
	short	chapter_type;
	bool	is_get_copper;
	bool	is_get_silver;
	bool	is_get_gold;
};

//obj_title
struct obj_title
{
	short	title_id;
	int	title_times;
};

//obj_gold
struct obj_gold
{
	short	gold_times;
	short	gold_rate;
};

//items_chip object
struct obj_chip
{
	int	item_id;
	int	count;
};

//items_god_weapon object
struct obj_god_weapon
{
	int	item_id;
	int	count;
};

//god inlay object
struct obj_godinlay
{
	int	order_id;
	int	god_id;
};

//player_account
struct player_account
{
	std::string		AccountID;
	std::string		PlatformID;
	unsigned int	PlayerID1;
	unsigned int	PlayerID2;
	unsigned int	PlayerID3;
	unsigned int	PlayerID4;
};

//player_attributes
struct player_attributes
{
	unsigned int	PlayerID;
	std::string		name;
	short	profession;
	int	energy;
	unsigned int	last_fight;
	unsigned long	experience;
	short	level;
	int	gold;
	int	diamond;
	int	head;
	int	neck;
	int	shoulder;
	int	weapon;
	int	body;
	int	waist;
	int	arm;
	int	leg;
	int	foot;
	int	wing;
	int	left_finger;
	int	right_finger;
	bool	IsNpc;
	unsigned int	last_login;
	int	login_times;
	short	equip_bags;
	short	gem_bags;
	short	material_bags;
	short	vip;
	int	charge;
	short	sp_level;
	int	sp_exp;
	int	head_1;
	int	neck_1;
	int	shoulder_1;
	int	weapon_1;
	int	body_1;
	int	waist_1;
	int	arm_1;
	int	leg_1;
	int	foot_1;
	int	wing_1;
	int	left_finger_1;
	int	right_finger_1;
	int	head_2;
	int	neck_2;
	int	shoulder_2;
	int	weapon_2;
	int	body_2;
	int	waist_2;
	int	arm_2;
	int	leg_2;
	int	foot_2;
	int	wing_2;
	int	left_finger_2;
	int	right_finger_2;
	int	ever_battle_layer;
	unsigned int	ever_battle_time;
};

//record do something in today
struct common_today_info
{
	unsigned int	PlayerID;
	short	energy_times;
	short	buy_gold_times;
	short	gold_level_times;
	short	exp_level_times;
	short	lottery_times;
	short	gamble_times;
	short	is_login;
	short	present_times;
	short	recieve_times;
	short	buy_gold_level;
	short	buy_exp_level;
	BitSet	shop_limit;
	short	treasure_num;
	BitSet	treasure_state;
	int	active_value;
	BitSet	active_rewards;
	short	bounty_task_times;
	short	month_card_reward;
	short	mystical_shop_times;
	short	buy_mystical_shop;
	short	boss_level_times;
	short	buy_boss_level;
	short	inter_shop_times;
	short	drink_wine_times;
	short	drink_wine_start;
	unsigned int	drink_interval_time;
	short	buy_treasure_times;
	short	left_treasure_times;
	short	enter_battle_times;
	short	buy_battle_times;
	short	left_battle_times;
	short	ever_battle_rewards;
	short	ever_battle_box_rewards;
};

//player_enhance
struct player_enhance
{
	unsigned int	PlayerID;
	std::string		head;
	std::string		neck;
	std::string		shoulder;
	std::string		weapon;
	std::string		body;
	std::string		waist;
	std::string		arm;
	std::string		leg;
	std::string		foot;
	std::string		wing;
	std::string		left_finger;
	std::string		right_finger;
	std::string		suit;
};

//player information
struct Player_Info
{
	player_attributes	m_Attributes;
	current_equip	m_CurEquip;
	current_skill	m_CurSkill;
	IP_PORT	m_ip;
};

//player fighting attributes
struct Fight_Attr
{
	float	m_life;
	float	m_defense;
	float	m_attack;
	float	m_critical;
	float	m_breake;
	float	m_injury;
	float	m_penetrate;
	float	m_pvppower;
	float	m_pvpresist;
	float	m_criticalresist;
	float	m_breakresist;
	float	m_dodge;
	float	m_energyadd;
	float	m_expadd;
	float	m_goldadd;
	float	m_missodds;
	float	m_hitodds;
	float	m_hurtdeepen;
	float	m_hurtremission;
	float	m_breakdefense;
	float	m_ragebonuses;
	int	m_fight_power;
};

//login server send to client data
struct choose_role_data
{
	unsigned int	player_id;
	std::string		player_name;
	int	profession;
	int	level;
	int	weapon;
	int	suit;
};

//main_task_info
struct main_task_info
{
	unsigned int	PlayerID;
	int	curr_id;
	bool	is_over;
	std::string		box_get;
	std::string		chapter1;
	std::string		chapter2;
	std::string		chapter3;
	std::string		chapter4;
	std::string		chapter5;
	std::string		chapter6;
	std::string		chapter7;
	std::string		chapter8;
	std::string		chapter9;
	std::string		chapter10;
	std::string		chapter11;
	std::string		chapter12;
	std::string		chapter13;
	std::string		chapter14;
	std::string		chapter15;
	std::string		chapter16;
	std::string		chapter17;
	std::string		chapter18;
	std::string		chapter19;
	std::string		chapter20;
};

//main_task_today_info
struct main_task_today_info
{
	unsigned int	PlayerID;
	std::string		chapter1;
	std::string		chapter2;
	std::string		chapter3;
	std::string		chapter4;
	std::string		chapter5;
	std::string		chapter6;
	std::string		chapter7;
	std::string		chapter8;
	std::string		chapter9;
	std::string		chapter10;
	std::string		chapter11;
	std::string		chapter12;
	std::string		chapter13;
	std::string		chapter14;
	std::string		chapter15;
	std::string		chapter16;
	std::string		chapter17;
	std::string		chapter18;
	std::string		chapter19;
	std::string		chapter20;
};

//use for update player position
struct SrVector
{
	float	x;
	float	y;
	float	z;
};

//common_single_use
struct common_single_use
{
	unsigned int	PlayerID;
	BitSet	level_gift;
	BitSet	pay_gift;
	BitSet	pay_money;
	BitSet	shop_limit;
	BitSet	symbol_reward;
	BitSet	exchange_gift;
	int	guide_id;
	unsigned int	practice_time;
	short	login_times;
	BitSet	login_rewards;
	unsigned int	login_time;
	unsigned int	month_end_time;
	BitSet	month_first_rewards;
	int	pvp_best_rank;
	BitSet	buy_vip_shop;
	unsigned int	gold_gamble;
	unsigned int	diamond_gamble;
	unsigned int	month_cardid;
	int	god_weapon_times;
	int	treasure_times;
	int	symbol_diamond;
	short	ever_battle_rewards;
	int	ever_battle_score;
	int	symbol_energy;
	int	month_card_recharge_count;
	int	ever_card_recharge_count;
	bool	month_card_state;
	bool	ever_card_state;
};

//boss active rank table
struct active_boss_rank
{
	unsigned int	PlayerID;
	int	result;
	int	times;
	int	rewards_get1;
	BitSet	rewards_get2;
	int	ranking;
};

//the boss rank data
struct boss_rank_data
{
	unsigned int	PlayerID;
	short	profession;
	int	suit;
	int	ranking;
	int	result;
	int	level;
	std::string		name;
};

//use for query rank from database
struct rank_data
{
	unsigned int	PlayerID;
	int	rank;
};

//friend data for list
struct friend_data
{
	unsigned int	PlayerID;
	std::string		PlayerName;
	short	profession;
	short	level;
	short	vip;
	int	suit;
	int	fight_power;
};

//energy data for list
struct energy_data
{
	friend_data	FriendData;
	bool	IsRecieve;
};

//send energy data for list
struct send_energy_data
{
	friend_data	FriendData;
	unsigned int	DayNum;
};

//receive energy data for list
struct receive_energy_data
{
	friend_data	FriendData;
	unsigned int	DayNum;
};

//PVP rank active data
struct active_pvp_rank
{
	int	rank;
	unsigned int	PlayerID;
	short	left_times;
	short	buy_times;
	unsigned int	feat_time;
	int	feat_sum;
	int	feat_total;
	int	rand1;
	int	rand2;
	int	rand3;
	std::string		info;
};

//only use for the PVP rank
struct pvp_rank_data
{
	int	rank;
	unsigned int	PlayerID;
	short	profession;
	int	suit;
	std::string		name;
	int	level;
	int	fight_power;
};

//only use for the PVP rank random data
struct PvpRandomData
{
	int	rank;
	unsigned int	PlayerID;
	short	profession;
	int	suit;
	int	weapon;
	int	level;
	int	fight_power;
	unsigned short	left_finger;
	std::string		name;
	bool	canFight;
};

//
struct item_find
{
	int	db_id;
	std::string		source;
};

//only use for ever battle rank
struct ever_battle_rank
{
	unsigned int	PlayerID;
	short	profession;
	short	level;
	int	rank;
	std::string		name;
	int	layer;
};

//treasure data for list
struct treasure_data
{
	unsigned int	PlayerID;
	int	count;
	Player_Info	m_PlayerInfo;
	Fight_Attr	m_FightAttr;
};

//only use for player treasure
struct player_treasure
{
	unsigned int	PlayerID;
	unsigned int	last_time;
	unsigned int	attack_time;
	std::string		info;
};

//only use for player achieve info
struct player_achieve_info
{
	unsigned int	PlayerID;
	BitSet	rewards;
	std::string		achieves;
};

//discount shop
struct player_other_info
{
	int	PlayerID;
	int	discount_end_time;
	std::string		buy_item_info;
	unsigned int	last_lost_map_id;
	std::string		last_lost_map_desc;
	unsigned int	last_level_update_time;
};

//drink wine info for list
struct drink_wine_info
{
	unsigned int	PlayerID;
	std::string		PlayerName;
	short	profession;
	short	level;
	short	vip;
	int	suit;
	int	fight_power;
	short	drink_times;
};

//
struct mail_data
{
	unsigned int	index;
	bool	is_get;
	bool	is_read;
	bool	is_del;
	short	ifield;
	unsigned int	itime;
	std::string		name;
	std::string		content;
	std::string		sender;
	std::string		attach;
	std::string		money;
};

//use for table player_order
struct order_data
{
	bool	IsAddMoney;
	int	ItemId;
	std::string		OrderId;
};

//use for table mail_gift
struct mail_gift
{
	int	db_id;
	std::string		items;
	short	date_type;
	int	keep_time;
	std::string		description;
};

//use for table active_reward
struct active_reward
{
	int	db_id;
	int	rank_1;
	int	rank_2;
	int	rank_3;
	int	rank_4;
	int	rank_5;
	int	rank_6;
	int	rank_7;
	int	rank_8;
	int	rank_9;
	int	rank_10;
	int	rank_11_50;
	int	rank_50_;
};

//use for table active_condition
struct active_condition
{
	int	db_id;
	int	level;
	int	gun;
};

//use for table marquee
struct marquee
{
	int	db_id;
	int	line_1;
	int	line_2;
	int	line_3;
	int	line_4;
	int	line_5;
	int	line_6;
	int	line_7;
	int	line_8;
	int	line_9;
	int	line_10;
};

//player rank
struct player_rank
{
	unsigned int	charid;
	int	rank;
	std::string		name;
	short	profession;
	int	num;
	int	id;
};

//mystical shop table
struct mystical_shop
{
	int	db_id;
	short	minlv;
	short	maxlv;
	short	vip;
	short	type;
	int	item_id;
	int	item_size;
	short	rate;
	short	money_type;
	int	money;
};

//use for boss fight real time sort
struct BossUserDam
{
	std::string		name;
	unsigned int	dam;
};

//boss level table
struct active_boss_level
{
	short	db_id;
	short	level_limit;
	int	drop_id;
	int	draw_card_id;
};

//month card
struct month_card
{
	int	db_id;
	int	diamond;
	int	first_rewards;
	int	today_rewards;
	int	overdue_time;
	int	renew_time;
	int	count;
};

//use for table active_inter_shop
struct active_inter_shop
{
	int	db_id;
	short	viplevel;
	short	plan_id;
	int	price;
	int	gold;
	int	diamond;
	int	itemid1;
	int	itemcount1;
	int	itemid2;
	int	itemcount2;
	int	itemid3;
	int	itemcount3;
	int	oldprice;
};

//use for table active_operate_desc
struct active_operate_desc
{
	int	db_id;
	int	starttime;
	int	endtime;
	int	interval;
	short	level;
};

//vip shop 
struct vip_shop
{
	int	level;
	int	itemid;
	int	price;
};

//use for god_weapon
struct god_weapon
{
	int	godweapon_id;
	int	condition;
	int	condition_num;
	int	level;
	int	begin_level;
	int	tips;
};

//use for god_weapon_upgrade
struct god_weapon_upgrade
{
	int	db_id;
	int	fight;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	critical_resist;
	int	break_resist;
	int	miss_odds;
	int	hit_odds;
	int	hurt_deepen;
	int	hurt_remission;
	int	break_defense;
	int	rage_bonuses;
};

//use for god_weapon_upgrade_stage
struct god_weapon_upgrade_stage
{
	int	db_id;
	int	godweapon_id;
	int	level;
	int	quality;
	int	cost_item;
	int	cost_item_num;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	pvp_power;
	int	pvp_resist;
	int	critical_resist;
	int	break_resist;
	int	miss_odds;
	int	hit_odds;
	int	hurt_deepen;
	int	hurt_remission;
	int	break_defense;
	int	rage_bonuses;
	int	skill_id;
	int	skill_lv;
	int	skill_name;
	int	skill_icon;
	int	skill_desc;
	std::string		buff_id;
	int	buff_name;
	int	buff_desc;
	int	buff_battle;
	int	buff_icon;
	int	effid;
};

//use for player_god_weapon
struct player_god_weapon
{
	unsigned int	PlayerID;
	std::string		Datas;
};

//use for player_god_weapon
struct god_weapon_net
{
	int	god_weapon_id;
	int	stage;
};

//discount shop
struct discount_shop
{
	int	db_id;
	int	item_id;
	int	item_count;
	int	money_type;
	int	price;
	int	countless;
	int	oldprice;
};

//player boss fight attr
struct boss_fight
{
	unsigned int	PlayerID;
	std::string		name;
	short	profession;
	int	weapon;
	int	body;
	int	left_finger;
};

//player title
struct player_title
{
	int	PlayerID;
	BitSet	title;
	std::string		titletimes;
};

//title info
struct title_info
{
	int	db_id;
	int	title_type;
	int	condition;
	int	parm1;
	int	parm2;
	int	is_time;
	int	time;
	int	weight;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	pvp_power;
	int	pvp_resist;
	int	critical_resist;
	int	break_resist;
	int	miss_odds;
	int	hit_odds;
	int	hurt_deepen;
	int	hurt_remission;
	int	break_defense;
	int	rage_bonuses;
	int	name_id;
};

//friend pvp data
struct friend_pvp
{
	int	pvp_val;
};

//server info
struct server_info
{
	unsigned int	zoneid;
	int	name;
	int	zonestate;
	int	status;
	std::string		ip;
	int	port;
};

//symbol suit
struct symbol_suit
{
	int	id;
	short	level;
	int	num;
	int	life;
	int	attack;
	int	defense;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	critical_resist;
	int	break_resist;
};

//ci_level_drop
struct ci_level_drop
{
	int	db_id;
	int	item_chance;
	int	item_limit;
	int	gold_chance;
	int	gold_limit;
	int	monster_count;
};

//active_spexp_level
struct active_spexp_level
{
	int	db_id;
	int	level_limit;
	int	monster_count;
	int	base_spexp;
	int	min_card_spexp;
	int	max_card_spexp;
	int	draw_card_id;
	std::string		killcount_reward;
};

//BoxData
struct BoxData
{
	int	Id;
	int	MaxKillCount;
	BitSet	Boxes;
};

//only use for achieve task
struct achieve_task
{
	int	db_id;
	int	name;
	int	type;
	int	param1;
	int	param2;
	int	task_id;
	short	level;
	int	reward1;
	int	reward1_num;
	int	reward2;
	int	reward2_num;
	int	tooltip;
	int	profession;
};

//only use for hard level
struct hard_level
{
	int	db_id;
	int	times;
};

//only use for level state
struct level_state
{
	int	db_id;
	int	times;
	int	star;
};

//only use for equip gem
struct equip_gem
{
	int	db_id;
	int	level;
	int	head;
	int	weapon;
	int	body;
	int	arm;
	int	foot;
	int	leg;
};

//general_shop
struct general_shop
{
	int	db_id;
	int	shop_type;
	int	level_min;
	int	level_max;
	int	vip_min;
	int	vip_max;
	int	item_id;
	int	item_size;
	int	item_buy_times;
	int	buy_limit_type;
	int	money_type;
	int	price;
};

//clothes
struct clothes
{
	int	db_id;
	int	name;
	int	profession;
	int	level;
};

//clothes_level
struct clothes_level
{
	int	db_id;
	int	clothes_id;
	int	level;
	int	item_id;
	int	item_num;
	int	life;
	int	attack;
	int	defense;
	int	breake;
	int	penetrate;
	int	break_resist;
	int	critical;
	int	critical_resist;
	int	injury;
	int	miss_odds;
	int	hit_odds;
	int	hurt_deepen;
	int	hurt_remission;
	int	break_defense;
	int	rage_bonuses;
};

//clothes_level_data
struct clothes_level_data
{
	int	id;
	int	level;
};

//gamble_data
struct gamble_data
{
	int	id;
	unsigned int	total_gamble_times;
	unsigned int	gamble_times;
	unsigned int	free_gamble_time;
	unsigned int	today_gamble_times;
};

//snatch_info
struct snatch_info
{
	unsigned int	charid;
	std::string		name;
	unsigned int	itemid;
	unsigned int	snatch_time;
	int	level;
	int	profession;
	int	fight_power;
	bool	issuccess;
};

//snatch_shop
struct snatch_shop
{
	int	db_id;
	int	time;
	int	cost_item;
	int	cost_money;
};

//snatch_pro
struct snatch_pro
{
	int	db_id;
	int	player_pro;
	int	npc_pro;
	int	item_id;
	int	drop_bag_group_id;
};

//snatch
struct snatch
{
	int	db_id;
	int	type;
	int	is_off;
	int	open_lv;
	int	piece_count;
	int	is_bulletin;
};

//snatch
struct snatch_drop
{
	int	player_lv;
	int	rewards_exp;
	int	rewards_gold;
};

//snatch
struct snatch_mate_info
{
	unsigned int	charid;
	int	level;
	int	profession;
	std::string		name;
	int	fight_power;
};

//extra_drop_item
struct extra_drop_item
{
	unsigned int	db_id;
	int	drop_bag_group_id;
	int	type;
};

//vip_item_shop
struct vip_item_shop
{
	unsigned int	db_id;
	int	money_type;
	int	price;
	int	original_price;
	int	times;
	int	item1_id;
	int	item1_num;
	int	item2_id;
	int	item2_num;
	int	item3_id;
	int	item3_num;
	int	item4_id;
	int	item4_num;
};

//shop_game
struct shop_game
{
	unsigned int	db_id;
	int	price;
	int	selltype;
	int	item_id;
	unsigned int	start_time;
	unsigned int	end_time;
	int	startday;
	int	sellday;
	int	resetday;
	int	viplimit;
};

//activity team level
struct activity_team_level
{
	int	db_id;
	int	level;
	int	type;
	int	item1_id;
	int	item1_num;
	int	item2_id;
	int	item2_num;
	int	item3_id;
	int	item3_num;
	int	drop_id;
	int	level_id;
	std::string		icon_id;
	int	level_name;
	unsigned int	fight_power_recommend;
	int	level_info;
};

//team level rule
struct team_level_rule
{
	int	level;
	int	rule;
};

//guild_info_list
struct guild_info_list
{
	unsigned int	guildid;
	std::string		name;
	unsigned int	curcount;
	unsigned int	level;
	unsigned int	fight_power;
};

//guild_info
struct guild_info
{
	unsigned int	guildid;
	std::string		name;
	std::string		note;
	std::string		notify;
	unsigned int	exp;
	unsigned int	level;
	unsigned int	fight_power;
	unsigned int	curcount;
	unsigned int	currank;
	unsigned int	VIP;
	std::string		masterName;
};

//guild_info
struct guild_player_info
{
	unsigned int	charid;
	std::string		name;
	unsigned int	profession;
	unsigned int	level;
	unsigned int	fight_power;
	unsigned int	VIP;
	unsigned int	occupation;
	unsigned int	Contribtion;
	unsigned int	online;
};

//carnival_task_info
struct carnival_task_info
{
	unsigned int	task_id;
	unsigned int	status;
	unsigned int	cur_data;
};

//god_weapon_location
struct god_weapon_location
{
	int	god_weapon_id;
	int	position;
	int	stage;
};

//
struct item_chip
{
	int	db_id;
	int	name;
	int	type;
	int	level;
	int	quality;
	int	order;
	int	color;
	int	data;
	int	trans_id;
	int	item_id1;
	int	item_num1;
	int	item_id2;
	int	item_num2;
	int	item_id3;
	int	item_num3;
	int	icon_id;
	int	tooltip;
	int	desc;
};

//equip advance
struct equip_advance
{
	int	db_id;
	int	life;
	int	defense;
	int	attack;
	int	critical;
	int	breake;
	int	injury;
	int	penetrate;
	int	criticalresist;
	int	breakresist;
	int	missodds;
	int	hitodds;
	int	hurtdeepen;
	int	hurtremission;
	int	breakdefense;
	int	ragebonuses;
	int	life_rate;
	int	defense_rate;
	int	attack_rate;
	int	critical_rate;
	int	breake_rate;
	int	injury_rate;
	int	penetrate_rate;
	int	criticalresist_rate;
	int	breakresist_rate;
	int	missodds_rate;
	int	hitodds_rate;
	int	hurtdeepen_rate;
	int	hurtremission_rate;
	int	breakdefense_rate;
	int	ragebonuses_rate;
	int	tooltip;
	int	last_id;
};

//guild_info
struct guild_player_worship
{
	unsigned int	charid;
	unsigned int	dbid;
	unsigned int	pvprank;
	std::string		name;
	unsigned int	profession;
	unsigned int	level;
	unsigned int	waepon;
	unsigned int	suit;
	unsigned int	lfinger;
};

//goldstone_data
struct goldstone_data
{
	unsigned int	charId;
	unsigned int	StartTime;
	std::string		Name;
	unsigned int	FightPower;
	unsigned int	Level;
	unsigned int	Profession;
	unsigned int	StoneType;
	unsigned int	StoneId;
	unsigned int	GuildId;
	unsigned int	Money;
};

//goldstone_grab
struct goldstone_grabinfo
{
	bool	fightsuccess;
	bool	IsGrab;
	unsigned int	charId;
	unsigned int	IncomeGold;
	unsigned int	GrabGold;
	std::string		Name;
};

//Operating Activity information
struct OperatingActivity
{
	unsigned int	m_Type;
	bool	m_IsPlaying;
	unsigned int	m_StartTime;
	unsigned int	m_EndTime;
	unsigned int	m_LevelLimit;
	std::string		m_Name;
	unsigned int	m_param;
};

//已团购了道具的玩家信息
struct stBoughtItemInfo
{
	unsigned int	itemId;
	unsigned int	itemNum;
	unsigned int	playerId;
	unsigned int	itemPrice;
	unsigned int	boughtTimes;
	unsigned int	todayBoughtTimes;
};

//已团购的道具信息
struct stGroupBuyingData
{
	unsigned int	itemId;
	unsigned int	itemNum;
	unsigned int	itemCurPrice;
	unsigned int	boughtSum;
};

}
#endif
