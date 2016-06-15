#include "MessageHandler.h"

#include "packet.h"
#include "SceneUser.h"

void MessageHandler::__RegisterHandleFunc (WORD msgid, HandleFunc func)
{
	handle_func_map_.insert(make_pair(msgid, func));
}

void MessageHandler::onMessage(SceneUser* pUser, WORD msgid, FIGHTING::Packet* pkt)
{
	auto iter = handle_func_map_.find(msgid);
	if (iter != handle_func_map_.end())
	{
#ifdef _CMD_DEBUG
		Fir::logger->trace("[消息接收]: 角色ID = %u", pUser->getCharid());
#endif
		(iter->second)(pUser->getCharid(), pkt);
	}
}