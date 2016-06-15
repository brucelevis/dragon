/**
 * \file
 * \version  $Id: TimeTick.h 24 2013-03-30 08:04:25Z  $
 * \author  ,@163.com
 * \date 2005年01月05日 21时03分02秒 CST
 * \brief 时间回调函数
 *
 * 
 */


#ifndef _TimeTick_h_
#define _TimeTick_h_

#include <iostream>
#include <string>

#include "Fir.h"
#include "zThread.h"
#include "zTime.h"

/**
 * \brief 网关定时器线程
 *
 */
class GatewayTimeTick : public zThread, public Singleton<GatewayTimeTick>
{
	friend class Singleton<GatewayTimeTick>;
	public:

		static zRTime currentTime;

		~GatewayTimeTick() {};

		void run();

	private:

		GatewayTimeTick() : zThread("TimeTick"), _1_sec(1) {};

		Timer _1_sec;

};

#define _COMMAND_TIME 1

#endif

