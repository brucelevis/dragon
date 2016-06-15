/**
 * \file	RecordUserManager.h
 * \version  	$Id: RecordUserManager.h 64 2013-04-23 02:05:08Z  $
 * \author  	, @ztgame.com 
 * \date 	2007年01月30日 08时15分17秒 CST
 * \brief 	档案用户管理器定义
 *
 * 
 */


#ifndef RECORDUSER_MANAGER_H
#define RECORDUSER_MANAGER_H

#include <unordered_map>
#include <unordered_set>
#include "zSingleton.h"
#include "zMisc.h"
#include "zRWLock.h"
#include "RecordCommand.h"
#include "common.h"

class RecordUser;

class RecordUserM : public Fir::Singleton<RecordUserM>
{
	public:

		/**
		 * \brief 默认构造函数
		 *
		 */
		RecordUserM() 
		{
		};

		~RecordUserM();

		bool init();
		bool add(RecordUser* u);

		/**
		 * \brief 通过角色ID获取角色
		 * \param charid 角色ID
		 * \return 角色
		 */
		RecordUser* getUserByCharid(DWORD charid);
		/**
		 * \brief 通过昵称获取角色ID
		 * \param nickname 昵称
		 * \return 角色ID，0为获取失败
		 */
		DWORD getCharidByNickname(const std::string& nickname);

		/**
		 * \brief 检查是否存在角色ID
		 * \param charid 角色ID
		 * \return 是\否
		 */
		bool existCharid(DWORD charid);
		/**
		 * \brief 检查是否存在昵称
		 * \param nickname 昵称
		 * \return 是\否
		 */
		bool existNickname(std::string nickname);
		
	private:

		typedef std::unordered_map<DWORD, RecordUser*> RecordUserHashmap;
		typedef RecordUserHashmap::iterator RecordUserHashmap_iterator;
		typedef RecordUserHashmap::value_type RecordUserHashmap_pair;
		RecordUserHashmap userMap;
			
		std::unordered_map<std::string, DWORD> m_mapNickNames;

		/**
		 * \brief 互斥变量
		 *
		 */
		zMutex mlock;
};

#endif
