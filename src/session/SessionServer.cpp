/**
 * \file
 * \version  $Id: SessionServer.cpp 72 2013-04-24 03:55:51Z  $
 * \author   ,
 * \date 2013年03月26日 16时55分53秒 CST
 * \brief Fir项目，全局会话服务器，负责社会关系，场景负载均衡等全局功能
 *
 */

#include "zSubNetService.h"
#include "Fir.h"
#include "zMisc.h"
#include "SessionServer.h"
#include "SessionTask.h"
#include "zDBConnPool.h"
#include "zConfile.h"
#include "SessionTaskManager.h"
#include "TimeTick.h"
#include "zMetaData.h"
#include "SessionUser.h"
#include "SessionUserManager.h"
#include "RecordClient.h"
#include "TimerMgr.h"
#include "GmManager.h"
#include "zCmdBuffer.h"
#include "FLCommand.h"
#include "seFLCmdHandler.h"
#include "proto/monitor_protocol.pb.h"

zDBConnPool *SessionService::dbConnPool = NULL;
SessionService *SessionService::instance = NULL;
MetaData* SessionService::metaData = NULL;
zLogger* SessionService::billlog = NULL;
zLogger * SessionService::login_logger = NULL;

DWORD SessionService::online_num = 0;

/**
 * \brief 初始化网络服务器程序
 *
 * 实现了虚函数<code>zService::init</code>
 *
 * \return 是否成功
 */
