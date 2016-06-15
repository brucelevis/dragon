/**
 * \file
 * \version  $Id: TimeTick.cpp 13 2013-03-20 02:35:18Z  $
 * \author  ,@163.com
 * \date 2005年01月05日 21时03分02秒 CST
 * \brief 时间回调函数
 *
 * 
 */


#include <iostream>
#include <string>
#include <sys/timeb.h>

#include "Fir.h"
#include "zThread.h"
#include "TimeTick.h"
#include "SuperServer.h"
#include "zDBConnPool.h"
#include "SuperCommand.h"
#include "ServerManager.h"
#include "FLClient.h"
#include "FLClientManager.h"
#include "TimerMgr.h"
#include "StringUtil.h"

zRTime SuperTimeTick::currentTime;
SuperTimeTick *SuperTimeTick::instance = NULL;

void SuperTimeTick::run()
{
	readTime();

	const int timeout_value = 500;

	while(!isFinal())
	{
		zThread::msleep(50);

		currentTime.now();


		{
			ServerManager::getMe().execEvery();
		}

		if(one_sec(currentTime))
		{
			Fir::qwGameTime = qwStartGameTime + startTime.elapse(currentTime) / 1000;
			//Fir::logger->debug("%lu", Fir::qwGameTime);

// 			if (0 != Fir::qwGameTime
// 					&& 0 == Fir::qwGameTime % 600)
// 			{
// 				saveTime();
// 			}

			/*
			   FLClientManager::getMe().timeAction(ct);
			   RoleregClientManager::getMe().timeAction(ct);
			   RoleregCache::getMe().timeAction(ct);
			   InfoClientManager::getMe().timeAction(ct);
			// */
			TimerMgr::getMe().Update(currentTime.local_sec());

			zTime ct;
			FLClientManager::getMe().timeAction(ct);
			ServerManager::getMe().checkDeadLock(currentTime.sec());

			CMD::SUPER::t_GameTime tCmd;
			tCmd.qwGameTime = Fir::qwGameTime;
			tCmd.qwStartTime = qwStartGameTime;
			ServerManager::getMe().broadcast(&tCmd, sizeof(tCmd));
		}

		zRTime e;
		int t = currentTime.elapse(e);
		if (t > timeout_value)
		{
			Fir::logger->debug("[SUPRE TIMETICK超时],0,0,0,处理时间过长 %u",t);
		}
		
	}

	//saveTime();
}

bool SuperTimeTick::readTime()
{
	qwStartGameTime = 0;
	std::string strsql = StringUtil::Format("SELECT gametime FROM t_gametime LIMIT 1" );
	RecordSetPtr recordset = SuperService::dbConnPool->exeSelect(strsql);
	if (recordset == NULL)
	{
		Fir::logger->error("初始化游戏时间错误");
		return false;
	}
	
	if(recordset->empty())
	{
		currentTime.now();
		qwStartGameTime = currentTime.sec();
// 		Fir::logger->debug("qwStartGameTime :%d", (int)qwStartGameTime);
		std::string strsql = StringUtil::Format("INSERT INTO t_gametime (gametime) VALUES (%I64d)" ,qwStartGameTime );
		unsigned int retcode = SuperService::dbConnPool->exeInsert(strsql);
		if ((unsigned int)-1 == retcode)
		{
			Fir::logger->error("初始化游戏时间错误");
			return false;
		}
	}
	else
	{
		Record *rec = recordset->get(0);
		if (rec == NULL)
		{
			Fir::logger->error("初始化游戏时间错误2");
			return false;
		}
		qwStartGameTime = rec->get("gametime");
		
	}
	Fir::logger->debug("qwStartGameTime :%d", (int)qwStartGameTime);	
	Fir::logger->debug("读取游戏时间成功");
	Fir::qwGameTime = qwStartGameTime;
	startTime.now();

	return true;
}

