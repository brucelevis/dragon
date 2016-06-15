/**
 * \file
 * \version  $Id: SuperServer.cpp 75 2013-04-25 05:10:39Z  $
 * \author  ,@163.com
 * \date 2004年11月29日 17时19分12秒 CST
 * \brief 实现服务器管理器
 *
 * 对一个区中的所有服务器进行管理
 * 
 */

#include <iostream>
#include <string>
#include <ext/numeric>

#include "zService.h"
#include "zThread.h"
#include "zSocket.h"
#include "zTCPServer.h"
#include "zNetService.h"
#include "ServerTask.h"
#include "zDBConnPool.h"
#include "SuperServer.h"
#include "Fir.h"
#include "zMisc.h"
#include "zArg.h"
#include "zConfile.h"
#include "TimeTick.h"
#include "ServerManager.h"
#include "FLClient.h"
#include "FLClientManager.h"
#include "zMetaData.h"
#include "HttpTask.h"
#include "StringUtil.h"
#include "WebClientManager.h"

zDBConnPool *SuperService::dbConnPool = NULL;
MetaData* SuperService::metaData = NULL;
SuperService *SuperService::instance = NULL;

/**
 * \brief 从数据库中获取服务器信息
 *
 * 如果数据库中没有服务器管理器的信息，需要初始化一条记录
 *
 */
bool SuperService::getServerInfo()
{
	std::string strsql = StringUtil::Format("select ID,TYPE,NAME,IP,PORT,EXTIP,EXTPORT,NETTYPE from t_serverlist where `TYPE`=%u", SUPERSERVER);
	RecordSetPtr pResult = dbConnPool->exeSelect(strsql);

	if (NULL==pResult || pResult->empty() || pResult->size() > 1)
	{
		Fir::logger->error("数据库中的记录条数不对");
		return false;
	}

	struct stServerItem
	{
		WORD wdServerID;
		WORD wdServerType;
		std::string name;
		std::string ip;
		WORD wdPort;
		std::string extip;
		WORD wdExtPort;
		WORD wdNetType;
		stServerItem()
		{
			wdServerID = 0;
			wdServerType = 0;
			wdPort = 0;
			wdExtPort = 0;
			wdNetType = 0;
		}
	};

	stServerItem item;
	Record *rec = pResult->get(0);
	item.wdServerID = rec->get("ID");
	item.wdServerType = rec->get("TYPE");
	item.name = rec->get("NAME").getString();
	item.ip = rec->get("IP").getString();
	item.wdPort = rec->get("PORT");
	item.extip = rec->get("EXTIP").getString();
	item.wdExtPort = rec->get("EXTPORT");
	item.wdNetType = rec->get("NETTYPE");

	//只有一条满足条件的记录
	if (strcmp(pstrIP, item.ip.c_str()) == 0)
	{
		wdServerID = item.wdServerID;
		strncpy(pstrName, item.name.c_str(), item.name.length());
		wdPort = item.wdPort;
		wdExtPort = item.wdExtPort;
		wdNetType = item.wdNetType;
	}
	else
	{
		Fir::logger->error("数据库中的记录不符合：%s, %s", pstrIP, item.ip.c_str());
		return false;
	}

	// 读出场景服务器信息，支持场景服务器动态增加
	std::string strsql2 = StringUtil::Format("select ID,IP from t_serverlist where `TYPE`=%u", SCENESSERVER);
	RecordSetPtr pResult2 = dbConnPool->exeSelect(strsql2);
	if (NULL==pResult2 || pResult2->empty())
	{
		for(unsigned int i = 0; i < pResult->size(); i++)
		{
			Record *rec = pResult->get(i);
			WORD serverid = rec->get("ID");
			std::string ip = rec->get("IP").getString();
			ServerManager::getMe().addSceneInfo(serverid,ip);
		}
	}

	return true;
}

/**
 * \brief 初始化网络服务器程序
 *
 * 实现纯虚函数<code>zService::init</code>
 *
 * \return 是否成功
 */
