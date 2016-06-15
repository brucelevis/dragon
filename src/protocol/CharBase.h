/**
 * \file
 * \version  $Id: CharBase.h 53 2013-04-17 05:03:41Z  $
 * \author  ,@163.com
 * \date 2005年01月07日 21时09分24秒 CST
 * \brief 定义人物角色相关信息
 *
 * 
 */

 
#ifndef _CharBase_h_
#define _CharBase_h_

#include "zType.h"
#pragma pack(1)

struct CharBase
{
	DWORD 	charid;				// 角色编号,charid,全区唯一编号
	char  	nickname[MAX_NAMESIZE];	// 角色名称
	DWORD   profession;
	QWORD   experience;
	DWORD   level;
	DWORD   gold;
	DWORD   diamond;
	DWORD   energy;
	DWORD   vip;
	DWORD   vip_charge;
	DWORD   current_weapon;
	DWORD   current_suit;
	DWORD	ever_battle_layer;
	DWORD   ever_battle_time;
	DWORD   createtime;                      //角色创建时间
	DWORD 	onlinetime;						//在线时间
	DWORD 	offlinetime;					//离线时间
	DWORD   accountid;
	char    platform[MAX_FLAT_LENGTH];
	char    account[MAX_ACCNAMESIZE];
	//CharBase()
	//{
	//	bzero(this,sizeof(*this));
	//}
};

enum ChangeType
{
	CHANGE_NONE             = 0,
	CHANGE_DAY              = 1,                // 按天
	CHANGE_WEEK             = 2,                // 按周
	CHANGE_MONTH            = 3,                // 按月
	CHANGE_PHASE            = 4,                // 按时间段
	CHANGE_DIY              = 5,                // 自定义时间
	CHANGE_SIXHOUR          = 6,                // 每天六点
	CHANGE_DEC_DAYBYDAY     = 7,				// 每天递减
};

enum CounterType
{
	IS_FIRST_LOGIN 					= 0,    //是否是新玩家
	TOTAL_ONLINE_TIME,						//总在线时长
	TODAY_ONLINE_TIME,						//今日在线时长
	SHOW_FIRST_RECHARGE_TIMES,				//首充弹出次数
	IS_GOT_FIRST_RECHARG_REWARD,			//是否获取过首充奖励
	TAKE_CONSUME_REWARD,					//领取消耗奖励
	TOTAL_ITEM_CONSUME,						//物品消耗
	OPEN_RED_ENVELOPE,						//拆红包
	FIRST_RECHARG_AMOUNT,					//首充金额
	GOT_GIFT_BAG,							//领取礼包
	GOT_COMEBACK_REWARD,					//回流奖励
	TAKE_REQUIRE_REWARD,					//条件类奖励
	GLOBAL_SERVICE,							//全服机制


	TOTAL_GAIN,								//累计获得
	TODAY_GAIN,								//今日获得
	TOTAL_CONSUME,							//累计消耗
	TODAY_CONSUME,							//今日消耗

	DAY_PAY_COUNT,							//单日充值数量
	DAY_CONSUME_COUNT_DIAMOND,				//单日消耗(元宝)
	DAY_CONSUME_COUNT_GOLD,					//单日消耗(金币)
	DAY_CONSUME_COUNT_VIM,					//单日消耗(体力)
	DAY_CONSUME_COUNT_VP,					//单日消耗(真气)
	DAY_CONSUME_COUNT_FEAT,					//单日消耗(战功)
	DAY_CONSUME_COUNT_SOUL,					//单日消耗(魂币)
	
	TAKE_OA_FIRST_PAY,						//领取首充奖励
	TAKE_OA_SINGLE_PAY,						//领取单笔充值
	TAKE_OA_DAILY_PAY,						//领取单日充值
	TAKE_OA_DAYS_PAY,						//领取多日充值
	TAKE_OA_SINGLE_CONSUME,					//领取单笔消耗
	TAKE_OA_TOTAL_PAY,						//领取累计充值
	TAKE_OA_TOTAL_CONSUME,					//领取累计消耗
	TAKE_OA_SUCCESSIVE_PAY,					//领取连续充值
	TAKE_OA_SUCCESSIVE_CONSUME,				//领取连续消耗
	TAKE_OA_ONLINE,							//领取在线奖励
	BUY_OA_FUND,							//购买基金分红
	TAKE_OA_FUND,							//领取基金分红
	TAKE_OA_DAILY_GIFT,						//领取每日礼包
	TAKE_OA_RANK,							//领取排行榜奖励
	TAKE_EARLY_LOGIN,						//领取前n次登录奖励
	TAKE_EARLY_LOGIN_TODAY,					//今日是否领取过前n次登录奖励


	TAKE_RED_ENVELOPE,						//领取红包

	LAST_TIME_DAILYREFRESH,					//上次刷新时间（每日刷新）
	ACTIVITY_ONLINE_TIME,					//在线活动总时长
	TAKE_ACTIVITY_ONLINE_TIME,				//上次领取在线活动时间
	LAST_TIME_WEEKREFRESH,					//上次刷新时间（每周刷新）
	LAST_TIME_MONTHREFRESH,					//上次刷新时间（每月刷新）
	LAST_TIME_VIP_LEVEL_UPDATE,             //vip 等级升级的时间

