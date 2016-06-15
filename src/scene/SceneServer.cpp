#include "zSubNetService.h"
#include "Fir.h"
#include "zMisc.h"
#include "SceneServer.h"
#include "SceneTask.h"
#include "zConfile.h"
#include "SceneTaskManager.h"
#include "TimeTick.h"
#include "SceneUser.h"
#include "RecordClient.h"
#include "SessionClient.h"
#include "SceneUserManager.h"
#include "GmManager.h"
#include "zDBConnPool.h"
#include "AllZoneCommand.h"
#include "scFLCmdHandler.h"
#include "SceneUserManager.h"
#include "XmlConfig.h"
#include "StringUtil.h"
#include "GlobalVars.h"
#include "TopListSystem.h"

SceneService *SceneService::instance = NULL;
zLogger *SceneService::userlogger = NULL;

DWORD SceneService::cmd_record[4];
char SceneService::cmd_recordNew[zSocket::MAX_DATASIZE];
DWORD SceneService::cmd_len;

zDBConnPool *SceneService::dbConnPool = NULL;
MetaData* SceneService::metaData = NULL;

/* *
* \brief 初始化各种表及逻辑的加载
*
* \return 是否成功
*/
bool SceneService::initLogic()
{
	if (!initDBConnPool())
		return false;

	if (!XmlConfig::getMe().Initialize())
	{
		Fir::logger->error("初始化xml配置失败");
		return false;
	}

	//有些模块初始化就用到了全局变量，所以先加载
	if (!g_GlobalVars->load())
	{ 
		return false;
	}

	scFLCmdHandler::getMe().Initialize();

	return true;
}


/* *
* \brief 初始化数据库连接池
*
* \return 是否成功
*/
bool SceneService::initDBConnPool()
{
	dbConnPool = zDBConnPool::newInstance(NULL);

	if (NULL == dbConnPool)
	{
		Fir::logger->error("[数据库初始化]SceneService::initDBConnPool连接数据库失败(1)");
		return false;
	}

	const char* pmysql = Fir::global["mysql"].c_str();

	if (!dbConnPool->putURL(0, pmysql, false))
	{
		Fir::logger->error("[数据库初始化]SceneService::initDBConnPool连接数据库失败(2)");
		return false;
	}

	metaData = MetaData::newInstance("");

	if (NULL == metaData)
	{
		Fir::logger->error("[数据库初始化]SceneService::initDBConnPool连接数据库失败(3)");
		return false;
	}

	std::string str_meta = Fir::global["mysql"];

	if (!metaData->init(str_meta))
	{
		Fir::logger->error("[数据库初始化]SceneService::initDBConnPool连接数据库失败(4)");
		return false;
	}
	return true;
}

/**
* \brief 初始化网络服务器程序
*
* 实现了虚函数<code>zService::init</code>
*
* \return 是否成功
*/
bool SceneService::init()
{
	//初始化连接线程池
	int state = state_none;
	Fir::to_lower(Fir::global["initThreadPoolState"]);
	if ("repair" == Fir::global["initThreadPoolState"]
	|| "maintain" == Fir::global["initThreadPoolState"])
		state = state_maintain;
	taskPool = new zTCPTaskPool(atoi(Fir::global["threadPoolCapacity"].c_str()), state);
	if (NULL == taskPool
		|| !taskPool->init())
		return false;

	strncpy(pstrIP, zSocket::getIPByIfName(Fir::global["ifname"].c_str()), MAX_IP_LENGTH - 1);
	if (!zSubNetService::init())
		return false;

	const CMD::SUPER::ServerEntry *serverEntry = NULL;

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

	if (!zMemDBPool::getMe().init() || (NULL == zMemDBPool::getMe().getMemDBHandle()))
	{
		Fir::logger->error("连接内存数据库失败");
		return false;
	}
	
	if (!initLogic())
	{
		Fir::logger->error("场景服务器加载基本配置失败");
		return false;
	}

	if (!GmManager::getMe().init())
	{
		return false;
	}

	// 初始化玩家日志
	initUserLogger();
	SceneUserManager::getMe().init();
	
	// 排行榜初始化要放在最后
	if (!TopListSystem::getMe().init())
	{
		return false;
	}
	
	//各种加载配置等初始化 一定要再TIMETICK之前

	/* 初始化加载配置等必须在此之前 !!! !!! !!!*/
	SceneTimeTick::getMe().start();
	return true;
}

