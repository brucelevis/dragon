/**
 * \file
 * \version  $Id: zTime.h 13 2013-03-20 02:35:18Z  $
 * \author  ,okyhc@263.sina.com
 * \date 2004年11月23日 16时42分46秒 CST
 * \brief 时间定义
 *
 * 
 */

#ifndef _ZTIME_H_
#define _ZTIME_H_

#include <time.h>
#include <sys/time.h>
#include <sstream>
#include <map>
#include <assert.h>

#include "zType.h"
#include "zMutex.h"

#define DAY_SECS (24 * 60 * 60)

/**
 * \brief 真实时间类,对timespec结构简单封装,提供一些常用时间函数
 * 时间精度精确到毫秒，
 * 关于timespec请man clock_gettime
 */
class zRTime
{

	private:

		/**
		 * \brief 真实时间换算为毫秒
		 *
		 */
		unsigned long long _msecs;

		/**
		 * \brief 得到当前真实时间
		 *
		 * \return 真实时间，单位毫秒
		 */
		unsigned long long _now()
		{
			unsigned long long retval = 0LL;
			struct timespec tv;
			clock_gettime(CLOCK_REALTIME, &tv);
			retval = tv.tv_sec;
			retval *= 1000LL;
			retval += (tv.tv_nsec / 1000000L);
			return retval;
		}

		/**
		 * \brief 得到当前真实时间延迟后的时间
		 * \param delay 延迟，可以为负数，单位毫秒
		 */
		void nowByDelay(int delay)
		{
			_msecs = _now();
			addDelay(delay);
		}

	public:

		/**
		 * \brief 构造函数
		 *
		 * \param delay 相对于现在时间的延时，单位毫秒
		 */
		zRTime(const int delay = 0)
		{
			nowByDelay(delay);
		}

		/**
		 * \brief 拷贝构造函数
		 *
		 * \param rt 拷贝的引用
		 */
		zRTime(const zRTime &rt)
		{
			_msecs = rt._msecs;
		}

		/**
		 * \brief 获取当前时间（UTC时间）
		 *
		 */
		void now()
		{
			_msecs = _now();
		}

		/**
		 * \brief 返回秒数（UTC时间）
		 *
		 * \return 秒数
		 */
		unsigned long sec() const
		{
			return _msecs / 1000L;
		}

		/**
		 * \brief 返回秒数（当前时区时间）
		 *
		 * \return 秒数
		 */
		unsigned long local_sec()
		{
			return local_msecs() / 1000L;
		}
		/**
		 * \brief 返回每日（0点）起始时间（utc）
		 *
		 * \return 秒数
		 */
		unsigned long daysec()
		{
			return local_msecs() / 1000L /(DAY_SECS)*(DAY_SECS) + zRTime::my_timezone;
		}
		/**
		 * \brief 返回毫秒数（UTC时间）
		 *
		 * \return 毫秒数
		 */
		unsigned long msec() const
		{
			return _msecs % 1000L;
		}

		/**
		 * \brief 返回总共的毫秒数（UTC时间）
		 *
		 * \return 总共的毫秒数
		 */
		unsigned long long msecs() const
		{
			return _msecs;
		}

		/**
		 * \brief 返回总共的毫秒数（当前时区时间）
		 *
		 * \return 总共的毫秒数
		 */
		unsigned long long local_msecs() const
		{
			return msecs() - zRTime::my_timezone*1000L;
		}

		/**
		 * \brief 设置总共的毫秒数（UTC时间）
		 *
		 * \return 总共的毫秒数
		 */
		void setmsecs(unsigned long long data)
		{
			_msecs = data;
		}
		/**
		 * \brief 加延迟偏移量
		 *
		 * \param delay 延迟，可以为负数，单位毫秒
		 */
		void addDelay(int delay)
		{
			_msecs += delay;
		}

		/**
		 * \brief 重载=运算符号
		 *
		 * \param rt 拷贝的引用
		 * \return 自身引用
		 */
		zRTime & operator= (const zRTime &rt)
		{
			_msecs = rt._msecs;
			return *this;
		}

		/**
		 * \brief 重构+操作符
		 *
		 */
		const zRTime & operator+ (const zRTime &rt)
		{
			_msecs += rt._msecs;
			return *this;
		}

