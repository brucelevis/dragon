#include "zCmdAnalysis.h"

#include "Fir.h"

#define _DEBUG_ANALYSIS

CmdAnalysis::CmdAnalysis(const char* name, const unsigned int& time_secs, const int& num_limit, const int& size_limit)
	: name_(name)
	, log_timer_(time_secs)
	, num_limit_(num_limit)
	, size_limit_(size_limit)
{
	switch_ = false;
	clear();
}

void CmdAnalysis::clear()
{ 
	datalist_.clear(); 
	bzero(&data_total_, sizeof(data_total_));
}

bool CmdAnalysis::add(const unsigned short& msgid, const int& size)
{
	// 开关控制
	if(!switch_) return true;

	Data& data = datalist_[msgid];
	data.num ++;
	data.size += size;
	data_total_.num ++;
	data_total_.size += size;

	if (!log_timer_(zRTime()))
		return true;

#ifdef _DEBUG_ANALYSIS
	showLog();
#endif
	// 限制检查
	bool is_right = checkLimit();

	clear();

	return is_right;
}

void CmdAnalysis::showLog() const
{
	Fir::logger->debug("%s: ", name_.c_str());
	for (auto iter = datalist_.begin(); iter != datalist_.end(); ++iter)
	{
		Fir::logger->debug("[cmdCount], msgid = %u, num = %d, size = %d", iter->first, iter->second.num, iter->second.size);
	}
	Fir::logger->debug("[cmdTotal], num_total = %d, size_total = %d", data_total_.num, data_total_.size);

}

bool CmdAnalysis::checkLimit() const
{
	if (num_limit_ > data_total_.num && size_limit_ > data_total_.size)
	{
		return true;
	}
	Fir::logger->warn("WARNING [cmdWarn], num_total = %d, num_limit = %d, size_total = %d, size_limit = %d", 
		data_total_.num, num_limit_, data_total_.size, size_limit_);
	return false;
}