//This file is auto generated. DO NOT MODIFY IT!

#ifndef _packet_h_
#define _packet_h_

#include "BasePacket.h"
#include "CommonStruct.h"

namespace FIGHTING
{

class Packet : public BasePacket 
{
public:
		Packet():
		  BasePacket()
		  { }
		  Packet(int size):
		  BasePacket(size)
		  { }
		  void swap(Packet& _Right);
public:
	virtual Packet &operator<<(Packet &p)
	{
		return *((Packet*)&BasePacket::operator<<((*(BasePacket*)&p)));
	}
	virtual Packet &operator<<(const std::string &str)
	{
		return *((Packet*)&BasePacket::operator<<(str));
	}
	virtual Packet &operator<<(const char *str)
	{
		return *((Packet*)&BasePacket::operator<<(str));
	}
	virtual Packet &operator<<(char *str)
	{
		return *((Packet*)&BasePacket::operator<<(str));
	}
	virtual Packet &operator>>(std::string &str)
	{
		return *((Packet*)&BasePacket::operator>>(str));
	}
	virtual Packet &operator>>(char *str)
	{
		return *((Packet*)&BasePacket::operator>>(str));
	}
	template<class T>
	Packet &operator<<(const T &value)
	{
		return *((Packet*)&BasePacket::operator<<(value));
	}
	template<class T>
	Packet &operator>>(T &value)
	{
		return *((Packet*)&BasePacket::operator>>(value));
	}
	Packet& operator<<(const NAT_TYPE& nt)
	{
		return *((Packet*)&BasePacket::operator<<((unsigned char)nt));
	}
	Packet &operator>>(NAT_TYPE &nt)
	{
		unsigned char temp;
		Packet& p = *((Packet*)&BasePacket::operator>>(temp));
		nt = (NAT_TYPE)temp;
		return p;
	}
	Packet& operator<<(const BitSet& b)
	{
		return *((Packet*)&BasePacket::operator<<(b));
	}
	Packet& operator>>(BitSet& b)
	{
		return *((Packet*)&BasePacket::operator>>(b));
	}

////////////////////////////////////////////////////////
	Packet &operator<<(const IP_PORT &val)
	{
		(*this)<< val.m_natType << val.m_exterIp << val.m_exterPort << val.m_interIp << val.m_interPort ;
		return (*this);
	}
	Packet &operator<<(const IPORT &val)
	{
		(*this)<< val.m_ip << val.m_port ;
		return (*this);
	}
	Packet &operator<<(const config_language &val)
	{
		(*this)<< val.db_id << val.info ;
		return (*this);
	}
	Packet &operator<<(const config_mixed &val)
	{
		(*this)<< val.db_id << val.int_val << val.str_val << val.type << val.desc ;
		return (*this);
	}
	Packet &operator<<(const equip_attribute &val)
	{
		(*this)<< val.db_id << val.color << val.type << val.level_limit << val.is_enhance << val.is_upgrade << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.pvp_power << val.pvp_resist << val.critical_resist << val.break_resist << val.energy_add << val.exp_add << val.gold_add << val.suit_id << val.enhance_rate << val.quality << val.order << val.advance_id << val.tooltip << val.advance_1 << val.advance_2 ;
		return (*this);
	}
	Packet &operator<<(const equip_bowman &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.model_id << val.desc ;
		return (*this);
	}
	Packet &operator<<(const equip_double_sword &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.model_id << val.desc ;
		return (*this);
	}
	Packet &operator<<(const equip_enhance &val)
	{
		(*this)<< val.db_id << val.type << val.level << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.pvp_power << val.pvp_resist << val.critical_resist << val.break_resist << val.energy_add << val.exp_add << val.gold_add ;
		return (*this);
	}
	Packet &operator<<(const equip_grow &val)
	{
		(*this)<< val.db_id << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.pvp_power << val.pvp_resist << val.critical_resist << val.break_resist << val.limit ;
		return (*this);
	}
	Packet &operator<<(const equip_hammer &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.model_id << val.desc ;
		return (*this);
	}
	Packet &operator<<(const equip_sword &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.model_id << val.desc ;
		return (*this);
	}
	Packet &operator<<(const equip_upgrade &val)
	{
		(*this)<< val.db_id << val.material1 << val.mat_size1 << val.material2 << val.mat_size2 << val.material3 << val.mat_size3 << val.rare_material << val.rare_size << val.gold << val.trans_id ;
		return (*this);
	}
	Packet &operator<<(const item_gem &val)
	{
		(*this)<< val.db_id << val.name << val.type << val.level << val.icon_id << val.value << val.trans_id << val.price << val.tooltip << val.desc << val.color << val.order << val.quality ;
		return (*this);
	}
	Packet &operator<<(const item_gift_bag &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.limit << val.items << val.tooltip << val.desc ;
		return (*this);
	}
	Packet &operator<<(const item_goods &val)
	{
		(*this)<< val.db_id << val.item_id << val.item_size << val.is_list << val.list_order << val.is_hot << val.is_new << val.gold << val.gem << val.money_type << val.level_limit << val.is_day << val.is_ever ;
		return (*this);
	}
	Packet &operator<<(const item_material &val)
	{
		(*this)<< val.db_id << val.name << val.type << val.level << val.data << val.trans_id << val.price << val.icon_id << val.tooltip << val.desc << val.quality << val.order << val.color << val.type_button ;
		return (*this);
	}
	Packet &operator<<(const item_snatch_chip &val)
	{
		(*this)<< val.db_id << val.name << val.type << val.level << val.data << val.trans_id << val.price << val.icon_id << val.tooltip << val.desc << val.quality << val.order << val.color << val.type_button ;
		return (*this);
	}
	Packet &operator<<(const item_money &val)
	{
		(*this)<< val.db_id << val.name << val.price << val.color << val.icon_id << val.tooltip << val.desc ;
		return (*this);
	}
	Packet &operator<<(const item_symbol &val)
	{
		(*this)<< val.db_id << val.name << val.color << val.attribute << val.base_exp << val.value << val.icon_id << val.trans_id << val.item_id << val.item_size << val.decompose_item << val.gold << val.max_level << val.desc ;
		return (*this);
	}
	Packet &operator<<(const item_symbol_limit &val)
	{
		(*this)<< val.pos1 << val.pos2 << val.pos3 << val.pos4 << val.pos5 << val.pos6 << val.pos7 << val.pos8 << val.pos9 << val.pos10 << val.pos11 ;
		return (*this);
	}
	Packet &operator<<(const item_symbol_upgrade &val)
	{
		(*this)<< val.level << val.scale << val.exp1 << val.exp2 << val.exp3 << val.exp4 << val.exp5 << val.exp6 << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.critical_resist << val.break_resist ;
		return (*this);
	}
	Packet &operator<<(const symbol_base &val)
	{
		(*this)<< val.db_id << val.level_limit << val.vip_limit << val.next_id << val.rate << val.gold << val.diamond << val.item_id << val.item_num << val.sp_type << val.sp_num << val.energy ;
		return (*this);
	}
	Packet &operator<<(const symbol_random &val)
	{
		(*this)<< val.symbol << val.com_rewards << val.sp_rewards << val.first_rewards << val.desc ;
		return (*this);
	}
	Packet &operator<<(const part_enhance &val)
	{
		(*this)<< val.level << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.critical_resist << val.break_resist << val.gold << val.com_crystal_id << val.com_crystal_num << val.per_crystal_id << val.per_crystal_num ;
		return (*this);
	}
	Packet &operator<<(const player_upgrade &val)
	{
		(*this)<< val.level << val.experience << val.gold << val.diamond << val.item_id << val.count << val.practice << val.treasure_id << val.bounty_exp << val.max_star << val.day_money << val.day_item << val.bounty_god << val.bounty_sp ;
		return (*this);
	}
	Packet &operator<<(const res_icon &val)
	{
		(*this)<< val.db_id << val.name << val.desc ;
		return (*this);
	}
	Packet &operator<<(const res_model &val)
	{
		(*this)<< val.db_id << val.name << val.desc ;
		return (*this);
	}
	Packet &operator<<(const res_sound &val)
	{
		(*this)<< val.db_id << val.name << val.desc ;
		return (*this);
	}
	Packet &operator<<(const skill_upgrade &val)
	{
		(*this)<< val.level << val.need_level << val.gold << val.add_attribute << val.add_attack << val.item_id << val.item_size << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.critical_resist << val.break_resist << val.fight_power << val.rank << val.main_id << val.break_defense ;
		return (*this);
	}
	Packet &operator<<(const skill_attribute &val)
	{
		(*this)<< val.db_id << val.type << val.attr_type ;
		return (*this);
	}
	Packet &operator<<(const skill_bowman &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.tooltip << val.model_id << val.desc ;
		return (*this);
	}
	Packet &operator<<(const skill_double_sword &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.tooltip << val.model_id << val.desc ;
		return (*this);
	}
	Packet &operator<<(const skill_hammer &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.tooltip << val.model_id << val.desc ;
		return (*this);
	}
	Packet &operator<<(const skill_sword &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.tooltip << val.model_id << val.desc ;
		return (*this);
	}
	Packet &operator<<(const vip_privilege &val)
	{
		(*this)<< val.level << val.charge_num << val.saodang_num << val.energy_num << val.buy_gold_num << val.reset_level_num << val.gold_free_times << val.gold_level_num << val.exp_free_times << val.exp_level_num << val.present_num << val.recieve_num << val.arena_free_times << val.pvp_rank_num << val.pr_feat_num << val.eb_free_num << val.ever_battle_num << val.practice_num << val.mystical_shop_num << val.boss_level_num << val.gold_critical_rate << val.gold_critical_times << val.gold_free_num << val.mystical_shop_item_num << val.buy_treasure_times << val.treasure_times << val.treasure_rate << val.maxtreasure_rate << val.treasure_desc1 << val.treasure_desc2 << val.treasure_desc3 << val.buy_hard_num << val.bounty_dis << val.bounty_yield << val.spexp_level_num << val.spexp_level_rate << val.vigour_num << val.guildbattle_times << val.reset_normallevel_num << val.cuilian_num << val.cuilian_high << val.fight_5 << val.mystical_shop_reset_num << val.goldstone_free_times << val.goldstone_level_num << val.shop_goditem_num ;
		return (*this);
	}
	Packet &operator<<(const vip_times_price &val)
	{
		(*this)<< val.db_id << val.energy << val.gold << val.reset_level << val.pvp_rank_times << val.gold_level_times << val.exp_level_times << val.ever_battle_times << val.boss_level_times << val.mystical_shop_times << val.buy_treasure_times << val.spexp_level_times << val.vigour_times << val.reset_normallevel << val.goldstone_level_times ;
		return (*this);
	}
	Packet &operator<<(const wheel_lottery &val)
	{
		(*this)<< val.db_id << val.gold_rate << val.diamond_rate << val.gold_rand_rate << val.diamond_rand_rate ;
		return (*this);
	}
	Packet &operator<<(const gamble_form &val)
	{
		(*this)<< val.db_id << val.get_times << val.free_times << val.interval_time << val.first_bag << val.free_bag << val.every_day << val.money_type << val.price << val.sp_min_times << val.sp_max_times << val.sp_success_rate << val.sp_bag << val.bag_1 << val.bag_1_rate << val.bag_2 << val.bag_2_rate << val.bag_3 << val.bag_3_rate << val.itemid << val.itemcount ;
		return (*this);
	}
	Packet &operator<<(const level_rewards &val)
	{
		(*this)<< val.db_id << val.level << val.item_id1 << val.item_size1 << val.item_id2 << val.item_size2 << val.item_id3 << val.item_size3 ;
		return (*this);
	}
	Packet &operator<<(const login_rewards &val)
	{
		(*this)<< val.item_id << val.item_size << val.vip_level ;
		return (*this);
	}
	Packet &operator<<(const pay_rewards &val)
	{
		(*this)<< val.level << val.item_id1 << val.item_size1 << val.item_id2 << val.item_size2 << val.item_id3 << val.item_size3 ;
		return (*this);
	}
	Packet &operator<<(const active_boss_monster &val)
	{
		(*this)<< val.db_id << val.type << val.name << val.level << val.life << val.defense << val.attack << val.breake << val.injury << val.penetrate << val.criticalresist << val.breakresist << val.attack_weight << val.desc ;
		return (*this);
	}
	Packet &operator<<(const active_boss_rewards1 &val)
	{
		(*this)<< val.db_id << val.item_id1 << val.size1 << val.item_id2 << val.size2 ;
		return (*this);
	}
	Packet &operator<<(const active_boss_rewards2 &val)
	{
		(*this)<< val.db_id << val.icon_id << val.target << val.type << val.value << val.item_id << val.item_size << val.tooltip ;
		return (*this);
	}
	Packet &operator<<(const active_desc &val)
	{
		(*this)<< val.db_id << val.name << val.show_name << val.start_time << val.end_time << val.start_time1 << val.end_time1 << val.level << val.free_times << val.charge << val.charge_add << val.charge_times << val.is_open << val.user_data1 << val.user_data2 << val.rewards_table << val.icon_id << val.tooltip ;
		return (*this);
	}
	Packet &operator<<(const active_pvp_rewards &val)
	{
		(*this)<< val.db_id << val.win_reward << val.defeat_reward << val.rank_reward << val.diamond_reward ;
		return (*this);
	}
	Packet &operator<<(const active_pvp_trade &val)
	{
		(*this)<< val.db_id << val.feat << val.item_id << val.item_size ;
		return (*this);
	}
	Packet &operator<<(const active_pvp_robot &val)
	{
		(*this)<< val.db_id << val.name << val.rank << val.level << val.profession << val.head << val.head_enhance << val.head_star << val.weapon << val.weapon_enhance << val.weapon_star << val.body << val.body_enhance << val.body_star << val.arm << val.arm_enhance << val.arm_star << val.leg << val.leg_enhance << val.leg_star << val.foot << val.foot_enhance << val.foot_star << val.left_finger << val.right_finger << val.suit << val.fight_power << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.criticalresist << val.breakresist << val.skill ;
		return (*this);
	}
	Packet &operator<<(const day_task &val)
	{
		(*this)<< val.db_id << val.name << val.level << val.value << val.order << val.icon_id << val.tooltip << val.reward_money << val.reward_moneynum << val.reward_item << val.reward_itemnum << val.param ;
		return (*this);
	}
	Packet &operator<<(const pay_money &val)
	{
		(*this)<< val.db_id << val.name << val.base_value << val.add_value1 << val.add_value2 << val.money << val.is_limit << val.is_recommend << val.order << val.icon_id << val.tooltip1 << val.tooltip2 ;
		return (*this);
	}
	Packet &operator<<(const gold_level &val)
	{
		(*this)<< val.db_id << val.level_limit << val.score << val.gold_rate << val.diamond_rate << val.level_id << val.tooltip << val.small << val.middle << val.big << val.icon_id << val.mo_total << val.killcount_reward << val.draw_card_id ;
		return (*this);
	}
	Packet &operator<<(const active_exp_level &val)
	{
		(*this)<< val.db_id << val.level_limit << val.max_exp << val.level_id << val.icon_id << val.tooltip << val.max_health << val.mo_attack << val.add_attack1 << val.add_attack2 << val.add_exp << val.add_rate << val.mo_total << val.killcount_reward << val.draw_card_id ;
		return (*this);
	}
	Packet &operator<<(const equip_split &val)
	{
		(*this)<< val.db_id << val.mat_id1 << val.mat_num1 << val.mat_id2 << val.mat_num2 << val.mat_id3 << val.mat_num3 << val.mat_id4 << val.mat_num4 ;
		return (*this);
	}
	Packet &operator<<(const exchange_shop &val)
	{
		(*this)<< val.db_id << val.item_id << val.mat_id << val.mat_num << val.level << val.index ;
		return (*this);
	}
	Packet &operator<<(const active_battle_rewards &val)
	{
		(*this)<< val.db_id << val.gift_id ;
		return (*this);
	}
	Packet &operator<<(const active_ever_battle &val)
	{
		(*this)<< val.db_id << val.com_gift << val.sp_gift << val.price << val.level_id << val.name << val.tooltip << val.desc ;
		return (*this);
	}
	Packet &operator<<(const activeness_rewards &val)
	{
		(*this)<< val.db_id << val.value << val.gold << val.diamond << val.gift_id ;
		return (*this);
	}
	Packet &operator<<(const continue_login_rewards &val)
	{
		(*this)<< val.db_id << val.gold << val.diamond << val.gift_id ;
		return (*this);
	}
	Packet &operator<<(const active_treasure_level &val)
	{
		(*this)<< val.db_id << val.item_id << val.size << val.max_size ;
		return (*this);
	}
	Packet &operator<<(const player_meridian &val)
	{
		(*this)<< val.db_id << val.name << val.level << val.is_up << val.item_id << val.item_size << val.sp_exp << val.gold << val.life << val.attack << val.defense << val.critical << val.breake << val.critical_resist << val.penetrate << val.injury << val.break_resist << val.up_rate << val.tooltip ;
		return (*this);
	}
	Packet &operator<<(const bounty_task &val)
	{
		(*this)<< val.db_id << val.type << val.name << val.gold << val.item_id << val.count << val.level_id << val.icon_id << val.tooltip ;
		return (*this);
	}
	Packet &operator<<(const bounty_refresh &val)
	{
		(*this)<< val.db_id << val.reward_rate << val.up_rate << val.down_rate << val.normal_rate << val.up_star << val.down_star ;
		return (*this);
	}
	Packet &operator<<(const equip_cultivate_attr &val)
	{
		(*this)<< val.db_id << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.critical_resist << val.break_resist << val.unlock_color ;
		return (*this);
	}
	Packet &operator<<(const equip_cultivate_max &val)
	{
		(*this)<< val.equip_color << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.critical_resist << val.break_resist ;
		return (*this);
	}
	Packet &operator<<(const equip_cultivate_type &val)
	{
		(*this)<< val.db_id << val.type << val.vip_level << val.attr_type << val.num_min << val.num_max << val.cost_item << val.cost_item_num << val.add_num << val.pro << val.times ;
		return (*this);
	}
	Packet &operator<<(const player_achieve &val)
	{
		(*this)<< val.db_id << val.type << val.value << val.money_type << val.money << val.item_id1 << val.count1 << val.item_id2 << val.count2 << val.achieve_type << val.tooltip ;
		return (*this);
	}
	Packet &operator<<(const achieve_rewards &val)
	{
		(*this)<< val.db_id << val.money_type << val.money << val.gift_id << val.tooltip ;
		return (*this);
	}
	Packet &operator<<(const part_upgrade &val)
	{
		(*this)<< val.db_id << val.min_exp << val.max_exp << val.rate << val.gold << val.item_id << val.count << val.add_rate << val.exp ;
		return (*this);
	}
	Packet &operator<<(const equip_suit &val)
	{
		(*this)<< val.db_id << val.name << val.suit_id << val.num << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.critical_resist << val.break_resist << val.miss_odds << val.hit_odds << val.hurt_deepen << val.hurt_remission << val.break_defense << val.rage_bonuses << val.is_show ;
		return (*this);
	}
	Packet &operator<<(const drink_wine &val)
	{
		(*this)<< val.db_id << val.gold << val.exp << val.sp_exp << val.energy << val.team_gold << val.team_exp << val.team_sp_exp << val.team_energy << val.captain_gold << val.captain_exp << val.captain_sp_exp << val.captain_energy << val.gold_item << val.exp_item << val.sp_exp_item << val.energy_item ;
		return (*this);
	}
	Packet &operator<<(const class_level_rating &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.times ;
		return (*this);
	}
	Packet &operator<<(const ci_chapter_map_describe &val)
	{
		(*this)<< val.db_id << val.icon_id << val.name << val.total << val.level1 << val.level2 << val.level3 << val.level4 << val.level5 << val.level6 << val.level7 << val.level8 << val.level9 << val.level10 << val.route_id << val.gold_box << val.silver_box << val.copper_box << val.gold_times << val.silver_times << val.copper_times << val.sp_gold_box << val.sp_silver_box << val.sp_copper_box << val.sp_gold_times << val.sp_silver_times << val.sp_copper_times ;
		return (*this);
	}
	Packet &operator<<(const ci_level_object &val)
	{
		(*this)<< val.db_id << val.name << val.icon_id << val.pass_ref << val.com_level_id << val.sp_level_id << val.level_limit << val.com_gift_id << val.sp_gift_id << val.com_gold << val.sp_gold << val.com_exp << val.sp_exp << val.desc_id << val.cutscene_id << val.icon << val.event_table << val.drop_id << val.com_ad_fight << val.sp_ad_fight << val.com_extra_drop_id << val.sp_extra_drop_id << val.fbbox << val.sp_fbbox << val.fbbubble_desc << val.sp_fbbubble_desc ;
		return (*this);
	}
	Packet &operator<<(const task_gift_bag &val)
	{
		(*this)<< val.db_id << val.rate << val.draw_card_id << val.com_drop_id << val.sp_drop_id << val.drop_com_exp << val.drop_sp_exp << val.show_gift ;
		return (*this);
	}
	Packet &operator<<(const ci_auto_task &val)
	{
		(*this)<< val.db_id << val.type << val.name << val.cutscene_id << val.chapter_id << val.level_id << val.next_id ;
		return (*this);
	}
	Packet &operator<<(const cut_scene &val)
	{
		(*this)<< val.db_id << val.scene_name << val.dialog_table << val.object_table << val.gold << val.exp ;
		return (*this);
	}
	Packet &operator<<(const current_equip &val)
	{
		(*this)<< val.PlayerID << val.head << val.neck << val.shoulder << val.weapon << val.body << val.waist << val.arm << val.leg << val.foot << val.wing << val.left_finger << val.right_finger << val.suit ;
		return (*this);
	}
	Packet &operator<<(const current_skill &val)
	{
		(*this)<< val.PlayerID << val.active1 << val.active2 << val.active3 << val.passive1 << val.passive2 << val.passive3 << val.unique1 ;
		return (*this);
	}
	Packet &operator<<(const current_epinlay &val)
	{
		(*this)<< val.PlayerID << val.head << val.neck << val.shoulder << val.weapon << val.body << val.waist << val.arm << val.leg << val.foot << val.wing << val.left_finger << val.right_finger ;
		return (*this);
	}
	Packet &operator<<(const current_symbol &val)
	{
		(*this)<< val.PlayerID << val.pos1 << val.pos2 << val.pos3 << val.pos4 << val.pos5 << val.pos6 << val.pos7 << val.pos8 << val.pos9 ;
		return (*this);
	}
	Packet &operator<<(const obj_team_level &val)
	{
		(*this)<< val.index << val.value ;
		return (*this);
	}
	Packet &operator<<(const mail_money &val)
	{
		(*this)<< val.type << val.value ;
		return (*this);
	}
	Packet &operator<<(const obj_achieve &val)
	{
		(*this)<< val.achieve_id << val.is_get ;
		return (*this);
	}
	Packet &operator<<(const int_short &val)
	{
		(*this)<< val.intId << val.shortId ;
		return (*this);
	}
	Packet &operator<<(const random_symbol &val)
	{
		(*this)<< val.item_id << val.rate ;
		return (*this);
	}
	Packet &operator<<(const int_bool &val)
	{
		(*this)<< val.value << val.is_get ;
		return (*this);
	}
	Packet &operator<<(const uint_bool &val)
	{
		(*this)<< val.value << val.is_get ;
		return (*this);
	}
	Packet &operator<<(const recieve_energy &val)
	{
		(*this)<< val.player_id << val.recieve_time << val.is_recieve ;
		return (*this);
	}
	Packet &operator<<(const int_uint &val)
	{
		(*this)<< val.intId << val.uintId ;
		return (*this);
	}
	Packet &operator<<(const obj_common &val)
	{
		(*this)<< val.item_id << val.count ;
		return (*this);
	}
	Packet &operator<<(const obj_equip &val)
	{
		(*this)<< val.item_id << val.count ;
		return (*this);
	}
	Packet &operator<<(const obj_gem &val)
	{
		(*this)<< val.item_id << val.count ;
		return (*this);
	}
	Packet &operator<<(const obj_material &val)
	{
		(*this)<< val.item_id << val.count ;
		return (*this);
	}
	Packet &operator<<(const obj_piece &val)
	{
		(*this)<< val.item_id << val.count ;
		return (*this);
	}
	Packet &operator<<(const obj_skill &val)
	{
		(*this)<< val.skill_id << val.level ;
		return (*this);
	}
	Packet &operator<<(const obj_symbol &val)
	{
		(*this)<< val.item_id << val.exp << val.level ;
		return (*this);
	}
	Packet &operator<<(const obj_epinlay &val)
	{
		(*this)<< val.order_id << val.gem_id ;
		return (*this);
	}
	Packet &operator<<(const obj_enhance &val)
	{
		(*this)<< val.attr_type << val.attr ;
		return (*this);
	}
	Packet &operator<<(const obj_box_get &val)
	{
		(*this)<< val.chapter_id << val.chapter_type << val.is_get_copper << val.is_get_silver << val.is_get_gold ;
		return (*this);
	}
	Packet &operator<<(const obj_title &val)
	{
		(*this)<< val.title_id << val.title_times ;
		return (*this);
	}
	Packet &operator<<(const obj_gold &val)
	{
		(*this)<< val.gold_times << val.gold_rate ;
		return (*this);
	}
	Packet &operator<<(const obj_chip &val)
	{
		(*this)<< val.item_id << val.count ;
		return (*this);
	}
	Packet &operator<<(const obj_god_weapon &val)
	{
		(*this)<< val.item_id << val.count ;
		return (*this);
	}
	Packet &operator<<(const obj_godinlay &val)
	{
		(*this)<< val.order_id << val.god_id ;
		return (*this);
	}
	Packet &operator<<(const player_account &val)
	{
		(*this)<< val.AccountID << val.PlatformID << val.PlayerID1 << val.PlayerID2 << val.PlayerID3 << val.PlayerID4 ;
		return (*this);
	}
	Packet &operator<<(const player_attributes &val)
	{
		(*this)<< val.PlayerID << val.name << val.profession << val.energy << val.last_fight << val.experience << val.level << val.gold << val.diamond << val.head << val.neck << val.shoulder << val.weapon << val.body << val.waist << val.arm << val.leg << val.foot << val.wing << val.left_finger << val.right_finger << val.IsNpc << val.last_login << val.login_times << val.equip_bags << val.gem_bags << val.material_bags << val.vip << val.charge << val.sp_level << val.sp_exp << val.head_1 << val.neck_1 << val.shoulder_1 << val.weapon_1 << val.body_1 << val.waist_1 << val.arm_1 << val.leg_1 << val.foot_1 << val.wing_1 << val.left_finger_1 << val.right_finger_1 << val.head_2 << val.neck_2 << val.shoulder_2 << val.weapon_2 << val.body_2 << val.waist_2 << val.arm_2 << val.leg_2 << val.foot_2 << val.wing_2 << val.left_finger_2 << val.right_finger_2 << val.ever_battle_layer << val.ever_battle_time ;
		return (*this);
	}
	Packet &operator<<(const common_today_info &val)
	{
		(*this)<< val.PlayerID << val.energy_times << val.buy_gold_times << val.gold_level_times << val.exp_level_times << val.lottery_times << val.gamble_times << val.is_login << val.present_times << val.recieve_times << val.buy_gold_level << val.buy_exp_level << val.shop_limit << val.treasure_num << val.treasure_state << val.active_value << val.active_rewards << val.bounty_task_times << val.month_card_reward << val.mystical_shop_times << val.buy_mystical_shop << val.boss_level_times << val.buy_boss_level << val.inter_shop_times << val.drink_wine_times << val.drink_wine_start << val.drink_interval_time << val.buy_treasure_times << val.left_treasure_times << val.enter_battle_times << val.buy_battle_times << val.left_battle_times << val.ever_battle_rewards << val.ever_battle_box_rewards ;
		return (*this);
	}
	Packet &operator<<(const player_enhance &val)
	{
		(*this)<< val.PlayerID << val.head << val.neck << val.shoulder << val.weapon << val.body << val.waist << val.arm << val.leg << val.foot << val.wing << val.left_finger << val.right_finger << val.suit ;
		return (*this);
	}
	Packet &operator<<(const Player_Info &val)
	{
		(*this)<< val.m_Attributes << val.m_CurEquip << val.m_CurSkill << val.m_ip ;
		return (*this);
	}
	Packet &operator<<(const Fight_Attr &val)
	{
		(*this)<< val.m_life << val.m_defense << val.m_attack << val.m_critical << val.m_breake << val.m_injury << val.m_penetrate << val.m_pvppower << val.m_pvpresist << val.m_criticalresist << val.m_breakresist << val.m_dodge << val.m_energyadd << val.m_expadd << val.m_goldadd << val.m_missodds << val.m_hitodds << val.m_hurtdeepen << val.m_hurtremission << val.m_breakdefense << val.m_ragebonuses << val.m_fight_power ;
		return (*this);
	}
	Packet &operator<<(const choose_role_data &val)
	{
		(*this)<< val.player_id << val.player_name << val.profession << val.level << val.weapon << val.suit ;
		return (*this);
	}
	Packet &operator<<(const main_task_info &val)
	{
		(*this)<< val.PlayerID << val.curr_id << val.is_over << val.box_get << val.chapter1 << val.chapter2 << val.chapter3 << val.chapter4 << val.chapter5 << val.chapter6 << val.chapter7 << val.chapter8 << val.chapter9 << val.chapter10 << val.chapter11 << val.chapter12 << val.chapter13 << val.chapter14 << val.chapter15 << val.chapter16 << val.chapter17 << val.chapter18 << val.chapter19 << val.chapter20 ;
		return (*this);
	}
	Packet &operator<<(const main_task_today_info &val)
	{
		(*this)<< val.PlayerID << val.chapter1 << val.chapter2 << val.chapter3 << val.chapter4 << val.chapter5 << val.chapter6 << val.chapter7 << val.chapter8 << val.chapter9 << val.chapter10 << val.chapter11 << val.chapter12 << val.chapter13 << val.chapter14 << val.chapter15 << val.chapter16 << val.chapter17 << val.chapter18 << val.chapter19 << val.chapter20 ;
		return (*this);
	}
	Packet &operator<<(const SrVector &val)
	{
		(*this)<< val.x << val.y << val.z ;
		return (*this);
	}
	Packet &operator<<(const common_single_use &val)
	{
		(*this)<< val.PlayerID << val.level_gift << val.pay_gift << val.pay_money << val.shop_limit << val.symbol_reward << val.exchange_gift << val.guide_id << val.practice_time << val.login_times << val.login_rewards << val.login_time << val.month_end_time << val.month_first_rewards << val.pvp_best_rank << val.buy_vip_shop << val.gold_gamble << val.diamond_gamble << val.month_cardid << val.god_weapon_times << val.treasure_times << val.symbol_diamond << val.ever_battle_rewards << val.ever_battle_score << val.symbol_energy << val.month_card_recharge_count << val.ever_card_recharge_count << val.month_card_state << val.ever_card_state ;
		return (*this);
	}
	Packet &operator<<(const active_boss_rank &val)
	{
		(*this)<< val.PlayerID << val.result << val.times << val.rewards_get1 << val.rewards_get2 << val.ranking ;
		return (*this);
	}
	Packet &operator<<(const boss_rank_data &val)
	{
		(*this)<< val.PlayerID << val.profession << val.suit << val.ranking << val.result << val.level << val.name ;
		return (*this);
	}
	Packet &operator<<(const rank_data &val)
	{
		(*this)<< val.PlayerID << val.rank ;
		return (*this);
	}
	Packet &operator<<(const friend_data &val)
	{
		(*this)<< val.PlayerID << val.PlayerName << val.profession << val.level << val.vip << val.suit << val.fight_power ;
		return (*this);
	}
	Packet &operator<<(const energy_data &val)
	{
		(*this)<< val.FriendData << val.IsRecieve ;
		return (*this);
	}
	Packet &operator<<(const send_energy_data &val)
	{
		(*this)<< val.FriendData << val.DayNum ;
		return (*this);
	}
	Packet &operator<<(const receive_energy_data &val)
	{
		(*this)<< val.FriendData << val.DayNum ;
		return (*this);
	}
	Packet &operator<<(const active_pvp_rank &val)
	{
		(*this)<< val.rank << val.PlayerID << val.left_times << val.buy_times << val.feat_time << val.feat_sum << val.feat_total << val.rand1 << val.rand2 << val.rand3 << val.info ;
		return (*this);
	}
	Packet &operator<<(const pvp_rank_data &val)
	{
		(*this)<< val.rank << val.PlayerID << val.profession << val.suit << val.name << val.level << val.fight_power ;
		return (*this);
	}
	Packet &operator<<(const PvpRandomData &val)
	{
		(*this)<< val.rank << val.PlayerID << val.profession << val.suit << val.weapon << val.level << val.fight_power << val.left_finger << val.name << val.canFight ;
		return (*this);
	}
	Packet &operator<<(const ever_battle_rank &val)
	{
		(*this)<< val.PlayerID << val.profession << val.level << val.rank << val.name << val.layer ;
		return (*this);
	}
	Packet &operator<<(const treasure_data &val)
	{
		(*this)<< val.PlayerID << val.count << val.m_PlayerInfo << val.m_FightAttr ;
		return (*this);
	}
	Packet &operator<<(const player_treasure &val)
	{
		(*this)<< val.PlayerID << val.last_time << val.attack_time << val.info ;
		return (*this);
	}
	Packet &operator<<(const player_achieve_info &val)
	{
		(*this)<< val.PlayerID << val.rewards << val.achieves ;
		return (*this);
	}
	Packet &operator<<(const player_other_info &val)
	{
		(*this)<< val.PlayerID << val.discount_end_time << val.buy_item_info << val.last_lost_map_id << val.last_lost_map_desc << val.last_level_update_time ;
		return (*this);
	}
	Packet &operator<<(const drink_wine_info &val)
	{
		(*this)<< val.PlayerID << val.PlayerName << val.profession << val.level << val.vip << val.suit << val.fight_power << val.drink_times ;
		return (*this);
	}
	Packet &operator<<(const mail_data &val)
	{
		(*this)<< val.index << val.is_get << val.is_read << val.is_del << val.ifield << val.itime << val.name << val.content << val.sender << val.attach << val.money ;
		return (*this);
	}
	Packet &operator<<(const order_data &val)
	{
		(*this)<< val.IsAddMoney << val.ItemId << val.OrderId ;
		return (*this);
	}
	Packet &operator<<(const mail_gift &val)
	{
		(*this)<< val.db_id << val.items << val.date_type << val.keep_time << val.description ;
		return (*this);
	}
	Packet &operator<<(const active_reward &val)
	{
		(*this)<< val.db_id << val.rank_1 << val.rank_2 << val.rank_3 << val.rank_4 << val.rank_5 << val.rank_6 << val.rank_7 << val.rank_8 << val.rank_9 << val.rank_10 << val.rank_11_50 << val.rank_50_ ;
		return (*this);
	}
	Packet &operator<<(const marquee &val)
	{
		(*this)<< val.db_id << val.line_1 << val.line_2 << val.line_3 << val.line_4 << val.line_5 << val.line_6 << val.line_7 << val.line_8 << val.line_9 << val.line_10 ;
		return (*this);
	}
	Packet &operator<<(const player_rank &val)
	{
		(*this)<< val.charid << val.rank << val.name << val.profession << val.num << val.id ;
		return (*this);
	}
	Packet &operator<<(const mystical_shop &val)
	{
		(*this)<< val.db_id << val.minlv << val.maxlv << val.vip << val.type << val.item_id << val.item_size << val.rate << val.money_type << val.money ;
		return (*this);
	}
	Packet &operator<<(const BossUserDam &val)
	{
		(*this)<< val.name << val.dam ;
		return (*this);
	}
	Packet &operator<<(const active_boss_level &val)
	{
		(*this)<< val.db_id << val.level_limit << val.drop_id << val.draw_card_id ;
		return (*this);
	}
	Packet &operator<<(const month_card &val)
	{
		(*this)<< val.db_id << val.diamond << val.first_rewards << val.today_rewards << val.overdue_time << val.renew_time << val.count ;
		return (*this);
	}
	Packet &operator<<(const active_inter_shop &val)
	{
		(*this)<< val.db_id << val.viplevel << val.plan_id << val.price << val.gold << val.diamond << val.itemid1 << val.itemcount1 << val.itemid2 << val.itemcount2 << val.itemid3 << val.itemcount3 << val.oldprice ;
		return (*this);
	}
	Packet &operator<<(const active_operate_desc &val)
	{
		(*this)<< val.db_id << val.starttime << val.endtime << val.interval << val.level ;
		return (*this);
	}
	Packet &operator<<(const vip_shop &val)
	{
		(*this)<< val.level << val.itemid << val.price ;
		return (*this);
	}
	Packet &operator<<(const god_weapon &val)
	{
		(*this)<< val.godweapon_id << val.condition << val.condition_num << val.level << val.begin_level << val.tips ;
		return (*this);
	}
	Packet &operator<<(const god_weapon_upgrade &val)
	{
		(*this)<< val.db_id << val.fight << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.critical_resist << val.break_resist << val.miss_odds << val.hit_odds << val.hurt_deepen << val.hurt_remission << val.break_defense << val.rage_bonuses ;
		return (*this);
	}
	Packet &operator<<(const god_weapon_upgrade_stage &val)
	{
		(*this)<< val.db_id << val.godweapon_id << val.level << val.quality << val.cost_item << val.cost_item_num << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.pvp_power << val.pvp_resist << val.critical_resist << val.break_resist << val.miss_odds << val.hit_odds << val.hurt_deepen << val.hurt_remission << val.break_defense << val.rage_bonuses << val.skill_id << val.skill_lv << val.skill_name << val.skill_icon << val.skill_desc << val.buff_id << val.buff_name << val.buff_desc << val.buff_battle << val.buff_icon << val.effid ;
		return (*this);
	}
	Packet &operator<<(const player_god_weapon &val)
	{
		(*this)<< val.PlayerID << val.Datas ;
		return (*this);
	}
	Packet &operator<<(const god_weapon_net &val)
	{
		(*this)<< val.god_weapon_id << val.stage ;
		return (*this);
	}
	Packet &operator<<(const discount_shop &val)
	{
		(*this)<< val.db_id << val.item_id << val.item_count << val.money_type << val.price << val.countless << val.oldprice ;
		return (*this);
	}
	Packet &operator<<(const boss_fight &val)
	{
		(*this)<< val.PlayerID << val.name << val.profession << val.weapon << val.body << val.left_finger ;
		return (*this);
	}
	Packet &operator<<(const player_title &val)
	{
		(*this)<< val.PlayerID << val.title << val.titletimes ;
		return (*this);
	}
	Packet &operator<<(const title_info &val)
	{
		(*this)<< val.db_id << val.title_type << val.condition << val.parm1 << val.parm2 << val.is_time << val.time << val.weight << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.pvp_power << val.pvp_resist << val.critical_resist << val.break_resist << val.miss_odds << val.hit_odds << val.hurt_deepen << val.hurt_remission << val.break_defense << val.rage_bonuses << val.name_id ;
		return (*this);
	}
	Packet &operator<<(const friend_pvp &val)
	{
		(*this)<< val.pvp_val ;
		return (*this);
	}
	Packet &operator<<(const server_info &val)
	{
		(*this)<< val.zoneid << val.name << val.zonestate << val.status << val.ip << val.port ;
		return (*this);
	}
	Packet &operator<<(const symbol_suit &val)
	{
		(*this)<< val.id << val.level << val.num << val.life << val.attack << val.defense << val.critical << val.breake << val.injury << val.penetrate << val.critical_resist << val.break_resist ;
		return (*this);
	}
	Packet &operator<<(const ci_level_drop &val)
	{
		(*this)<< val.db_id << val.item_chance << val.item_limit << val.gold_chance << val.gold_limit << val.monster_count ;
		return (*this);
	}
	Packet &operator<<(const active_spexp_level &val)
	{
		(*this)<< val.db_id << val.level_limit << val.monster_count << val.base_spexp << val.min_card_spexp << val.max_card_spexp << val.draw_card_id << val.killcount_reward ;
		return (*this);
	}
	Packet &operator<<(const BoxData &val)
	{
		(*this)<< val.Id << val.MaxKillCount << val.Boxes ;
		return (*this);
	}
	Packet &operator<<(const achieve_task &val)
	{
		(*this)<< val.db_id << val.name << val.type << val.param1 << val.param2 << val.task_id << val.level << val.reward1 << val.reward1_num << val.reward2 << val.reward2_num << val.tooltip << val.profession ;
		return (*this);
	}
	Packet &operator<<(const hard_level &val)
	{
		(*this)<< val.db_id << val.times ;
		return (*this);
	}
	Packet &operator<<(const level_state &val)
	{
		(*this)<< val.db_id << val.times << val.star ;
		return (*this);
	}
	Packet &operator<<(const equip_gem &val)
	{
		(*this)<< val.db_id << val.level << val.head << val.weapon << val.body << val.arm << val.foot << val.leg ;
		return (*this);
	}
	Packet &operator<<(const general_shop &val)
	{
		(*this)<< val.db_id << val.shop_type << val.level_min << val.level_max << val.vip_min << val.vip_max << val.item_id << val.item_size << val.item_buy_times << val.buy_limit_type << val.money_type << val.price ;
		return (*this);
	}
	Packet &operator<<(const clothes &val)
	{
		(*this)<< val.db_id << val.name << val.profession << val.level ;
		return (*this);
	}
	Packet &operator<<(const clothes_level &val)
	{
		(*this)<< val.db_id << val.clothes_id << val.level << val.item_id << val.item_num << val.life << val.attack << val.defense << val.breake << val.penetrate << val.break_resist << val.critical << val.critical_resist << val.injury << val.miss_odds << val.hit_odds << val.hurt_deepen << val.hurt_remission << val.break_defense << val.rage_bonuses ;
		return (*this);
	}
	Packet &operator<<(const clothes_level_data &val)
	{
		(*this)<< val.id << val.level ;
		return (*this);
	}
	Packet &operator<<(const gamble_data &val)
	{
		(*this)<< val.id << val.total_gamble_times << val.gamble_times << val.free_gamble_time << val.today_gamble_times ;
		return (*this);
	}
	Packet &operator<<(const snatch_info &val)
	{
		(*this)<< val.charid << val.name << val.itemid << val.snatch_time << val.level << val.profession << val.fight_power << val.issuccess ;
		return (*this);
	}
	Packet &operator<<(const snatch_shop &val)
	{
		(*this)<< val.db_id << val.time << val.cost_item << val.cost_money ;
		return (*this);
	}
	Packet &operator<<(const snatch_pro &val)
	{
		(*this)<< val.db_id << val.player_pro << val.npc_pro << val.item_id << val.drop_bag_group_id ;
		return (*this);
	}
	Packet &operator<<(const snatch &val)
	{
		(*this)<< val.db_id << val.type << val.is_off << val.open_lv << val.piece_count << val.is_bulletin ;
		return (*this);
	}
	Packet &operator<<(const snatch_drop &val)
	{
		(*this)<< val.player_lv << val.rewards_exp << val.rewards_gold ;
		return (*this);
	}
	Packet &operator<<(const snatch_mate_info &val)
	{
		(*this)<< val.charid << val.level << val.profession << val.name << val.fight_power ;
		return (*this);
	}
	Packet &operator<<(const extra_drop_item &val)
	{
		(*this)<< val.db_id << val.drop_bag_group_id << val.type ;
		return (*this);
	}
	Packet &operator<<(const vip_item_shop &val)
	{
		(*this)<< val.db_id << val.money_type << val.price << val.original_price << val.times << val.item1_id << val.item1_num << val.item2_id << val.item2_num << val.item3_id << val.item3_num << val.item4_id << val.item4_num ;
		return (*this);
	}
	Packet &operator<<(const shop_game &val)
	{
		(*this)<< val.db_id << val.price << val.selltype << val.item_id << val.start_time << val.end_time << val.startday << val.sellday << val.resetday << val.viplimit ;
		return (*this);
	}
	Packet &operator<<(const activity_team_level &val)
	{
		(*this)<< val.db_id << val.level << val.type << val.item1_id << val.item1_num << val.item2_id << val.item2_num << val.item3_id << val.item3_num << val.drop_id << val.level_id << val.icon_id << val.level_name << val.fight_power_recommend << val.level_info ;
		return (*this);
	}
	Packet &operator<<(const team_level_rule &val)
	{
		(*this)<< val.level << val.rule ;
		return (*this);
	}
	Packet &operator<<(const guild_info_list &val)
	{
		(*this)<< val.guildid << val.name << val.curcount << val.level << val.fight_power ;
		return (*this);
	}
	Packet &operator<<(const guild_info &val)
	{
		(*this)<< val.guildid << val.name << val.note << val.notify << val.exp << val.level << val.fight_power << val.curcount << val.currank << val.VIP << val.masterName ;
		return (*this);
	}
	Packet &operator<<(const guild_player_info &val)
	{
		(*this)<< val.charid << val.name << val.profession << val.level << val.fight_power << val.VIP << val.occupation << val.Contribtion << val.online ;
		return (*this);
	}
	Packet &operator<<(const carnival_task_info &val)
	{
		(*this)<< val.task_id << val.status << val.cur_data ;
		return (*this);
	}
	Packet &operator<<(const god_weapon_location &val)
	{
		(*this)<< val.god_weapon_id << val.position << val.stage ;
		return (*this);
	}
	Packet &operator<<(const item_chip &val)
	{
		(*this)<< val.db_id << val.name << val.type << val.level << val.quality << val.order << val.color << val.data << val.trans_id << val.item_id1 << val.item_num1 << val.item_id2 << val.item_num2 << val.item_id3 << val.item_num3 << val.icon_id << val.tooltip << val.desc ;
		return (*this);
	}
	Packet &operator<<(const equip_advance &val)
	{
		(*this)<< val.db_id << val.life << val.defense << val.attack << val.critical << val.breake << val.injury << val.penetrate << val.criticalresist << val.breakresist << val.missodds << val.hitodds << val.hurtdeepen << val.hurtremission << val.breakdefense << val.ragebonuses << val.life_rate << val.defense_rate << val.attack_rate << val.critical_rate << val.breake_rate << val.injury_rate << val.penetrate_rate << val.criticalresist_rate << val.breakresist_rate << val.missodds_rate << val.hitodds_rate << val.hurtdeepen_rate << val.hurtremission_rate << val.breakdefense_rate << val.ragebonuses_rate << val.tooltip << val.last_id ;
		return (*this);
	}
	Packet &operator<<(const guild_player_worship &val)
	{
		(*this)<< val.charid << val.dbid << val.pvprank << val.name << val.profession << val.level << val.waepon << val.suit << val.lfinger ;
		return (*this);
	}
	Packet &operator<<(const goldstone_data &val)
	{
		(*this)<< val.charId << val.StartTime << val.Name << val.FightPower << val.Level << val.Profession << val.StoneType << val.StoneId << val.GuildId << val.Money ;
		return (*this);
	}
	Packet &operator<<(const goldstone_grabinfo &val)
	{
		(*this)<< val.fightsuccess << val.IsGrab << val.charId << val.IncomeGold << val.GrabGold << val.Name ;
		return (*this);
	}
	Packet &operator<<(const OperatingActivity &val)
	{
		(*this)<< val.m_Type << val.m_IsPlaying << val.m_StartTime << val.m_EndTime << val.m_LevelLimit << val.m_Name << val.m_param ;
		return (*this);
	}
	Packet &operator<<(const stBoughtItemInfo &val)
	{
		(*this)<< val.itemId << val.itemNum << val.playerId << val.itemPrice << val.boughtTimes << val.todayBoughtTimes ;
		return (*this);
	}
	Packet &operator<<(const stGroupBuyingData &val)
	{
		(*this)<< val.itemId << val.itemNum << val.itemCurPrice << val.boughtSum ;
		return (*this);
	}
	Packet &operator>>(IP_PORT &val)
	{
		(*this)>> val.m_natType >> val.m_exterIp >> val.m_exterPort >> val.m_interIp >> val.m_interPort ;
		return (*this);
	}

