
#include "EncDec/EncDec.h"
#include "zMisc.h"
#include "HttpTask.h"
#include "HttpTaskManager.h"

/**
 * \brief 向容器中添加一个连接
 *
 * \param task 一个连接任务
 * \return 添加是否成功
 */
bool HttpTaskManager::add(HttpTask *task)
{
	if (task)
	{
		zRWLock_scope_wrlock scope_wrlock(rwlock);
		task->setState(task->sync);
		task->genTempID();
		HttpTaskHashmap_const_iterator it = loginTaskSet.find(task->getTempID());
		if (it != loginTaskSet.end())
		{
			Fir::logger->error("向容器中添加一个连接error");
			return false;
		}
		DWORD taskID = task->getTempID();
		Fir::logger->debug("向容器中添加一个连接,ID:%d", taskID);
		std::pair<HttpTaskHashmap_iterator, bool> p = loginTaskSet.insert(HttpTaskHashmap_pair(task->getTempID(), task));
		return p.second;
	}
	else
	{
		Fir::logger->error("连接任务error");
		return false;
	}
}

/**
 * \brief 从一个容器中移除一个连接
 *
 * \param task 一个连接任务
 */
void HttpTaskManager::remove(HttpTask *task)
{
	if (task)
	{
		zRWLock_scope_wrlock scope_wrlock(rwlock);
		Fir::logger->debug("从容器中删除一个连接,ID:%d", task->getTempID());
		loginTaskSet.erase(task->getTempID());
	}
}

/**
 * \brief 广播指令到指定的登陆连接
 *
 * \param loginTempID 登陆连接的唯一编号
 * \param pstrCmd 待转发的指令
 * \param nCmdLen 待转发的指令长度
 * \return 转发是否成功
 */

bool HttpTaskManager::broadcast(const DWORD loginTempID, const void *pstrCmd, int nCmdLen)
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);
	HttpTaskHashmap_iterator it = loginTaskSet.find(loginTempID);
	if (it != loginTaskSet.end())
		return it->second->sendCmd(pstrCmd, nCmdLen);
	else
	{
		Fir::logger->info("广播指令到指定的登陆连接error，连接ID = %u", loginTempID);
		return false;
	}
}

void HttpTaskManager::verifyReturn(const DWORD loginTempID, std::string buf)
{
	zRWLock_scope_rdlock scope_rdlock(rwlock);

	HttpTaskHashmap_iterator it = loginTaskSet.find(loginTempID);
	if (it == loginTaskSet.end())
		return;
	it->second->sendCmd(buf.c_str(), buf.size());
	it->second->Terminate();
}

void HttpTaskManager::verifyReturn(const DWORD loginTempID,const BYTE retcode ,std::string buf)
{
	//zRWLock_scope_rdlock scope_rdlock(rwlock);
	//DWORD tempID = (loginTempID-HTTP_TYPE)/CONNECT_TYPENUM;
	//HttpTaskHashmap_iterator it = loginTaskSet.find(tempID);
	//if (it != loginTaskSet.end())
	//{
		//HttpTask *task = it->second;
		//switch(retcode)
		//{
			//case GETROLE_OK:
				//{
					//task->LoginReturn(OK , buf);
				//}
				//break;
			//case GETROLENAME_NONE:
				//{
					//Fir::logger->debug("未查找到任何角色名");
					//task->LoginReturn(GETROLE_NONE);
				//}
				//break;
			//case GETROLENAME_FAILURE:	
				//{
					//Fir::logger->debug("查找角色名失败");
					//task->LoginReturn(GETROLE_ERR);
				//}
				//break;
			//case GETMESSAGE_NONE:
				//{
					//Fir::logger->debug("未查找角色信息");
					//task->LoginReturn(GETMSG_NONE);
				//}
				//break;
			//case GETMESSAGE_FAILURE:
				//{
					//Fir::logger->debug("查找角色信息失败");
					//task->LoginReturn(GETMSG_ERR);
				//}
				//break;
			//case UPDATEOFFICER_FAILURE:
				//{
					//Fir::logger->debug("更新角色史官等级失败");
					//task->LoginReturn(UPDOFF_ERR);
				//}
				//break;
			//case SYSTEM_ERROR:
				//{
					//Fir::logger->debug("系统错误");
					//task->LoginReturn(SYS_ERR);
				//}
				//break;
		//}
	//}
	//else
		//Fir::logger->error("发送指令到指定连接error,%d",loginTempID);
}

