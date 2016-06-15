#include "GlobalVars.h"

#include "RecordCommand.h"
#include "RecordClient.h"
#include "SceneServer.h"

// GlobalVars唯一实例
GlobalVars* g_GlobalVars = new GlobalVars();
// GlobalTempVars唯一实例
GlobalTempVars* g_GlobalTempVars = new GlobalTempVars();

void GlobalVars::setVar(const KeyType& key, const ValueType& value)
{
	if (getVar(key) == value)
		return;
	vars_[key] = value;
	save(key, value);
}

GlobalVars::ValueType GlobalVars::getVar(const KeyType& key) const
{
	VarMap::const_iterator iter = vars_.find(key);
	if (iter != vars_.end())
	{
		return iter->second;
	}
	return 0;
}

bool GlobalVars::load()
{
	std::string strsql = "SELECT * FROM t_globalvars";

	RecordSetPtr pResult = SceneService::dbConnPool->exeSelect(strsql);
	if (NULL == pResult)
	{
		Fir::logger->error("[全局变量]加载失败，访问数据库失败,strsql=%s", strsql.c_str());
		return false;
	}

	for (DWORD i = 0; i < pResult->size(); i++)
	{
		Record* rec = pResult->get(i);
		if (NULL == rec) 
			continue;
		KeyType key = rec->get("f_key");
		ValueType value = rec->get("f_value");

		vars_[key] = value;
	}

	return true;
}

void GlobalVars::save(const KeyType& key, const ValueType& value)
{
	CMD::RECORD::t_WriteGlobalVar_SceneRecord cmd;
	cmd.key = key;
	cmd.value = value;

	recordClient->sendCmd(&cmd, sizeof(cmd));
}

void GlobalTempVars::setVar(const KeyType& key, const ValueType& value)
{
	vars_[key] = value;
}

GlobalTempVars::ValueType GlobalTempVars::getVar(const KeyType& key) const
{
	VarMap::const_iterator iter = vars_.find(key);
	if (iter != vars_.end())
	{
		return iter->second;
	}
	return 0;
}