	Packet &operator>>(IPORT &val)
	{
		(*this)>> val.m_ip >> val.m_port ;
		return (*this);
	}

	Packet &operator>>(config_language &val)
	{
		(*this)>> val.db_id >> val.info ;
		return (*this);
	}

	Packet &operator>>(config_mixed &val)
	{
		(*this)>> val.db_id >> val.int_val >> val.str_val >> val.type >> val.desc ;
		return (*this);
	}

	Packet &operator>>(equip_attribute &val)
	{
		(*this)>> val.db_id >> val.color >> val.type >> val.level_limit >> val.is_enhance >> val.is_upgrade >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.pvp_power >> val.pvp_resist >> val.critical_resist >> val.break_resist >> val.energy_add >> val.exp_add >> val.gold_add >> val.suit_id >> val.enhance_rate >> val.quality >> val.order >> val.advance_id >> val.tooltip >> val.advance_1 >> val.advance_2 ;
		return (*this);
	}

	Packet &operator>>(equip_bowman &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.model_id >> val.desc ;
		return (*this);
	}

	Packet &operator>>(equip_double_sword &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.model_id >> val.desc ;
		return (*this);
	}

	Packet &operator>>(equip_enhance &val)
	{
		(*this)>> val.db_id >> val.type >> val.level >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.pvp_power >> val.pvp_resist >> val.critical_resist >> val.break_resist >> val.energy_add >> val.exp_add >> val.gold_add ;
		return (*this);
	}