/**
* \brief 初始化玩家日志接口
*
* \return 是否成功
*/
bool SceneService::initUserLogger()
{
	userlogger = FIR_NEW zLogger("USER");
	userlogger->setLevel(Fir::global["log"]);
	if ("" != Fir::global["userlogfile"])
	{
		userlogger->addLocalFileLog(Fir::global["logfilepath"]+ Fir::global["userlogfile"]);
		userlogger->removeConsoleLog();
	}

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
void SceneService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	//Fir::logger->debug(__PRETTY_FUNCTION__);
	SceneTask *tcpTask = new SceneTask(taskPool, sock, addr);
	if (NULL == tcpTask)
		//内存不足，直接关闭连接
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//得到了一个正确连接，添加到验证队列中
		SAFE_DELETE(tcpTask);
	}
}

bool SuperCmdQueue::cmdMsgParse(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::SUPER;

	if (CMD::SUPER::CMD_SCENE == ptNullCmd->cmd)
	{
		switch(ptNullCmd->para)
		{
		case CMD::SUPER::PARA_ZONE_ID_SCENE:
			{
				CMD::SUPER::t_ZoneID_Scene *rev = (CMD::SUPER::t_ZoneID_Scene *)ptNullCmd;
				SceneService::getMe().zoneID = rev->zone;
				Fir::logger->debug("收到区编号 %u", SceneService::getMe().zoneID);
				return true;
			}
			break;
		case CMD::SUPER::PARA_SUPER2SCENE_FROM_FL:
			{
				CMD::SUPER::t_Super2Scene_FromFL* rev = (CMD::SUPER::t_Super2Scene_FromFL*)ptNullCmd;
				std::string content((const char*)rev->data,rev->datasize);
				return scFLCmdHandler::getMe().handleFLCmd(content);
			}
			break;
		default:
			break;
		}
		return true;
	}

	return true;
}

