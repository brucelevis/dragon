/**
 * Copyright 2015 GDE Inc.
 *
 * \author huangxiaohui
 * \date 2016-02-22 15:20:15 CST
 * \brief 角色任务变量管理.
 *
 */
#ifndef SCENESERVER_PLAYERVARS_H_
#define SCENESERVER_PLAYERVARS_H_

#include "Vars.h"
#include "PlayerVarsDef.h"

class SceneUser;
class PbPlayerVars;
/**
 * \brief 角色任务变量管理. 
 */
class PlayerVars
	: public Vars<unsigned int, long long>
{
	friend class SceneUser;

public:
	PlayerVars(SceneUser* owner);

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
	 * \brief 客户端设置任务变量
	 * \param key 变量键
	 * \return 变量值
	 */
	bool onClientSetVar(const KeyType& key, const ValueType& value);

protected:
	/**
	 * \brief 保存
	 */
	void save(PbPlayerVars& binary);
	/**
	 * \brief 加载
	 */
	void load(const PbPlayerVars& binary);

	/**
	 * \brief 同步单个任务变量
	 * \param key 变量键
	 */
	void syncOne(const KeyType& key);
	/**
	 * \brief 同步所有任务变量
	 */
	void syncAll();

private:
	/**
	 * \brief 任务变量是否需要保存
	 * \param key 变量键
	 */
	bool needSave(const KeyType& key);
	/**
	 * \brief 任务变量是否需要同步
	 * \param key 变量键
	 */
	bool needSync(const KeyType& key);
	
	/**
	 * \brief 是否客户端变量
	 * \param key 变量键
	 */
	bool isClient(const KeyType& key) const;

private:
	SceneUser* owner_;
};

#endif