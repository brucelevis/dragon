/**
 * \file
 * \version  $Id: GatewayServer.cpp 64 2013-04-23 02:05:08Z  $
 * \author  ,okyhc@263.sina.com
 * \date 2004年12月10日 10时55分53秒 CST
 * \brief zebra项目Gateway服务器,负责用户指令检查转发、加密解密等
 */

#include "zSubNetService.h"
#include "Fir.h"
#include "zMisc.h"
#include "GatewayServer.h"
#include "GatewayTask.h"
#include "SceneClient.h"
#include "RecordClient.h"
#include "GateUserManager.h"
#include "zConfile.h"
#include "GatewayTaskManager.h"
#include "TimeTick.h"
#include "RecordClient.h"
#include "zXMLParser.h"
#include "SceneClientManager.h"
#include <time.h>
#include "StringUtil.h"
#include "gFLCmdHandler.h"
#include "WordFilter.h"

zTCPTaskPool * GatewayService::taskPool = NULL;

/**
 * \brief 初始化网络服务器程序
 *
 * 实现了虚函数<code>zService::init</code>
 *
 * \return 是否成功
 */
bool GatewayService::init()
{
	//初始化连接线程池
	int state = state_none;
	Fir::to_lower(Fir::global["initThreadPoolState"]);
	if ("repair" == Fir::global["initThreadPoolState"]
			|| "maintain" == Fir::global["initThreadPoolState"])
		state = state_maintain;

	taskPool = new zTCPTaskPool(atoi(Fir::global["gateway_maxconns"].c_str()), state, 50000);
	if (NULL == taskPool
			|| !taskPool->init())
		return false;

	strncpy(pstrIP, zSocket::getIPByIfName(Fir::global["ifname"].c_str()), MAX_IP_LENGTH - 1);
	//Fir::logger->debug("%s", pstrIP);

	if (!zSubNetService::init())
	{
		return false;
	}

	const CMD::SUPER::ServerEntry *serverEntry = NULL;

	//连接Session服务器
	serverEntry = getServerEntryByType(SESSIONSERVER);
	if (NULL == serverEntry)
	{
		Fir::logger->error("不能找到Session服务器相关信息，不能连接Session服务器");
		return false;
	}
	sessionClient = new SessionClient("Session服务器", serverEntry->pstrExtIP, serverEntry->wdExtPort);
	if (NULL == sessionClient)
	{
		Fir::logger->error("没有足够内存，不能建立Session服务器客户端实例");
		return false;
	}
	if (!sessionClient->connectToSessionServer())
	{
		Fir::logger->error("连接Session服务器失败 %s", __PRETTY_FUNCTION__);
		return false;
	}
	sessionClient->start();
	
	//连接所有的场景服务器
	serverEntry = getServerEntryByType(SCENESSERVER);
	if(serverEntry)
	{
		if (!SceneClientManager::getMe().init())
			return false;
	}

	//连接所有的档案服务器
	serverEntry = getServerEntryByType(RECORDSERVER);
	if (NULL == serverEntry)
	{
		Fir::logger->error("不能找到Record服务器相关信息，不能连接Record服务器");
		return false;
	}

	recordClient = new RecordClient("Record服务器", serverEntry->pstrExtIP, serverEntry->wdExtPort);
	if (NULL == recordClient)
	{
		Fir::logger->error("没有足够内存，不能建立Record服务器客户端实例");
		return false;
	}
	if (!recordClient->connectToRecordServer())
	{
		Fir::logger->error("连接Record服务器失败 %s", __PRETTY_FUNCTION__);
		return false;
	}
	recordClient->start();

	if (!zMemDBPool::getMe().init() || (NULL == zMemDBPool::getMe().getMemDBHandle()))
	{
		Fir::logger->error("连接内存数据库失败");
		return false;
	}

	maxRoleOfGate = atoi(Fir::global["gateway_max_role"].c_str());

	// 消息性能检测
	GatewayService::getMe().timeCmdLog = atoi(Fir::global["timecmdlog"].c_str());

	gFLCmdHandler::getMe().Initialize();
	WordFilter::getMe().init();
	GatewayTimeTick::getMe().start();

	return true;
}

/**
 * \brief 新建立一个连接任务
 *
 * 实现纯虚函数<code>zNetService::newTCPTask</code>
 *
 * \param sock TCP/IP连接
 * \param addr 地址
 */
void GatewayService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	//Fir::logger->debug(__PRETTY_FUNCTION__);
	GatewayTask *tcpTask = new GatewayTask(taskPool, sock, addr, bCompress);
	if (NULL == tcpTask)
		//内存不足，直接关闭连接
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//得到了一个正确连接，添加到验证队列中
		SAFE_DELETE(tcpTask);
	}
}

