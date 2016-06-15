#include "GmManager.h"
#include "StringUtil.h"
#include "zSocket.h"
#include "zMisc.h"
#include "RecordCommand.h"
#include "RecordTaskManager.h"
#include "zMetaData.h"
#include "RecordServer.h"
#include "RecordUserManager.h"

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

bool GmManager::handleCmd(WORD temp_server_id, DWORD httpid, zProperties &paras)
{
	Fir::logger->info("%s,temp_server_id=%d,httpid=%u,paras=%s", __PRETTY_FUNCTION__, temp_server_id, httpid, paras.toString().c_str());

	if(!paras.find("CmdId"))
	{
		Fir::logger->error("[充值] 参数不对");
		return false;
	}

	temp_server_id_ = temp_server_id;

	DWORD cmdid = (DWORD)StringUtil::Atoi64(paras["CmdId"].c_str());
	auto iter = handle_func_map_.find(cmdid);
	if (iter != handle_func_map_.end())
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

void GmManager::__RegisterHandleFunc(int id, HANDLE_FUNC func)
{
	handle_func_map_.insert(make_pair(id, func));
}