	Packet &operator>>(equip_grow &val)
	{
		(*this)>> val.db_id >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.pvp_power >> val.pvp_resist >> val.critical_resist >> val.break_resist >> val.limit ;
		return (*this);
	}

	Packet &operator>>(equip_hammer &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.model_id >> val.desc ;
		return (*this);
	}

	Packet &operator>>(equip_sword &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.model_id >> val.desc ;
		return (*this);
	}

	Packet &operator>>(equip_upgrade &val)
	{
		(*this)>> val.db_id >> val.material1 >> val.mat_size1 >> val.material2 >> val.mat_size2 >> val.material3 >> val.mat_size3 >> val.rare_material >> val.rare_size >> val.gold >> val.trans_id ;
		return (*this);
	}

	Packet &operator>>(item_gem &val)
	{
		(*this)>> val.db_id >> val.name >> val.type >> val.level >> val.icon_id >> val.value >> val.trans_id >> val.price >> val.tooltip >> val.desc >> val.color >> val.order >> val.quality ;
		return (*this);
	}

	Packet &operator>>(item_gift_bag &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.limit >> val.items >> val.tooltip >> val.desc ;
		return (*this);
	}

	Packet &operator>>(item_goods &val)
	{
		(*this)>> val.db_id >> val.item_id >> val.item_size >> val.is_list >> val.list_order >> val.is_hot >> val.is_new >> val.gold >> val.gem >> val.money_type >> val.level_limit >> val.is_day >> val.is_ever ;
		return (*this);
	}

