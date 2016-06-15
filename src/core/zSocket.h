/**
 * \file
 * \version  $Id: zSocket.h 13 2013-03-20 02:35:18Z  $
 * \author  ,@163.com
 * \date 2004年11月09日 16时05分44秒 CST
 * \brief 定义zSocket类，用于对套接口底层进行封装
 */

#ifndef _zSocket_h_
#define _zSocket_h_

#include <errno.h>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <zlib.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <assert.h>
#include <iostream>
#include <queue>
#include <ext/pool_allocator.h>
#include <ext/mt_allocator.h>

#include "zNoncopyable.h"
#include "zMutex.h"
#include "zNullCmd.h"
#include "Fir.h"
#include "zTime.h"

#include "EncDec/EncDec.h"

#include <sys/poll.h>
#include <sys/epoll.h>

const unsigned int trunkSize = 64 * 1024;
#define unzip_size(zip_size) ((zip_size) * 120 / 100 + 12)
const unsigned int PACKET_ZIP_BUFFER	=	unzip_size(trunkSize - 1) + sizeof(unsigned int) + 8;	/**< 压缩需要的缓冲 */

#define CHECK_CURR_PTR(size) if(((unsigned int)-1-_currPtr) <= (size)) \
				Fir::logger->fatal("[缓冲异常]%s,%u,%u,%u",__PRETTY_FUNCTION__,_currPtr,(size),_maxSize) \
/**
 * 字节缓冲，用于套接口接收和发送数据的缓冲
 * \param _type 缓冲区数据类型
 */
template <typename _type>
class ByteBuffer
{

	public:

		/**
		 * 构造函数
		 */
		ByteBuffer();

		/**
		 * 向缓冲填入数据
		 * \param buf 待填入缓冲的数据
		 * \param size 待填入缓冲数据的长度
		 */
		inline void put(const unsigned char *buf, const unsigned int size)
		{
			//首先确认缓冲内存是否足够
			wr_reserve(size);

			bcopy(buf, &_buffer[_currPtr], size);
			
			CHECK_CURR_PTR(size);	
			_currPtr += size;
		}

		/**
		 * 得到当前可写bf的地址
		 * 保证在调用此函数写入数据之前需要调用wr_reserve(size)来预留缓冲区大小
		 * \return 可写入缓冲开始地址
		 */
		inline unsigned char *wr_buf()
		{
			return &_buffer[_currPtr];
		}

		/**
		 * 返回缓冲中有效数据的开始地址
		 * \return 有效数据地址
		 */
		inline unsigned char *rd_buf()
		{
			return &_buffer[_offPtr];
		}

		/**
		 * 判断缓冲中时候有有效数据
		 * \return 返回缓冲中是否有有效数据
		 */
		//inline bool rd_ready() const
		inline bool rd_ready() const
		{
        		if (_maxSize < _currPtr)
        		{       
                		Fir::logger->trace("[缓冲异常]%s,%u,%u,%u",__PRETTY_FUNCTION__,__LINE__,_currPtr, _maxSize);
        		}  
			return _currPtr > _offPtr;
		}

		/**
		 * 得到缓冲中有效数据的大小
		 * \return 返回缓冲中有效数据大小
		 */
		inline unsigned int rd_size() const
		{
		//	return (_currPtr - _offPtr)>(10*1024)?(10*1024):(_currPtr - _offPtr);
        		if (_maxSize < _currPtr)
        		{       
                		Fir::logger->trace("[缓冲异常]%s,%u,%u,%u",__PRETTY_FUNCTION__,__LINE__,_currPtr, _maxSize);
        		}  
			return _currPtr - _offPtr;
		}

		/**
		 * 当缓冲的有效数据被使用以后，需要对缓冲进行整理
		 * \param size 最后一次使用的有效数据长度
		 */
		inline void rd_flip(unsigned int size)
		{
			_offPtr += size;
			if (_currPtr > _offPtr)	//_currPtr一定要大于_offPtr,否则需要报警
			{
				unsigned int tmp = _currPtr - _offPtr;
				if(_offPtr >= tmp)
				{
					memmove(&_buffer[0], &_buffer[_offPtr], tmp);
					_offPtr = 0;
					_currPtr = tmp;
				}
			}
			else
			{
				_offPtr = 0;
				_currPtr = 0;
			}
		}

		/**
		 * 得到缓冲可写入数据的大小
		 * \return 可写入数据的大小
		 */
		inline unsigned int wr_size() const
		{
			return _maxSize - _currPtr;
		}

