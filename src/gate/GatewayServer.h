/**
 * \file
 * \version  $Id: GatewayServer.h 36 2013-04-07 11:42:48Z  $
 * \author  ,okyhc@263.sina.com
 * \date 2004年12月10日 10时55分53秒 CST
 * \brief zebra项目Gateway服务器,负责用户指令检查转发、加密解密等
 */

#ifndef _GatewayServer_h_
#define _GatewayServer_h_
#include <vector>
#include <string>

#include "zSubNetService.h"
#include "Fir.h"
#include "zMisc.h"
#include "SuperCommand.h"
#include "SessionClient.h"
#include "zMemDBPool.h"


/**
 * \brief 定义网关服务类
 *
 * 这个类使用了Singleton设计模式，保证了一个进程中只有一个类的实例
 *
 */
class GatewayService : public zSubNetService, public Singleton<GatewayService>
{
	friend class Singleton<GatewayService>;
	public:

		bool msgParse_SuperService(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);

		const int getPoolSize() const
		{
			if(taskPool)
			{
			return taskPool->getSize();
			}
			else
			{
				return 0;
			}
		}

		const int getPoolState() const
		{
			return taskPool->getState();
		}

		bool notifyLoginServer();

		/**
		 * \brief 虚析构函数
		 *
		 */
		~GatewayService()
		{
			if (sessionClient)
			{
				sessionClient->final();
				sessionClient->join();
				SAFE_DELETE(sessionClient);
			}
			
		}

		void reloadconfig();
		bool isSequeueTerminate() 
		{
			return taskPool == NULL;
		}

		const GameZone_t getZoneID() const
		{
			return zoneID;
		}

		bool printOnlineRoles();
		// 是否已达最大负载
		bool isReachFullLoad(); 
		/**
		 * \brief 获取网关最大容纳玩家数量
		 *
		 */
		DWORD getMaxRoleOfGate(){return maxRoleOfGate;}
	private:

		static zTCPTaskPool *taskPool;				/**< TCP连接池的指针 */


		//国家名称(地图)信息

		/**
		 * \brief 构造函数
		 *
		 */
		GatewayService() : zSubNetService("网关服务器", GATEWAYSERVER)
		{
			taskPool = NULL;
			maxRoleOfGate = 3000;
			bCompress = false;
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();

		/**
		 * \brief 确认服务器初始化成功，即将进入主回调函数
		 *
		 * 向服务器发送t_Startup_OK指令来确认服务器启动成功
		 * 并且通知所有登陆服务器，这台网关服务器准备好了
		 *
		 * \return 确认是否成功
		 */
		virtual bool validate()
		{
			zSubNetService::validate();

			return notifyLoginServer();
		}

	public:
		/**
		 * \brief 定义游戏区
		 *
		 */
		GameZone_t zoneID;

		DWORD maxRoleOfGate;	//网关最大负载
		int timeCmdLog;			// 消息性能检测日志时间
		bool bCompress;			// 底层数据传输是否支持压缩
};

#endif