	Packet &operator>>(item_material &val)
	{
		(*this)>> val.db_id >> val.name >> val.type >> val.level >> val.data >> val.trans_id >> val.price >> val.icon_id >> val.tooltip >> val.desc >> val.quality >> val.order >> val.color >> val.type_button ;
		return (*this);
	}

	Packet &operator>>(item_snatch_chip &val)
	{
		(*this)>> val.db_id >> val.name >> val.type >> val.level >> val.data >> val.trans_id >> val.price >> val.icon_id >> val.tooltip >> val.desc >> val.quality >> val.order >> val.color >> val.type_button ;
		return (*this);
	}

	Packet &operator>>(item_money &val)
	{
		(*this)>> val.db_id >> val.name >> val.price >> val.color >> val.icon_id >> val.tooltip >> val.desc ;
		return (*this);
	}

	Packet &operator>>(item_symbol &val)
	{
		(*this)>> val.db_id >> val.name >> val.color >> val.attribute >> val.base_exp >> val.value >> val.icon_id >> val.trans_id >> val.item_id >> val.item_size >> val.decompose_item >> val.gold >> val.max_level >> val.desc ;
		return (*this);
	}

	Packet &operator>>(item_symbol_limit &val)
	{
		(*this)>> val.pos1 >> val.pos2 >> val.pos3 >> val.pos4 >> val.pos5 >> val.pos6 >> val.pos7 >> val.pos8 >> val.pos9 >> val.pos10 >> val.pos11 ;
		return (*this);
	}

