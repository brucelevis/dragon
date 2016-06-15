

#include "zMisc.h"
#include "HttpTask.h"
#include "HttpTaskManager.h"
#include "TimeTick.h"
#include "SuperServer.h"
#include <json/json.h>

DWORD HttpTask::uniqueID = 0;
bool HttpTask::addToContainer()
{
	return HttpTaskManager::getMe().add(this);
}

void HttpTask::removeFromContainer()
{
	HttpTaskManager::getMe().remove(this);
}

int HttpTask::httpCore()
{
	int retcode = pSocket->recvToBuf_NoPoll();
	if (retcode > 0)
	{
		char pstrCmd[zSocket::MAX_DATASIZE];
		int nCmdLen = pSocket->recvToHttpCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
		if (nCmdLen <= 0)
			//这里只是从缓冲取数据包，所以不会出错，没有数据直接返回
			return 0;
		else
		{
			pstrCmd[nCmdLen] = '\0';
			Fir::logger->debug("[http请求] ***************************** ");
			Fir::logger->debug("[http请求] %s, %d, %s", pSocket->getIP(), nCmdLen, pstrCmd);

			std::string buf;
			std::string cmd;
			int ret = parseURLData(pstrCmd, cmd, buf);
			if(OK == ret) 	//初步处理成功，需要继续处理
			{
				if (addToContainer())
				{
					if (processCmd(cmd, buf))
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
			else
			{
				return -1;	//端开连接
			}
		}
	}

	return -1;
}

static std::string getCmd(const std::string &urlData)
{
	std::vector<std::string> url;
	Fir::stringtok(url, urlData, " /");
	if (url.size() < 3)
	{
		Fir::logger->error("输入信息错误/");
		return "";
	}
	return url[1];
}

int HttpTask::parseURLData(const std::string &urlData, std::string &cmd, std::string &buf)
{
	Fir::logger->info("[充值],urlData");
	Fir::logger->info("/////////////////////////////////////////////////////////////////////////");
	Fir::logger->info("%s", urlData.c_str());
	Fir::logger->info("/////////////////////////////////////////////////////////////////////////");

	std::size_t pos = urlData.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		Fir::logger->error("[充值],找不到空行");
		return -1;
	}
	std::string json = urlData.substr(pos+4);

	Fir::logger->info("[充值],json");
	Fir::logger->info("/////////////////////////////////////////////////////////////////////////");
	Fir::logger->info("%s", json.c_str());
	Fir::logger->info("/////////////////////////////////////////////////////////////////////////");
	pos = json.find_first_of('{');
	if (pos == std::string::npos)
	{
		Fir::logger->error("[充值],找不到大括号");
		return -1;
	}
	buf = json.substr(pos);
	Fir::logger->info("%s", buf.c_str());
	Fir::logger->info("/////////////////////////////////////////////////////////////////////////");

	Fir::url_decode(buf);

	cmd = getCmd(urlData);

	return 0;
}

bool HttpTask::processCmd(const std::string &cmd, const std::string &buf)
{
	Fir::logger->info("[HTTP],cmd=%s", cmd.c_str());
}

void HttpTask::returnAndTerminate(const std::string& result)
{	
	std::ostringstream oss;
	oss << "HTTP/1.1 200 OK\n";
	oss << "Content-Length: " << result.length() << "\n";
	oss << "Content-Type: application/json\n";
	oss << "Connection: close\n";
	oss << "\n";
	oss << result;
	std::string strsend = oss.str();
	sendCmd(strsend.c_str(), strsend.length());

	Terminate();
}