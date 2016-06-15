/**
 * Copyright 2015 GDE Inc.
 *
 * \author huangxiaohui
 * \date 2015-12-16 09:49:15 CST
 * \brief 禁言字符过滤.
 *
 */
#ifndef GATESERVER_WORLDFILTER_H_
#define GATESERVER_WORLDFILTER_H_

#include <set>

#include "zSingleton.h"

/**
 * \brief 禁言过虑类
 *
 */
class WordFilter : public Singleton<WordFilter>
{
	friend class Singleton<WordFilter>;
	// 字串列表
	typedef std::set<std::string> WordsSet;

public:
	WordFilter();
	/**
	 * \brief 初始化
	 */
	bool init();
	/**
	 * \brief 禁用字符过滤检查
	 * \param text 带过滤字符串
	 * \return 是否通过检查
	 */
	bool doFilter(const std::string text) const;

private:
	// 禁用列表
	WordsSet forbidWords;
};

#endif
