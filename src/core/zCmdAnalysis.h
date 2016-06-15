/**
 * Copyright 2015 GDE Inc.
 *
 * \author huangxiaohui
 * \date 2015-12-18 10:57:15 CST
 * \brief 用于消息分析.
 *
 */

#ifndef CMD_ANALYSIS_H_
#define CMD_ANALYSIS_H_

#include <map>
#include <string>

#include "zTime.h"

/**
 * \brief 消息分析类
 */
class CmdAnalysis
{
	/**
	* \brief 消息数据
	*/
	struct Data
	{
		int num;
		int size;

		Data()
			: num(0)
			, size(0)
		{

		}
	};
	typedef std::map<unsigned short, Data> DataMap;
	
public:
	CmdAnalysis(const char* name, const unsigned int& time_secs, const int& num_limit, const int& size_limit);

	/**
	 * \brief 开启
	 */
	void on(){ switch_ = true; }
	/**
	 * \brief 关闭
	 */
	void off(){ switch_ = false; }

	/**
	 * \brief 添加消息
	 * \param msgid 消息ID
	 * \param size 消息大小
	 * \return 是否正常（true：是 false：否）
	 */
	bool add(const unsigned short& msgid, const int& size);

private:
	/**
	 * \brief 清理数据
	 */
	void clear();
	/**
	 * \brief 显示统计日志
	 */
	void showLog() const;
	/**
	 * \brief 判定是否通过限制检查
	 * \return 是/否
	 */
	bool checkLimit() const;

private:
	//开关
	bool switch_;

	// 名称
	std::string name_;
	// 日志间隔
	Timer log_timer_;
	// 数量限制
	int num_limit_;
	// 大小限制
	int size_limit_;

	// 消息数据列表
	DataMap datalist_;
	// 消息数据累计
	Data data_total_;
};

#endif
