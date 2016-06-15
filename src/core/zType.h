/**
 * \file
 * \version  $Id: zType.h 15 2013-04-10 10:29:09Z  $
 * \author  ,@163.com
 * \date 2004年12月15日 19时16分10秒 CST
 * \brief 定义基本类型
 *
 * 
 */

#ifndef _zType_h_
#define _zType_h_

#include <time.h>
#include <strings.h>
#include <bitset>
#include <vector>

#define SAFE_DELETE(x) { if (x) { delete (x); (x) = NULL; } }
#define SAFE_DELETE_VEC(x) { if (x) { delete [] (x); (x) = NULL; } }


typedef signed char				INT8, *PINT8;
typedef signed short				INT16, *PINT16;
typedef signed int					INT32, *PINT32;

typedef unsigned char			UINT8, *PUINT8;
typedef unsigned short			UINT16, *PUINT16;
typedef unsigned int				UINT32, *PUINT32;

typedef signed long long		INT64, *PINT64;
typedef unsigned long long  UINT64, *PUINT64;


/**
 * \brief 单字节字符
 *
 */
typedef char CHAR;
/**
 * \brief 单字节无符号整数
 *
 */
typedef unsigned char BYTE;

/**
 * \brief 双字节无符号整数
 *
 */
typedef unsigned short WORD;

/**
 * \brief 双字节符号整数
 *
 */
typedef signed short SWORD;

/**
 * \brief 双字节符号整数
 *
 */
typedef short SHORT;

/**
 * \brief 四字节无符号整数
 *
 */
typedef unsigned int DWORD;

/**
 * \brief 四字节符号整数
 *
 */
typedef signed int SDWORD;

/**
 * \brief 四字节符号整数
 *
 */
typedef int INT;

/**
 * \brief 八字节无符号整数
 *
 */
typedef unsigned long long QWORD;

/**
 * \brief 八字节符号整数
 *
 */
typedef signed long long SQWORD;

/**
 * \brief 名字的最大长度
 */
#define MAX_NAMESIZE 32

/**
 * \brief MAC地址的最大长度
 */
#define MAX_MAC_ADDR 24

/**
 * \brief UUID最大长度
 */
#define MAX_IPHONE_UUID 40

/**
 * \brief 账号最大长度
 */
#define MAX_ACCNAMESIZE	48

/**
 * \brief IP地址最大长度
 *
 */
#define MAX_IP_LENGTH	16

// 最大平台字符串长度
#define MAX_FLAT_LENGTH 32

/**
 * \brief 密码最大长度
 *
 */
#define MAX_PASSWORD  16

/**
 * \brief MD5码长度
 *
 */
#define MD5_LENGTH	16

/**
 * \brief 屏宽
 */
#define SCREEN_WIDTH 13

#define MAX_CHATINFO	256
/**
 * \brief 屏高
 */
#define SCREEN_HEIGHT 19

#define MAX_HOSTSIZE	32
#define MAX_USERSIZE	32
#define MAX_DBSIZE		32
#define MAX_TOKENSIZE 32
#define MAX_MEM_DB 1

/*
 * \brief 最大角色ID的长度，（角色ID为数字）
 */
#define MAX_CHAR_ID_LENGTH (10)

/*
 * \brief 内部订单号中角色ID长度
 */
#define INNER_ORDER_ROLEID_LENGTH 6
/*
 * \brief 内部订单号中充值数量长度
 */
#define INNER_ORDER_RECHARGECOUNT_LENGTH 4
 /*
* \brief 内部订单号长度
 */
#define INNER_ORDER_LENGTH (INNER_ORDER_ROLEID_LENGTH + INNER_ORDER_RECHARGECOUNT_LENGTH)

/*
 * \brief 订单支付类型
*/
enum PayType
{
	PAYTYPE_NORMAL				= 4,	// 正常充值
	PAYTYPE_DIRECTCOMPENSATE	= 100,	// 手动直发补偿
	PAYTYPE_MANUALREPLENISMENT	= 101,	// 手动重发货
	PAYTYPE_MANUALREPAIRORDER	= 102,	// 手动补单
	PAYTYPE_AUTOREPAIRORDER		= 103,	// 自动补单
};

#define MAX_NOTIFY_LENGTH 160
#define MAX_BUFFER_LENGTH 1600
/**
 * \brief 连接线程池的状态标记位
 *
 */
enum {
	state_none	=	0,							/**< 空的状态 */
	state_maintain	=	1 ,						/**< 维护中，暂时不允许建立新的连接 */
	state_normal_maintain   =       2,                                      /**< 正常维护中，暂时不允许建立新的连接 */
};

/**
 * \brief 数字密码
 */
#ifndef MAX_NUMPASSWORD
#define MAX_NUMPASSWORD	32
#endif

void print_function_exception_log(const char*file, DWORD line, const char* func);


#define ENTER_FUNCTION {try{
						
#define LEAVEL_FUNCTION }catch(...){print_function_exception_log(__FILE__,__LINE__,__PRETTY_FUNCTION__);}}


#pragma pack(1)
/**
 * \brief 定义游戏区
 * 对游戏进行分类，然后在同种游戏中再分区
 */
//struct GameZone_t
//{
//	union
//	{
//		/**
//		 * \brief 唯一编号
//		 */
//		DWORD id;
//		struct
//		{
//			/**
//			 * \brief 游戏分区编号
//			 */
//			WORD zone;
//			/**
//			 * \brief 游戏种类编号
//			 */
//			WORD game;
//		};
//	};
//
//	GameZone_t()
//	{
//		this->game = 0;
//		this->zone = 0;
//	}
//	GameZone_t(const GameZone_t &gameZone)
//	{
//		this->id = gameZone.id;
//	}
//	GameZone_t & operator= (const GameZone_t &gameZone)
//	{
//		this->id = gameZone.id;
//		return *this;
//	}
//	bool operator== (const GameZone_t &gameZone) const
//	{
//		return this->id == gameZone.id;
//	}
//};
typedef unsigned int GameZone_t;

typedef std::vector<std::string>		STRING_VEC;
typedef std::vector<int>					INT_VEC;
typedef std::vector<UINT32>			UINT_VEC;
typedef std::vector<DWORD>			UINT_VEC;

class BitSet : public std::bitset<32>
{
public:
	void operator=(UINT32 u)
	{
		for (int i=0; i<32; ++i)
		{
			(u & (1 << i)) ? this->set(i, true) : this->set(i, false);
		}
	}
};


#define paserMessage(msg,msglen,pat)  \
do \
{ \
	va_list ap; \
	bzero(msg, msglen); \
	va_start(ap, pat);      \
	vsnprintf(msg, msglen - 1, pat, ap);    \
	va_end(ap); \
}while(false)

#define MAX_UZLIB_CHAR  (400 * 1024) 

#pragma pack()
#endif

