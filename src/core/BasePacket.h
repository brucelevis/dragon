/********************************************************************
	created:	2013/01/25
	created:	25:1:2013   15:31
	filename: 	E:\work\winne\Shooting\src\AgentServer\include\net\packet.h
	file path:	E:\work\winne\Shooting\src\AgentServer\include\net
	file base:	packet
	file ext:	h
	author:		piyejun
	
	purpose:	
*********************************************************************/

#ifndef __BASEPACKET_H__
#define __BASEPACKET_H__

#include <vector>
#include <string>
#ifndef WIN32
#include <string.h>
#endif
#include "zType.h"



#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__GNUC__)	// If using Microsoft or Borland or GNU C++ compiler, do the following.
	#pragma pack(push, 8)						// To confirm the alignment of the structure members is the same under any case. Otherwise, strange bugs will be resulted under multiple projects of different alignment.
												// (Please place this line after all other #include<>.)
#endif

#define  PACKET_TEMP_BUF_SIZE  128


class BasePacket : public std::vector<char>
{
public:

    BasePacket();
	BasePacket(int size);

	// only copy p's data to dest.
	BasePacket &operator<<(BasePacket &p);
	//BasePacket &operator>>(BasePacket &p);

	// only read psrc: m_position->end
	BasePacket& read(BasePacket &psrc);
	
	// template operator << for saving data into BasePacket
	template<typename T>
	BasePacket &operator<<(const T &value)
	{
		unsigned int newpos = m_Position + sizeof(T);
		if(newpos > size()) resize(newpos);

		memcpy(&(*this)[m_Position], &value, sizeof(T));
		m_Position = newpos;

		return (*this);
	}

	// template operator >> for extracting data from BasePacket
	template<typename T>
	BasePacket &operator>>(T &value)
	{
		if(sizeof(T) <= (size() - m_Position) && size()!=0)
		{
			memcpy(&value, &(*this)[m_Position], sizeof(T));
			m_Position += sizeof(T);
		}

		return (*this);
	}
	

	// in/out for ANSI string
	virtual BasePacket &operator<<(const std::string &str);
	BasePacket &operator<<(const char *str);
	BasePacket &operator<<(char *str);
	BasePacket &operator<<(const BitSet& b);

	BasePacket &operator>>(std::string &str);
	BasePacket &operator>>(char *str);
	BasePacket &operator>>(BitSet& b);

	// in/out for UNICODE string
	BasePacket &operator<<(const std::wstring &str);
	BasePacket &operator<<(const wchar_t *str);
	BasePacket &operator<<(wchar_t *str);
	BasePacket &operator>>(std::wstring &str);
	BasePacket &operator>>(wchar_t *str);

	int Write(const void *buf, int length);
	int Read(void *buf, int length);
	int ReadLine(char *buf, int bufsize);
	unsigned int SeekSet(int offset);
	unsigned int SeekCur(int offset);
	unsigned int SeekEnd(int offset);
	unsigned int Tell() const { return m_Position; }
	void Clear(){ clear(); m_Position = 0; }
	virtual bool IsEnd(){ return m_Position == (int)size(); }
	int getPosition(){return m_Position;}
	void resetPosition(){m_Position=0;}
protected:
	int			m_Position;
	
};


/**
 * class DeltaPacket
 * Differential packet to save packet transfer size
 */

class DeltaPacket : public std::vector<char>
{
public:

    DeltaPacket():m_bEncoded(false){}
    
    bool SetLatestVersion(const DeltaPacket &p);
    bool SetLatestVersion(const BasePacket &buf);
    BasePacket GetLatestVersion() const { return m_LatestVersion; }
    bool IsEncoded() const { return m_bEncoded; }

public:

    BasePacket m_LatestVersion;
    bool m_bEncoded;
    
    friend BasePacket &operator<<(BasePacket &p, const DeltaPacket &diff);
    friend BasePacket &operator>>(BasePacket &p, DeltaPacket &diff);
};

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__GNUC__)	// If using Microsoft or Borland or GNU C++ compiler, do the following.
	#pragma pack(pop)							// Restore member alignment size. (Do not affect outside world)
#endif

#endif