		/**
		 * 实际向缓冲写入了数据，需要对缓冲进行整理
		 * \param size 实际写入的数据
		 */
		inline void wr_flip(const unsigned int size)
		{
			CHECK_CURR_PTR(size);
			_currPtr += size;
		}

		/**
		 * 重值缓冲中的数据，清空无用的垃圾数据
		 */
		inline void reset()
		{
			_offPtr = 0;
			_currPtr = 0;
		}

		/**
		 * 返回缓冲最大大小
		 * \return 缓冲最大大小
		 */
		inline unsigned int maxSize() const
		{
			return _maxSize;
		}

		inline unsigned int offPtr() const
		{
			return _offPtr;
		}

		inline unsigned int currPtr() const
		{
			return _currPtr;
		}

		/**
		 * 对缓冲的内存进行重新整理，向缓冲写数据，如果缓冲大小不足，重新调整缓冲大小，
		 * 大小调整原则按照trunkSize的整数倍进行增加
		 * \param size 向缓冲写入了多少数据
		 */
		inline void wr_reserve(const unsigned int size);

	private:

		unsigned int _maxSize;
		unsigned int _offPtr;
		unsigned int _currPtr;
		_type _buffer;

};

/**
 * 动态内存的缓冲区，可以动态扩展缓冲区大小
 */
typedef ByteBuffer<std::vector<unsigned char> > t_BufferCmdQueue;

/**
 * 模板偏特化
 * 对缓冲的内存进行重新整理，向缓冲写数据，如果缓冲大小不足，重新调整缓冲大小，
 * 大小调整原则按照trunkSize的整数倍进行增加
 * \param size 向缓冲写入了多少数据
 */
template <>
inline void t_BufferCmdQueue::wr_reserve(const unsigned int size)
{
	if (_maxSize < _currPtr)
	{
		Fir::logger->trace("[缓冲异常]%s,%u,%u,%u",__PRETTY_FUNCTION__,__LINE__, _currPtr, _maxSize);
	}

	if (wr_size() < (size+8))
	{
#define trunkCount(size) (((size) + trunkSize - 1) / trunkSize)
		unsigned int midd = (trunkSize * trunkCount(size+8));
		if(((unsigned int)-1-_maxSize)<=midd) Fir::logger->fatal("[缓冲异常]%s,%u,%u,%u,%u",__PRETTY_FUNCTION__,__LINE__,_currPtr,midd,_maxSize);
		_maxSize += midd;

		_buffer.resize(_maxSize);

		/*
		_buffer.reserve(_maxSize);
		if (_buffer.capacity() < size)
		{
			Fir::logger->trace("%s: 内存分配异常,实际大小: %u, 要求大小: %u, 需要大小: %u", __PRETTY_FUNCTION__, _buffer.capacity(), _maxSize, size);
		}
		*/
	}
}


/**
 * 静态大小的缓冲区，以栈空间数组的方式来分配内存，用于一些临时变量的获取
 */
typedef ByteBuffer<unsigned char [PACKET_ZIP_BUFFER]> t_StackCmdQueue;

/**
 * 模板偏特化
 * 对缓冲的内存进行重新整理，向缓冲写数据，如果缓冲大小不足，重新调整缓冲大小，
 * 大小调整原则按照trunkSize的整数倍进行增加
 * \param size 向缓冲写入了多少数据
 */
template <>
inline void t_StackCmdQueue::wr_reserve(const unsigned int size)
{
	/*
	if (wr_size() < size)
	{
		//不能动态扩展内存
		assert(false);
	}
	// */
}

/**
 * \brief 封装套接口底层函数，提供一个比较通用的接口
 */
class zSocket : private zNoncopyable
{

	public:

		static const int T_RD_MSEC					=	2100;					/**< 读取超时的毫秒数 */
		static const int T_WR_MSEC					=	5100;					/**< 发送超时的毫秒数 */

		static const unsigned int PH_LEN 			=	sizeof(unsigned int);	/**< 数据包包头大小 */
		static const unsigned int HTTP_MIN_LEN 		=	5;						/**< 数据包Http最小大小 */
		static const unsigned int PACKET_ZIP_MIN	=	32;						/**< 数据包压缩最小大小 */

		static const unsigned int PACKET_ZIP		=	0x40000000;				/**< 数据包压缩标志 */
		static const unsigned int INCOMPLETE_READ	=	0x00000001;				/**< 上次对套接口进行读取操作没有读取完全的标志 */
		static const unsigned int INCOMPLETE_WRITE	=	0x00000002;				/**< 上次对套接口进行写入操作煤油写入完毕的标志 */

