#include <iostream>
#include <string>

#include "Fir.h"
#include "zThread.h"
#include "zTime.h"
#include "TimeTick.h"
#include "GatewayServer.h"
#include "GatewayTaskManager.h"
#include "SessionClient.h"
#include "RecordClient.h"
#include "SceneClientManager.h"

zRTime GatewayTimeTick::currentTime;


/**
 * \brief 线程主函数
 *
 */
void GatewayTimeTick::run()
{
	while(!isFinal())
	{
		zThread::msleep(10);
		currentTime.now();

		{
			//My_FunctionTime_wrapperMT("RU"_S(__LINE__));
			sessionClient->doCmd();
		}

		{
			//My_FunctionTime_wrapperMT("RU"_S(__LINE__));
			recordClient->doCmd();
		}

		{
			//My_FunctionTime_wrapperMT("RU"_S(__LINE__));
			GatewayTaskManager::getMe().execEvery();
		}

		{
			//My_FunctionTime_wrapperMT("RU"_S(__LINE__));
			SceneClientManager::getMe().execEvery();
		}

		if(_1_sec(currentTime))
		{
			GatewayService::getMe().printOnlineRoles();
		}
	}
}

