/**
 * \file
 * \version  $Id: zNullCmd.h 11 2013-04-01 10:17:41Z  $
 * \author  ,@163.com
 * \date 2004年12月11日 14时36分43秒 CST
 * \brief 定义空的指令
 *
 * 负责服务器内部交换使用，和客户端交互的指令需要另外定义
 * 
 */


#ifndef _zNullCmd_h_
#define _zNullCmd_h_

#include "zType.h"

#pragma pack(1)


namespace CMD
{
	//const BYTE CMD_NULL = 0;		/**< 空的指令 */
	//const BYTE PARA_NULL = 0;		/**< 空的指令参数 */

	const WORD CMD_SERVER_HEART = 0;
	const WORD CMD_CLINET_HEART  =1;

	struct t_NullCmd 
	{
		explicit t_NullCmd (const unsigned short id) : _id(id), _req(0)
		{   
		}   


		explicit t_NullCmd(const BYTE _cmd, const BYTE _para) : cmd(_cmd), para(_para), _req(0)
		{   
		}   

		union
		{   
			unsigned short  _id;
			struct
			{
				unsigned char byCmd;
				unsigned char byParam;
			};
			struct
			{
				unsigned char cmd; 
				unsigned char para;
			};

		};  
		unsigned char _req;
	};


	template <unsigned short cmdid>
		struct ServerCmdTemplate : public t_NullCmd 
	{
		enum { ID = cmdid };
		ServerCmdTemplate() : t_NullCmd(cmdid) {}
	};

#define	DECLARE_CMD(name,NAMESPACE)	\
name : public ServerCmdTemplate<CMD::NAMESPACE::name>

	//const BYTE SERVER_PARA_NULL = 0;
	struct t_ServerNullCmd:public t_NullCmd
	{
		t_ServerNullCmd():t_NullCmd(CMD_SERVER_HEART)
		{
		}
	};
	
	//const BYTE CLIENT_PARA_NULL = 1;
	struct t_ClientNullCmd:public t_NullCmd
	{
		t_ClientNullCmd():t_NullCmd(CMD_CLINET_HEART)
		{
		}
	};

	const BYTE NULL_USERCMD_PARA = 0;
	struct stNullUserCmd : public t_NullCmd {
		stNullUserCmd() : t_NullCmd(0,NULL_USERCMD_PARA)
		{
			dwTimestamp = 0;
		}

		DWORD   dwTimestamp;
	};
};

#pragma pack()

#endif

