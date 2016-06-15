/**
 * \file
 * \version  $Id: zTCPTask.cpp 42 2013-04-10 07:33:59Z  $
 * \author  ,@163.com
 * \date 2004年11月18日 14时19分29秒 CST
 * \brief 实现线程池类，用于处理多连接服务器
 *
 * 
 */


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>

#include "zSocket.h"
#include "zThread.h"
#include "zTCPTask.h"
#include "zTCPTaskPool.h"
#include "Fir.h"

//CmdAnalysis zTCPTask::analysis("Task指令接收统计",600);
/**
 * \brief 向套接口发送指令，如果缓冲标志设置，则发送是直接拷贝到缓冲区队列中，实际的发送动作在另外一个线程做
 *
 *
 * \param pstrCmd 待发送的指令
 * \param nCmdLen 待发送指令的大小
 * \return 发送是否成功
 */
bool zTCPTask::sendCmd(const void *pstrCmd, int nCmdLen)
{
	//Fir::logger->debug(__PRETTY_FUNCTION__);
	//static CmdAnalysis analysis("Task指令发送统计",600);
	/*
	CMD::t_NullCmd *ptNullCmd = (CMD::t_NullCmd *)pstrCmd;
	analysis.add(ptNullCmd->cmd,ptNullCmd->para,nCmdLen);
	// */
	return mSocket.sendCmd(pstrCmd, nCmdLen, buffered);
}

bool zTCPTask::sendCmdNoPack(const void *pstrCmd, int nCmdLen)
{
	return mSocket.sendCmdNoPack(pstrCmd, nCmdLen, buffered);
}

/**
 * \brief 从套接口中接受数据，并且拆包进行处理，在调用这个函数之前保证已经对套接口进行了轮询
 *
 * \param needRecv 是否需要真正从套接口接受数据，false则不需要接收，只是处理缓冲中剩余的指令，true需要实际接收数据，然后才处理
 * \return 接收是否成功，true表示接收成功，false表示接收失败，可能需要断开连接 
 */
bool zTCPTask::ListeningRecv(bool needRecv)
{
	int retcode = 0;
	if (needRecv) {
		retcode = mSocket.recvToBuf_NoPoll();
	}
	//struct timeval tv_2;
	if (-1 == retcode)
	{
		//Fir::logger->error("%s", __PRETTY_FUNCTION__);
		return false;
	}
	else
	{
		do
		{
			unsigned char pstrCmd[zSocket::MAX_DATASIZE];
			int nCmdLen = mSocket.recvToCmd_NoPoll(pstrCmd, sizeof(pstrCmd));
			if (nCmdLen <= 0)
			{
				//这里只是从缓冲取数据包，所以不会出错，没有数据直接返回
				break;
			}
			else if (nCmdLen < (int)sizeof(CMD::t_NullCmd))
			{
				Fir::logger->warn("服务器接收到错误大小的数据包，数据包大小 = %u", nCmdLen);
				return false;
			}
			else
			{
				const CMD::t_NullCmd *ptNullCmd = (CMD::t_NullCmd *)pstrCmd;
				// 请求序号检查
				if (!checkReq(ptNullCmd->_req))
				{
					Fir::logger->warn("服务器接收消息请求序号错误，服务端序号 = %u，客户端序号 = %u", getReq(), ptNullCmd->_req);
					return false;
				}
			
				if (CMD::CMD_SERVER_HEART == ptNullCmd->_id)
				{
					//接收到客户端返回的测试指令，需要清除标志
					//Fir::logger->debug("服务器接收心跳消息,清除标志,check_count=%u",check_count);
					check_count = 0;
					clearTick();
				}
				else if (CMD::CMD_CLINET_HEART == ptNullCmd->_id)
				{
					//接收到客户端发送的测试指令，需要按照原始指令返回
					//Fir::logger->debug("服务端收到客户端发送的测试信号，反射信号");
					sendCmd(pstrCmd, nCmdLen);
				}

				else
				{
					msgParse(ptNullCmd, nCmdLen);
					/*
					   analysis.add(ptNullCmd->cmd,ptNullCmd->para,nCmdLen);
					// */
				}
			}
		}
		while(true);
	}

	return true;
}

/**
 * \brief 发送缓冲中的数据到套接口，再调用这个之前保证已经对套接口进行了轮询
 *
 * \return 发送是否成功，true表示发送成功，false表示发送失败，可能需要断开连接
 */
bool zTCPTask::ListeningSend()
{
	return mSocket.sync();
}

/**
 * \brief 把TCP连接任务交给下一个任务队列，切换状态
 *
 */
void zTCPTask::getNextState()
{
	zTCPTask_State old_state = getState();

	switch(old_state)
	{
		case notuse:
			setState(verify);
			break;
		case verify:
			setState(sync);
			break;
		case sync:
			buffered = true;
			addToContainer();
			setState(okay);
			break;
		case okay:
			removeFromContainer();
			setState(recycle);
			break;
		case recycle:
			setState(notuse);
			break;
	}

	Fir::logger->debug("%s(%s, %s -> %s)", __FUNCTION__,  getIP(), getStateString(old_state), getStateString(getState()));
}

/**
 * \brief 重值连接任务状态，回收连接
 *
 */
void zTCPTask::resetState()
{
	zTCPTask_State old_state = getState();

	switch(old_state)
	{
		case notuse:
		/*
		 * whj 
		 * 如果sync情况下添加到okay管理器失败会出现okay状态resetState的可能性
		 */
		//case okay:
		case recycle:
			//不可能的
			Fir::logger->fatal("%s:不可能 recycle -> recycle", __FUNCTION__);
			break;
		case verify:
		case sync:
		case okay:
			//TODO 相同的处理方式
			break;
	}

	setState(recycle);
	Fir::logger->debug("%s(%s, %s -> %s)", __FUNCTION__,  getIP(), getStateString(old_state), getStateString(getState()));
}

void zTCPTask::checkSignal(const zRTime &ct)
{
	if (ifCheckSignal())
	{
		if (checkInterval(ct))
		{
			if (checkTick())
			{
				//Fir::logger->debug("%s,心跳ss,step3", __FUNCTION__);
				if (check_count > 6)
				{
					//测试信号在指定时间范围内没有返回
					Fir::logger->info("套接口检查测试信号失败,check_count=%u:(%s:%u),发送缓冲大小: %u", check_count, mSocket.getIP(), mSocket.getPort(), mSocket.snd_queue_size());
					Terminate(zTCPTask::terminate_active);
				}
				else
				{
					check_count++;
					CMD::t_ServerNullCmd tNullCmd;
					//Fir::logger->debug("服务端发送测试信号");
					if (sendCmd(&tNullCmd, sizeof(tNullCmd)))
						setTick();

					//Fir::logger->debug("服务器发送心跳消息,check_count=%u",check_count);
				}
			}
			else
			{
				//发送测试信号
				CMD::t_ServerNullCmd tNullCmd;
				//Fir::logger->debug("服务端发送测试信号");
				if (sendCmd(&tNullCmd, sizeof(tNullCmd)))
					setTick();
				//Fir::logger->debug("服务器发送心跳消息,check_count=%u",check_count);
			}
		}
	}
}

bool zTCPTask::checkReq(unsigned char req)
{
	if (ifCheckReq())
	{
		return ++_req == req;
	}
	else
	{
		return true;
	}
}

