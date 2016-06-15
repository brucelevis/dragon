#include <iostream>
#include <list>

#include "seFLCmdHandler.h"
#include "zSocket.h"
#include "zMisc.h"
#include "SessionServer.h"

bool seFLCmdHandler::handleFLCmd(const std::string& content)
{
	fl_session::PbMessage msg;
	if (!msg.ParseFromString(content))
	{
		Fir::logger->error("[FL消息]，解析指令失败");
		return false;
	}

	auto itr=m_HandleFuncMap.find(msg.msgid());
	if (itr==m_HandleFuncMap.end())
	{
		Fir::logger->error("[FL消息]，查找消息id失败,msgid=%u",(DWORD)msg.msgid());
		return  false;
	}

	return (itr->second)(msg.content());    
}

void seFLCmdHandler::Initialize()
{
	__RegisterHandleFunc(fl_session::MSG_ID_TEST_RETURN, std::bind(&seFLCmdHandler::handleTestReturn, this, std::placeholders::_1));
}

void seFLCmdHandler::__RegisterHandleFunc (fl_session::MSG_ID msgid, HANDLE_FUNC func)
{
	m_HandleFuncMap.insert (make_pair (msgid, func));
}

bool seFLCmdHandler::handleTestReturn(const std::string& cmd_content)
{
	fl_session::PbMsgTestReturn msg;
	if (!msg.ParseFromString(cmd_content))
	{
		Fir::logger->error("[解析FL指令]，PbMsgTest失败");
		return false;
	}
	
	std::string talk = msg.talk();
	Fir::logger->info("[解析FL指令]:测试成功,talk=%s",talk.c_str());
	return true;
}

bool seFLCmdHandler::sendCmdToFL(const void *pstrCmd, const int nCmdLen)
{
	if ((DWORD)nCmdLen > zSocket::MAX_DATASIZE)
	{
		Fir::logger->error("sendCmdToFL,发送消息过大:%d",nCmdLen);
		return false;
	}

	using namespace CMD::SUPER;

	BYTE buf[zSocket::MAX_DATASIZE];
	t_Session2Super_Forward2FL*scmd=(t_Session2Super_Forward2FL *)buf;
	constructInPlace(scmd);

	scmd->datasize = nCmdLen;
	bcopy(pstrCmd, scmd->data, nCmdLen);
	return SessionService::getMe().sendCmdToSuperServer(scmd, scmd->getSize());	
}

bool seFLCmdHandler::sendCmdToFL(const std::string& content)
{
	return sendCmdToFL(content.c_str(),content.size());
}

void seFLCmdHandler::testMsg()
{
	//Fir::logger->debug("%s", __PRETTY_FUNCTION__);

	fl_session::PbMsgTest test;
	test.set_talk("[[[[[[[session->fl我爱你]]]]]]");
	sendCmdToFL(fl_session::MSG_ID_TEST,test);
}