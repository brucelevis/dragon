#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include "Fir.h"
#include "CommonStruct.h"

using namespace FIGHTING;

const std::string KEY_STR = "!@#$%AbCd";


struct st_role_base_info
{
        player_attributes _player_attributes;
        current_equip _current_equip;
        current_epinlay _current_epinlay;
        current_skill _current_skill;
        current_symbol _current_symbo;
        player_enhance _player_enhance;
        common_today_info _common_today_info;
        common_single_use _common_single_use;
        Fight_Attr _fight_attr;
};

struct player_rank_data
{
	unsigned int		PlayerID;
	std::string			name;
	short				profession;
	int					experience;
	short				level;
	int					gold;
	int					fight_power;
	int					GoldRank;
	int					LevelRank;
	int					FightRank;
	player_rank_data():
	PlayerID(0),
		profession(0),
		experience(0),
		level(1),
		gold(0),
		fight_power(0),
		GoldRank(0),
		LevelRank(0),
		FightRank(0)
	{

	}
};

// 今日信息
struct st_common_today_info
{
	short		energy_times;  // 购买体力次数
	short		buy_gold_times; // 购买金币次数
	short		gold_level_times;
	short		exp_level_times;
	short		lottery_times;
	short		gamble_times;
	short		is_login;
	short		present_times;
	short		recieve_times;
	short		buy_gold_level;
	short		buy_exp_level;
	BitSet		shop_limit;
	short		treasure_num;
	BitSet		treasure_state;
	int		active_value;
	BitSet		active_rewards;
	short		bounty_task_times;
	short		month_card_reward;
	short		mystical_shop_times;
	short		buy_mystical_shop;
	short		boss_level_times;
	short		buy_boss_level;
	short		inter_shop_times;
	short		drink_wine_times;
	short		drink_wine_start;
	unsigned int		drink_interval_time;
	short		buy_treasure_times;
	short		left_treasure_times;
	short		enter_battle_times;
	short		buy_battle_times;
	short		left_battle_times;
	short		ever_battle_rewards;
	short		ever_battle_box_rewards;
	short		free_mystical_times;
	short		free_mystical_time;
	st_common_today_info()
	{
		energy_times = 0;
		buy_gold_times = 0;
		gold_level_times = 0;
		exp_level_times = 0;
		lottery_times = 0;
		gamble_times = 0;
		is_login = 0;
		present_times = 0;
		recieve_times = 0;
		buy_gold_level = 0;
		buy_exp_level = 0;
		treasure_num = 0;
		active_value = 0;
		bounty_task_times = 0;
		month_card_reward = 0;
		mystical_shop_times = 0;
		buy_mystical_shop = 0;
		boss_level_times = 0;
		buy_boss_level = 0;
		inter_shop_times = 0;
		drink_wine_times = 0;
		drink_wine_start = 0;
		drink_interval_time = 0;
		buy_treasure_times = 0;
		left_treasure_times = 0;
		enter_battle_times = 0;
		buy_treasure_times = 0;
		left_battle_times = 0;
		ever_battle_rewards = 0;
		ever_battle_box_rewards = 0;
		free_mystical_times = 0;
		free_mystical_time = 0;
	}
};

// 玩家属性
struct st_player_attributes
{
	DWORD accountid;
	DWORD charid;
	std::string account;
	std::string platform;
	DWORD   createtime;                      //角色创建时间
	DWORD 	onlinetime;						//在线时间
	DWORD 	offlinetime;					//离线时间
	std::string		name;
	short		profession;
	int		energy;
	unsigned int		last_fight;
	QWORD		experience;
	short		level;
	int		gold;
	int		diamond;
	int		head;
	int		neck;
	int		shoulder;
	int		weapon;
	int		body;
	int		waist;
	int		arm;
	int		leg;
	int		foot;
	int		wing;
	int		left_finger;
	int		right_finger;
	bool		IsNpc;
	unsigned int		last_login;
	int		login_times;
	short		equip_bags;
	short		gem_bags;
	short		material_bags;
	short		vip;
	int		charge;
	short		sp_level;
	int		sp_exp;
	int		head_1;
	int		neck_1;
	int		shoulder_1;
	int		weapon_1;
	int		body_1;
	int		waist_1;
	int		arm_1;
	int		leg_1;
	int		foot_1;
	int		wing_1;
	int		left_finger_1;
	int		right_finger_1;
	int		head_2;
	int		neck_2;
	int		shoulder_2;
	int		weapon_2;
	int		body_2;
	int		waist_2;
	int		arm_2;
	int		leg_2;
	int		foot_2;
	int		wing_2;
	int		left_finger_2;
	int		right_finger_2;
	int		ever_battle_layer;
	unsigned int		ever_battle_time;
	int     turn_score;

	st_player_attributes()
	{
		createtime = 0;
		onlinetime = 0;
		offlinetime = 0;
		accountid = 0;
		charid = 0;
		profession = 0;
		energy = 0;
		last_fight = 0;
		experience = 0;
		level = 0;
		gold = 0;
		diamond = 0;
		head = 0;
		neck = 0;
		shoulder = 0;
		weapon = 0;
		body = 0;
		waist = 0;
		arm = 0;
		leg = 0;
		foot = 0;
		wing = 0;
		left_finger = 0;
		right_finger = 0;
		IsNpc = 0;
		last_login = 0;
		login_times = 0;
		equip_bags = 0;
		gem_bags = 0;
		material_bags = 0;
		vip = 0;
		charge = 0;
		sp_level = 0;
		sp_exp = 0;
		head_1 = 0;
		neck_1 = 0;
		shoulder_1 = 0;
		weapon_1 = 0;
		body_1 = 0;
		waist_1 = 0;
		arm_1 = 0;
		leg_1 = 0;
		foot_1 = 0;
		wing_1 = 0;
		left_finger_1 = 0;
		right_finger_1 = 0;
		head_2 = 0;
		neck_2 = 0;
		shoulder_2 = 0;
		weapon_2 = 0;
		body_2 = 0;
		waist_2 = 0;
		arm_2 = 0;
		leg_2 = 0;
		foot_2 = 0;
		wing_2 = 0;
		left_finger_2 = 0;
		right_finger_2 = 0;
		ever_battle_layer = 0;
		ever_battle_time = 0;
		turn_score = 0;
	}
};


//当前装备
struct st_current_equip
{
	int		head;
	int		neck;
	int		shoulder;
	int		weapon;
	int		body;
	int		waist;
	int		arm;
	int		leg;
	int		foot;
	int		wing;
	int		left_finger;
	int		right_finger;
	int		suit;
	st_current_equip()
	{
		head = 0;
		neck = 0;
		shoulder = 0;
		weapon = 0;
		body = 0;
		waist = 0;
		arm = 0;
		leg = 0;
		foot = 0;
		wing = 0;
		left_finger = 0;
		right_finger = 0;
		suit = 0;
	}
};

//当前技能
struct st_current_skill
{
	int		active1;
	int		active2;
	int		active3;
	int		passive1;
	int		passive2;
	int		passive3;
	int		unique1;
	st_current_skill()
	{
		active1 = 0;
		active2 = 0;
		active3 = 0;
		passive1 = 0;
		passive2 = 0;
		passive3 = 0;
		unique1 = 0;
	}
};

#endif
