#ifndef _XmlConfig_h_
#define _XmlConfig_h_

#include <map>

#include "zSingleton.h"

struct stInitItem
{
	int gold;
	int diamond;
	int energy;

	int head;
	int neck;
	int shoulder;
	int weapon;
	int body;
	int waist;
	int arm;
	int leg;
	int foot;
	int wing;
	int left_finger;
	int right_finger;
	int suit;

	int active1;
	int active2;
	int active3;
	int passive1;
	int unique1;

	int equip_bags;
	int gem_bags;
	int material_bags;

	stInitItem()
	{
		gold = 0;
		diamond = 0;
		energy = 0;

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

		active1 = 0;
		active2 = 0;
		active3 = 0;
		passive1 = 0;
		unique1 = 0;

		equip_bags = 0;
		gem_bags = 0;
		material_bags = 0;
	}
};

class  XmlConfig:  public Singleton <XmlConfig>
{
	public:

	public:
		XmlConfig();

		~XmlConfig();

	public:
		bool Initialize();

		const stInitItem getInitItem(){return m_initItem;}
	protected:
		bool loadNewUserInit();

	protected:
		stInitItem m_initItem;
};

#endif
