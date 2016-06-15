/**
 * @file zHttpClientPool.h
 * $Id$
 * @brief	HTTP CLIENT连接池
 * @author zhongjunwei
 * @date 2013-07-22
 */

#ifndef _HTTPCLIENTPOOL_H
#define _HTTPCLIENTPOOL_H

#include <functional>
#include "Fir.h"
#include "zThread.h"

enum HttpReqType
{
	HttpReqType_Post		= 1,
	HttpReqType_Get			= 2,
	HttpReqType_PostJson	= 3,
};

typedef std::function<void (const std::string &)> post_callback;

class zHttpReq
{
	public:
		std::string url;
		std::string postdata;
		post_callback callback;
		//unsigned int gatewayid;
		//unsigned int tempid;	//临时id

		zHttpReq(const std::string& url, const std::string& postdata) : url(url),postdata(postdata),type(HttpReqType_Get)
		{

		}

		zHttpReq(const std::string url, const std::string postdata, const post_callback &cb) : url(url), postdata(postdata), callback(cb),type(HttpReqType_Get)
		{
		}

		//设置http请求类型
		void setType(BYTE _type)
		{
			this->type = _type;
		}

		//得到http请求类型
		BYTE getType()
		{
			return this->type;
		}
	private:
		BYTE type;			//请求类型
};

/**
 * \brief 定义轻量级http client连接池
 */
class zHttpClientPool : private zNoncopyable
{
	public:
		/**
		 * \brief 构造函数
		 */
		zHttpClientPool(const int maxHttpConns=512) : maxHttpConns(maxHttpConns)
		{
		};
	
		/**
		 * \brief 析构函数，销毁一个线程池对象
		 *
		 */
		~zHttpClientPool()
		{
			final();
		}

		bool addHttp(zHttpReq* task);
		const int getSize();
		inline const int getMaxConns() const { return maxHttpConns; }
		bool init();
		void final();

	private:

		static const int maxHttpThreads = 8;					/**< 最大连接线程数 */
		zThreadGroup httpThreads;								/**< http服务处理线程组 */
		const int maxHttpConns;								/**< 线程池并行处理连接的最大数量 */
};

#endif