		/**
		 * \brief 重构-操作符
		 *
		 */
		const zRTime & operator- (const zRTime &rt)
		{
			_msecs -= rt._msecs;
			return *this;
		}

		/**
		 * \brief 重构>操作符，比较zRTime结构大小
		 *
		 */
		bool operator > (const zRTime &rt) const
		{
			return _msecs > rt._msecs;
		}

		/**
		 * \brief 重构>=操作符，比较zRTime结构大小
		 *
		 */
		bool operator >= (const zRTime &rt) const
		{
			return _msecs >= rt._msecs;
		}

		/**
		 * \brief 重构<操作符，比较zRTime结构大小
		 *
		 */
		bool operator < (const zRTime &rt) const
		{
			return _msecs < rt._msecs;
		}

		/**
		 * \brief 重构<=操作符，比较zRTime结构大小
		 *
		 */
		bool operator <= (const zRTime &rt) const
		{
			return _msecs <= rt._msecs;
		}

		/**
		 * \brief 重构==操作符，比较zRTime结构是否相等
		 *
		 */
		bool operator == (const zRTime &rt) const
		{
			return _msecs == rt._msecs;
		}

		/**
		 * \brief 计时器消逝的时间，单位毫秒
		 * \param rt 当前时间
		 * \return 计时器消逝的时间，单位毫秒
		 */
		unsigned long long elapse(const zRTime &rt) const
		{
			if (rt._msecs > _msecs)
				return (rt._msecs - _msecs);
			else
				return 0LL;
		}

		static long my_timezone;
		static const std::string & getLocalTZ();
		static void getLocalTime(struct tm & tv, time_t timValue)
		{
			timValue -= my_timezone;
			gmtime_r(&timValue, &tv);
		}
		static void getTimeLocal(struct tm tv, time_t & timValue)
		{
			timValue = timegm(&tv);
			timValue += my_timezone;
		}

		/**
		 * 保留的时区信息
		 * 程序初始化的时候设置，运行过程中一般不变
		 */
		static void save_timezone(std::string &tzstr);
		/**
		* \brief 恢复时区信息
		* \param tzstr 缓冲字符串
		*/
		static void restore_timezone(const std::string &tzstr);
};

/**
 * \brief 时间类,对struct tm结构简单封装
 */
class zTime
{

	public:

		/**
		 * \brief 构造函数
		 */
		zTime()
		{
			time(&secs);
			zRTime::getLocalTime(tv, secs);
		}
		
		/**
		 * \brief 拷贝构造函数
		 */
		zTime(const zTime &ct)
		{
			secs = ct.secs;
			zRTime::getLocalTime(tv, secs);
		}

		/**
		 *\brief 根据指定时间初始化
		 *
		 *\param _secs UTC时间
		 */
		zTime(time_t _secs)
		{
			secs = _secs;
			zRTime::getLocalTime(tv, secs);
		}

		/**
		 *\brief 根据格式化时间初始化
		 *
		 *\param str 格式化时间
		 *\param fmt 时间格式
		 */
		zTime(const char* str, const char* fmt = "%Y-%m-%d %H:%M:%S")
		{
			if (strptime(str, fmt, &tv) == NULL)
				return;
			zRTime::getTimeLocal(tv, secs);
		}

		/**
		 * \brief 获取当前时间
		 */
		void now()
		{
			time(&secs);
			zRTime::getLocalTime(tv, secs);
		}

		/**
		 * \brief 返回存储的时间（UTC时间）
		 * \return 时间，秒
		 */
		time_t sec() const
		{
			return secs;
		}

		/**
		 * \brief 返回存储的时间（当前时区时间）
		 * \return 时间，秒
		 */
		time_t local_sec() const
		{
			return secs - zRTime::my_timezone;
		}

		/**
		 * \brief 重载=运算符号
		 * \param rt 拷贝的引用
		 * \return 自身引用
		 */
		zTime & operator= (const zTime &rt)
		{
			secs = rt.secs;
			zRTime::getLocalTime(tv, secs);
			return *this;
		}

		/**
		 * \brief 重构+操作符
		 */
		const zTime & operator+ (const zTime &rt)
		{
			secs += rt.secs;
			return *this;
		}

		/**
		 * \brief 重构-操作符
		 */
		const zTime & operator- (const zTime &rt)
		{
			secs -= rt.secs;
			return *this;
		}

