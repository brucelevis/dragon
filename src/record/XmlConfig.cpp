#include "XmlConfig.h"
#include "zXMLParser.h"

XmlConfig::XmlConfig()
{
}

XmlConfig::~XmlConfig()
{
}

bool XmlConfig::Initialize()
{
	if (!loadNewUserInit())
	{
		Fir::logger->error("[xml配置加载失败]loadNewUserInit!");
		return false;
	}

	return true;
}

bool XmlConfig::loadNewUserInit()
{
	std::string xmlpath = Fir::global["configdir"] + "xml/new_user_init.xml";

	zXMLParser xml;
	if (!xml.initFile(xmlpath))
	{
		Fir::logger->error("[玩家配置初始化]打开 new_user_init.xml 配置文件失败!");
		return false;
	}

	_xmlNode* xmlRoot = xml.getRootNode("Config");
	if (!xmlRoot)
	{
		Fir::logger->error("[玩家配置初始化]解析new_user_init.xml配置文件失败，找不到Config!");
		return false;
	}

	_xmlNode* moneyNode = xml.getChildNode(xmlRoot, "money");
	if (!moneyNode)
	{
		Fir::logger->error("[玩家配置初始化]解析new_user_init.xml配置文件失败，找不到money!");
		return false;
	}

	xml.getNodePropNum(moneyNode, "gold", &m_initItem.gold, sizeof(m_initItem.gold));
	xml.getNodePropNum(moneyNode, "diamond", &m_initItem.diamond, sizeof(m_initItem.diamond));
	xml.getNodePropNum(moneyNode, "energy", &m_initItem.energy, sizeof(m_initItem.energy));

	_xmlNode* equipNode = xml.getChildNode(xmlRoot, "equip");
	if (!equipNode)
	{
		Fir::logger->error("[玩家配置初始化]解析new_user_init.xml配置文件失败，找不到equip!");
		return false;
	}

	xml.getNodePropNum(equipNode, "head", &m_initItem.head, sizeof(m_initItem.head));
	xml.getNodePropNum(equipNode, "neck", &m_initItem.neck, sizeof(m_initItem.neck));
	xml.getNodePropNum(equipNode, "shoulder", &m_initItem.shoulder, sizeof(m_initItem.shoulder));
	xml.getNodePropNum(equipNode, "weapon", &m_initItem.weapon, sizeof(m_initItem.weapon));
	xml.getNodePropNum(equipNode, "body", &m_initItem.body, sizeof(m_initItem.body));
	xml.getNodePropNum(equipNode, "waist", &m_initItem.waist, sizeof(m_initItem.waist));
	xml.getNodePropNum(equipNode, "arm", &m_initItem.arm, sizeof(m_initItem.arm));
	xml.getNodePropNum(equipNode, "leg", &m_initItem.leg, sizeof(m_initItem.leg));
	xml.getNodePropNum(equipNode, "foot", &m_initItem.foot, sizeof(m_initItem.foot));
	xml.getNodePropNum(equipNode, "wing", &m_initItem.wing, sizeof(m_initItem.wing));
	xml.getNodePropNum(equipNode, "left_finger", &m_initItem.left_finger, sizeof(m_initItem.left_finger));
	xml.getNodePropNum(equipNode, "right_finger", &m_initItem.right_finger, sizeof(m_initItem.right_finger));
	xml.getNodePropNum(equipNode, "suit", &m_initItem.suit, sizeof(m_initItem.suit));

	_xmlNode* skillNode = xml.getChildNode(xmlRoot, "skill");
	if (!skillNode)
	{
		Fir::logger->error("[玩家配置初始化]解析new_user_init.xml配置文件失败，找不到skill!");
		return false;
	}

	xml.getNodePropNum(skillNode, "active1", &m_initItem.active1, sizeof(m_initItem.active1));
	xml.getNodePropNum(skillNode, "active2", &m_initItem.active2, sizeof(m_initItem.active2));
	xml.getNodePropNum(skillNode, "active3", &m_initItem.active3, sizeof(m_initItem.active3));
	xml.getNodePropNum(skillNode, "passive1", &m_initItem.passive1, sizeof(m_initItem.passive1));
	xml.getNodePropNum(skillNode, "unique1", &m_initItem.unique1, sizeof(m_initItem.unique1));

	_xmlNode* baglimitNode = xml.getChildNode(xmlRoot, "baglimit");
	if (!baglimitNode)
	{
		Fir::logger->error("[玩家配置初始化]解析new_user_init.xml配置文件失败，找不到baglimit!");
		return false;
	}

	xml.getNodePropNum(baglimitNode, "equip_bags", &m_initItem.equip_bags, sizeof(m_initItem.equip_bags));
	xml.getNodePropNum(baglimitNode, "gem_bags", &m_initItem.gem_bags, sizeof(m_initItem.gem_bags));
	xml.getNodePropNum(baglimitNode, "material_bags", &m_initItem.material_bags, sizeof(m_initItem.material_bags));

	xml.final();
	return true;
}