
#ifndef _HttpTask_h_
#define _HttpTask_h_

#include <time.h>
#include <gd.h>
#include <gdfontl.h>
#include <gdfontg.h>
#include <gdfontmb.h>
#include <iostream>
#include <gdfontmb.h>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <ctime>
#include <vector>
#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>
#include "Fir.h"
#include "zTCPTask.h"
#include "zTCPServer.h"
#include "zNewHttpTask.h"
#include "zService.h"
#include "zMisc.h"
#include "zDBConnPool.h"
#include "zString.h"
#include "zProperties.h"
#include "zUrl.h"
#include "zMetaData.h"
#include "zXMLParser.h"
#include "zRegex.h"
#include "zMisc.h"

//定义登陆返回的错误码
#define OK			  		0		// 操作成功
#define FORMAT_ERR			-1		//http格式错误
#define IP_ERR              -2     //请求的IP非法
#define SYS_ERR             -3      //系统错误
#define CMD_ERR				-4		//指令错误
#define PARA_ERR			-5		//参数错误
#define ACCID_ERR  			-6		//accid非法
#define ROLENAME_ERR		-7		//角色名含非法字符
#define GETROLE_ERR			-8		//查询角色名失败
#define GETMSG_ERR			-9		//查询角色信息失败
#define GETROLE_NONE		-10		//未查找到任何角色
#define GETMSG_NONE			-11		//未查找到角色信息
#define REQUEST_MAX			-12		//该请求区本分钟流量已满
#define UPDOFF_ERR			-13		//更新角色史官等级失败
#define GETACCOUNT_ERR		-14		//未查找到该帐号的登陆信息
#define GETACCOUNTROLE_ERR	-15		//未查找到该帐号的角色
#define GETFRIEND_ERR		-16		//未查找到该角色的好友信息
#define GETCOMMANDERINFO_NONE	-17	//未查找到该指挥官信息
#define DEDUCTPOINT_VALUE_ERR	-18	//扣除指挥官积分值错误

/**
 * \brief 定义http的连接任务类
 */
class HttpTask : public zNewHttpTask
{
	public:

		/**
		 * \brief 构造函数
		 * \param pool 所属的连接池
		 * \param sock TCP/IP套接口
		 * \param addr 地址
		 */
		HttpTask(zNewHttpTaskPool *pool, const int sock, const struct sockaddr_in *addr = NULL) 
			: zNewHttpTask(pool, sock, addr)
			{
				tempid = 0;
			}

		/**
		 * \brief 虚析构函数
		 */
		~HttpTask()
		{
		}
		bool addToContainer();
		void removeFromContainer();

		/**
		 * \brief 生成唯一的连接号
		 */
		inline void genTempID()	
		{
			//tempid = (uniqueID % (ZT2Service::getInstance().getMaxPoolSize())) + 1;
			tempid = uniqueID++;
		}

		/**
		 * \brief 得到连接号
		 */
		inline const DWORD getTempID() const
		{
			return tempid;
		}
		
		/**
		 * \brief http任务主处理函数(在zNewHttpTask.h里有虚函数定义)
		 * \return 是否成功，1表示还要需要其它服务器的处理，0，表示还要继续等待，-1，表示失败(或者处理结束，断开连接)
		 */
		int httpCore();

		/**
		 * \brief 分解URL传入的参数
		 * \param urlData:url请求
		 * \param cmd:请求的指令
		 * \param buf
		 * \return 见错误码
		 */
		int parseURLData(const std::string &urlData, std::string &cmd, std::string &buf);
		
		/**
		 * \brief 处理各个指令
		 * \param cmd:请求的指令
		 * \param buf
		 * \return 
		 */
		bool processCmd(const std::string &cmd, const std::string &buf);

		/**
		 * \brief 返回结果并结束
		 * \param result 结果
		 */
		void returnAndTerminate(const std::string& result);

	private:
		/**
		 * \brief 检查输入参数
		 * \param &paras URL传入的各种参数的引用
		 * \return 检查结果
		 */
		int check_parameter(const std::string &cmd, zProperties &paras);

		/*
		 * \brief 帐号
		 */
		char account[MAX_ACCNAMESIZE];
		/*
		 * \brief 临时唯一编号
		 */
		DWORD tempid;
		/* 
		 * \brief 临时唯一编号分配器
		 */
		static DWORD uniqueID;
};

#endif

