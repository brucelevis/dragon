/**
 * \file
 * \version  
 * \author  
 * \date
 * 
 * \brief 
 *
 *  定义了游戏内的各种事件
 */

#ifndef _ZGAMEEVENTDICTIONARY_H_
#define _ZGAMEEVENTDICTIONARY_H_

////log数据变化类型
//namespace DataChangeWay
//{
	enum DCW
	{
		///<系统支持分类ID段[0,100)
		DCW_NONE                      =0, ///<不记录
		DCW_FIRST_LOGIN               =1, ///<首次登录
		DCW_GM                        =2, ///<gm指令
		DCW_RECHARGE                  =3, ///<用户充值
		DCW_SYS_GIVE                  =4, ///<系统赠送
		DCW_EXCHANGE_CODE             =5, ///<兑换码
		DCW_INVITE_CODE               =6, ///<邀请码
		DCW_CONSECUTIVE_LOGIN         =7, ///<七日连续登录
		DCW_SIGN                      =8, ///<签到
		DCW_LEVEL_GIFT                =9, ///<等级礼包
		DCW_VIP_GIFT                  =10, ///<VIP特权礼包
		DCW_ENERGY_BY_TIME            =11, ///<体力恢复
		DCW_ACTIVE_WORSHIP            =12, ///<膜拜大神
		DCW_CLOTHES_LEVELUP           =13, ///<时装升级
		DCW_OPEN_CHEST                =14, ///<开宝箱
		DCW_ACTIVENESS                =15, ///<活跃度
		DCW_ONLINE_REWARD             =16, ///<在线奖励
		DCW_FIRST_CHARGE              =17, ///<首充
		DCW_RESTORE_VIGOUR            =18, ///<恢复精力
		DCW_NOT_RECORD                =19, ///<程序通用(不记录)

		///<功能模块分类ID段[100,200)
		DCW_MAIL                      =100, ///<邮件【二次分类ID段:[200,300)】
		DCW_ECTYPE                    =101, ///<副本【二次分类ID段:[300,400)】
		DCW_TASK                      =102, ///<任务【二次分类ID段:[400,500)】
		DCW_SHOP                      =103, ///<商店【二次分类ID段:[500,600)】
		DCW_DIAMOND_CHANGE_ATTR       =104, ///<元宝兑换属性【二次分类ID段:[600,700)】
		DCW_LUCK_DRAW                 =105, ///<抽奖【二次分类ID段:[700,800)】
		DCW_PACKAGE                   =106, ///<背包【二次分类ID段:[800,900)】
		DCW_LEVELUP                   =107, ///<玩家升级【二次分类ID段:[900,1000)】
		DCW_RANKING_LIST              =108, ///<排行榜【二次分类ID段:[1000,1100)】
		DCW_GUILD                     =109, ///<公会【二次分类ID段:[1100,1200)】
		DCW_SHENBING                  =110, ///<神兵【二次分类ID段:[1200,1300)】
		DCW_FORGING_EQUIPMENT         =111, ///<装备锻造【二次分类ID段:[1300,1400)】
		DCW_LEARN_SKILL               =112, ///<技能学习【二次分类ID段:[1400,1500)】
		DCW_JINGMAI                   =113, ///<经脉【二次分类ID段:[1500,1600)】
		DCW_RUNES                     =114, ///<符文【二次分类ID段:[1600,1700)】
		DCW_ACHIEVEMENT               =115, ///<成就【二次分类ID段:[1700,1800)】
		DCW_LIFE_GAMBLE               =116, ///<生死门【二次分类ID段:[1800,1900)】
		DCW_WORLD_BOSS                =117, ///<世界boss【二次分类ID段:[1900,2000)】
		DCW_XIULUOMENBANG             =118, ///<修罗门榜【二次分类ID段:[2000,2100)】
		DCW_DUOHUNJIEBAO              =119, ///<夺魂劫宝【二次分类ID段:[2100,2200)】
		DCW_ACTIVITY                  =120, ///<活动【二次分类ID段:[2200,2300)】
		DCW_MATERIAL                  =121, ///<材料【二次分类ID段:[2300,2400)】
		DCW_DROW_CARD                 =122, ///<抽牌【二次分类ID段:[2400,2500)】
		DCW_TREASURE                  =123, ///<宝岛争夺【二次分类ID段:[2500,2600)】
		DCW_REALTIME_PVP              =124, ///<实时对战【二次分类ID段:[2600,2700)】
		DCW_CLIENT_VERSION_UPDATE     =125, ///<客户端版本更新【二次分类ID段:[2700,2800)】


		///<邮件--->二次分类ID段[200,300)
		DCW_FETCH_ATTACH              =200, ///<收取邮件附件
		DCW_OFFLINE_MAIL              =201, ///<离线邮件
		DCW_MAIL_PACKETFULL           =202, ///<背包满的邮件


		///<副本--->二次分类ID段[300,400)
		DCW_COMMON_ECTYPE_PASS        =300, ///<普通副本通关
		DCW_COMMON_ECTYPE_DRAW_CARD   =301, ///<普通副本抽牌
		DCW_COMMON_ECTYPE_SWEEPING    =302, ///<普通副本扫荡
		DCW_COMMON_ECTYPE_LEVEL_BOX   =303, ///<普通副本关卡宝箱
		DCW_COMMON_ECTYPE_CHAPTER_BOX =304, ///<普通副本章节宝箱
		DCW_COMMON_ECTYPE_BUY_TIMES   =305, ///<普通副本重置挑战次数
		DCW_ELITE_ECTYPE_PASS         =306, ///<精英副本通关
		DCW_ELITE_ECTYPE_DRAW_CARD    =307, ///<精英副本抽牌
		DCW_ELITE_ECTYPE_SWEEPING     =308, ///<精英副本扫荡
		DCW_ELITE_ECTYPE_LEVEL_BOX    =309, ///<精英副本关卡宝箱
		DCW_ELITE_ECTYPE_CHAPTER_BOX  =310, ///<精英副本章节宝箱
		DCW_ELITE_ECTYPE_BUY_TIMES    =311, ///<精英副本重置挑战次数
		DCW_GROUP_ECTYPE_REWARD       =312, ///<组队副本奖励
		DCW_GOLD_ECTYPE_REWARD        =313, ///<金币副本通关奖励
		DCW_GOLD_ECTYPE_EXTRA_REWARD  =314, ///<金币副本额外奖励
		DCW_GOLD_ECTYPE_BUY_TIMES     =315, ///<金币副本购买次数
		DCW_EXP_ECTYPE_REWARD         =316, ///<经验副本通关奖励
		DCW_EXP_ECTYPE_EXTRA_REWARD   =317, ///<经验副本额外奖励
		DCW_EXP_ECTYPE_BUY_TIMES      =318, ///<经验副本购买次数
		DCW_ZHENQI_ECTYPE_REWARD      =319, ///<真气副本通关奖励
		DCW_ZHENQI_ECTYPE_EXTRA_REWARD=320, ///<真气副本额外奖励
		DCW_ZHENQI_ECTYPE_BUY_TIMES   =321, ///<真气副本购买次数
		DCW_DIFFICULTY_ECTYPE         =322, ///<困难副本
		DCW_ZHENQI_ECTYPE_DRAW_CARD   =323, ///<真气副本抽牌
		DCW_EXP_ECTYPE_DRAW_CARD      =324, ///<经验副本抽牌
		DCW_GOLD_ECTYPE_DRAW_CARD     =325, ///<金币副本抽牌
		DCW_GROUP_ECTYPE_DRAW_CARD    =326, ///<组队副本抽牌
		DCW_DIFFICULTY_DRAW_CARD      =327, ///<困难副本抽牌
		DCW_GROUP_ECTYPE_PASS         =328, ///<组队副本

		///<任务--->二次分类ID段[400,500)
		DCW_MAIN_TASK                 =400, ///<主线任务
		DCW_REWARD_TASK               =401, ///<悬赏任务
		DCW_AIM_TASK                  =402, ///<目标任务
		DCW_TODAY_TASK                =403, ///<今日任务
		DCW_BOUNTY_TASK               =404, ///<赏金任务
		DCW_BOUNTY_TASK_UPSTAR        =405, ///<赏金任务升星
		DCW_BOUNTY_TASK_REFRESH       =406, ///<赏金任务刷新
		DCW_TACHIEVE_TASK             =407, ///<支线任务


		///<商店--->二次分类ID段[500,600)
		DCW_COMMON_SHOP_BUY_ITEM      =500, ///<普通商店购买道具
		DCW_COMMON_SHOP_SELL_ITEM     =501, ///<普通商店卖出道具
		DCW_MYSTERY_SHOP_BUY_ITEM     =502, ///<神秘商店购买道具
		DCW_MYSTERY_SHOP_REFRESH      =503, ///<神秘商店刷新


		///<元宝兑换属性--->二次分类ID段[600,700)
		DCW_DIAMOND_CHANGE_GOLD       =600, ///<元宝兑换金币
		DCW_DIAMOND_CHANGE_ENERGY     =601, ///<元宝兑换体力
		DCW_DIAMOND_CHANGE_SAODANGJUAN=602, ///<元宝购买扫荡卷
		DCW_DIAMOND_CHANGE_VIGOUR     =603, ///<元宝兑换精力

		///<抽奖--->二次分类ID段[700,800)
		DCW_FREE_DRAW                 =700, ///<免费抽奖
		DCW_SINGLE_DRAW               =701, ///<抽奖一次
		DCW_TEN_DRAW                  =702, ///<抽奖十连抽


		///<背包--->二次分类ID段[800,900)
		DCW_PACK_SELL_ITEM            =800, ///<背包物品卖出
		DCW_PACK_OPEN_GIFT            =801, ///<背包礼包开启
		DCW_PACK_CHIP_COMPOUND        =802, ///<碎片合成
		DCW_PACK_CHIP_SPLIT           =803, ///<碎片分解


		///<玩家升级--->二次分类ID段[900,1000)
		DCW_LEVELUP_REWARD            =900, ///<玩家升级奖励
		DCW_PLAYER_PRACTICE           =901, ///<玩家修练


		///<排行榜--->二次分类ID段[1000,1100)
		DCW_LEVEL_RANKING_LIST        =1000, ///<等级排行榜
		DCW_BATTLE_RANKING_LIST       =1001, ///<战斗力排行榜
		DCW_GOLD_RANKING_LIST         =1002, ///<金币排行榜


		///<公会--->二次分类ID段[1100,1200)
		DCW_CREATE_GUILD              =1100, ///<创建公会
		DCW_GUILD_DONATE              =1101, ///<公会捐献
		DCW_GUILD_WORSHIP             =1102, ///<公会膜拜
		DCW_HUNBI_CHANGE_ITEM         =1103, ///<公会兑换
		DCW_GUILD_BATTLE              =1104, ///<公会战
		DCW_GUILD_SKILL_LEVLEUP       =1105, ///<公会技能升级
		DCW_GUILD_SHOP                =1106, ///<公会商店


		///<神兵--->二次分类ID段[1200,1300)
		DCW_GODWEAPON_LEVELUP         =1200, ///<神兵升级
		DCW_GODWEAPON_UPGRADE         =1201, ///<神兵升星


		///<装备锻造--->二次分类ID段[1300,1400)
		DCW_EQUIP_ENHANCE             =1300, ///<装备强化
		DCW_EQUIP_UPSTAR              =1301, ///<装备升星
		DCW_EQUIP_INLAYGEM            =1302, ///<宝石镶嵌
		DCW_EQUIP_ATTR_ENHANCE        =1303, ///<装备淬炼
		DCW_EQUIP_UPGRADE             =1304, ///<装备进阶
		DCW_EQUIP_SPLIT_ITEM          =1305, ///<装备分解
		DCW_EQUIP_COMPOUND            =1306, ///<宝石合成


		///<技能学习--->二次分类ID段[1400,1500)
		DCW_SKILL_LEVELUP             =1400, ///<技能升级


		///<经脉--->二次分类ID段[1500,1600)
		DCW_MERIDIANS_LEVELUP         =1500, ///<经脉升级


		///<符文--->二次分类ID段[1600,1700)
		DCW_RUNES_ENHANCE             =1600, ///<符文强化
		DCW_RUNES_UPGRADE             =1601, ///<符文升阶
		DCW_RUNES_SPLIT               =1602, ///<符文分解
		DCW_RUNES_COMPOUND            =1603, ///<符文合成
		DCW_RUNES_SHOP_CHANGE         =1604, ///<符文商店兑换
		DCW_RUNES_WISH                =1605, ///<符文祈愿


		///<成就--->二次分类ID段[1700,1800)
		DCW_ACHIEVEMENT_AIM_REWARD    =1700, ///<成就目标奖励
		DCW_ACHIEVEMENT_PROGRESS_REWARD         =1701, ///<成就章节进度奖励


		///<生死门--->二次分类ID段[1800,1900)
		DCW_LIFE_GAMBLE_BUY_TIMES     =1800, ///<生死门-购买挑战次数
		DCW_LIFE_GAMBLE_BATTLE_REWARD =1801, ///<生死门-挑战奖励
		DCW_LIFE_GAMBLE_SWEEPING      =1802, ///<生死门-扫荡
		DCW_LIFE_GAMBLE_CHANGE        =1803, ///<生死门-兑换


		///<世界boss--->二次分类ID段[1900,2000)
		DCW_WORLDBOSS_TOPLIST         =1900, ///<世界boss排行榜
		DCW_WORLDBOSS_AIM_REWARD      =1901, ///<世界boss目标奖励
		DCW_WORLDBOSS_BUY_TIMES       =1902, ///<世界boss购买次数
		DCW_WORDBOSS_FIGHTER_RWARD    =1903, ///<世界boss战斗奖励


		///<修罗门榜--->二次分类ID段[2000,2100)
		DCW_XLM_RANKING_REWARD        =2000, ///<修罗门-每日排行榜奖励
		DCW_XLM_BATTLE_REWARD         =2001, ///<修罗门-挑战奖励
		DCW_XLM_BEST_RNAG_UP_REWARD   =2002, ///<修罗门-突破奖励
		DCW_XLM_CLEAN_CD              =2003, ///<修罗门-刷新CD
		DCW_XLM_CLEAN__REFRESH        =2004, ///<修罗门-刷新
		DCW_XLM_BUY_TIMES             =2005, ///<修罗门-购买次数
		DCW_XLM_SHOP                  =2006, ///<竞技场商店


		///<夺魂劫宝--->二次分类ID段[2100,2200)
		DCW_DHJB_QUICK_ROB            =2100, ///<夺魂劫宝-快速抢夺
		DCW_DHJB_FIVE_ROB             =2101, ///<夺魂劫宝-五连夺
		DCW_DHJB_REFRESH              =2102, ///<夺魂劫宝-换一批
		DCW_DHJB_REFUSE_BATTLE        =2103, ///<夺魂劫宝-免战


		///<活动--->二次分类ID段[2200,2300)
		DCW_ACT_MONTH_CARD            =2200, ///<月卡
		DCW_ACT_EVER_CARD             =2201, ///<终身卡
		DCW_ACT_TODAY_AMASS_CHARGE    =2202, ///<每日累计充值
		DCW_ACT_AMASS_CHARGE          =2203, ///<累计充值
		DCW_ACT_CHARGE_MOST           =2204, ///<充值达人
		DCW_ACT_BATTLE_HIGHEST        =2205, ///<战力达人
		DCW_ACT_COST_AMASS            =2206, ///<累计消耗
		DCW_ACT_BUY_FUND              =2207, ///<购买基金
		DCW_ACT_OPEN_REDENVELOPE      =2208, ///<领取红包
		DCW_ACT_CHANGE_ITEM           =2209, ///<道具兑换
		DCW_ACT_LIMIT_TIME_BUY_ITEM   =2210, ///<限时购买道具
		DCW_ACT_DISCOUNT_SHOP_BUY     =2211, ///<折扣商店购买道具
		DCW_ACT_HIGHEST_LEVEL         =2212, ///<竞技达人
		DCW_ACT_HIGHEST_BATTLE        =2213, ///<战力狂人
		DCW_ACT_CONTINUE_LOGIN        =2214, ///<连续登录
		DCW_ACT_CONTINUE_CHARGE       =2215, ///<连续充值
		DCW_ACT_CONTINUE_COST         =2216, ///<连续消耗
		DCW_ACT_EVERYDAY_REWARD       =2217, ///<每日礼包
		DCW_ACT_EVENTS                =2218, ///<活跃活动
		DCW_ACT_HEALTHY               =2219, ///<健康达人
		DCW_ACT_GROUPBUYING =2220,///<团购活动

		///<材料--->二次分类ID段[2300,2400)
		DCW_MATERIAL_COMPOUND         =2300, ///<材料合成
		DCW_USE_MATERIAL              =2301, ///<使用材料

		///<抽牌--->二次分类ID段[2400,2500)

		///<宝岛争夺--->二次分类ID段[2500,2600)


		DCW_REALTIME_PVP_MONEY        =2600, ///实时对战金币

		DCW_CLIENT_VERSION_UPDATE_REWARD = 2700, ///实时对战金币


	};
