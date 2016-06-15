
#ifndef _WebClientManager_h_
#define _WebClientManager_h_

#include <unordered_map>
#include <set>

#include "zHttpClient.h"
#include "zHttpClientPool.h"
#include "zTime.h"
#include "zRWLock.h"
#include "zSingleton.h"

/**
 * \brief 定义服务器信息采集连接的客户端管理容器
 */
class WebClientManager : public Fir::Singleton<WebClientManager>
{

	public:
		WebClientManager();
		~WebClientManager();

		bool init();
		void add(zHttpReq *httpPost);
		void remove(zHttpReq *httpPost);
		bool broadcastOne2(const std::string& type, const std::string& postdata);
		bool broadcastOne(const std::string& type, const std::string& postdata, post_callback cb);
		bool broadcastOnePost(const std::string& type, const std::string& postdata, post_callback cb);
		bool broadcastOnePostJson(const std::string& type, const std::string& json_buf, post_callback cb);

	private:

		/**
		 * \brief 客户端连接管理池
		 */
		zHttpClientPool *webClientPool;
		std::unordered_map<std::string,std::string> serverList;//服务器类型，服务器URL

		/**
		 * \brief 容器访问读写锁
		 */
		zRWLock rwlock;
};

#endif

