#ifndef _Fir_h_
#define _Fir_h_

#include "zProperties.h"
#include "zLogger.h"
#include "zSingleton.h"

#define FIR_NEW new

namespace Fir
{
	/**
	 * \brief 游戏时间
	 *
	 */
	extern volatile QWORD qwGameTime;
	/**
	 * \brief 游戏启动时时
	 */
	extern volatile QWORD qwStartTime;

	struct stRedisItem
	{
		QWORD id;
		std::string ip;
		DWORD port;
		stRedisItem()
		{
			id = 0;
			port = 0;
		}
	};

	extern std::map<QWORD,stRedisItem> _mapRedisConfig;

	/**
	 * \brief 日志指针
	 *
	 */
	extern zLogger *logger;

	/**
	 * \brief 存取全局变量的容器
	 *
	 */
	extern zProperties global;

	/**
	 * \brief 存储一些全局调试信息
	 *
	 */
	extern DWORD debug;
	extern DWORD currPtr;
	extern DWORD size;
	extern DWORD wait_size;
	extern DWORD max_size;
	extern DWORD function_times; // 用来开启function_times功能

	extern void initGlobal();
	extern void finalGlobal();

};
using namespace Fir;




#endif