bool GatewayService::notifyLoginServer()
{
	using namespace CMD::SUPER;
	t_GYList_Gateway tCmd;

	tCmd.wdServerID = wdServerID;
	bcopy(pstrExtIP, tCmd.pstrIP, sizeof(tCmd.pstrIP));
	tCmd.wdPort = wdExtPort;
	if(!GatewayService::getMe().isTerminate())
	{
		tCmd.wdNumOnline = GateUserManager::getMe().getUserCount();

		//Fir::logger->debug("[GS],0,0,0,网关目前在线人数:%d, %d",tCmd.wdNumOnline, getPoolSize());
	}
	else
	{
		tCmd.wdNumOnline = 0;
	}

	
	tCmd.state = getPoolState();
	//tCmd.zoneGameVersion = verify_client_version;
	tCmd.wdNetType = wdNetType;

	return sendCmdToSuperServer(&tCmd, sizeof(tCmd));
}

bool GatewayService::printOnlineRoles()
{
	DWORD dwAccountCount = GateUserManager::getMe().getUserCount();
	DWORD dwRoleCount = GateUserManager::getMe().getRoleCount();

	Fir::logger->debug("[GS],0,0,0,网关目前在线人数:%d, %d, %d",dwAccountCount, dwRoleCount, getPoolSize());

	return true;
}

// 是否已达最大负载
bool GatewayService::isReachFullLoad()
{
	DWORD dwUserCount = GateUserManager::getMe().getUserCount();
	if (dwUserCount >= getMaxRoleOfGate())
	{
		Fir::logger->debug("[最大负载]dwUserCount=%u,max_load=%u", dwUserCount, getMaxRoleOfGate());
		return true;
	}
	return false;	
}

/**
 * \brief 解析来自服务器管理器的指令
 *
 * 这些指令是网关和服务器管理器交互的指令<br>
 * 实现了虚函数<code>zSubNetService::msgParse_SuperService</code>
 *
 * \param ptNullCmd 待解析的指令
 * \param nCmdLen 待解析的指令长度
 * \return 解析是否成功
 */