		/**
		 * \brief 重构-操作符
		 */
		const zTime & operator-= (const time_t s)
		{
			secs -= s;
			return *this;
		}

		/**
		 * \brief 重构>操作符，比较zTime结构大小
		 */
		bool operator > (const zTime &rt) const
		{
			return secs > rt.secs;
		}

		/**
		 * \brief 重构>=操作符，比较zTime结构大小
		 */
		bool operator >= (const zTime &rt) const
		{
			return secs >= rt.secs;
		}

		/**
		 * \brief 重构<操作符，比较zTime结构大小
		 */
		bool operator < (const zTime &rt) const
		{
			return secs < rt.secs;
		}

		/**
		 * \brief 重构<=操作符，比较zTime结构大小
		 */
		bool operator <= (const zTime &rt) const
		{
			return secs <= rt.secs;
		}

		/**
		 * \brief 重构==操作符，比较zTime结构是否相等
		 */
		bool operator == (const zTime &rt) const
		{
			return secs == rt.secs;
		}

		/**
		 * \brief 计时器消逝的时间，单位秒
		 * \param rt 当前时间
		 * \return 计时器消逝的时间，单位秒
		 */
		time_t elapse(const zTime &rt) const
		{
			if (rt.secs > secs)
				return (rt.secs - secs);
			else
				return 0;
		}

		/**
		 * \brief 计时器消逝的时间，单位秒
		 * \return 计时器消逝的时间，单位秒
		 */
		time_t elapse() const
		{
			zTime rt;
			return (rt.secs - secs);
		}

		/**
		 * \brief 得到当前秒，范围0-59点
		 *
		 * \return 
		 */
		int getSec()
		{
			return tv.tm_sec;
		}
	
		/**
		 * \brief 得到当前分钟，范围0-59点
		 *
		 * \return 
		 */
		int getMin()
		{
			return tv.tm_min;
		}
		
		/**
		 * \brief 得到当前小时，范围0-23点
		 *
		 * \return 
		 */
		int getHour()
		{
			return tv.tm_hour;
		}
		
		/**
		 * \brief 得到天数，范围1-31
		 *
		 * \return 
		 */
		int getMDay()
		{
			return tv.tm_mday;
		}

		/**
		 * \brief 得到当前星期几，从星期天开始，范围0-6
		 *
		 * \return 
		 */
		int getWDay()
		{
			return tv.tm_wday;
		}

		int getYDay()
		{
			return tv.tm_yday;
		}

		/**
		 * \brief 得到当前月份，范围1-12
		 *
		 * \return 
		 */
		int getMonth()
		{
			return tv.tm_mon+1;
		}
		
		/**
		 * \brief 得到当前年份
		 *
		 * \return 
		 */
		int getYear()
		{
			return tv.tm_year+1900;
		}	

		/**
		 * \brief 格式化时间
		 *
		 * \param fmt 时间格式
		 *
		 * \return 格式化后的时间字符串
		 */
		std::string toString(const char* fmt = "%Y-%m-%d %H:%M:%S")
		{
			char buf[32]={0};
			strftime(buf, sizeof(buf), fmt, &tv);
			
			return buf;
		}

	private:

		/**
		 * \brief 存储时间，单位秒
		 */
		time_t secs;
		
		/**
		 * \brief tm结构，方便访问
		 */
		struct tm tv;


};

/**
 * \brief 计时器，延时触发
 */
class Timer
{

	public:

		/**
		 * \brief 构造函数
		 *
		 * \param how_long 延时触发的时间，单位秒
		 * \param delay 相对于现在时间的延时，单位秒
		 */
		Timer(const unsigned int how_long, const int delay=0) : _long(how_long*1000), _timer(delay*1000)
		{

		}

		/**
		 * \brief 构造函数
		 *
		 * \param how_long 延时触发的时间，单位秒
		 * \param current 现在时间
		 */
		Timer(const unsigned int how_long, const zRTime &current) : _long(how_long*1000), _timer(current)
		{
			_timer.addDelay(_long);
		}

		/**
		 * \brief 检测是否到时，到时则重新开始
		 *
		 * \param current 现在时间
		 * \return 
		 */
		bool operator() (const zRTime& current)
		{
			if (_timer <= current) {
				_timer = current;
				_timer.addDelay(_long);
				return true;
			}

			return false;
		}

