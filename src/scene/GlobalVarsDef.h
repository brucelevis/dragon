/**
 * Copyright 2015 GDE Inc.
 *
 * \author huangxiaohui
 * \date 2016-02-23 19:20:15 CST
 * \brief 全局任务变量定义.
 *
 */
#ifndef SCENESERVER_GLOBALVARS_DEF_H_
#define SCENESERVER_GLOBALVARS_DEF_H_

/**
 * \brief 全局变量类型
 */
enum GlobalVarsType
{
	GLOBAL_VAR_NONE		= 0,
	GLOBAL_VAR_GROUP_BUYING_REFREASH_PRICE_STATE = 1,	//团购刷新假数据的状态
	GLOBAL_VAR_GROUP_BUYING_LAST_PERIODS_ID = 2,	//团购上次第几周期数
};

/**
 * \brief 全局临时变量类型
 */
enum GlobalTempVarsType
{
	GLOBAL_TEMP_VAR_NONE = 0,
};

#endif