#include <iostream>
#include <list>

#include "scFLCmdHandler.h"
#include "zSocket.h"
#include "zMisc.h"
#include "SceneServer.h"

bool scFLCmdHandler::handleFLCmd(const std::string& content)
{
	fl_scene::PbMessage msg;
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

void scFLCmdHandler::Initialize()
{
	__RegisterHandleFunc(fl_scene::MSG_ID_TEST_RETURN, std::bind(&scFLCmdHandler::handleTestReturn, this, std::placeholders::_1));
}

void scFLCmdHandler::__RegisterHandleFunc (fl_scene::MSG_ID msgid, HANDLE_FUNC func)
{
	m_HandleFuncMap.insert (make_pair (msgid, func));
}

bool scFLCmdHandler::handleTestReturn(const std::string& cmd_content)
{
	fl_scene::PbMsgTestReturn msg;
	if (!msg.ParseFromString(cmd_content))
	{
		Fir::logger->error("[解析FL指令]，PbMsgTest失败");
		return false;
	}
	
	std::string talk = msg.talk();
	Fir::logger->info("[解析FL指令]:测试成功,talk=%s",talk.c_str());
	return true;
}

bool scFLCmdHandler::sendCmdToFL(const void *pstrCmd, const int nCmdLen)
{
	if ((DWORD)nCmdLen > zSocket::MAX_DATASIZE)
	{
		Fir::logger->error("sendCmdToFL,发送消息过大:%d",nCmdLen);
		return false;
	}

	using namespace CMD::SUPER;

	BYTE buf[zSocket::MAX_DATASIZE];
	t_Scene2Super_Forward2FL*scmd=(t_Scene2Super_Forward2FL *)buf;
	constructInPlace(scmd);

	scmd->datasize = nCmdLen;
	bcopy(pstrCmd, scmd->data, nCmdLen);
	return SceneService::getMe().sendCmdToSuperServer(scmd, scmd->getSize());	
}

bool scFLCmdHandler::sendCmdToFL(const std::string& content)
{
	return sendCmdToFL(content.c_str(),content.size());
}

void scFLCmdHandler::testMsg()
{
	//Fir::logger->debug("%s", __PRETTY_FUNCTION__);

	fl_scene::PbMsgTest test;
	test.set_talk("[[[[[[[scene->allzone我爱你]]]]]]");
	sendCmdToFL(fl_scene::MSG_ID_TEST,test);
}