	Packet &operator>>(item_symbol_upgrade &val)
	{
		(*this)>> val.level >> val.scale >> val.exp1 >> val.exp2 >> val.exp3 >> val.exp4 >> val.exp5 >> val.exp6 >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.critical_resist >> val.break_resist ;
		return (*this);
	}

	Packet &operator>>(symbol_base &val)
	{
		(*this)>> val.db_id >> val.level_limit >> val.vip_limit >> val.next_id >> val.rate >> val.gold >> val.diamond >> val.item_id >> val.item_num >> val.sp_type >> val.sp_num >> val.energy ;
		return (*this);
	}

	Packet &operator>>(symbol_random &val)
	{
		(*this)>> val.symbol >> val.com_rewards >> val.sp_rewards >> val.first_rewards >> val.desc ;
		return (*this);
	}

	Packet &operator>>(part_enhance &val)
	{
		(*this)>> val.level >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.critical_resist >> val.break_resist >> val.gold >> val.com_crystal_id >> val.com_crystal_num >> val.per_crystal_id >> val.per_crystal_num ;
		return (*this);
	}

	Packet &operator>>(player_upgrade &val)
	{
		(*this)>> val.level >> val.experience >> val.gold >> val.diamond >> val.item_id >> val.count >> val.practice >> val.treasure_id >> val.bounty_exp >> val.max_star >> val.day_money >> val.day_item >> val.bounty_god >> val.bounty_sp ;
		return (*this);
	}

	Packet &operator>>(res_icon &val)
	{
		(*this)>> val.db_id >> val.name >> val.desc ;
		return (*this);
	}

	Packet &operator>>(res_model &val)
	{
		(*this)>> val.db_id >> val.name >> val.desc ;
		return (*this);
	}

	Packet &operator>>(res_sound &val)
	{
		(*this)>> val.db_id >> val.name >> val.desc ;
		return (*this);
	}

	Packet &operator>>(skill_upgrade &val)
	{
		(*this)>> val.level >> val.need_level >> val.gold >> val.add_attribute >> val.add_attack >> val.item_id >> val.item_size >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.critical_resist >> val.break_resist >> val.fight_power >> val.rank >> val.main_id >> val.break_defense ;
		return (*this);
	}

	Packet &operator>>(skill_attribute &val)
	{
		(*this)>> val.db_id >> val.type >> val.attr_type ;
		return (*this);
	}

	Packet &operator>>(skill_bowman &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.tooltip >> val.model_id >> val.desc ;
		return (*this);
	}

	Packet &operator>>(skill_double_sword &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.tooltip >> val.model_id >> val.desc ;
		return (*this);
	}

	Packet &operator>>(skill_hammer &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.tooltip >> val.model_id >> val.desc ;
		return (*this);
	}

	Packet &operator>>(skill_sword &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.tooltip >> val.model_id >> val.desc ;
		return (*this);
	}

	Packet &operator>>(vip_privilege &val)
	{
		(*this)>> val.level >> val.charge_num >> val.saodang_num >> val.energy_num >> val.buy_gold_num >> val.reset_level_num >> val.gold_free_times >> val.gold_level_num >> val.exp_free_times >> val.exp_level_num >> val.present_num >> val.recieve_num >> val.arena_free_times >> val.pvp_rank_num >> val.pr_feat_num >> val.eb_free_num >> val.ever_battle_num >> val.practice_num >> val.mystical_shop_num >> val.boss_level_num >> val.gold_critical_rate >> val.gold_critical_times >> val.gold_free_num >> val.mystical_shop_item_num >> val.buy_treasure_times >> val.treasure_times >> val.treasure_rate >> val.maxtreasure_rate >> val.treasure_desc1 >> val.treasure_desc2 >> val.treasure_desc3 >> val.buy_hard_num >> val.bounty_dis >> val.bounty_yield >> val.spexp_level_num >> val.spexp_level_rate >> val.vigour_num >> val.guildbattle_times >> val.reset_normallevel_num >> val.cuilian_num >> val.cuilian_high >> val.fight_5 >> val.mystical_shop_reset_num >> val.goldstone_free_times >> val.goldstone_level_num >> val.shop_goditem_num ;
		return (*this);
	}

	Packet &operator>>(vip_times_price &val)
	{
		(*this)>> val.db_id >> val.energy >> val.gold >> val.reset_level >> val.pvp_rank_times >> val.gold_level_times >> val.exp_level_times >> val.ever_battle_times >> val.boss_level_times >> val.mystical_shop_times >> val.buy_treasure_times >> val.spexp_level_times >> val.vigour_times >> val.reset_normallevel >> val.goldstone_level_times ;
		return (*this);
	}

	Packet &operator>>(wheel_lottery &val)
	{
		(*this)>> val.db_id >> val.gold_rate >> val.diamond_rate >> val.gold_rand_rate >> val.diamond_rand_rate ;
		return (*this);
	}

	Packet &operator>>(gamble_form &val)
	{
		(*this)>> val.db_id >> val.get_times >> val.free_times >> val.interval_time >> val.first_bag >> val.free_bag >> val.every_day >> val.money_type >> val.price >> val.sp_min_times >> val.sp_max_times >> val.sp_success_rate >> val.sp_bag >> val.bag_1 >> val.bag_1_rate >> val.bag_2 >> val.bag_2_rate >> val.bag_3 >> val.bag_3_rate >> val.itemid >> val.itemcount ;
		return (*this);
	}

	Packet &operator>>(level_rewards &val)
	{
		(*this)>> val.db_id >> val.level >> val.item_id1 >> val.item_size1 >> val.item_id2 >> val.item_size2 >> val.item_id3 >> val.item_size3 ;
		return (*this);
	}

	Packet &operator>>(login_rewards &val)
	{
		(*this)>> val.item_id >> val.item_size >> val.vip_level ;
		return (*this);
	}

	Packet &operator>>(pay_rewards &val)
	{
		(*this)>> val.level >> val.item_id1 >> val.item_size1 >> val.item_id2 >> val.item_size2 >> val.item_id3 >> val.item_size3 ;
		return (*this);
	}

	Packet &operator>>(active_boss_monster &val)
	{
		(*this)>> val.db_id >> val.type >> val.name >> val.level >> val.life >> val.defense >> val.attack >> val.breake >> val.injury >> val.penetrate >> val.criticalresist >> val.breakresist >> val.attack_weight >> val.desc ;
		return (*this);
	}

	Packet &operator>>(active_boss_rewards1 &val)
	{
		(*this)>> val.db_id >> val.item_id1 >> val.size1 >> val.item_id2 >> val.size2 ;
		return (*this);
	}

	Packet &operator>>(active_boss_rewards2 &val)
	{
		(*this)>> val.db_id >> val.icon_id >> val.target >> val.type >> val.value >> val.item_id >> val.item_size >> val.tooltip ;
		return (*this);
	}

	Packet &operator>>(active_desc &val)
	{
		(*this)>> val.db_id >> val.name >> val.show_name >> val.start_time >> val.end_time >> val.start_time1 >> val.end_time1 >> val.level >> val.free_times >> val.charge >> val.charge_add >> val.charge_times >> val.is_open >> val.user_data1 >> val.user_data2 >> val.rewards_table >> val.icon_id >> val.tooltip ;
		return (*this);
	}

	Packet &operator>>(active_pvp_rewards &val)
	{
		(*this)>> val.db_id >> val.win_reward >> val.defeat_reward >> val.rank_reward >> val.diamond_reward ;
		return (*this);
	}

	Packet &operator>>(active_pvp_trade &val)
	{
		(*this)>> val.db_id >> val.feat >> val.item_id >> val.item_size ;
		return (*this);
	}

	Packet &operator>>(active_pvp_robot &val)
	{
		(*this)>> val.db_id >> val.name >> val.rank >> val.level >> val.profession >> val.head >> val.head_enhance >> val.head_star >> val.weapon >> val.weapon_enhance >> val.weapon_star >> val.body >> val.body_enhance >> val.body_star >> val.arm >> val.arm_enhance >> val.arm_star >> val.leg >> val.leg_enhance >> val.leg_star >> val.foot >> val.foot_enhance >> val.foot_star >> val.left_finger >> val.right_finger >> val.suit >> val.fight_power >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.criticalresist >> val.breakresist >> val.skill ;
		return (*this);
	}

