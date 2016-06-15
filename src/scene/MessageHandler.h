/**
 * Copyright 2015 GDE Inc.
 *
 * \author huangxiaohui
 * \date 2015-12-07 10:49:15 CST
 * \brief 用来处理用户基本的消息回调.
 *
 */
#ifndef SCENESERVER_MESSAGEHANDLER_H_
#define SCENESERVER_MESSAGEHANDLER_H_

#include <map>
#include <functional>

#include "zType.h"
#include "zNoncopyable.h"

class SceneUser;
namespace FIGHTING { class Packet; }

/**
 * \brief 用户消息处理基类. 
 */
class MessageHandler : public zNoncopyable
{
	// 消息回调函数
	typedef std::function<void (DWORD, FIGHTING::Packet*)> HandleFunc;
	// 消息回调列表
	typedef std::map<WORD, HandleFunc> HandleFuncMap;

public:
	/**
	 * \brief 注册消息回调
	 * \param msgid 消息ID
	 * \param func 消息回调函数
	 */
	void __RegisterHandleFunc (WORD msgid, HandleFunc func);
	/**
	 * \brief 收到消息
	 * \param pUser 玩家
	 * \param msgid 消息ID
	 * \param pkt 消息内容
	 */
	void onMessage(SceneUser* pUser, WORD msgid, FIGHTING::Packet* pkt);

protected:
	HandleFuncMap handle_func_map_;
};

#endif