bool SessionService::init()
{
	dbConnPool = zDBConnPool::newInstance(NULL);
	if (NULL == dbConnPool
			|| !dbConnPool->putURL(0, Fir::global["mysql"].c_str(), false))
	{
		Fir::logger->error("连接数据库失败");
		return false;
	}

	if (strncmp(Fir::global["time_version"].c_str(), "true", MAX_NAMESIZE) == 0)
	{
		Fir::logger->error("非免费版，禁止启动");
		return false;
	}

	metaData = MetaData::newInstance("");

	if (NULL == metaData
			|| !metaData->init(Fir::global["mysql"]))
	{
		Fir::logger->error("连接数据库失败");
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

	if (!zSubNetService::init())
		return false;

	if (!GmManager::getMe().init())
	{
		return false;
	}
	

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

	if (!zMemDBPool::getMe().init() || (NULL == zMemDBPool::getMe().getMemDBHandle()))
	{
		Fir::logger->error("连接内存数据库失败");
		return false;
	}
	
	/*UrlInfo ui1(0,Fir::global["mysql"],true);
	if(!DbManager::getMe().InitDynamicDbCons(ui1.host, ui1.dbName, ui1.user, ui1.passwd, 5)) 
		return false;*/

	if (!initAllData())
		return false;

// 	if(!initLogger())
// 		return false;

	//各种初始化必须放这上面！！！！！
	SessionTimeTick::getMe().start();

	return true;
}

void SessionService::sendOnlineCount()
{
	UINT32 dwUserNum = SessionUserManager::getMe().getUserNum();
	DWORD dwCharNum = SessionUserManager::getMe().getCharNum();
	DWORD dwOfflineNum = SessionUserManager::getMe().getOfflineNum();
	DWORD dwOnlineNum = dwUserNum - dwOfflineNum;

	zCmdBufferAdapterT<CMD::SUPER::t_OnlineCount_Session> send;

	send->onlinecount = dwUserNum;
	sendCmdToSuperServer(send.data(),send.size());

	Fir::logger->info("目前在线人数：账号 = %u, 角色 = %u, 连接 = %u", dwUserNum, dwCharNum, dwOnlineNum);
};

void SessionService::sendGMContent(DWORD httpid, const std::string& content)
{
	Fir::logger->info("%s,httpid=%u,content=%s", __PRETTY_FUNCTION__, httpid, content.c_str());

	// 向super返回gm结果
	zCmdBufferAdapterT<CMD::SUPER::t_RetGM_Result_Session2Super> send;

	PbGmReply binary;
	binary.set_httpid(httpid);
	binary.set_json(content);
	send->datasize = content.length();

	if ((DWORD)binary.ByteSize() > zSocket::MAX_DATASIZE)
	{
		Fir::logger->error("[GM],二进制过大,binary size %u", (DWORD)binary.ByteSize());
		return;
	}

	char buffer[zSocket::MAX_DATASIZE];
	bzero(buffer, sizeof(buffer));
	binary.SerializeToArray(buffer,binary.ByteSize());
	send->datasize = binary.ByteSize();

	if (send.write(&buffer,send->datasize))
	{
		sendCmdToSuperServer(send.data(),send.size());
	}
}

//初始化数据库和xml配置
bool SessionService::initAllData()
{
	seFLCmdHandler::getMe().Initialize();
	
	return true;
}

void SessionService::newTCPTask(const int sock, const struct sockaddr_in *addr)
{
	//Fir::logger->debug(__PRETTY_FUNCTION__);
	SessionTask *tcpTask = new SessionTask(taskPool, sock, addr);
	if (NULL == tcpTask)
		//内存不足，直接关闭连接
		TEMP_FAILURE_RETRY(::close(sock));
	else if(!taskPool->addVerify(tcpTask))
	{
		//得到了一个正确连接，添加到验证队列中
		SAFE_DELETE(tcpTask);
	}
}

bool SessionService::msgParse_SuperService(const CMD::t_NullCmd *ptNullCmd, const unsigned int nCmdLen)
{
	using namespace CMD::SUPER;

	if (CMD::SUPER::CMD_SESSION == ptNullCmd->cmd)
	{
		switch(ptNullCmd->para)
		{
			case CMD::SUPER::PARA_ZONE_ID_SESSION:
				{
					CMD::SUPER::t_ZoneID_Session *rev = (CMD::SUPER::t_ZoneID_Session *)ptNullCmd;					
					SessionService::getMe().zoneID = rev->zone;	
					SessionService::getMe().zonename = rev->name;
					Fir::logger->debug("收到区编号 %u,zonename=%s", SessionService::getMe().zoneID, SessionService::getMe().zonename.c_str());
				}
				break;
			case CMD::SUPER::PARA_REQ_GM_SUPER2SESSION:
				{
					CMD::SUPER::t_ReqGM_Super2Session* ptCmd = (CMD::SUPER::t_ReqGM_Super2Session*)ptNullCmd;
					
					PbGmRequest binary;
					binary.ParseFromArray(ptCmd->data,ptCmd->datasize);

					zProperties paras;
					for (int i=0; i<binary.pb_items_size(); i++)
					{
						const PbPropertyItem& pbitem = binary.pb_items(i);
						paras[pbitem.name()] = pbitem.value();
					}
					
					std::string content((const char*)&ptCmd->data[0],ptCmd->datasize);
					GmManager::getMe().handle_gm_cmd(binary.httpid(),paras,content);
				}
				break;
			case CMD::SUPER::PARA_PAY_NOTIFY_SUPER2SESSION:
				{
					CMD::SUPER::t_PayNotify_Super2Session* ptCmd = (CMD::SUPER::t_PayNotify_Super2Session*)ptNullCmd;

					std::string content((const char*)&ptCmd->data[0],ptCmd->datasize);
					GmManager::getMe().handle_recharge(content);
				}
				break;
			case CMD::SUPER::PARA_SUPER2SESSION_FROM_FL:
				{
					CMD::SUPER::t_Super2Session_FromFL* rev = (CMD::SUPER::t_Super2Session_FromFL*)ptNullCmd;
					std::string content((const char*)rev->data,rev->datasize);
					return seFLCmdHandler::getMe().handleFLCmd(content);
				}
				break;
			case CMD::SUPER::PARA_PAY_QUERY_SUPER2SESSION:
				{
					CMD::SUPER::t_PayQuery_Super2Session* ptCmd = (CMD::SUPER::t_PayQuery_Super2Session*)ptNullCmd;

					DWORD http_id = ptCmd->http_id;
					std::string content((const char*)&ptCmd->data[0],ptCmd->datasize);
					return  GmManager::getMe().handle_query_recharge(http_id, content);
				}
				break;
		}
		return true;
	}

	return true;
}

/**
 * \brief 结束网络服务器
 *
 * 实现了纯虚函数<code>zService::final</code>
 *
 */
void SessionService::final()
{
	SessionTimeTick::getMe().final();
	SessionTimeTick::getMe().join();
	//增加释放处理

	//华丽分割线=== 各种释放都放上面 !!!!!!	

	if(taskPool)
	{
		SAFE_DELETE(taskPool);
	}


	zSubNetService::final();

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
static char account_doc[] = "\nSessionServer\n" "\t小游戏服务器。";

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
void SessionService::reloadconfig()
{
	Fir::logger->debug("%s", __PRETTY_FUNCTION__);
	MiniConfile rc(Fir::global["configfile"].c_str());
	rc.parse("sessionserver");
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
	Fir::logger=new zLogger("sessionserver");

	//设置缺省参数
	Fir::global["configfile"] = "config.xml";
	Fir::global["mysql"] = "mysql://Fir:Fir@192.168.1.162:3306/sessionserver";
	Fir::global["logfilepath"] = "/tmp/";
	Fir::global["configdir"] = "config/";

	//解析命令行参数
	zArg::getArg()->add(account_options, account_parse_opt, 0, account_doc);
	zArg::getArg()->parse(argc, argv);
	//Fir::global.dump(std::cout);

	//解析配置文件参数
	MiniConfile rc(Fir::global["configfile"].c_str());
	if (!rc.parse("sessionserver"))
		return EXIT_FAILURE;

	//设置日志级别
	Fir::logger->setLevel(Fir::global["log"]);
	//设置写本地日志文件
	Fir::logger->addLocalFileLog(Fir::global["logfilepath"] + "sessionserver.log");

	// 消息性能检测
	SessionService::getMe().timeCmdLog = atoi(Fir::global["timecmdlog"].c_str());

	//是否以后台进程的方式运行
	if ("true" == Fir::global["daemon"]) {
		Fir::logger->info("Program will be run as a daemon");
		Fir::logger->removeConsoleLog();
		daemon(1, 1);
	}

	SessionService::getMe().main();
	google::protobuf::ShutdownProtobufLibrary();
	Fir::finalGlobal();
	return EXIT_SUCCESS;
}
