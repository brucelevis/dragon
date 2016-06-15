/**
 * \file	SceneUser.h
 * \version  	$Id: .h 37 2013-04-08 01:52:56Z  $
 * \author  	,
 * \date 	2013年04月07日 15时48分31秒 CST
 * \brief 	场景用户管理器定义
 *
 * 
 */

#ifndef _SCENEUSERMANAGER_H_
#define _SCENEUSERMANAGER_H_

#include <functional>
#include <map>
#include <unordered_map>

#include "SceneUser.h"
#include "zSingleton.h"
#include "TopListDataDef.h"

class  SceneUserManager: public Singleton<SceneUserManager>
{
	friend class Singleton<SceneUserManager>;
	private:

	SceneUserManager();
	~SceneUserManager();

	protected:
	std::unordered_map<DWORD,SceneUser*> m_mapUsers;

	public:

	void init();

	void print_online_roles();

	bool addUser(SceneUser *user);
	bool removeUser(SceneUser *user);

	SceneUser* getUserByID(DWORD charid);

	void update();

	void saveAll();
	void delAll();
	// 全体玩家下线
	void offlineAll();

	typedef std::function<void (SceneUser*)> UserFunction;
	void execAll(UserFunction func);

	public:

	void SendMsg2AllOnlinePlayer( Packet& pkt );

	public:

	std::unordered_map<DWORD,SceneUser*>& getUserByIDsMap(){return m_mapUsers;}

	DWORD getSceneUserCount(){return m_mapUsers.size();}

	void SendErrorMessage(DWORD charid, WORD MsgId, WORD ErrId)
	{
		SceneUser* u = getUserByID(charid);
		if(u == NULL) return;
		u->SendErrorMessage(MsgId,ErrId);
	}

	template<typename T1>
		void SendMessage2Ci(DWORD charid, WORD msgId, T1& p1)
		{	
			SceneUser* u = getUserByID(charid);
			if(u == NULL) return;
			u->SendMessage2Ci(msgId,p1);	
		}
};

#define  SEND_ERROR_MSG(charid,msgid,errid) \
{ \
	SceneUserManager::getMe().SendErrorMessage(charid, msgid, errid);	\
	print_error_msg(__FILE__,__LINE__,__FUNCTION__,charid,msgid,errid);\
}


#endif