//}/// namespace DataChangeWay


enum GED
{
	//gm指令
	GM =0,
	//首次登陆
	FIRSTLOGIN = 1,
	//签到奖励
	REGISTRATION =2,
	//最终签到奖励
	REGISTRATIONFINAL =3,
	//邀请码
	INVITECODE = 4,
	//竞技场刷新
	WARMMATCH_REFRESH = 5,
	//副本通关
	DUPLICATEPASS = 6,
	//元宝大转盘
	INGOTTURNTABLE = 7,
	//金币大转盘
	GOLDTURNTABLE = 8,
	//邮件附件
	MAILAFFIX = 9,
	//任务奖励
	QUESTREWARD = 10,
	//小鸡快跑（金币副本）
	CHICKENRUN = 11,
	//经验副本
	EXP = 12,
	//未知冒险
	UNKNOWNADV = 13,
	//竞技场
	WARMMATCH = 14,
	//活动
	LOG_ACTIVITY = 15,
	//元宝购买金币
	LOG_BUYGOLD = 16,
	//每日任务
	LOG_DAILY_QUEST = 17,
	//竞技场获胜
	WARMMATCH_WIN = 18,
	//普通商店购买道具
	SHOP_ITEM_BUY = 19,
	//购买体力
	BUYPHYFORCE = 20,
	//抽奖商店单抽
	SHOP_SINGLE_STONE = 21,
	//抽奖商店十连抽
	SHOP_TEN_STONE = 22,
	//世界boss排行
	WORDBOSSRANKER = 23,
	//世界boss战斗奖励
	WORDBOSSFIGHTER = 24,
	//技能学习
	SKILL_LEARN = 25,
	//用户充值
	CHARGE =26,
	//充值月卡
	LOG_CHARGE_MONTH_CARD=27,
	//酒宴
	DRINK_WINE = 28,
	//背包卖出材料
	SELL_MATERIAL =29,
	//背包卖出宝石
	SELL_GEM = 30,
	//竞技场失败
	WARMMATCH_FAIL = 31,
	//升级奖励
	LEVEL_UPGRADE = 32,
	//竞技场购买次数
	WARMMATCH_BUY_TIMES = 33,
	//竞技场刷新CD
	WARMMATCH_CLEAN_CD = 34,
	//抽牌
	DRAW_CARD = 35,
	//真气副本
	SP_MAP = 36,
	// 等级排行榜
	LEVELRANK_REWARD = 37,
	// 战力排行榜
	FIGHTRANK_REWARD = 38,
	// 恢复
	RESTORE = 39,
	//竞技场挑战
	WARMMATCH_CHALLENGE = 40,
	// 公会捐赠
	GUILD_DONATE = 41,
	// 购买
	BUY = 42,

