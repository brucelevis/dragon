#include "PlayerVars.h"

#include "SceneUser.h"
#include "HMessages.h"
#include "proto/serialize.pb.h"

PlayerVars::PlayerVars(SceneUser* owner)
	: owner_(owner)
{
}

void PlayerVars::setVar(const KeyType& key, const ValueType& value)
{
	int last_value = getVar(key);
	if (last_value == value)
	{
		return;
	}
	vars_[key] = value;

	if (needSync(key))
	{
		syncOne(key);
	}
}

PlayerVars::ValueType PlayerVars::getVar(const KeyType& key) const
{
	VarMap::const_iterator iter = vars_.find(key);
	if (iter != vars_.end())
	{
		return iter->second;
	}
	return 0;
}

bool PlayerVars::onClientSetVar(const KeyType& key, const ValueType& value)
{
	if (!isClient(key))
	{
		Fir::logger->warn("[任务变量]客户端设置失败，非客户端变量，角色ID = %u，变量ID = %u", owner_->getCharid(), key);
		return false;
	}
	vars_[key] = value;
	
	return true;
}

void PlayerVars::save(PbPlayerVars& binary)
{
	for (VarMap::const_iterator iter = vars_.begin(); iter != vars_.end(); ++iter)
	{
		if (!needSave(iter->first))
			continue;
		PbPlayerVars::PbVar* pvar = binary.add_vars();
		if (!pvar)
		{
			Fir::logger->error("[任务变量]保存失败，分配内存失败，角色ID = %u", owner_->getCharid());
		}
		pvar->set_key(iter->first);
		pvar->set_value(iter->second);
	}
}

void PlayerVars::load(const PbPlayerVars& binary)
{
	int count = binary.vars_size();

	for (int i = 0; i < count; ++i)
	{
		const PbPlayerVars::PbVar& var = binary.vars(i);
		
		vars_[var.key()] = var.value();
	}
}

void PlayerVars::syncOne(const KeyType& key)
{
	Packet packet;
	packet << key << getVar(key);

	owner_->SendMessage2Ci(FIGHTING::ID_Gs2Ci_PlayerOneVarSync, packet);
}

void PlayerVars::syncAll()
{
	int num = 0;

	Packet packet;
	packet << num;
	for (VarMap::const_iterator iter = vars_.begin(); iter != vars_.end(); ++iter)
	{
		if (!needSync(iter->first))
			continue;
		packet << iter->first << iter->second;
		num++;
	}
	packet.SeekSet(0);
	packet << num;

	owner_->SendMessage2Ci(FIGHTING::ID_Gs2Ci_PlayerAllVarsSync, packet);
}

bool PlayerVars::isClient(const KeyType& key) const
{
	//playervar_setting* p_setting = ExcelConfig::getMe().GetPlayerVarSetting(key);
	//if (!p_setting)
	//{
	//	return false;
	//}
	//return p_setting->client;
	return true;
}

bool PlayerVars::needSave(const KeyType& key)
{
	//playervar_setting* p_setting = ExcelConfig::getMe().GetPlayerVarSetting(key);
	//if (!p_setting)
	//{
	//	return false;
	//}
	//return p_setting->save || p_setting->client;
	return true;
}

bool PlayerVars::needSync(const KeyType& key)
{
	//playervar_setting* p_setting = ExcelConfig::getMe().GetPlayerVarSetting(key);
	//if (!p_setting)
	//{
	//	return false;
	//}
	//return p_setting->sync || p_setting->client;
	return true;
}