	Packet &operator>>(day_task &val)
	{
		(*this)>> val.db_id >> val.name >> val.level >> val.value >> val.order >> val.icon_id >> val.tooltip >> val.reward_money >> val.reward_moneynum >> val.reward_item >> val.reward_itemnum >> val.param ;
		return (*this);
	}

	Packet &operator>>(pay_money &val)
	{
		(*this)>> val.db_id >> val.name >> val.base_value >> val.add_value1 >> val.add_value2 >> val.money >> val.is_limit >> val.is_recommend >> val.order >> val.icon_id >> val.tooltip1 >> val.tooltip2 ;
		return (*this);
	}

	Packet &operator>>(gold_level &val)
	{
		(*this)>> val.db_id >> val.level_limit >> val.score >> val.gold_rate >> val.diamond_rate >> val.level_id >> val.tooltip >> val.small >> val.middle >> val.big >> val.icon_id >> val.mo_total >> val.killcount_reward >> val.draw_card_id ;
		return (*this);
	}

	Packet &operator>>(active_exp_level &val)
	{
		(*this)>> val.db_id >> val.level_limit >> val.max_exp >> val.level_id >> val.icon_id >> val.tooltip >> val.max_health >> val.mo_attack >> val.add_attack1 >> val.add_attack2 >> val.add_exp >> val.add_rate >> val.mo_total >> val.killcount_reward >> val.draw_card_id ;
		return (*this);
	}

	Packet &operator>>(equip_split &val)
	{
		(*this)>> val.db_id >> val.mat_id1 >> val.mat_num1 >> val.mat_id2 >> val.mat_num2 >> val.mat_id3 >> val.mat_num3 >> val.mat_id4 >> val.mat_num4 ;
		return (*this);
	}

	Packet &operator>>(exchange_shop &val)
	{
		(*this)>> val.db_id >> val.item_id >> val.mat_id >> val.mat_num >> val.level >> val.index ;
		return (*this);
	}

	Packet &operator>>(active_battle_rewards &val)
	{
		(*this)>> val.db_id >> val.gift_id ;
		return (*this);
	}

	Packet &operator>>(active_ever_battle &val)
	{
		(*this)>> val.db_id >> val.com_gift >> val.sp_gift >> val.price >> val.level_id >> val.name >> val.tooltip >> val.desc ;
		return (*this);
	}

	Packet &operator>>(activeness_rewards &val)
	{
		(*this)>> val.db_id >> val.value >> val.gold >> val.diamond >> val.gift_id ;
		return (*this);
	}

	Packet &operator>>(continue_login_rewards &val)
	{
		(*this)>> val.db_id >> val.gold >> val.diamond >> val.gift_id ;
		return (*this);
	}

	Packet &operator>>(active_treasure_level &val)
	{
		(*this)>> val.db_id >> val.item_id >> val.size >> val.max_size ;
		return (*this);
	}

	Packet &operator>>(player_meridian &val)
	{
		(*this)>> val.db_id >> val.name >> val.level >> val.is_up >> val.item_id >> val.item_size >> val.sp_exp >> val.gold >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.critical_resist >> val.penetrate >> val.injury >> val.break_resist >> val.up_rate >> val.tooltip ;
		return (*this);
	}

	Packet &operator>>(bounty_task &val)
	{
		(*this)>> val.db_id >> val.type >> val.name >> val.gold >> val.item_id >> val.count >> val.level_id >> val.icon_id >> val.tooltip ;
		return (*this);
	}

	Packet &operator>>(bounty_refresh &val)
	{
		(*this)>> val.db_id >> val.reward_rate >> val.up_rate >> val.down_rate >> val.normal_rate >> val.up_star >> val.down_star ;
		return (*this);
	}

	Packet &operator>>(equip_cultivate_attr &val)
	{
		(*this)>> val.db_id >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.critical_resist >> val.break_resist >> val.unlock_color ;
		return (*this);
	}

	Packet &operator>>(equip_cultivate_max &val)
	{
		(*this)>> val.equip_color >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.critical_resist >> val.break_resist ;
		return (*this);
	}

	Packet &operator>>(equip_cultivate_type &val)
	{
		(*this)>> val.db_id >> val.type >> val.vip_level >> val.attr_type >> val.num_min >> val.num_max >> val.cost_item >> val.cost_item_num >> val.add_num >> val.pro >> val.times ;
		return (*this);
	}

	Packet &operator>>(player_achieve &val)
	{
		(*this)>> val.db_id >> val.type >> val.value >> val.money_type >> val.money >> val.item_id1 >> val.count1 >> val.item_id2 >> val.count2 >> val.achieve_type >> val.tooltip ;
		return (*this);
	}

	Packet &operator>>(achieve_rewards &val)
	{
		(*this)>> val.db_id >> val.money_type >> val.money >> val.gift_id >> val.tooltip ;
		return (*this);
	}

	Packet &operator>>(part_upgrade &val)
	{
		(*this)>> val.db_id >> val.min_exp >> val.max_exp >> val.rate >> val.gold >> val.item_id >> val.count >> val.add_rate >> val.exp ;
		return (*this);
	}

	Packet &operator>>(equip_suit &val)
	{
		(*this)>> val.db_id >> val.name >> val.suit_id >> val.num >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.critical_resist >> val.break_resist >> val.miss_odds >> val.hit_odds >> val.hurt_deepen >> val.hurt_remission >> val.break_defense >> val.rage_bonuses >> val.is_show ;
		return (*this);
	}

	Packet &operator>>(drink_wine &val)
	{
		(*this)>> val.db_id >> val.gold >> val.exp >> val.sp_exp >> val.energy >> val.team_gold >> val.team_exp >> val.team_sp_exp >> val.team_energy >> val.captain_gold >> val.captain_exp >> val.captain_sp_exp >> val.captain_energy >> val.gold_item >> val.exp_item >> val.sp_exp_item >> val.energy_item ;
		return (*this);
	}

	Packet &operator>>(class_level_rating &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.times ;
		return (*this);
	}

	Packet &operator>>(ci_chapter_map_describe &val)
	{
		(*this)>> val.db_id >> val.icon_id >> val.name >> val.total >> val.level1 >> val.level2 >> val.level3 >> val.level4 >> val.level5 >> val.level6 >> val.level7 >> val.level8 >> val.level9 >> val.level10 >> val.route_id >> val.gold_box >> val.silver_box >> val.copper_box >> val.gold_times >> val.silver_times >> val.copper_times >> val.sp_gold_box >> val.sp_silver_box >> val.sp_copper_box >> val.sp_gold_times >> val.sp_silver_times >> val.sp_copper_times ;
		return (*this);
	}

	Packet &operator>>(ci_level_object &val)
	{
		(*this)>> val.db_id >> val.name >> val.icon_id >> val.pass_ref >> val.com_level_id >> val.sp_level_id >> val.level_limit >> val.com_gift_id >> val.sp_gift_id >> val.com_gold >> val.sp_gold >> val.com_exp >> val.sp_exp >> val.desc_id >> val.cutscene_id >> val.icon >> val.event_table >> val.drop_id >> val.com_ad_fight >> val.sp_ad_fight >> val.com_extra_drop_id >> val.sp_extra_drop_id >> val.fbbox >> val.sp_fbbox >> val.fbbubble_desc >> val.sp_fbbubble_desc ;
		return (*this);
	}

	Packet &operator>>(task_gift_bag &val)
	{
		(*this)>> val.db_id >> val.rate >> val.draw_card_id >> val.com_drop_id >> val.sp_drop_id >> val.drop_com_exp >> val.drop_sp_exp >> val.show_gift ;
		return (*this);
	}

	Packet &operator>>(ci_auto_task &val)
	{
		(*this)>> val.db_id >> val.type >> val.name >> val.cutscene_id >> val.chapter_id >> val.level_id >> val.next_id ;
		return (*this);
	}

	Packet &operator>>(cut_scene &val)
	{
		(*this)>> val.db_id >> val.scene_name >> val.dialog_table >> val.object_table >> val.gold >> val.exp ;
		return (*this);
	}

	Packet &operator>>(current_equip &val)
	{
		(*this)>> val.PlayerID >> val.head >> val.neck >> val.shoulder >> val.weapon >> val.body >> val.waist >> val.arm >> val.leg >> val.foot >> val.wing >> val.left_finger >> val.right_finger >> val.suit ;
		return (*this);
	}

	Packet &operator>>(current_skill &val)
	{
		(*this)>> val.PlayerID >> val.active1 >> val.active2 >> val.active3 >> val.passive1 >> val.passive2 >> val.passive3 >> val.unique1 ;
		return (*this);
	}

	Packet &operator>>(current_epinlay &val)
	{
		(*this)>> val.PlayerID >> val.head >> val.neck >> val.shoulder >> val.weapon >> val.body >> val.waist >> val.arm >> val.leg >> val.foot >> val.wing >> val.left_finger >> val.right_finger ;
		return (*this);
	}

	Packet &operator>>(current_symbol &val)
	{
		(*this)>> val.PlayerID >> val.pos1 >> val.pos2 >> val.pos3 >> val.pos4 >> val.pos5 >> val.pos6 >> val.pos7 >> val.pos8 >> val.pos9 ;
		return (*this);
	}

	Packet &operator>>(obj_team_level &val)
	{
		(*this)>> val.index >> val.value ;
		return (*this);
	}

	Packet &operator>>(mail_money &val)
	{
		(*this)>> val.type >> val.value ;
		return (*this);
	}

	Packet &operator>>(obj_achieve &val)
	{
		(*this)>> val.achieve_id >> val.is_get ;
		return (*this);
	}

	Packet &operator>>(int_short &val)
	{
		(*this)>> val.intId >> val.shortId ;
		return (*this);
	}

	Packet &operator>>(random_symbol &val)
	{
		(*this)>> val.item_id >> val.rate ;
		return (*this);
	}

	Packet &operator>>(int_bool &val)
	{
		(*this)>> val.value >> val.is_get ;
		return (*this);
	}

	Packet &operator>>(uint_bool &val)
	{
		(*this)>> val.value >> val.is_get ;
		return (*this);
	}

	Packet &operator>>(recieve_energy &val)
	{
		(*this)>> val.player_id >> val.recieve_time >> val.is_recieve ;
		return (*this);
	}

	Packet &operator>>(int_uint &val)
	{
		(*this)>> val.intId >> val.uintId ;
		return (*this);
	}

	Packet &operator>>(obj_common &val)
	{
		(*this)>> val.item_id >> val.count ;
		return (*this);
	}

	Packet &operator>>(obj_equip &val)
	{
		(*this)>> val.item_id >> val.count ;
		return (*this);
	}

	Packet &operator>>(obj_gem &val)
	{
		(*this)>> val.item_id >> val.count ;
		return (*this);
	}

	Packet &operator>>(obj_material &val)
	{
		(*this)>> val.item_id >> val.count ;
		return (*this);
	}

	Packet &operator>>(obj_piece &val)
	{
		(*this)>> val.item_id >> val.count ;
		return (*this);
	}

	Packet &operator>>(obj_skill &val)
	{
		(*this)>> val.skill_id >> val.level ;
		return (*this);
	}

	Packet &operator>>(obj_symbol &val)
	{
		(*this)>> val.item_id >> val.exp >> val.level ;
		return (*this);
	}

	Packet &operator>>(obj_epinlay &val)
	{
		(*this)>> val.order_id >> val.gem_id ;
		return (*this);
	}

	Packet &operator>>(obj_enhance &val)
	{
		(*this)>> val.attr_type >> val.attr ;
		return (*this);
	}

	Packet &operator>>(obj_box_get &val)
	{
		(*this)>> val.chapter_id >> val.chapter_type >> val.is_get_copper >> val.is_get_silver >> val.is_get_gold ;
		return (*this);
	}

	Packet &operator>>(obj_title &val)
	{
		(*this)>> val.title_id >> val.title_times ;
		return (*this);
	}

	Packet &operator>>(obj_gold &val)
	{
		(*this)>> val.gold_times >> val.gold_rate ;
		return (*this);
	}

