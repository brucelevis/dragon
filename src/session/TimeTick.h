/**
 * \file
 * \version  $Id: TimeTick.h 29 2013-04-07 01:39:43Z  $
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
#include "zTime.h"
#include "zThread.h"

class SessionTimeTick : public zThread, public Singleton<SessionTimeTick>
{
	friend class Singleton<SessionTimeTick>;
	public:

		~SessionTimeTick() {};
		
		void run();

		// 当前时间
		static zRTime currentTime;
	private:
		SessionTimeTick();

		Timer _30_sec;
};

#define _COMMAND_TIME 1

#endif