	PAY_CARD_REMAIN_DAY,					//充值卡剩余天数	
	PAY_CARD_DAY_TASK_REWARD,				//充值卡每日任务是否领取

	LAST_TIME_ANTICHECK,					//上次外挂检查时间
	LAST_VALUE_ANTICHECK,					//上次外挂检查值

	IS_FIRST_PVP,							//是否第一次PVP
	

	//boxt 2015-11-19
	IS_FIRST_TIME_INTO_ZHENQI_INSTANCE,		//是否是首次进入真气副本  0否，1是
	IS_FIRST_TIME_INTO_HARD_INSTANCE,		//是否是首次进入困难副本  0否，1是	

	FIRST_TAKE_OA_SUCCESSIVE__LOGIN_TIME,	//第一次领取七日登录的时间
	TAKE_OA_SUCCESSIVE_LOGIN,				//某天的连续登录奖励是否被领取	

	IS_FIRST_INSTANCE,                      //是否首次挑战某副本
	IS_FIRST_EXP_INSTANCE,                  //是否首次挑战经验副本
	IS_FIRST_GOLD_INSTANCE,                 //是否首次挑战金币副本
	IS_FIRST_LDD,							//是否首次挑战生死门
	IS_LIMIT_PURCHASE,                      //限时购买是否已经已经购买过
	DAY_PAY_CASH_COUNT,                     //每日充值多少元
	TOTAL_PAY_CASH_COUNT,                   //总充值多少元
	PASS_LEVEL_HARD_TIMES,                  //通关精英副本的次数
	PASS_LEVEL_TIMES,                       //通关普通副本的次数 

	USER_ATTR_EXTRA_VIPEXP,					//额外VIP经验
	TAKE_ACTIVITY_EVENTS_REWARD,            //领取活跃奖励
	JOIN_CARNIVAL_TIME,                     //上次参加七日活动的时间
	EVENTS_SYMBOL_SNATCH_COUNT,             //符文夺宝次数(活跃活动)
	EVENTS_PASS_LEVEL_HARD,                 //通关精英副本的次数(活跃活动）
	EVENTS_PVP_COUNTS,                      //修罗门之战的次数(活跃活动)
	EVENTS_PASS_LEVEL,                      //通关精英副本的次数(活跃活动)
	EVENTS_LDD_COUNT,                       //生死门之战的次数(活跃活动)

	//运营活动相关
	JOIN_OA_FIRST_PAY_TIME,					//参加首充奖励时间
	JOIN_OA_SINGLE_PAY,						//参加单笔充值时间
	JOIN_OA_DAYS_PAY,						//参加多日充值时间
	JOIN_OA_SINGLE_CONSUME,					//参加单笔消耗时间
	JOIN_OA_TOTAL_PAY,						//参加累计充值时间
	JOIN_OA_TOTAL_CONSUME,					//参加累计消耗时间
	JOIN_OA_SUCCESSIVE_PAY,					//参加连续充值时间
	JOIN_OA_SUCCESSIVE_CONSUME,				//参加连续消耗时间
	JOIN_OA_ONLINE,							//参加在线奖励时间
	JOIN_OA_DAILY_GIFT,						//参加每日礼包时间
	JOIN_OA_RANK,							//参加排行榜奖励时间
	JOIN_EARLY_LOGIN,						//参加前n次登录奖励时间
	JOIN_OA_SUCCESSIVE_LOGIN,               //参加连续登陆时间 
	JOIN_ACTIVITY_EVENTS_REWARD,            //参加活跃活动时间
	IS_OA_DAILY_PAY_RESET,                  //每日充值领取状态是否重置
	IS_OA_HEALTHY_RESET,                    //健康达人活动领取状态是否重置
	TAKE_OA_HEALTHY_REWARD,                 //领取健康达人奖励
	JOIN_OA_ITEM_EXCHANGE,                  //参加道具兑换的时间
	IS_FIRST_ENTER_INSTANCE,                //是否第一次进入副本
	DAY_TURN_FREE_NUM,                      //每日转盘转的次数
	FREE_TURN_TABLE_TIME,                   //记录上一次免费转盘的时间

};

struct AccountItem
{
	DWORD accountid;
	DWORD playerid1;
	DWORD playerid2;
	DWORD playerid3;
	DWORD playerid4;
	char token[MAX_TOKENSIZE + 1];
	DWORD tokentime;
	AccountItem()
	{
		bzero(this,sizeof(*this));
	}

	bool hasCharid(DWORD charid)
	{
		if (playerid1 == charid)
		{
			return true;
		}
		
		if (playerid2 == charid)
		{
			return true;
		}
		
		if (playerid3 == charid)
		{
			return true;
		}

		if (playerid4 == charid)
		{
			return true;
		}
		
		return false;
	}
};

struct ChooseRoleData
{
	DWORD charid;
	char nickname[MAX_NAMESIZE];
	int  profession;
	int  level;
	int  weapon;
	int suit;
	ChooseRoleData()
	{
		 bzero(this,sizeof(*this));
	}
};

#pragma pack()

#endif