	Packet &operator>>(obj_chip &val)
	{
		(*this)>> val.item_id >> val.count ;
		return (*this);
	}

	Packet &operator>>(obj_god_weapon &val)
	{
		(*this)>> val.item_id >> val.count ;
		return (*this);
	}

	Packet &operator>>(obj_godinlay &val)
	{
		(*this)>> val.order_id >> val.god_id ;
		return (*this);
	}

	Packet &operator>>(player_account &val)
	{
		(*this)>> val.AccountID >> val.PlatformID >> val.PlayerID1 >> val.PlayerID2 >> val.PlayerID3 >> val.PlayerID4 ;
		return (*this);
	}

	Packet &operator>>(player_attributes &val)
	{
		(*this)>> val.PlayerID >> val.name >> val.profession >> val.energy >> val.last_fight >> val.experience >> val.level >> val.gold >> val.diamond >> val.head >> val.neck >> val.shoulder >> val.weapon >> val.body >> val.waist >> val.arm >> val.leg >> val.foot >> val.wing >> val.left_finger >> val.right_finger >> val.IsNpc >> val.last_login >> val.login_times >> val.equip_bags >> val.gem_bags >> val.material_bags >> val.vip >> val.charge >> val.sp_level >> val.sp_exp >> val.head_1 >> val.neck_1 >> val.shoulder_1 >> val.weapon_1 >> val.body_1 >> val.waist_1 >> val.arm_1 >> val.leg_1 >> val.foot_1 >> val.wing_1 >> val.left_finger_1 >> val.right_finger_1 >> val.head_2 >> val.neck_2 >> val.shoulder_2 >> val.weapon_2 >> val.body_2 >> val.waist_2 >> val.arm_2 >> val.leg_2 >> val.foot_2 >> val.wing_2 >> val.left_finger_2 >> val.right_finger_2 >> val.ever_battle_layer >> val.ever_battle_time ;
		return (*this);
	}

	Packet &operator>>(common_today_info &val)
	{
		(*this)>> val.PlayerID >> val.energy_times >> val.buy_gold_times >> val.gold_level_times >> val.exp_level_times >> val.lottery_times >> val.gamble_times >> val.is_login >> val.present_times >> val.recieve_times >> val.buy_gold_level >> val.buy_exp_level >> val.shop_limit >> val.treasure_num >> val.treasure_state >> val.active_value >> val.active_rewards >> val.bounty_task_times >> val.month_card_reward >> val.mystical_shop_times >> val.buy_mystical_shop >> val.boss_level_times >> val.buy_boss_level >> val.inter_shop_times >> val.drink_wine_times >> val.drink_wine_start >> val.drink_interval_time >> val.buy_treasure_times >> val.left_treasure_times >> val.enter_battle_times >> val.buy_battle_times >> val.left_battle_times >> val.ever_battle_rewards >> val.ever_battle_box_rewards ;
		return (*this);
	}

	Packet &operator>>(player_enhance &val)
	{
		(*this)>> val.PlayerID >> val.head >> val.neck >> val.shoulder >> val.weapon >> val.body >> val.waist >> val.arm >> val.leg >> val.foot >> val.wing >> val.left_finger >> val.right_finger >> val.suit ;
		return (*this);
	}

	Packet &operator>>(Player_Info &val)
	{
		(*this)>> val.m_Attributes >> val.m_CurEquip >> val.m_CurSkill >> val.m_ip ;
		return (*this);
	}

	Packet &operator>>(Fight_Attr &val)
	{
		(*this)>> val.m_life >> val.m_defense >> val.m_attack >> val.m_critical >> val.m_breake >> val.m_injury >> val.m_penetrate >> val.m_pvppower >> val.m_pvpresist >> val.m_criticalresist >> val.m_breakresist >> val.m_dodge >> val.m_energyadd >> val.m_expadd >> val.m_goldadd >> val.m_missodds >> val.m_hitodds >> val.m_hurtdeepen >> val.m_hurtremission >> val.m_breakdefense >> val.m_ragebonuses >> val.m_fight_power ;
		return (*this);
	}

	Packet &operator>>(choose_role_data &val)
	{
		(*this)>> val.player_id >> val.player_name >> val.profession >> val.level >> val.weapon >> val.suit ;
		return (*this);
	}

	Packet &operator>>(main_task_info &val)
	{
		(*this)>> val.PlayerID >> val.curr_id >> val.is_over >> val.box_get >> val.chapter1 >> val.chapter2 >> val.chapter3 >> val.chapter4 >> val.chapter5 >> val.chapter6 >> val.chapter7 >> val.chapter8 >> val.chapter9 >> val.chapter10 >> val.chapter11 >> val.chapter12 >> val.chapter13 >> val.chapter14 >> val.chapter15 >> val.chapter16 >> val.chapter17 >> val.chapter18 >> val.chapter19 >> val.chapter20 ;
		return (*this);
	}

	Packet &operator>>(main_task_today_info &val)
	{
		(*this)>> val.PlayerID >> val.chapter1 >> val.chapter2 >> val.chapter3 >> val.chapter4 >> val.chapter5 >> val.chapter6 >> val.chapter7 >> val.chapter8 >> val.chapter9 >> val.chapter10 >> val.chapter11 >> val.chapter12 >> val.chapter13 >> val.chapter14 >> val.chapter15 >> val.chapter16 >> val.chapter17 >> val.chapter18 >> val.chapter19 >> val.chapter20 ;
		return (*this);
	}

	Packet &operator>>(SrVector &val)
	{
		(*this)>> val.x >> val.y >> val.z ;
		return (*this);
	}

	Packet &operator>>(common_single_use &val)
	{
		(*this)>> val.PlayerID >> val.level_gift >> val.pay_gift >> val.pay_money >> val.shop_limit >> val.symbol_reward >> val.exchange_gift >> val.guide_id >> val.practice_time >> val.login_times >> val.login_rewards >> val.login_time >> val.month_end_time >> val.month_first_rewards >> val.pvp_best_rank >> val.buy_vip_shop >> val.gold_gamble >> val.diamond_gamble >> val.month_cardid >> val.god_weapon_times >> val.treasure_times >> val.symbol_diamond >> val.ever_battle_rewards >> val.ever_battle_score >> val.symbol_energy >> val.month_card_recharge_count >> val.ever_card_recharge_count >> val.month_card_state >> val.ever_card_state ;
		return (*this);
	}

	Packet &operator>>(active_boss_rank &val)
	{
		(*this)>> val.PlayerID >> val.result >> val.times >> val.rewards_get1 >> val.rewards_get2 >> val.ranking ;
		return (*this);
	}

	Packet &operator>>(boss_rank_data &val)
	{
		(*this)>> val.PlayerID >> val.profession >> val.suit >> val.ranking >> val.result >> val.level >> val.name ;
		return (*this);
	}

	Packet &operator>>(rank_data &val)
	{
		(*this)>> val.PlayerID >> val.rank ;
		return (*this);
	}

	Packet &operator>>(friend_data &val)
	{
		(*this)>> val.PlayerID >> val.PlayerName >> val.profession >> val.level >> val.vip >> val.suit >> val.fight_power ;
		return (*this);
	}

	Packet &operator>>(energy_data &val)
	{
		(*this)>> val.FriendData >> val.IsRecieve ;
		return (*this);
	}

	Packet &operator>>(send_energy_data &val)
	{
		(*this)>> val.FriendData >> val.DayNum ;
		return (*this);
	}

	Packet &operator>>(receive_energy_data &val)
	{
		(*this)>> val.FriendData >> val.DayNum ;
		return (*this);
	}

	Packet &operator>>(active_pvp_rank &val)
	{
		(*this)>> val.rank >> val.PlayerID >> val.left_times >> val.buy_times >> val.feat_time >> val.feat_sum >> val.feat_total >> val.rand1 >> val.rand2 >> val.rand3 >> val.info ;
		return (*this);
	}

	Packet &operator>>(pvp_rank_data &val)
	{
		(*this)>> val.rank >> val.PlayerID >> val.profession >> val.suit >> val.name >> val.level >> val.fight_power ;
		return (*this);
	}

	Packet &operator>>(PvpRandomData &val)
	{
		(*this)>> val.rank >> val.PlayerID >> val.profession >> val.suit >> val.weapon >> val.level >> val.fight_power >> val.left_finger >> val.name >> val.canFight ;
		return (*this);
	}

	Packet &operator>>(ever_battle_rank &val)
	{
		(*this)>> val.PlayerID >> val.profession >> val.level >> val.rank >> val.name >> val.layer ;
		return (*this);
	}

	Packet &operator>>(treasure_data &val)
	{
		(*this)>> val.PlayerID >> val.count >> val.m_PlayerInfo >> val.m_FightAttr ;
		return (*this);
	}

	Packet &operator>>(player_treasure &val)
	{
		(*this)>> val.PlayerID >> val.last_time >> val.attack_time >> val.info ;
		return (*this);
	}

	Packet &operator>>(player_achieve_info &val)
	{
		(*this)>> val.PlayerID >> val.rewards >> val.achieves ;
		return (*this);
	}

	Packet &operator>>(player_other_info &val)
	{
		(*this)>> val.PlayerID >> val.discount_end_time >> val.buy_item_info >> val.last_lost_map_id >> val.last_lost_map_desc >> val.last_level_update_time ;
		return (*this);
	}

	Packet &operator>>(drink_wine_info &val)
	{
		(*this)>> val.PlayerID >> val.PlayerName >> val.profession >> val.level >> val.vip >> val.suit >> val.fight_power >> val.drink_times ;
		return (*this);
	}

	Packet &operator>>(mail_data &val)
	{
		(*this)>> val.index >> val.is_get >> val.is_read >> val.is_del >> val.ifield >> val.itime >> val.name >> val.content >> val.sender >> val.attach >> val.money ;
		return (*this);
	}

	Packet &operator>>(order_data &val)
	{
		(*this)>> val.IsAddMoney >> val.ItemId >> val.OrderId ;
		return (*this);
	}

	Packet &operator>>(mail_gift &val)
	{
		(*this)>> val.db_id >> val.items >> val.date_type >> val.keep_time >> val.description ;
		return (*this);
	}

	Packet &operator>>(active_reward &val)
	{
		(*this)>> val.db_id >> val.rank_1 >> val.rank_2 >> val.rank_3 >> val.rank_4 >> val.rank_5 >> val.rank_6 >> val.rank_7 >> val.rank_8 >> val.rank_9 >> val.rank_10 >> val.rank_11_50 >> val.rank_50_ ;
		return (*this);
	}

	Packet &operator>>(marquee &val)
	{
		(*this)>> val.db_id >> val.line_1 >> val.line_2 >> val.line_3 >> val.line_4 >> val.line_5 >> val.line_6 >> val.line_7 >> val.line_8 >> val.line_9 >> val.line_10 ;
		return (*this);
	}

	Packet &operator>>(player_rank &val)
	{
		(*this)>> val.charid >> val.rank >> val.name >> val.profession >> val.num >> val.id ;
		return (*this);
	}

	Packet &operator>>(mystical_shop &val)
	{
		(*this)>> val.db_id >> val.minlv >> val.maxlv >> val.vip >> val.type >> val.item_id >> val.item_size >> val.rate >> val.money_type >> val.money ;
		return (*this);
	}

	Packet &operator>>(BossUserDam &val)
	{
		(*this)>> val.name >> val.dam ;
		return (*this);
	}

	Packet &operator>>(active_boss_level &val)
	{
		(*this)>> val.db_id >> val.level_limit >> val.drop_id >> val.draw_card_id ;
		return (*this);
	}

	Packet &operator>>(month_card &val)
	{
		(*this)>> val.db_id >> val.diamond >> val.first_rewards >> val.today_rewards >> val.overdue_time >> val.renew_time >> val.count ;
		return (*this);
	}

	Packet &operator>>(active_inter_shop &val)
	{
		(*this)>> val.db_id >> val.viplevel >> val.plan_id >> val.price >> val.gold >> val.diamond >> val.itemid1 >> val.itemcount1 >> val.itemid2 >> val.itemcount2 >> val.itemid3 >> val.itemcount3 >> val.oldprice ;
		return (*this);
	}

