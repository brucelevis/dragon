/**
 * Copyright 2015 GDE Inc.
 *
 * \author huangxiaohui
 * \date 2016-02-23 19:20:15 CST
 * \brief 任务变量基础.
 *
 */
#ifndef VARS_H_
#define VARS_H_

#include <unordered_map>

/**
 * \brief 任务变量基础类. 
 */
template<typename K, typename V>
class Vars
{
protected:
	// 键类型
	typedef K KeyType;
	// 值类型
	typedef V ValueType;

	// 变量列表类型
	typedef std::unordered_map<KeyType, ValueType> VarMap;

public:
	/**
	 * \brief 设置任务变量
	 * \param key 变量键
	 * \param value 变量值
	 */
	virtual void setVar(const KeyType& key, const ValueType& value) = 0;
	/**
	 * \brief 获取任务变量（未设置默认为0）
	 * \param key 变量键
	 * \return 变量值
	 */
	virtual ValueType getVar(const KeyType& key) const = 0;

protected:
	// 变量列表
	VarMap vars_;
};

#endif