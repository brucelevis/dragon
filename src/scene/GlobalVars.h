/**
 * Copyright 2015 GDE Inc.
 *
 * \author huangxiaohui
 * \date 2016-02-23 19:20:15 CST
 * \brief 全局任务变量管理.
 *
 */
#ifndef SCENESERVER_GLOBALVARS_H_
#define SCENESERVER_GLOBALVARS_H_

#include "Vars.h"
#include "GlobalVarsDef.h"

/**
 * \brief 全局任务变量管理（会自动保存）. 
 */
class GlobalVars 
	: public Vars<unsigned long long, long long>
{
public:
	/**
	 * \brief 设置任务变量
	 * \param key 变量键
	 * \param value 变量值
	 */
	virtual void setVar(const KeyType& key, const ValueType& value);
	/**
	 * \brief 获取任务变量（未设置默认为0）
	 * \param key 变量键
	 * \return 变量值
	 */
	virtual ValueType getVar(const KeyType& key) const;

	/**
	* \brief 加载
	*/
	bool load();

protected:
	/**
	 * \brief 保存
	 */
	void save(const KeyType& key, const ValueType& value);
};

extern GlobalVars* g_GlobalVars;

/**
 * \brief 全局临时任务变量管理. 
 */
class GlobalTempVars 
	: public Vars<unsigned long long, long long>
{
public:
	/**
	 * \brief 设置任务变量
	 * \param key 变量键
	 * \param value 变量值
	 */
	virtual void setVar(const KeyType& key, const ValueType& value);
	/**
	 * \brief 获取任务变量（未设置默认为0）
	 * \param key 变量键
	 * \return 变量值
	 */
	virtual ValueType getVar(const KeyType& key) const;
};

extern GlobalTempVars* g_GlobalTempVars;

#endif