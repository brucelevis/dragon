
#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "WebClientManager.h"
#include "zXMLParser.h"

/**
 * \brief 构造函数
 */
WebClientManager::WebClientManager()
{
	webClientPool = NULL;
}

/**
 * \brief 析构函数
 */
WebClientManager::~WebClientManager()
{
	SAFE_DELETE(webClientPool);
}

/**
 * \brief 初始化管理器
 * \return 初始化是否成功
 */
bool WebClientManager::init()
{
	webClientPool = new zHttpClientPool();
	if (NULL == webClientPool
			|| !webClientPool->init())
		return false;

	zXMLParser xml;
	if (!xml.initFile(Fir::global["configfile"].c_str()))
	{
		Fir::logger->error("WebClientManager加载 config.xml 失败");
		return false;
	}
	_xmlNode* root = xml.getRootNode("Fir");
	if (root == NULL)
	{
		Fir::logger->error("WebClientManager加载 config.xml 失败, 找不到Fir");
		return false;
	}


	_xmlNode* supernode = xml.getChildNode(root, "superserver");
	if(NULL == supernode)
	{
		Fir::logger->error("WebClientManager加载 config.xml 失败, 找不到superserver");
		return false;
	}

	_xmlNode* serverlistnode = xml.getChildNode(supernode, "AsyncMoneyServerList");
	if(NULL == serverlistnode)
	{
		Fir::logger->error("WebClientManager加载 config.xml 失败, 找不到AsyncMoneyServerList");
		return true;
	}

	_xmlNode* servernode = xml.getChildNode(serverlistnode, "server");
	while(servernode)
	{
		std::string serverPlatform;
		std::string serverType;
		std::string serverURL;

		if (xml.getNodePropStr(servernode, "platform", serverPlatform) && 
			xml.getNodePropStr(servernode, "type", serverType) && 
			xml.getNodePropStr(servernode, "url", serverURL))
		{
			serverPlatform.append(serverType.c_str());
			serverList[serverPlatform] = serverURL;
			Fir::logger->debug("WebServer: %s, %s", serverPlatform.c_str(), serverURL.c_str());
		}
		servernode = xml.getNextNode(servernode, "server");
	}

	Fir::logger->info("WebClientManager加载 config.xml 成功");
	return true;
}


/**
 * \brief 向容器中添加已经成功的连接
 * \param infoClient 待添加的连接
 */
void WebClientManager::add(zHttpReq* httpPost)
{
}

/**
 * \brief 从容器中移除断开的连接
 * \param infoClient 待移除的连接
 */
void WebClientManager::remove(zHttpReq* httpPost)
{
}

/**
 * \brief 向成功的所有连接广播指令
 * \param pstrCmd 待广播的指令
 * \param nCmdLen 待广播指令的长度
 */
bool WebClientManager::broadcastOne2(const std::string& type, const std::string& postdata)
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);
	zHttpReq* p = NULL;
	if (serverList.find(type) != serverList.end())
		p = new zHttpReq(serverList[type], postdata);
	else
		return false;

	webClientPool->addHttp(p);

	return true;
}

bool WebClientManager::broadcastOne(const std::string& type, const std::string& postdata, post_callback cb)
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);
        zHttpReq* p = NULL;
        if (serverList.find(type) != serverList.end())
        {
               	Fir::logger->info("postdata:%s, req_url:%s",postdata.c_str(),serverList[type].c_str());
                p = new zHttpReq(serverList[type], postdata, cb);
        }
        else
                return false;

        webClientPool->addHttp(p);

        return true;	
}

bool WebClientManager::broadcastOnePost(const std::string& type, const std::string& postdata, post_callback cb)
{
	Fir::logger->info("Enter[%s], type = %s, postdata = %s", __PRETTY_FUNCTION__, type.c_str(), postdata.c_str());
	zRWLock_scope_rdlock scope_rdlock(rwlock);
	zHttpReq* p = NULL;
	if (serverList.find(type) != serverList.end())
	{
		Fir::logger->info("Enter[%s], req_url:%s", __PRETTY_FUNCTION__, serverList[type].c_str());
		p = new zHttpReq(serverList[type], postdata, cb);
		p->setType(HttpReqType_Post);
	}
	else
	{
		Fir::logger->warn("[%s], serverList.find(type) != serverList.end()", __PRETTY_FUNCTION__);
		return false;
	}
	webClientPool->addHttp(p);

	return true;	
}

bool WebClientManager::broadcastOnePostJson(const std::string& type, const std::string& json_buf, post_callback cb)
{
	Fir::logger->info("Enter[%s], type = %s, json_buf = %s", __PRETTY_FUNCTION__, type.c_str(), json_buf.c_str());
	zRWLock_scope_rdlock scope_rdlock(rwlock);
	zHttpReq* p = NULL;
	if (serverList.find(type) != serverList.end())
	{
		Fir::logger->info("Enter[%s], req_url:%s", __PRETTY_FUNCTION__, serverList[type].c_str());
		p = new zHttpReq(serverList[type], json_buf, cb);
		p->setType(HttpReqType_PostJson);
	}
	else
	{
		Fir::logger->warn("[%s], serverList.find(type) != serverList.end()", __PRETTY_FUNCTION__);
		return false;
	}
	webClientPool->addHttp(p);

	return true;	
}