
#include <google/protobuf/stubs/common.h>

#include "zSubNetService.h"
#include "Fir.h"
#include "zMisc.h"
#include "RecordServer.h"
#include "RecordTask.h"
#include "zDBConnPool.h"
#include "zConfile.h"
#include "RecordUserManager.h"
#include "RecordUser.h"
#include "zMetaData.h"
#include "TimeTick.h"
#include "zProperties.h"
#include "RecordTaskManager.h"
#include "AccountMgr.h"
#include "XmlConfig.h"
#include "RedisMgr.h"
#include "RecordTopList.h"
#include "GmManager.h"

zDBConnPool *RecordService::dbConnPool = NULL;
MetaData* RecordService::metaData = NULL;

/**
 * \brief 初始化网络服务器程序
 *
 * 实现了虚函数<code>zService::init</code>
 *
 * \return 是否成功
 */
bool RecordService::init()
{
	//Fir::logger->debug(__PRETTY_FUNCTION__);

	if (!XmlConfig::getMe().Initialize())
	{
		Fir::logger->error("初始化xml配置失败");
		return false;
	}

	dbConnPool = zDBConnPool::newInstance(NULL);
	
	if (NULL == dbConnPool)
	{
		Fir::logger->error("连接数据库失败");
		return false;
	}

	const char* pmysql = Fir::global["mysql"].c_str(); 

	if (!dbConnPool->putURL(0, pmysql, false))
	{   
		Fir::logger->error("连接数据库失败");
		return false;
	}

	metaData = MetaData::newInstance("");

	if (NULL == metaData)
	{
		Fir::logger->error("连接数据库失败");
		return false;
	}

	std::string str_meta = Fir::global["mysql"];

	if (!metaData->init(str_meta))
	{   
		Fir::logger->error("连接数据库失败");
		return false;
	}

	if (!zMemDBPool::getMe().init() || (NULL == zMemDBPool::getMe().getMemDBHandle()))
	{
		Fir::logger->error("连接内存数据库失败");
		return false;
	}

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

	/*UrlInfo ui1(0,Fir::global["mysql"],true);
	if(!DbManager::getMe().InitDynamicDbCons(ui1.host, ui1.dbName, ui1.user, ui1.passwd, 5)) 
		return false;*/

	if (!zSubNetService::init())
	{
		return false;
	}

	if (RecordUserM::getMe().init())
	{
		Fir::logger->info("初始化RecordUser管理器模块...成功");
	}
	else
	{
		Fir::logger->error("初始化RecordUser管理器模块...失败");
		return false;
	}

	if(!AccountMgr::getMe().init())
	{
		Fir::logger->info("初始化AccountMgr失败");
		return false;
	}
	if (!RedisMgr::getMe().init())
	{
		Fir::logger->info("初始化RedisMgr失败");
		return false;
	}
	if (!RecordTopList::getMe().init())
	{
		Fir::logger->info("初始化RecordTopList失败");
		return false;
	}

	if (!GmManager::getMe().init())
	{
		Fir::logger->info("初始化GmManager失败");
		return false;
	}

	if(RecordTimeTick::getMe().start())
		Fir::logger->info("初始化RecordTimeTick模块...成功");

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
void RecordService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	RecordTask *tcpTask = new RecordTask(taskPool, sock, addr);
	if (NULL == tcpTask)
		//内存不足，直接关闭连接
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//得到了一个正确连接，添加到验证队列中
		SAFE_DELETE(tcpTask);
	}
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
bool RecordService::msgParse_SuperService(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
#ifdef _ALL_SUPER_GM
	Fir::logger->trace("%s(%u, %u, %u)", __PRETTY_FUNCTION__, ptNullCmd->cmd, ptNullCmd->para, nCmdLen);
#endif	

	switch (ptNullCmd->cmd)
	{
		case CMD::SUPER::CMD_RECORD:
			{
				switch (ptNullCmd->para)
				{   
					case CMD::SUPER::PARA_ZONE_ID_RECORD:
						{  
							CMD::SUPER::t_ZoneID_Record *rev = (CMD::SUPER::t_ZoneID_Record *)ptNullCmd;										
							RecordService::getMe().zoneID = rev->zone;									
							Fir::logger->debug("收到区编号 %u", RecordService::getMe().zoneID);
							return true;
						}   
						break;
					default:
						break;
				}
			}
			break;
		default:
			break;
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
void RecordService::final()
{
	//场景链接是否回收了
	while(!RecordTaskManager::getMe().sceneTaskEmpty())
	{
		zThread::msleep(100);
	}
	if(taskPool)
	{
		taskPool->final();
		SAFE_DELETE(taskPool);
	}

	zSubNetService::final();

	zDBConnPool::delInstance(&dbConnPool);
	Fir::logger->debug(__PRETTY_FUNCTION__);
}

/**
 * \brief 命令行参数
 *
 */
static struct argp_option record_options[] =
{
	{"configfile",	'c',	"config",	0,	"Config file name",								0},
	{"daemon",		'd',	0,			0,	"Run service as daemon",						0},
	{"log",			'l',	"level",	0,	"Log level",									0},
	{"logfilename",	'f',	"filename",	0,	"Log file name",								0},
	{"objlogfilename",	'o',	"objfilename",	0,	"object Log file name",								0},
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
static error_t record_parse_opt(int key, char *arg, struct argp_state *state)
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
				Fir::global["logfilename"]=arg;
			}
			break;
		case 'o':
            {
                Fir::global["objlogfilename"]=arg;
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
static char record_doc[] = "\nRecordServer\n" "\t档案服务器。";


/**
 * \brief 读取配置文件
 *
 */
class RecordConfile:public zConfile
{
public:
	RecordConfile(const char*confile) : zConfile(confile)
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
void RecordService::reloadconfig()
{
	Fir::logger->debug("[配置重载]，开始");

	RecordConfile rc(Fir::global["configfile"].c_str());
	rc.parse("recordserver");

	if (!XmlConfig::getMe().Initialize())
	{
		Fir::logger->error("初始化xml配置失败");
	}

	Fir::logger->debug("[配置重载]，结束");
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
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	Fir::logger=new zLogger("RS"); 

	//设置缺省参数
	Fir::global["configfile"] = "config.xml";
	Fir::global["mysql"] = "mysql://Fir:Fir@192.168.1.162:3306/RecordServer";
	Fir::global["logfilepath"] = "/tmp/"; 
	Fir::global["configdir"] = "config/";

	//解析命令行参数
	zArg::getArg()->add(record_options, record_parse_opt, 0, record_doc);
	zArg::getArg()->parse(argc, argv);
	//Fir::global.dump(std::cout);

	//解析配置文件参数
	RecordConfile rc(Fir::global["configfile"].c_str());
	if (!rc.parse("recordserver"))
		return EXIT_FAILURE;

	//设置日志级别
	Fir::logger->setLevel(Fir::global["log"]);
	//设置写本地日志文件
	Fir::logger->addLocalFileLog(Fir::global["logfilepath"] + "recordserver.log");

	// 消息性能检测
	RecordService::getMe().timeCmdLog = atoi(Fir::global["timecmdlog"].c_str());

	//是否以后台进程的方式运行
	if ("true" == Fir::global["daemon"]) {
		Fir::logger->info("Program will be run as a daemon");
		Fir::logger->removeConsoleLog();
		daemon(1, 1);
	}

	RecordService::getMe().main();
	google::protobuf::ShutdownProtobufLibrary();
	Fir::finalGlobal();
	return EXIT_SUCCESS;
}

