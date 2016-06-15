#include <iostream>
#include <list>

#include "gFLCmdHandler.h"
#include "zSocket.h"
#include "zMisc.h"
#include "GatewayServer.h"
#include "GatewayTaskManager.h"
#include "GatewayTask.h"

bool gFLCmdHandler::handleFLCmd(const std::string& content)
{
	fl_gateway::PbMessage msg;
	if (!msg.ParseFromString(content))
	{
		Fir::logger->warn("[FL消息]，解析指令失败");
		return false;
	}

	auto itr=m_HandleFuncMap.find(msg.msgid());
	if (itr==m_HandleFuncMap.end())
	{
		Fir::logger->warn("[FL消息]，查找消息id失败,msgid=%u",(DWORD)msg.msgid());
		return  false;
	}

	return (itr->second)(msg.content());    
}

void gFLCmdHandler::Initialize()
{
	__RegisterHandleFunc(fl_gateway::MSG_ID_TEST_RETURN, std::bind(&gFLCmdHandler::handleTestReturn, this, std::placeholders::_1));

}

void gFLCmdHandler::__RegisterHandleFunc (fl_gateway::MSG_ID msgid, HANDLE_FUNC func)
{
	m_HandleFuncMap.insert (make_pair (msgid, func));
}

bool gFLCmdHandler::handleTestReturn(const std::string& cmd_content)
{
	fl_gateway::PbMsgTestReturn msg;
	if (!msg.ParseFromString(cmd_content))
	{
		Fir::logger->warn("[解析FL指令]，PbMsgTest失败");
		return false;
	}
	
	std::string talk = msg.talk();
	Fir::logger->debug("[解析FL指令]:测试成功,talk=%s",talk.c_str());
	return true;
}

bool gFLCmdHandler::sendCmdToFL(const void *pstrCmd, const int nCmdLen)
{
	//Fir::logger->debug("%s", __PRETTY_FUNCTION__);

	if ((DWORD)nCmdLen > zSocket::MAX_DATASIZE)
	{
		Fir::logger->error("sendCmdToFL,发送消息过大:%d",nCmdLen);
		return false;
	}

	using namespace CMD::SUPER;

	BYTE buf[zSocket::MAX_DATASIZE];
	t_Gate2Super_Forward2FL *scmd=(t_Gate2Super_Forward2FL *)buf;
	constructInPlace(scmd);

	scmd->datasize = nCmdLen;
	bcopy(pstrCmd, scmd->data, nCmdLen);
	return GatewayService::getMe().sendCmdToSuperServer(scmd, scmd->getSize());	
}

bool gFLCmdHandler::sendCmdToFL(const std::string& content)
{
	return sendCmdToFL(content.c_str(),content.size());
}

void gFLCmdHandler::testMsg()
{
	//Fir::logger->debug("%s", __PRETTY_FUNCTION__);

	fl_gateway::PbMsgTest test;
	test.set_talk("我爱你");
	sendCmdToFL(fl_gateway::MSG_ID_TEST,test);
}