bool SuperService::init()
{
	Fir::logger->debug("%s", __PRETTY_FUNCTION__);

	using namespace std::placeholders;
	//初始化依赖服务器
	this->initServerSequence();

	dbConnPool = zDBConnPool::newInstance(NULL);
	if (NULL == dbConnPool
			|| !dbConnPool->putURL(0, Fir::global["mysql"].c_str(), false))
	{
		Fir::logger->error("连接数据库失败");
		return false;
	}

	metaData = MetaData::newInstance("");            

	if (NULL == metaData
			|| !metaData->init(Fir::global["mysql"]))
	{
		Fir::logger->error("连接数据库失败");
		return false;
	}

	if (!zMemDBPool::getMe().init() || (NULL == zMemDBPool::getMe().getMemDBHandle()))
	{
		Fir::logger->error("连接内存数据库失败");
		return false;
	}
	zMemDBPool::getMe().flushALLMemDB();

	strncpy(pstrIP, zSocket::getIPByIfName(Fir::global["ifname"].c_str()), MAX_IP_LENGTH - 1);
	Fir::logger->debug("%s,%s", pstrIP,Fir::global["ifname"].c_str());

	if (strncmp(Fir::global["time_version"].c_str(), "true", MAX_NAMESIZE) == 0)
	{
		Fir::logger->error("非免费版，禁止启动");
		return false;
	}

	if (!getServerInfo())
		return false;

	if (!FLClientManager::getMe().init())
		return false;

	if (!WebClientManager::getMe().init())
		return false;
		
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

	this->http_port = atoi(Fir::global["http_port"].c_str());
	//初始化HTTP连接线程池
	httpTaskPool = new zNewHttpTaskPool(atoi(Fir::global["httpThreadPoolCapacity"].c_str()));
	if (NULL == httpTaskPool
		|| !httpTaskPool->init())
		return false;

	if (!zMNetService::init())
	{
		return false;
	}

	if(!zMNetService::bind("SuperServer端口",wdPort))
	{
		Fir::logger->error("绑定SuperServer端口出错。");
		return false;
	}

	if(!zMNetService::bind("http端口", http_port))
	{
		Fir::logger->error("绑定http端口出错。");
		return false;
	}

//	initLogger();

	SuperTimeTick::getMe().start();

	return true;
}

void SuperService::initServerSequence()
{
	serverSequence[UNKNOWNSERVER]   =   std::vector<int>();
	serverSequence[SUPERSERVER] =   std::vector<int>();
	serverSequence[LOGINSERVER] =   std::vector<int>();
	serverSequence[RECORDSERVER]    =   std::vector<int>();
	serverSequence[GATEWAYSERVER] = std::vector<int>(); //测试用

	int data0[] = { RECORDSERVER};
	//会话依赖档案服务器
	serverSequence[SESSIONSERVER]   =   std::vector<int>(data0, data0 + sizeof(data0) / sizeof(int));
	int data1[] = { RECORDSERVER, SESSIONSERVER};
	//场景服务器依赖档案服务器，会话服务器
	serverSequence[SCENESSERVER]    =   std::vector<int>(data1, data1 + sizeof(data1) / sizeof(int));
	int data2[] = { RECORDSERVER, SESSIONSERVER, SCENESSERVER};
	//网关依赖档案服务器，会话服务器，场景服务器
	serverSequence[GATEWAYSERVER]   =   std::vector<int>(data2, data2 + sizeof(data2) / sizeof(int));
}

std::vector<int> SuperService::getServerSes(int wdServerType)
{
	auto it = serverSequence.find(wdServerType);
	if (it != serverSequence.end())
	{
		return it->second;
	}
	return std::vector<int>();
}

#ifdef _OP_TOOL_VERSION
//运维属性察看工具
bool SuperService::reSetDBPool(std::string str)
{
	zDBConnPool::delInstance(&dbConnPool);
	dbConnPool = zDBConnPool::newInstance(NULL);

	if(!dbConnPool->putURL(0,str.c_str(), false))
	{
		Fir::logger->error("连接数据库失败");
		return false;
	}
	return true;
}
#endif

/**
 * \brief 新建立一个连接任务
 *
 * 实现纯虚函数<code>zNetService::newTCPTask</code>
 *
 * \param sock TCP/IP连接
 * \param addr 地址
 */
//void SuperService::newTCPTask(const int sock, const struct sockaddr_in *addr)
//{
//ServerTask *tcpTask = new ServerTask(taskPool, sock, addr);
//if (NULL == tcpTask)
////内存不足，直接关闭连接
//TEMP_FAILURE_RETRY(::close(sock));
//else if(!taskPool->addVerify(tcpTask))
//{
////得到了一个正确连接，添加到验证队列中
//SAFE_DELETE(tcpTask);
//}
//}

void SuperService::newTCPTask(const int sock, const unsigned short srcPort)
{
	if (srcPort == http_port)
	{   
		// 来自http端口的连接
		zNewHttpTask *httpTask = new HttpTask(httpTaskPool, sock);
		if (NULL == httpTask)
			TEMP_FAILURE_RETRY(::close(sock));
		else if(!httpTaskPool->addHttp(httpTask))
			SAFE_DELETE(httpTask);
	}   
	else
	{   
		zTCPTask* tcpTask = new ServerTask(taskPool, sock, NULL);

		if (NULL == tcpTask)
			//内存不足，直接关闭连接
			TEMP_FAILURE_RETRY(::close(sock));
		else if(!taskPool->addVerify(tcpTask))
		{   
			//得到了一个正确连接，添加到验证队列中
			SAFE_DELETE(tcpTask);
		}   
	}
}