bool GatewayService::msgParse_SuperService(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::SUPER;

	if (CMD_GATEWAY == ptNullCmd->cmd)
	{
		switch(ptNullCmd->para)
		{
			case PARA_GATEWAY_RQGYLIST:
				{
					//t_GYList_Gateway *ptCmd = (t_GYList_Gateway *)ptNullCmd;

					return notifyLoginServer();
				}
				break;
			case PARA_GATE_RECONNECT_SCENE:
				{
#ifdef _PQQ_DEBUG
					Fir::logger->debug("[重连场景] 收到Super消息，重连场景");
#endif
					t_GateReconnectScene* rev = (t_GateReconnectScene*)ptNullCmd;
					return SceneClientManager::getMe().reConnectScene(&rev->entry);	
				}
				break;
			case PARA_GATE_TERMINATE_CONNECT:
				{
#ifdef _PQQ_DEBUG
					Fir::logger->debug("[重连场景] 收到Super消息，主动断开与场景连接");
#endif
					t_GateTerminateConnect* rev = (t_GateTerminateConnect*)ptNullCmd;
					SceneClientManager::getMe().setTaskReconnect(rev->pstrIP,rev->port, false);
					return true;
				}
				break;
			case PARA_ZONE_ID_GATE:
				{
					CMD::SUPER::t_ZoneID_Gate *rev = (CMD::SUPER::t_ZoneID_Gate *)ptNullCmd;					
					GatewayService::getMe().zoneID = rev->zone;					
					Fir::logger->debug("收到区编号 %u", GatewayService::getMe().zoneID);
				}
				break;
			case CMD::SUPER::PARA_SUPER2GATE_FROM_FL:
				{
					CMD::SUPER::t_Super2Gate_FromFL* rev = (CMD::SUPER::t_Super2Gate_FromFL*)ptNullCmd;
					std::string content((const char*)rev->data,rev->datasize);
					return gFLCmdHandler::getMe().handleFLCmd(content);
				}
				break;
		}
	}

	Fir::logger->info("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
 * \brief 结束网络服务器
 *
 * 实现了纯虚函数<code>zService::final</code>
 *
 */
void GatewayService::final()
{

	GatewayTimeTick::getMe().final();
	GatewayTimeTick::getMe().join();
	GatewayTaskManager::getMe().unregEveryByClose();

	if(taskPool)
	{
		taskPool->final();
		SAFE_DELETE(taskPool);
	}
	// */
	if(sessionClient)
	{
		sessionClient->final();
		sessionClient->join();
		SAFE_DELETE(sessionClient);
	}
	if(recordClient)
	{
		recordClient->final();
		recordClient->join();
		SAFE_DELETE(recordClient);
	}

	zSubNetService::final();

	SceneClientManager::getMe().final();

	Fir::logger->debug(__PRETTY_FUNCTION__);
}

/**
 * \brief 命令行参数
 *
 */
static struct argp_option gateway_options[] =
{
	{"config",	'c',		"config",	0,	"Config file path",								0},
	{"daemon",		'd',	0,			0,	"Run service as daemon",						0},
	{"log",			'l',	"level",	0,	"Log level",									0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",								0},
	{"mysql",		'y',	"mysql",	0,	"MySQL[mysql://user:passwd@host:port/dbName]",	0},
	{"ifname",		'i',	"ifname",	0,	"Local network device",							0},
	{"server",		's',	"ip",		0,	"Super server ip address",						0},
	{"port",		'p',	"port",		0,	"Super server port number",						0},
	{"maintain",	'm',	0,			0,	"Run service as maintain mode",					0},
	{0,				0,		0,			0,	0,												0}
};

/**
 * \brief 命令行参数解析器
 *
 * \param key 参数缩写
 * \param arg 参数值
 * \param state 参数状态
 * \return 返回错误代码
 */
static error_t gateway_parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		case 'c':
			{
				Fir::global["configfile"] = arg;
			}
			break;
		case 'd':
			{
				Fir::global["daemon"] = "true";
			}
			break;
		case 'm':
			{
				Fir::global["initThreadPoolState"]="maintain";
			}
			break;
		case 'p':
			{
				Fir::global["port"]=arg;
			}
			break;
		case 's':
			{
				Fir::global["server"]=arg;
			}
			break;
		case 'l':
			{
				Fir::global["log"]=arg;
			}
			break;
		case 'f':
			{
				Fir::global["logid"] = arg;
				Fir::global["logfilename"]= StringUtil::Format("gateserver%s.log", arg) ;
			}
			break;
		case 'y':
			{
				Fir::global["mysql"]=arg;
			}
			break;
		case 'i':
			{
				Fir::global["ifname"]=arg;
			}
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/**
 * \brief 简短描述信息
 *
 */
static char gateway_doc[] = "\nGatewayServer\n" "\t网关服务器。";

/**
 * \brief 读取配置文件
 *
 */
class GatewayConfile:public zConfile
{
public:
	GatewayConfile(const char*confile) : zConfile(confile)
	{
	}

private:
	bool parseYour(_xmlNode* node)
	{
		if (node)
		{
			_xmlNode* child=parser.getChildNode(node,NULL);
			while(child)
			{
				parseNormal(child);
				child=parser.getNextNode(child,NULL);
			}
			return true;
		}
		else
			return false;
	}
};

/**
 * \brief 重新读取配置文件，为HUP信号的处理函数
 *
 */
void GatewayService::reloadconfig()
{
	Fir::logger->debug("%s", __PRETTY_FUNCTION__);
	GatewayConfile gc(Fir::global["configfile"].c_str());
	gc.parse("gateserver");

	WordFilter::getMe().init();
}
/**
 * \brief 主程序入口
 *
 * \param argc 参数个数
 * \param argv 参数列表
 * \return 运行结果
 */
int main(int argc, char **argv)
{
	Fir::initGlobal();
	Fir::logger=new zLogger("GS");

	//设置缺省参数
	Fir::global["configfile"] = "config.xml";
	Fir::global["logfilepath"] = "/tmp/";
	Fir::global["logfilename"] = "gatewayserver.log";
	Fir::global["configdir"] = "config/";

	//解析命令行参数
	zArg::getArg()->add(gateway_options, gateway_parse_opt, 0, gateway_doc);
	zArg::getArg()->parse(argc, argv);
	//Fir::global.dump(std::cout);
	
	//解析配置文件参数
	GatewayConfile gc(Fir::global["configfile"].c_str());
	if (!gc.parse("gateserver"))
		return EXIT_FAILURE;

	//设置日志级别
	Fir::logger->setLevel(Fir::global["log"]);
	//设置写本地日志文件
	Fir::logger->addLocalFileLog(Fir::global["logfilepath"]+Fir::global["logfilename"]);

	// 底层数据传输是否支持压缩
	if(Fir::global["compress"] == "true" )
	{
		GatewayService::getMe().bCompress = true;
	}
	else
	{
		GatewayService::getMe().bCompress = false;
	}

	//是否以后台进程的方式运行
	if ("true" == Fir::global["daemon"]) {
		Fir::logger->info("Program will be run as a daemon");
		Fir::logger->removeConsoleLog();
		daemon(1, 1);
	}

	GatewayService::getMe().main();

	Fir::finalGlobal();
	return EXIT_SUCCESS;
}

