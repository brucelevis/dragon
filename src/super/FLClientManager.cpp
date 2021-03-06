/**
 * \file
 * \version  $Id: FLClientManager.cpp 13 2013-03-20 02:35:18Z  $
 * \author  ,@163.com
 * \date 2005年04月01日 15时09分53秒 CST
 * \brief 实现统一用户平台客户端连接的管理容器
 */


#include "zTCPClientTask.h"
#include "zTCPClientTaskPool.h"
#include "FLClient.h"
#include "FLClientManager.h"
#include "zXMLParser.h"
#include "TimerMgr.h"
#include "zDBConnPool.h"
#include "SuperServer.h"
#include <libxml/parser.h>

/**
 * \brief 构造函数
 */
FLClientManager::FLClientManager()
{
	flClientPool = NULL;
}

/**
 * \brief 析构函数
 */
FLClientManager::~FLClientManager()
{
	final();
}

bool FLClientManager::reload()
{
	zXMLParser xml;
	if (!xml.initFile(Fir::global["loginServerListFile"]))
	{
		Fir::logger->error("加载统一用户平台登陆服务器列表文件 %s 失败", Fir::global["loginServerListFile"].c_str());
		return false;
	}
	_xmlNode* root = xml.getRootNode("Fir");
	if (root)
	{
		_xmlNode* fir_node = xml.getChildNode(root, "LoginServerList");
		while(fir_node)
		{
			if (strcmp((char *)fir_node->name, "LoginServerList") == 0)
			{
				_xmlNode* node = xml.getChildNode(fir_node, "server");
				while(node)
				{
					if (strcmp((char *)node->name, "server") == 0)
					{
						Fir::global["FLServer"] = "";
						Fir::global["FLPort"] = "";
						if (xml.getNodePropStr(node, "ip", Fir::global["FLServer"])
								&& xml.getNodePropStr(node, "port", Fir::global["FLPort"]))
						{
							Fir::logger->debug("LoginServer: %s, %s", Fir::global["FLServer"].c_str(), Fir::global["FLPort"].c_str());
							flClientPool->put(new FLClient(Fir::global["FLServer"], atoi(Fir::global["FLPort"].c_str())));
						}
					}

					node = xml.getNextNode(node, NULL);
				}
			}

			fir_node = xml.getNextNode(fir_node, NULL);
		}
	}

//	Fir::logger->info("加载统一用户平台登陆服务器列表文件成功");
	return true;

}

/**
 * \brief 初始化管理器
 * \return 初始化是否成功
 */
bool FLClientManager::init()
{
	flClientPool = new zTCPClientTaskPool();
	if (NULL == flClientPool
			|| !flClientPool->init())
		return false;

	zXMLParser xml;
	if (!xml.initFile(Fir::global["loginServerListFile"]))
	{
		Fir::logger->error("加载统一用户平台登陆服务器列表文件 %s 失败", Fir::global["loginServerListFile"].c_str());
		return false;
	}
	xmlNodePtr root = xml.getRootNode("Fir");
	if (root)
	{
		xmlNodePtr fir_node = xml.getChildNode(root, "LoginServerList");
		while(fir_node)
		{
			if (strcmp((char *)fir_node->name, "LoginServerList") == 0)
			{
				xmlNodePtr node = xml.getChildNode(fir_node, "server");
				while(node)
				{
					if (strcmp((char *)node->name, "server") == 0)
					{
						Fir::global["FLServer"] = "";
						Fir::global["FLPort"] = "";
						if (xml.getNodePropStr(node, "ip", Fir::global["FLServer"])
								&& xml.getNodePropStr(node, "port", Fir::global["FLPort"]))
						{
							Fir::logger->debug("LoginServer: %s, %s", Fir::global["FLServer"].c_str(), Fir::global["FLPort"].c_str());
							flClientPool->put(new FLClient(Fir::global["FLServer"], atoi(Fir::global["FLPort"].c_str())));
						}
					}

					node = xml.getNextNode(node, NULL);
				}
			}

			fir_node = xml.getNextNode(fir_node, NULL);
		}
	}

	Fir::logger->info("加载统一用户平台登陆服务器列表文件成功");
	return true;
}

void FLClientManager::final()
{
	if (flClientPool)
	{
		flClientPool->final();
	}
	SAFE_DELETE(flClientPool);
}

/**
 * \brief 周期间隔进行连接的断线重连工作
 * \param ct 当前时间
 */
void FLClientManager::timeAction(const zTime &ct)
{
	if (actionTimer.elapse(ct) > 8)
	{
		if (flClientPool)
		{
			if (flClientPool->isReload())
			{
#ifdef _ZJW_DEBUG
				Fir::logger->debug("所有登录服务器不可连接，重新加载配置文件");
#endif				
				this->reload();
			}

		//	flClientPool->timeAction(ct);
		}
		actionTimer = ct;
	}
}

void FLClientManager::resetState()
{
	if (flClientPool) flClientPool->resetAll();
}


/**
 * \brief 向容器中添加已经成功的连接
 * \param flClient 待添加的连接
 */
void FLClientManager::add(FLClient *flClient)
{
	if (flClient)
	{
		zRWLock_scope_wrlock scope_wrlock(rwlock);
		const_iter it = allClients.find(flClient->getTempID());
		if (it == allClients.end())
		{
			allClients.insert(value_type(flClient->getTempID(), flClient));
		}
	}
}

/**
 * \brief 从容器中移除断开的连接
 * \param flClient 待移除的连接
 */
void FLClientManager::remove(FLClient *flClient)
{
	if (flClient)
	{
		zRWLock_scope_wrlock scope_wrlock(rwlock);
		iter it = allClients.find(flClient->getTempID());
		if (it != allClients.end())
		{
			allClients.erase(it);
		}
	}
}

/**
 * \brief 向成功的所有连接广播指令
 * \param pstrCmd 待广播的指令
 * \param nCmdLen 待广播指令的长度
 */
void FLClientManager::broadcast(const void *pstrCmd, int nCmdLen)
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);
	for(iter it = allClients.begin(); it != allClients.end(); ++it)
	{
		it->second->sendCmd(pstrCmd, nCmdLen);
	}
}

/**
 * \brief 向指定的成功连接广播指令
 * \param tempid 待广播指令的连接临时编号
 * \param pstrCmd 待广播的指令
 * \param nCmdLen 待广播指令的长度
 */
void FLClientManager::sendTo(const WORD tempid, const void *pstrCmd, int nCmdLen)
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);
	iter it = allClients.find(tempid);
	if (it != allClients.end())
	{
		it->second->sendCmd(pstrCmd, nCmdLen);
	}
}