		static const unsigned int PACKET_MASK			=	trunkSize - 1;	/**< 最大数据包长度掩码 */
		static const unsigned int MAX_DATABUFFERSIZE	=	PACKET_MASK;						/**< 数据包最大长度，包括包头4字节 */
		static const unsigned int MAX_DATASIZE			=	(MAX_DATABUFFERSIZE - PH_LEN);		/**< 数据包最大长度 */
		static const unsigned int MAX_USERDATASIZE		=	(MAX_DATASIZE - 128);				/**< 用户数据包最大长度 */

		static const char *getIPByIfName(const char *ifName);

		zSocket(const int sock, const struct sockaddr_in *addr = NULL, const bool compress = false);
		~zSocket();

		int recvToCmd(void *pstrCmd, const int nCmdLen, const bool wait);
		bool sendCmd(const void *pstrCmd, const int nCmdLen, const bool buffer = false);
		bool sendCmdNoPack(const void *pstrCmd, const int nCmdLen, const bool buffer = false);
		bool sync();
		void force_sync();

		int checkIOForRead();
		int checkIOForWrite();
		int recvToBuf_NoPoll();
		int recvToBuf_NoPoll(void *pstrCmd, const int nCmdLen);
		int recvToCmd_NoPoll(void *pstrCmd, const int nCmdLen);
		int recvToHttpCmd_NoPoll(void *pstrCmd, const int nCmdLen);

		/**
		 * \brief 获取套接口对方的地址
		 * \return IP地址
		 */
		inline const char *getIP() const { return inet_ntoa(addr.sin_addr); }
		inline const DWORD getAddr() const { return addr.sin_addr.s_addr; }

		/**
		 * \brief 获取套接口对方端口
		 * \return 端口
		 */
		inline const unsigned short getPort() const { return ntohs(addr.sin_port); }

		/**
		 * \brief 获取套接口本地的地址
		 * \return IP地址
		 */
		inline const char *getLocalIP() const { return inet_ntoa(local_addr.sin_addr); }

		/**
		 * \brief 获取套接口本地端口
		 * \return 端口
		 */
		inline const unsigned short getLocalPort() const { return ntohs(local_addr.sin_port); }

		/**
		 * \brief 设置读取超时
		 * \param msec 超时，单位毫秒 
		 * \return 
		 */
		inline void setReadTimeout(const int msec) { rd_msec = msec; }

		/**
		 * \brief 设置写入超时
		 * \param msec 超时，单位毫秒 
		 * \return 
		 */
		inline void setWriteTimeout(const int msec) { wr_msec = msec; }

		/**
		 * \brief 添加检测事件到epoll描述符
		 * \param kdpfd epoll描述符
		 * \param events 待添加的事件
		 * \param ptr 额外参数
		 */
		inline bool addEpoll(int kdpfd, __uint32_t events, void *ptr)
		{
			struct epoll_event ev;
			ev.events = events;
			ev.data.ptr = ptr;
			if (-1 == epoll_ctl(kdpfd, EPOLL_CTL_ADD, sock, &ev))
			{
				char _buf[100];
				bzero(_buf, sizeof(_buf));
				strerror_r(errno, _buf, sizeof(_buf));
				Fir::logger->fatal("%s:%s", __FUNCTION__, _buf);
				return false;
			}

			return true;
		}
		/**
		 * \brief 从epoll描述符中删除检测事件
		 * \param kdpfd epoll描述符
		 * \param events 待添加的事件
		 */
		inline bool delEpoll(int kdpfd, __uint32_t events)
		{
			struct epoll_event ev;
			ev.events = events;
			ev.data.ptr = NULL;
			if (-1 == epoll_ctl(kdpfd, EPOLL_CTL_DEL, sock, &ev))
			{
				char _buf[100];
				bzero(_buf, sizeof(_buf));
				strerror_r(errno, _buf, sizeof(_buf));
				Fir::logger->fatal("%s:%s", __FUNCTION__, _buf);
				return false;
			}

			return true;
		}
		/**
		 * \brief 填充pollfd结构
		 * \param pfd 待填充的结构
		 * \param events 等待的事件参数
		 */
		inline void fillPollFD(struct pollfd &pfd, short events)
		{
			pfd.fd = sock;
			pfd.events = events;
			pfd.revents = 0;
		}

