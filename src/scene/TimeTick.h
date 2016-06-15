/**
 * \file
 * \version  $Id: TimeTick.h 67 2013-04-23 09:44:20Z  $
 * \author  , 
 * \date 2013年04月05日 21时03分02秒 CST
 * \brief 时间回调线程
 *
 * 
 */


#ifndef _TimeTick_h_
#define _TimeTick_h_

#include <iostream>
#include <string>

#include "Fir.h"
#include "zTime.h"
#include "zThread.h"

class SceneTimeTick : public zThread, public  Singleton<SceneTimeTick>
{
	friend class Singleton<SceneTimeTick>;
	public:

		~SceneTimeTick() {};

		static WORD secCountNum;
		/// 当前时间
		static zRTime currentTime;

		void run();
	private:

		static SceneTimeTick *instance;

		SceneTimeTick();

		Timer _5_min;
		Timer _1_min;
		Timer _1_sec;
		Timer _3_sec;
};

class My_FunctionTime
{
	private:
		struct My_Times
		{
			My_Times():_times(0),_total_time(0) {}
			unsigned long _times;			//调用次数
			unsigned long _total_time;		//调用时间
		};
		std::map<std::string, My_Times> _times; 
		Timer _log_timer;
	public:
		My_FunctionTime(const int how_long) : _log_timer(how_long) {}
		~My_FunctionTime() {}
		void inc(const std::string &func, const unsigned long total)
		{
			My_Times &mt = _times[func];
			if (mt._times)
			{
				++mt._times;
				mt._total_time += total;
			}
			else
			{
				++mt._times;
				mt._total_time = total;
			}
		}
		void reset(const zRTime &ct, const bool force_print)
		{
			if (force_print || _log_timer(ct))
			{
				Fir::logger->debug("[分时统计]：%s, %lu", force_print ? "force" : "timer", _times.size());
				for(std::map<std::string, My_Times>::iterator it = _times.begin(); it != _times.end(); ++it)
				{
					if (it->second._times)
					{
						Fir::logger->debug("[分时统计]：%s, %lu毫秒, %lu次, %lu毫秒/次",
								it->first.c_str(),
								it->second._total_time / 1000000L, it->second._times, (it->second._total_time / 1000000L) / it->second._times);
						//it->second._times = 0;
					}
				}
			}/*
			else
			{
				for(std::map<std::string, My_Times>::iterator it = _times.begin(); it != _times.end(); ++it)
				{
					it->second._times = 0;
				}
			}*/
			_times.clear();
		}
};

class My_FunctionTime_wrapper
{
	private:
		const std::string _func;
		struct timespec _tv_1;
	public:
		static My_FunctionTime my_func;
		My_FunctionTime_wrapper(const std::string &func) : _func(func)
		{
			clock_gettime(CLOCK_REALTIME, &_tv_1);
		}
		~My_FunctionTime_wrapper()
		{
			struct timespec _tv_2;
			clock_gettime(CLOCK_REALTIME, &_tv_2);

			unsigned long end=(unsigned long)_tv_2.tv_sec*1000000000L + _tv_2.tv_nsec;
			unsigned long begin=(unsigned long)_tv_1.tv_sec*1000000000L + _tv_1.tv_nsec;

			//if (Fir::global["openstat"] == "yes" || strstr(_func.c_str(), "RU"))
			my_func.inc(_func, end-begin);
		}
};

#define My_FunctionTime_wrapperMT(func)  My_FunctionTime_wrapper func_time(func)
#define My_FunctionTime_wrapperM(func)
#define __MY_FUNCTIONTIME_WRAPPER__

#define _COMMAND_TIME 1

#endif