	//神兵
	SB = 1001,
	//经脉
	JM = 1002,
	//符文
	FW = 1003,
	//装备升阶
	ZBSJ = 1005,
	//装备分解
	ZBFJ = 1006,
	//装备升星
	ZBSX = 1007,
	//宝石镶嵌
	BSXQ = 1008,
	//装备附魔
	ZBFM = 1009,
	//成就
	CJ = 1010,
	//世界boss贡献
	WORDBOSSCONT = 1011,
	//神秘商店
	SMSD = 1012,
	//竞技场排名结算
	JJCPM = 1014,
	//竞技场破纪录
	JJCPJL = 1015,
	//竞技场商店
	JJCSD = 1016,
	//生死门
	SSM = 1017,
	//宝藏争夺
	BZZD = 1018,
	//赏金任务
	SJRW = 1019,
	//连续登录
	LXDL = 1020,
	//活跃度
	HYD = 1021,
	//修炼离线
	XL = 1022,
	//半年卡
	BNK = 1023,
	//终身卡
	ZSK = 1024,
	//折扣商店
	ZDSD = 1026,
	//VIP商店
	VIPSD = 1027,
	//兑换商店
	DHSD = 1028,
	//精英副本
	JYFB = 1029,
	//困难副本
	KNFB = 1030,
	//赏金任务升星
	SJRWSX = 1031,
	//内购商店
	INNER_SHOP = 1032,
	//世界BOSS购买次数
	WORDBOSS_BUYTIME = 1033,
	//使用材料
	USE_MATERIAL = 1034,
	//副本失败
	DUPLICATEFAIL = 1035,
	//VIP
	VIP = 1037,
	//午餐，晚餐赠送体力
	ENERGY_BY_NOON_OR_NIGHT = 1038,
	//符文强化
	FW_ENHANCE = 1039,
	//符文进阶
	FW_UPGREADE = 1040,
	//宝石合成
	GEM_COMPOUND = 1041,
	//材料合成
	MATERIAL_COMPOUND = 1042,
	//离线邮件
	MAIL_OFFLINE = 1043,
	//包裹满
	MAIL_PACKETFULL = 1044,
	//兑换码
	EXCHANGE_CODE = 1045,
	//开宝箱
	OPEN_CHEST = 1046,
	//基金
	FUND = 1047,
	//红包
	RED_BAG = 1048,

