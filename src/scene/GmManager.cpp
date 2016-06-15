#include "GmManager.h"
#include "StringUtil.h"
#include "SceneUserManager.h"
#include "SessionClient.h"
#include "HMessages.h"
#include "SceneServer.h"
#include "RedisMgr.h"
#include "TimeTick.h"
#include "proto/monitor_protocol.pb.h"
#include <json/json.h>

// GM错误码
enum GM_ERROR_CODE
{
	GM_ERROR_OK = 0,			// 成功
	GM_ERROR_API = 1001,		// 接口名称错误
	GM_ERROR_VER = 1002,		// 版本号错误
	GM_ERROR_SIGEN = 1003,		// 签名错误
	GM_ERROR_DEFINE = 1008,		// 自定义错误
	GM_ERROR_SYS = 1010,		// 系统错误
};

// 处理gm命令
bool GmManager::handle_gm_cmd(DWORD httpid, zProperties &paras)
{

	Fir::logger->info("%s,httpid=%u,paras=%s", __PRETTY_FUNCTION__, httpid, paras.toString().c_str());

	if(!paras.find("CmdId"))
	{
		Fir::logger->error("[充值] 参数不对");
		return false;
	}

	DWORD cmdid = (DWORD)StringUtil::Atoi64(paras["CmdId"].c_str());
	auto iter = m_HandleFuncMap.find(cmdid);
	if (iter != m_HandleFuncMap.end())
	{
		(iter->second)(httpid,paras);    
		return true;
	}

	return true;
}

bool GmManager::init()
{
	return true;
}

void GmManager::__RegisterHandleFunc (int id, HANDLE_FUNC func)
{
	m_HandleFuncMap.insert (make_pair (id, func));
}