	private:

		/**
		 * \brief 延迟时间，单位毫秒
		 */
		unsigned int _long;

		/**
		 * \brief 计时时间
		 */
		zRTime _timer;
};

/**
 * \brief 定时器，定时触发
 */
class zClocker
{

	public:

		/**
		 * \brief 构造函数
		 *
		 * \param how_long 定时时间，单位秒
		 * \param interval 循环周期时间，单位秒
		 */
		zClocker(const unsigned int how_long, const unsigned int interval) : _long(how_long*1000),_interval(interval*1000),_timer()
		{
			assert(_long <= _interval);
			_timer.addDelay(getNextDelay());
		}

		/**
		 * \brief 检测是否到时，到时则重新开始
		 *
		 * \param current 现在时间
		 * \return 
		 */
		bool operator() (const zRTime& current)
		{
			if (_timer <= current) {
				_timer = current;
				_timer.addDelay(getNextDelay());
				return true;
			}

			return false;
		}

		/**
		 * \brief 还有多长时间闹钟(单位:s)
		 *
		 * \param current 现在时间
		 * \return 
		 */
		unsigned int remainderTime(const zRTime& current)
		{
			return (current.elapse(_timer)/1000L);
		}

	private:

		/**
		 * \brief 下次时间偏移，单位毫秒
		 *
		 * \return 
		 */
		unsigned long getNextDelay()
		{
			unsigned long lastmsec = _timer.local_msecs() % _interval;
			if (lastmsec < _long)
			{
				return _long - lastmsec;
			}
			else
			{
				return _interval - lastmsec + _long;
			}
		}

		/**
		 * \brief 定时时间，单位毫秒
		 */
		unsigned long _long;

		/**
		 * \brief 周期时间，单位毫秒
		 */
		unsigned long _interval;

		/**
		 * \brief 计时时间
		 */
		zRTime _timer;
};

/**
 * \brief 随机计时器，延时触发
 */
class RandTimer
{
	public:
		RandTimer(const float how_long, const int delay=0) : _long((int)(how_long*1000)), _timer(delay*1000)
		{

		}

		RandTimer(const float how_long, const zRTime &cur);
		void next(const zRTime &cur);
		bool operator() (const zRTime& current);
	private:
		int _long;
		zRTime _timer;
};

/**
 * \brief 时间工具
 */
class zTimeTool
{

	public:
	
		/**
		 * \brief 是否同年
		 *
		 * \param t1 时间1（UTC时间）
		 * \param t2 时间2（UTC时间）
		 */
		static bool isSameYear(const time_t& t1, const time_t& t2);

		/**
		 * \brief 是否同月
		 *
		 * \param t1 时间1（UTC时间）
		 * \param t2 时间2（UTC时间）
		 */
		static bool isSameMonth(const time_t& t1, const time_t& t2);
		
		/**
		 * \brief 是否同周
		 *
		 * \param t1 时间1（UTC时间）
		 * \param t2 时间2（UTC时间）
		 */
		static bool isSameWeek(const time_t& t1, const time_t& t2);

		/**
		 * \brief 是否同天
		 *
		 * \param t1 时间1（UTC时间）
		 * \param t2 时间2（UTC时间）
		 */
		static bool isSameDay(const time_t& t1, const time_t& t2);

		/**
		 * \brief 获取一天起始时间
		 *
		 * \param t1 时间1（UTC时间）
		 * \param t2 时间2（UTC时间）
		 *
		 * \return 返回一天起始时间（UTC时间，单位秒）
		 */
		static time_t getDayStartTime(const time_t& t);

		/**
		 * \brief 获取一周起始时间（星期一）
		 *
		 * \param t1 时间1（UTC时间）
		 * \param t2 时间2（UTC时间）
		 *
		 * \return 获取一周起始时间（星期一）（UTC时间，单位秒）
		 */
		static time_t getWeekStartTime(const time_t& t);
};

