#include <iostream>
#include <string>

#include "Fir.h"
#include "zThread.h"
#include "TimeTick.h"
#include "SessionServer.h"
#include "SessionTaskManager.h"
#include "RecordClient.h"
#include "TimerMgr.h"
#include "SessionUserManager.h"

zRTime SessionTimeTick::currentTime;

SessionTimeTick::SessionTimeTick() : 
	zThread("TimeTick")
	,_30_sec(30)
{
}
void SessionTimeTick::run()
{
	while(!isFinal())
	{
		zThread::msleep(50);
		currentTime.now();

		recordClient->doCmd();

		SessionTaskManager::getMe().execEvery();

		TimerMgr::getMe().Update(currentTime.local_sec());

		if(_30_sec(currentTime))
		{
			SessionUserManager::getMe().update(currentTime);
			SessionService::getMe().sendOnlineCount();
		}
	}
}