	Packet &operator>>(active_operate_desc &val)
	{
		(*this)>> val.db_id >> val.starttime >> val.endtime >> val.interval >> val.level ;
		return (*this);
	}

	Packet &operator>>(vip_shop &val)
	{
		(*this)>> val.level >> val.itemid >> val.price ;
		return (*this);
	}

	Packet &operator>>(god_weapon &val)
	{
		(*this)>> val.godweapon_id >> val.condition >> val.condition_num >> val.level >> val.begin_level >> val.tips ;
		return (*this);
	}

	Packet &operator>>(god_weapon_upgrade &val)
	{
		(*this)>> val.db_id >> val.fight >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.critical_resist >> val.break_resist >> val.miss_odds >> val.hit_odds >> val.hurt_deepen >> val.hurt_remission >> val.break_defense >> val.rage_bonuses ;
		return (*this);
	}

	Packet &operator>>(god_weapon_upgrade_stage &val)
	{
		(*this)>> val.db_id >> val.godweapon_id >> val.level >> val.quality >> val.cost_item >> val.cost_item_num >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.pvp_power >> val.pvp_resist >> val.critical_resist >> val.break_resist >> val.miss_odds >> val.hit_odds >> val.hurt_deepen >> val.hurt_remission >> val.break_defense >> val.rage_bonuses >> val.skill_id >> val.skill_lv >> val.skill_name >> val.skill_icon >> val.skill_desc >> val.buff_id >> val.buff_name >> val.buff_desc >> val.buff_battle >> val.buff_icon >> val.effid ;
		return (*this);
	}

	Packet &operator>>(player_god_weapon &val)
	{
		(*this)>> val.PlayerID >> val.Datas ;
		return (*this);
	}

	Packet &operator>>(god_weapon_net &val)
	{
		(*this)>> val.god_weapon_id >> val.stage ;
		return (*this);
	}

	Packet &operator>>(discount_shop &val)
	{
		(*this)>> val.db_id >> val.item_id >> val.item_count >> val.money_type >> val.price >> val.countless >> val.oldprice ;
		return (*this);
	}

	Packet &operator>>(boss_fight &val)
	{
		(*this)>> val.PlayerID >> val.name >> val.profession >> val.weapon >> val.body >> val.left_finger ;
		return (*this);
	}

	Packet &operator>>(player_title &val)
	{
		(*this)>> val.PlayerID >> val.title >> val.titletimes ;
		return (*this);
	}

	Packet &operator>>(title_info &val)
	{
		(*this)>> val.db_id >> val.title_type >> val.condition >> val.parm1 >> val.parm2 >> val.is_time >> val.time >> val.weight >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.pvp_power >> val.pvp_resist >> val.critical_resist >> val.break_resist >> val.miss_odds >> val.hit_odds >> val.hurt_deepen >> val.hurt_remission >> val.break_defense >> val.rage_bonuses >> val.name_id ;
		return (*this);
	}

	Packet &operator>>(friend_pvp &val)
	{
		(*this)>> val.pvp_val ;
		return (*this);
	}

	Packet &operator>>(server_info &val)
	{
		(*this)>> val.zoneid >> val.name >> val.zonestate >> val.status >> val.ip >> val.port ;
		return (*this);
	}

	Packet &operator>>(symbol_suit &val)
	{
		(*this)>> val.id >> val.level >> val.num >> val.life >> val.attack >> val.defense >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.critical_resist >> val.break_resist ;
		return (*this);
	}

	Packet &operator>>(ci_level_drop &val)
	{
		(*this)>> val.db_id >> val.item_chance >> val.item_limit >> val.gold_chance >> val.gold_limit >> val.monster_count ;
		return (*this);
	}

	Packet &operator>>(active_spexp_level &val)
	{
		(*this)>> val.db_id >> val.level_limit >> val.monster_count >> val.base_spexp >> val.min_card_spexp >> val.max_card_spexp >> val.draw_card_id >> val.killcount_reward ;
		return (*this);
	}

	Packet &operator>>(BoxData &val)
	{
		(*this)>> val.Id >> val.MaxKillCount >> val.Boxes ;
		return (*this);
	}

	Packet &operator>>(achieve_task &val)
	{
		(*this)>> val.db_id >> val.name >> val.type >> val.param1 >> val.param2 >> val.task_id >> val.level >> val.reward1 >> val.reward1_num >> val.reward2 >> val.reward2_num >> val.tooltip >> val.profession ;
		return (*this);
	}

	Packet &operator>>(hard_level &val)
	{
		(*this)>> val.db_id >> val.times ;
		return (*this);
	}

	Packet &operator>>(level_state &val)
	{
		(*this)>> val.db_id >> val.times >> val.star ;
		return (*this);
	}

	Packet &operator>>(equip_gem &val)
	{
		(*this)>> val.db_id >> val.level >> val.head >> val.weapon >> val.body >> val.arm >> val.foot >> val.leg ;
		return (*this);
	}

	Packet &operator>>(general_shop &val)
	{
		(*this)>> val.db_id >> val.shop_type >> val.level_min >> val.level_max >> val.vip_min >> val.vip_max >> val.item_id >> val.item_size >> val.item_buy_times >> val.buy_limit_type >> val.money_type >> val.price ;
		return (*this);
	}

	Packet &operator>>(clothes &val)
	{
		(*this)>> val.db_id >> val.name >> val.profession >> val.level ;
		return (*this);
	}

	Packet &operator>>(clothes_level &val)
	{
		(*this)>> val.db_id >> val.clothes_id >> val.level >> val.item_id >> val.item_num >> val.life >> val.attack >> val.defense >> val.breake >> val.penetrate >> val.break_resist >> val.critical >> val.critical_resist >> val.injury >> val.miss_odds >> val.hit_odds >> val.hurt_deepen >> val.hurt_remission >> val.break_defense >> val.rage_bonuses ;
		return (*this);
	}

	Packet &operator>>(clothes_level_data &val)
	{
		(*this)>> val.id >> val.level ;
		return (*this);
	}

	Packet &operator>>(gamble_data &val)
	{
		(*this)>> val.id >> val.total_gamble_times >> val.gamble_times >> val.free_gamble_time >> val.today_gamble_times ;
		return (*this);
	}

	Packet &operator>>(snatch_info &val)
	{
		(*this)>> val.charid >> val.name >> val.itemid >> val.snatch_time >> val.level >> val.profession >> val.fight_power >> val.issuccess ;
		return (*this);
	}

	Packet &operator>>(snatch_shop &val)
	{
		(*this)>> val.db_id >> val.time >> val.cost_item >> val.cost_money ;
		return (*this);
	}

	Packet &operator>>(snatch_pro &val)
	{
		(*this)>> val.db_id >> val.player_pro >> val.npc_pro >> val.item_id >> val.drop_bag_group_id ;
		return (*this);
	}

	Packet &operator>>(snatch &val)
	{
		(*this)>> val.db_id >> val.type >> val.is_off >> val.open_lv >> val.piece_count >> val.is_bulletin ;
		return (*this);
	}

	Packet &operator>>(snatch_drop &val)
	{
		(*this)>> val.player_lv >> val.rewards_exp >> val.rewards_gold ;
		return (*this);
	}

	Packet &operator>>(snatch_mate_info &val)
	{
		(*this)>> val.charid >> val.level >> val.profession >> val.name >> val.fight_power ;
		return (*this);
	}

	Packet &operator>>(extra_drop_item &val)
	{
		(*this)>> val.db_id >> val.drop_bag_group_id >> val.type ;
		return (*this);
	}

	Packet &operator>>(vip_item_shop &val)
	{
		(*this)>> val.db_id >> val.money_type >> val.price >> val.original_price >> val.times >> val.item1_id >> val.item1_num >> val.item2_id >> val.item2_num >> val.item3_id >> val.item3_num >> val.item4_id >> val.item4_num ;
		return (*this);
	}

	Packet &operator>>(shop_game &val)
	{
		(*this)>> val.db_id >> val.price >> val.selltype >> val.item_id >> val.start_time >> val.end_time >> val.startday >> val.sellday >> val.resetday >> val.viplimit ;
		return (*this);
	}

	Packet &operator>>(activity_team_level &val)
	{
		(*this)>> val.db_id >> val.level >> val.type >> val.item1_id >> val.item1_num >> val.item2_id >> val.item2_num >> val.item3_id >> val.item3_num >> val.drop_id >> val.level_id >> val.icon_id >> val.level_name >> val.fight_power_recommend >> val.level_info ;
		return (*this);
	}

	Packet &operator>>(team_level_rule &val)
	{
		(*this)>> val.level >> val.rule ;
		return (*this);
	}

	Packet &operator>>(guild_info_list &val)
	{
		(*this)>> val.guildid >> val.name >> val.curcount >> val.level >> val.fight_power ;
		return (*this);
	}

	Packet &operator>>(guild_info &val)
	{
		(*this)>> val.guildid >> val.name >> val.note >> val.notify >> val.exp >> val.level >> val.fight_power >> val.curcount >> val.currank >> val.VIP >> val.masterName ;
		return (*this);
	}

	Packet &operator>>(guild_player_info &val)
	{
		(*this)>> val.charid >> val.name >> val.profession >> val.level >> val.fight_power >> val.VIP >> val.occupation >> val.Contribtion >> val.online ;
		return (*this);
	}

	Packet &operator>>(carnival_task_info &val)
	{
		(*this)>> val.task_id >> val.status >> val.cur_data ;
		return (*this);
	}

	Packet &operator>>(god_weapon_location &val)
	{
		(*this)>> val.god_weapon_id >> val.position >> val.stage ;
		return (*this);
	}

	Packet &operator>>(item_chip &val)
	{
		(*this)>> val.db_id >> val.name >> val.type >> val.level >> val.quality >> val.order >> val.color >> val.data >> val.trans_id >> val.item_id1 >> val.item_num1 >> val.item_id2 >> val.item_num2 >> val.item_id3 >> val.item_num3 >> val.icon_id >> val.tooltip >> val.desc ;
		return (*this);
	}

	Packet &operator>>(equip_advance &val)
	{
		(*this)>> val.db_id >> val.life >> val.defense >> val.attack >> val.critical >> val.breake >> val.injury >> val.penetrate >> val.criticalresist >> val.breakresist >> val.missodds >> val.hitodds >> val.hurtdeepen >> val.hurtremission >> val.breakdefense >> val.ragebonuses >> val.life_rate >> val.defense_rate >> val.attack_rate >> val.critical_rate >> val.breake_rate >> val.injury_rate >> val.penetrate_rate >> val.criticalresist_rate >> val.breakresist_rate >> val.missodds_rate >> val.hitodds_rate >> val.hurtdeepen_rate >> val.hurtremission_rate >> val.breakdefense_rate >> val.ragebonuses_rate >> val.tooltip >> val.last_id ;
		return (*this);
	}

	Packet &operator>>(guild_player_worship &val)
	{
		(*this)>> val.charid >> val.dbid >> val.pvprank >> val.name >> val.profession >> val.level >> val.waepon >> val.suit >> val.lfinger ;
		return (*this);
	}

	Packet &operator>>(goldstone_data &val)
	{
		(*this)>> val.charId >> val.StartTime >> val.Name >> val.FightPower >> val.Level >> val.Profession >> val.StoneType >> val.StoneId >> val.GuildId >> val.Money ;
		return (*this);
	}

	Packet &operator>>(goldstone_grabinfo &val)
	{
		(*this)>> val.fightsuccess >> val.IsGrab >> val.charId >> val.IncomeGold >> val.GrabGold >> val.Name ;
		return (*this);
	}

	Packet &operator>>(OperatingActivity &val)
	{
		(*this)>> val.m_Type >> val.m_IsPlaying >> val.m_StartTime >> val.m_EndTime >> val.m_LevelLimit >> val.m_Name >> val.m_param ;
		return (*this);
	}

	Packet &operator>>(stBoughtItemInfo &val)
	{
		(*this)>> val.itemId >> val.itemNum >> val.playerId >> val.itemPrice >> val.boughtTimes >> val.todayBoughtTimes ;
		return (*this);
	}

	Packet &operator>>(stGroupBuyingData &val)
	{
		(*this)>> val.itemId >> val.itemNum >> val.itemCurPrice >> val.boughtSum ;
		return (*this);
	}

};
} //end of namespace
#endif