struct FunctionInterval
{
	struct timespec _tv_1;
	struct timespec _tv_2;
	const unsigned long _need_log;
	const char *_fun_name;
	FunctionInterval(const unsigned long interval):_need_log(interval)
	{
		_tv_1.tv_sec=-1;
		_tv_1.tv_nsec=-1;
	}
	void interval(const char *func=NULL);
};
struct FunctionTime
{
	private:
	struct timespec _tv_1;
	struct timespec _tv_2;
	const unsigned long _need_log;
	const char *_fun_name;
	const char *_dis;
	const int _dis_len;
	public:
	FunctionTime(const unsigned long interval,const char *func=NULL,const char *dis=NULL,const int dis_len=16):_need_log(interval),_fun_name(func),_dis(dis),_dis_len(dis_len)
	{
		clock_gettime(CLOCK_REALTIME, &_tv_1);
	}
	~FunctionTime();
};

struct CommandTime
{
private:
	struct timespec _tv_1;
	struct timespec _tv_2;
	const unsigned long _need_log;
	const char *_where;
	const BYTE _cmd;
	const BYTE _para;
public:
	CommandTime(const unsigned long interval, const char *where, BYTE cmd, BYTE para) 
		: _need_log(interval), _where(where), _cmd(cmd), _para(para)
	{
		clock_gettime(CLOCK_REALTIME, &_tv_1);
	}
	~CommandTime();
};

struct BlockTime
{
private:
	struct timespec _tv_1;
	struct timespec _tv_2;
	const unsigned long _interval;
	const char* _where;
public:
	BlockTime(const unsigned long interval, const char *where)
		: _interval(interval), _where(where)
	{
		clock_gettime(CLOCK_REALTIME, &_tv_1);
	}
	~BlockTime() {}
	void elapse();
};

#define TIMES_HASH 1024

struct FunctionTimes
{
	struct Times
	{
		//Times();
		Times():_log_timer(600),_times(0),_total_time(0)
		{
			bzero(_dis,sizeof(_dis));
		}
		Timer _log_timer;
		char _dis[TIMES_HASH];
		unsigned long _times;
		unsigned long _total_time;
		zMutex _mutex;
	};
	private:
	static Times _times[TIMES_HASH]; 
	int _which;
	struct timespec _tv_1;
	struct timespec _tv_2;
	public:
	FunctionTimes(const int which , const char *dis);
	~FunctionTimes();
};

//直接从数据库操作函数中计算执行时间
struct FunctionTimesV2
{
	struct Times
	{
		Times():_log_timer(600),_times(0),_total_time(0)
		{
			bzero(_sqlOP, sizeof(_sqlOP));
		}
		Timer _log_timer;
		QWORD _times;
		QWORD _total_time;
		char _sqlOP[16];
		zMutex _mutex;
	};
	private:
	static std::map<QWORD, Times*> _times;
	static bool clearMapFlag;
	QWORD _addr;
	struct timespec _tv_1;
	struct timespec _tv_2;
	public:
	FunctionTimesV2(const char *sqlFunc, const QWORD &addr);
	~FunctionTimesV2();
};
/*
struct CmdAnalysis
{
	CmdAnalysis(const char *disc,DWORD time_secs):_log_timer(time_secs)
	{
		bzero(_disc,sizeof(disc));
		strncpy(_disc,disc,sizeof(_disc)-1);
		bzero(_data,sizeof(_data));
		_switch=false;
	}
	struct
	{
		DWORD num;
		DWORD size;
	}_data[256][256] ;
	zMutex _mutex;
	Timer _log_timer;
	char _disc[256];
	bool _switch;//开关
	void add(const BYTE &cmd, const BYTE &para , const DWORD &size)
	{
		if(!_switch)
		{
			return ;
		}
		_mutex.lock(); 
		_data[cmd][para].num++;
		_data[cmd][para].size +=size;
		zRTime ct;
		if(_log_timer(ct))
		{
			for(int i = 0 ; i < 256 ; i ++)
			{
				for(int j = 0 ; j < 256 ; j ++)
				{
					if(_data[i][j].num)
						Fir::logger->debug("%s:%d,%d,%d,%d",_disc,i,j,_data[i][j].num,_data[i][j].size);
				}
			}
			bzero(_data,sizeof(_data));
		}
		_mutex.unlock(); 
	}
};
// */
/*
struct FunctionTimes
{
	private:
		int _times;
		Timer _timer;
	public:
		FunctionTimes():_times(0),_timer(60)
		{
		}
		void operator();
		
};
// */
#endif