		inline void setEncMethod(CEncrypt::encMethod m) { enc.setEncMethod(m); }
		inline void set_key_rc5(const unsigned char *data, int nLen, int rounds = RC5_8_ROUNDS) { enc.set_key_rc5(data, nLen, rounds); }
		inline void set_key_des(const_DES_cblock *key) { enc.set_key_des(key); }
		inline void set_key_rc4(const unsigned char *data, int nLen) { enc.set_key_rc4(data, nLen); }
		inline void setEncMask(const unsigned int m){enc.setEncMask(m);}
		inline void setDecMask(const unsigned int m){enc.setDecMask(m);}

		inline unsigned int snd_queue_size() { return _snd_queue.rd_size() + _enc_queue.rd_size(); }

		bool sendRawDataIM(const void *pBuffer, const int nSize);

		inline unsigned int getBufferSize() const {return _rcv_queue.maxSize() + _snd_queue.maxSize();}
	private:
		int sock;									/**< 套接口 */
		struct sockaddr_in addr;					/**< 套接口地址 */
		struct sockaddr_in local_addr;				/**< 套接口地址 */
		int rd_msec;								/**< 读取超时，毫秒 */
		int wr_msec;								/**< 写入超时，毫秒 在内核停留*/

		t_BufferCmdQueue _rcv_queue;				/**< 接收缓冲指令队列 */
		unsigned int _rcv_raw_size;					/**< 接收缓冲解密数据大小 */
		t_BufferCmdQueue _snd_queue;				/**< 加密缓冲指令队列 */
		t_BufferCmdQueue _enc_queue;				/**< 加密缓冲指令队列 */
		unsigned int _current_cmd;
		zMutex mutex;								/**< 锁 */

		zTime last_check_time;						/**< 最后一次检测时间 */

		unsigned int bitmask;						/**< 标志掩码 */
		CEncrypt enc;								/**< 加密方式 */

		inline void set_flag(unsigned int _f) { bitmask |= _f; }
		inline bool isset_flag(unsigned int _f) const { return bitmask & _f; }
		inline void clear_flag(unsigned int _f) { bitmask &= ~_f; }
		inline bool need_enc() const { return CEncrypt::ENCDEC_NONE!=enc.getEncMethod(); }
		/**
		 * \brief 返回数据包包头最小长度
		 * \return 最小长度
		 */
		inline unsigned int packetMinSize() const { return PH_LEN; }

		/**
		 * \brief 返回整个数据包的长度
		 * \param in 数据包
		 * \return 返回整个数据包的长度
		 */
		inline unsigned int packetSize(const unsigned char *in) const { return PH_LEN + ((*((unsigned int *)in)) & PACKET_MASK); }
		
		/**
		 * \brief 返回Http数据包包头最小长度
		 * \return 最小长度
		 */
		inline unsigned int packetHttpMinSize() const { return HTTP_MIN_LEN; }
		/**
		 * \brief 返回整个http数据包的长度
		 * \param in 数据包
		 * \return 返回整个数据包的长度
		 */
		unsigned int packetHttpSize(const char *in) const;

		inline int sendRawData(const void *pBuffer, const int nSize);
		//inline bool sendRawDataIM(const void *pBuffer, const int nSize);
		inline int sendRawData_NoPoll(const void *pBuffer, const int nSize);
		inline bool setNonblock();
		inline int waitForRead();
		inline int waitForWrite();
		inline int recvToBuf();

		inline unsigned int packetUnpack(unsigned char *in, const unsigned int nPacketLen, unsigned char *out);
		template<typename buffer_type>
		inline unsigned int packetAppend(const void *pData, const unsigned int nLen, buffer_type &cmd_queue);
		template<typename buffer_type>
		inline unsigned int packetAppendNoEnc(const void *pData, const unsigned int nLen, buffer_type &cmd_queue);
		template<typename buffer_type>
		inline unsigned int packetPackEnc(buffer_type &cmd_queue, const unsigned int current_cmd, const unsigned int offset = 0);
		template<typename buffer_type>
		static inline unsigned int packetPackZip(const void *pData, const unsigned int nLen, buffer_type &cmd_queue, const bool _compress = true);
};

/**
 * \brief 定义了消息处理接口，所有接收到的TCP数据指令需要通过这个接口来处理
 */
class zProcessor
{
	public:
		virtual bool msgParse(const CMD::t_NullCmd *, const unsigned int) = 0;

		static void PrintDataRecv(const void *pstrCmd, int nCmdLen);
		static void PrintDataSend(const void *pstrCmd, int nCmdLen);
};

#endif