	//七日登录
	QRDL = 1049,
	//在线礼包
	ZXLB = 1050,
	//等级礼包
	DJLB = 1051,
	//元宝消耗
	YBXH = 1052,
	//体力消耗
	TLXH = 1053,
	//战力狂人
	ZLKR = 1054,
	//竞技达人
	JJDR = 1055,

	// 购买扫荡卷
	GMSDJ = 1056,
	// 购买商城物品
	GMSCWP = 1057,

	GUILDWAR = 1058,

	// 组队副本
	ZZFB = 1059,
	//普通副本
	PTFB = 1060,
	// 时装升级
	SZSJ = 1061,
	// 免战
	MZ = 1062,
	// 购买道具
	PLAYER_BUY_ITEM = 1063,
	//装备合成
	ZBHC = 1064,

	//碎片合成
	SPHC = 1065,
	//碎片分解
	SPFJ = 1066,
	//分解符文
	FJFW = 1067,
	//消耗体力
	COST_VIGOUR = 1068,
	//消耗精力
	COST_ENERGY = 1069,
	// 道具兑换
	ITEM_EXCHANGE = 1070,
	// 夺魂劫宝
	DHJB =  1073,
	// 道具出售
	PROPSSALE = 1074,
	//主任务进度
	MAINSTEP = 1075,
	//每日任务进度
	EVERRYDAYSTEP = 1076,
	//赏金任务进度
	SJRWJD = 1077,
	//赏金任务完成
	SJRWWC = 1078,
	//公会商店
	STOREGANG = 1079,
	// 限时购买
	LIMIT_TIME_PURCHASE = 1080,
	// 神兵祭坛升级
	GODWEAPON_ALTAS_LEVELUP = 1081,
	// 神秘商店刷新
	MYSTICALSHOP_REFRESH = 1082,
	// 运营活动  -- 活跃活动
	OA_ACTIVITY_EVENT = 1083,
	// GM设置战功
	GM_SET_BATTLE_FEAT= 8991,

	// GM设置真气
	GM_SET_VITAL= 8992,

	// GM设置魂币
	GM_SET_SOUL_COIN= 8993,

	// GM设置体力
	GM_SET_POWER= 8994,

	// GM设置VIP
	GM_SET_VIP = 8995,

	// GM设置经验
	GM_SET_EXP = 8996,

	// GM设置等级
	GM_SET_LEVEL = 8997,

	// GM设置金币
	GM_SET_GOLD = 8998,

	// GM设置元宝
	GM_SET_DIAMOND = 8999,
	// 系统设置元宝
	SYS_SET_DIAMOND = 9999,
	
	NONE = 99999
};

enum MONEYTYPE
{
	//金币
	GOLD = 0,
	//元宝宝石
	DIAMOND = 1,
	//魂石
	SOULSTONE = 2,

	MTNONE = 99999
};

//增加还是减少
enum AORD
{
	ADD = 0 ,
	REDUCE = 1
};


#endif