bool SceneService::msgParse_SuperService(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::SUPER;

	if (CMD::SUPER::CMD_SCENE == ptNullCmd->cmd)
	{
		superCmd.msgParse(ptNullCmd, nCmdLen);
		return true;
	}

	Fir::logger->error("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
	return false;
}

/**
* \brief 结束网络服务器
*
* 实现了纯虚函数<code>zService::final</code>
*
*/
void SceneService::final()
{
	Fir::logger->info("%s", __PRETTY_FUNCTION__);

	SceneUserManager::getMe().offlineAll();

	SceneUserManager::getMe().delAll();
	SceneTimeTick::getMe().final();
	SceneTimeTick::getMe().join();
	
	zArg::removeArg();

	zSubNetService::final();
	if(taskPool)
	{
		SAFE_DELETE(taskPool);
	}

	if (sessionClient)
	{
		sessionClient->final();
		sessionClient->join();
		SAFE_DELETE(sessionClient);
	}

	if (recordClient)
	{
		recordClient->final();
		recordClient->join();
		SAFE_DELETE(recordClient);
	}

	Fir::logger->debug(__PRETTY_FUNCTION__);
}

/**
* \brief 命令行参数
*
*/
static struct argp_option account_options[] =
{
	{"configfile",	'c',	"config",	0,	"Config file name",								0},
	{"daemon",		'd',	0,			0,	"Run service as daemon",						0},
	{"log",			'l',	"level",	0,	"Log level",									0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",								0},
	{"mysql",		'y',	"mysql",	0,	"MySQL[mysql://user:passwd@host:port/dbName]",	0},
	{"ifname",		'i',	"ifname",	0,	"Local network device",							0},
	{"server",		's',	"ip",		0,	"Super server ip address",						0},
	{"port",		'p',	"port",		0,	"Super server port number",						0},
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
static error_t account_parse_opt(int key, char *arg, struct argp_state *state)
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
				Fir::global["logfilename"]= StringUtil::Format("sceneserver%s.log", arg) ;
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
static char account_doc[] = "\nSceneServer\n" "\t小游戏服务器。";


/**
* \brief 读取配置文件
*
*/
class MiniConfile:public zConfile
{
public:
	MiniConfile(const char*confile) : zConfile(confile)
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
void SceneService::reloadconfig()
{
	Fir::logger->debug("[配置重载]，开始");

	MiniConfile rc(Fir::global["configfile"].c_str());
	rc.parse("sceneserver");

	//设置MD5验证开关
	if(Fir::global["md5Verify"] == "true" )
	{
		SceneService::getMe().md5Verify = true;
	}

	//设置存档间隔
	if(atoi(Fir::global["writebackgroup"].c_str()) && (atoi(Fir::global["writebackgroup"].c_str()) > 0))
	{
		SceneService::getMe().writeBackGroup= atoi(Fir::global["writebackgroup"].c_str());
	}

	//设置腾讯充值区ID
	SceneService::getMe().tx_recharge_zone_id_ = 1;
	if (Fir::global.find("TxRechargeZoneId"))
	{
		SceneService::getMe().tx_recharge_zone_id_ = atoi(Fir::global["TxRechargeZoneId"].c_str());
	}

	// 消息性能检测
	SceneService::getMe().timeCmdLog = atoi(Fir::global["timecmdlog"].c_str());

	// xml配置
	if (!XmlConfig::getMe().Initialize())
	{
		Fir::logger->error("初始化xml配置失败");
	}

	Fir::logger->debug("[配置重载]，结束");
}

int main(int argc, char **argv)
{
	Fir::initGlobal();
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	Fir::logger=new zLogger("sceneserver");

	//设置缺省参数
	Fir::global["configfile"] = "config.xml";
	Fir::global["mysql"] = "mysql://Fir:Fir@192.168.1.162:3306/SceneServer";
	Fir::global["logfilepath"] = "/tmp/";
	Fir::global["logfilename"] = "sceneserver.log";
	Fir::global["userlogfile"] = "usersceneserver.log";
	Fir::global["configdir"] = "config/";

	//解析命令行参数
	zArg::getArg()->add(account_options, account_parse_opt, 0, account_doc);
	zArg::getArg()->parse(argc, argv);
	//Fir::global.dump(std::cout);
	
	//解析配置文件参数
	MiniConfile rc(Fir::global["configfile"].c_str());
	if (!rc.parse("sceneserver"))
		return EXIT_FAILURE;

	//设置日志级别
	Fir::logger->setLevel(Fir::global["log"]);
	//设置写本地日志文件
	Fir::logger->addLocalFileLog(Fir::global["logfilepath"] + Fir::global["logfilename"]);

	//设置MD5验证开关
	if(Fir::global["md5Verify"] == "true")
	{
		SceneService::getMe().md5Verify = true;
	}

	//设置腾讯充值区ID
	SceneService::getMe().tx_recharge_zone_id_ = 1;
	if (Fir::global.find("TxRechargeZoneId"))
	{
		SceneService::getMe().tx_recharge_zone_id_ = atoi(Fir::global["TxRechargeZoneId"].c_str());
	}

	//设置存档间隔
	if(atoi(Fir::global["writebackgroup"].c_str()) && (atoi(Fir::global["writebackgroup"].c_str()) > 0))
	{
		SceneService::getMe().writeBackGroup = atoi(Fir::global["writebackgroup"].c_str());
	}

	// 消息性能检测
	SceneService::getMe().timeCmdLog = atoi(Fir::global["timecmdlog"].c_str());

	//是否以后台进程的方式运行
	if ("true" == Fir::global["daemon"]) {
		Fir::logger->info("Program will be run as a daemon");
		Fir::logger->removeConsoleLog();
		daemon(1, 1);
	}

	SceneService::getMe().main();

	google::protobuf::ShutdownProtobufLibrary();
	Fir::finalGlobal();
	return EXIT_SUCCESS;
}