/**
 * \brief 结束网络服务器
 *
 * 实现纯虚函数<code>zService::final</code>
 *
 */
void SuperService::final()
{
	SuperTimeTick::getMe().final();
	SuperTimeTick::getMe().join();

	if (taskPool)
	{
		taskPool->final();
		SAFE_DELETE(taskPool);
	}
	if (httpTaskPool)
	{   
		httpTaskPool->final();
		SAFE_DELETE(httpTaskPool);
	}  

	zMNetService::final();

	FLClientManager::getMe().final();

	Fir::logger->debug("%s", __PRETTY_FUNCTION__);
}

/**
 * \brief 命令行参数
 *
 */
static struct argp_option super_options[] =
{
	{"configfile",	'c',	"config",	0,	"Config file name",								0},
	{"daemon",		'd',	0,			0,	"Run service as daemon",						0},
	{"log",			'l',	"level",	0,	"Log level",									0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",								0},
	{"mysql",		'y',	"mysql",	0,	"MySQL[mysql://user:passwd@host:port/dbName]",	0},
	{"ifname",		'i',	"ifname",	0,	"Local network device",							0},
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
static error_t super_parse_opt(int key, char *arg, struct argp_state *state)
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
		case 'l':
			{
				Fir::global["log"]=arg;
			}
			break;
		case 'f':
			{
				Fir::global["logfilename"]=arg;
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
static char super_doc[] = "\nSuperServer\n" "\t服务器管理器。";

/**
 * \brief 读取配置文件
 *
 */
class SuperConfile:public zConfile
{
	public:
		SuperConfile(const char*confile) : zConfile(confile)
	{
	}

	private:
		bool parseYour(_xmlNode* node)
		{
			if(node)
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
void SuperService::reloadconfig()
{
	Fir::logger->debug("%s", __PRETTY_FUNCTION__);
	SuperConfile sc(Fir::global["configfile"].c_str());
	sc.parse("superserver");

	if (Fir::global["reconn"] == "true")
	{
		FLClientManager::getMe().resetState();
	}
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
	Fir::logger=new zLogger("SuperServer");

#if 0
	const int count = 10000;
	int tick = 5;
	while(--tick >= 0)
	{
		{
			FunctionTime func_time(0,__PRETTY_FUNCTION__,"整个任务遍历需要的时间" , 32);
			for(int i = 0; i < count; i++)
			{
				std::vector<int> v;
				v.resize(count, 1);
			}
		}
		{
			FunctionTime func_time(0,__PRETTY_FUNCTION__,"整个任务遍历需要的时间" , 32);
			for(int i = 0; i < count; i++)
			{
				std::vector<int> v(count, 1);
			}
		}
		{
			FunctionTime func_time(0,__PRETTY_FUNCTION__,"整个任务遍历需要的时间" , 32);
			for(int i = 0; i < count; i++)
			{
				std::vector<int> v;
				v.reserve(count);
				bzero(&v[0], v.size() * sizeof(int));
			}
		}
		{
			FunctionTime func_time(0,__PRETTY_FUNCTION__,"整个任务遍历需要的时间" , 32);
			for(int i = 0; i < count; i++)
			{
				std::vector<int> v;
				v.reserve(count);
				fill(v.begin(), v.begin() + v.capacity(), 1);
			}
		}
	}
	return 0;
#endif

	//设置缺省参数
	Fir::global["loginServerListFile"] = "loginServerList.xml";
	Fir::global["mysql"] = "mysql://Fir:Fir@192.168.1.162:3306/SuperServer";
	Fir::global["logfilepath"] = "/tmp/";
	Fir::global["configfile"] = "config.xml";
	Fir::global["configdir"] = "config/";

	//解析命令行参数
	zArg::getArg()->add(super_options, super_parse_opt, 0, super_doc);
	zArg::getArg()->parse(argc, argv);
	//Fir::global.dump(std::cout);

	//解析配置文件参数
	SuperConfile sc(Fir::global["configfile"].c_str());
	if (!sc.parse("superserver"))
		return EXIT_FAILURE;

	//设置日志级别
	Fir::logger->setLevel(Fir::global["log"]);
	//设置写本地日志文件
	Fir::logger->addLocalFileLog(Fir::global["logfilepath"] + "superserver.log");

	// 消息性能检测
	SuperService::getMe().timeCmdLog = atoi(Fir::global["timecmdlog"].c_str());

	//是否以后台进程的方式运行
	if ("true" == Fir::global["daemon"]) {
		Fir::logger->info("Program will be run as a daemon");
		Fir::logger->removeConsoleLog();
		daemon(1, 1);
	}

	SuperService::getMe().main();
	Fir::finalGlobal();
	return EXIT_SUCCESS;
}

