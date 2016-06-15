/********************************************************************
	created:	2013/01/25
	created:	25:1:2013   15:32
	filename: 	E:\work\winne\Shooting\src\AgentServer\src\net\packet.cpp
	file path:	E:\work\winne\Shooting\src\AgentServer\src\net
	file base:	packet
	file ext:	cpp
	author:		piyejun
	
	purpose:	
*********************************************************************/

#include "BasePacket.h"
#include "packet.h"
#include "StringUtil.h"


BasePacket::BasePacket():
	m_Position(0)
        { }

        BasePacket::BasePacket(int size):
        std::vector<char>(size),
        m_Position(0)
        {}


//&& modify at 20130204, for unified with client.
BasePacket &BasePacket::operator<<(const std::string &str)
{
	(*this)<<(int)str.size();
	Write(str.c_str(), str.size());
	return *this;
}

// Overload operator<< for c-string
BasePacket &BasePacket::operator<<(const char *str)
{
    Write(str, strlen(str)+1);
    return *this;
}

// Overload operator<< for c-string
BasePacket &BasePacket::operator<<(char *str)
{
    Write(str, strlen(str)+1);
    return *this;
}

BasePacket &BasePacket::operator<<(const BitSet& b)
{
	unsigned int newpos = m_Position + sizeof(DWORD);
	if(newpos > size()) resize(newpos);
	DWORD u;
	StringUtil::BitSet2DWORD(b, u);
	memcpy(&(*this)[m_Position], &u, sizeof(DWORD));
	m_Position = newpos;

	return (*this);
}

BasePacket &BasePacket::operator>>(BitSet& b)
{
	DWORD u;
	if(sizeof(DWORD) <= (size() - m_Position) && size()!=0)
	{
		memcpy(&u, &(*this)[m_Position], sizeof(DWORD));
		m_Position += sizeof(DWORD);
		StringUtil::DWORDtoBitSet(u, b);
	}

	return (*this);
}

// Overload operator>> for std::string
// BasePacket &BasePacket::operator>>(std::string &value)
// {
// 	if(m_Position < (int)size())
// 	{
// 		const char *src = &*(begin()+m_Position);
// 		int i, bufferSize = size() - m_Position;
// 
// 		for(i=0; i< bufferSize; i++)
// 		{
// 			if(src[i]==0)	// check null character
// 			{
// 				value = src;
// 				m_Position += i+1;
// 				break;
// 			}
// 		}
// 
// 		if(i == bufferSize) m_Position = size(); // force to end the packet
// 	}
// 
//     return *this;
// }

//&& modify at 20130204, for unified with client.
BasePacket &BasePacket::operator>>(std::string &value)
{
	int len = 0;
	(*this) >> len;
	if (len<=0)
	{
		value = "";
		return *this;
	}
	if(m_Position+len <= (int)size())
	{
		const char *src = &*(begin()+m_Position);
		value.resize(len);  // no need +1
#ifdef WIN32
		memcpy(value._Myptr(), src, len);
#else
		std::string str(src, len);
		value = str;
#endif
		
		m_Position += len;

		if(m_Position>(int)size()) m_Position = size(); // force to end the packet
	}

	return *this;
}

// Overload operator>> for c-string
BasePacket &BasePacket::operator>>(char *str)
{
	if(m_Position < (int)size())
	{
		const char *src = &*(begin()+m_Position);
		int i, bufferSize = size() - m_Position;

		for(i=0; i< bufferSize; i++)
		{
			if(src[i]==0)	// check null character
			{
				memcpy(str, src, i+1);
				m_Position += i+1;
				break;
			}
		}

		if(i == bufferSize) m_Position = size(); // force to end the packet
	}

    return *this;
}

// Overload operator<< for std::wstring
BasePacket &BasePacket::operator<<(const std::wstring &str)
{
    Write(str.c_str(), (str.size()+1)*sizeof(wchar_t));
    return *this;
}

// Overload operator<< for unicode string
BasePacket &BasePacket::operator<<(const wchar_t *str)
{
    Write(str, (wcslen(str)+1)*sizeof(wchar_t));
    return *this;
}

// Overload operator<< for unicode string
BasePacket &BasePacket::operator<<(wchar_t *str)
{
    Write(str, (wcslen(str)+1)*sizeof(wchar_t));
    return *this;
}

// Overload operator>> for std::wstring
BasePacket &BasePacket::operator>>(std::wstring &value)
{
	if(m_Position < (int)size())
	{
		const wchar_t *src = (const wchar_t *)&*(begin()+m_Position);
		int i, bufferSize = (size()-m_Position)/sizeof(wchar_t);

		for(i=0; i< bufferSize; i++)
		{
			if(src[i]==0)	// check null character
			{
				value = src;
				m_Position += (i+1)*sizeof(wchar_t);
				break;
			}
		}

		if(i == bufferSize) m_Position = size(); // force to end the packet
	}

    return *this;
}

// Overload operator>> for unicode string
BasePacket &BasePacket::operator>>(wchar_t *str)
{
	if(m_Position < (int)size())
	{
		const wchar_t *src = (const wchar_t *)&*(begin()+m_Position);
		int i, bufferSize = (size()-m_Position)/sizeof(wchar_t);

		for(i=0; i< bufferSize; i++)
		{
			if(src[i]==0)	// check null character
			{
				wcscpy(str, src);
				m_Position += (i+1)*sizeof(wchar_t);
				break;
			}
		}

		if(i == bufferSize) m_Position = size(); // force to end the packet
	}

    return *this;
}

// Write data into the packet.
int BasePacket::Write(const void *buf, int length)
{
	if(length > 0)
	{
		unsigned int newpos = m_Position + length;
		if(newpos>size()) resize(newpos);
     
		memcpy(&*(begin()+m_Position), buf, length);
		m_Position = newpos;
	}
     
    return length;
}

// Read data from the packet
int BasePacket::Read(void *buf, int length)
{
	if(length > 0)
	{
		int av = size() - m_Position;
		if(length > av) length = av;
     
		memcpy(buf, &*(begin()+m_Position), length);
		m_Position += length;
	}
     
    return length;
}

// Read a text-line from the packet.
int BasePacket::ReadLine(char *buf, int bufsize)
{
    int end = size();
    bufsize --;
     
    for(int offset = m_Position; offset < end; offset++)
    {
        char *p = &*begin()+offset;
        
        if(*p=='\n' || *p==0 || offset==end-1)
        {
        	int len = offset - m_Position + 1;

        	if(*p=='\n') 
        	{
        	     len --;
        	     if(*(p-1)=='\r') len --;
        	}
        	
        	if(len > bufsize) len = bufsize;

        	if(offset< end-1) memcpy(buf, &*(begin()+m_Position), len);

        	buf[len] = 0;
        	m_Position = offset+1;
        	return len;
        }
    }
    
    return 0;
}

// Set position by offset  
unsigned int BasePacket::SeekSet(int offset)
{
    m_Position = offset;
    int s = size();
     
    if(m_Position>s) m_Position = s;
    else if(m_Position<0) m_Position = 0;
     
    return m_Position;	
}


// Move position from the current position
unsigned int BasePacket::SeekCur(int offset)
{
    return SeekSet(m_Position+offset);
}

// Move position from the end of packet
unsigned int BasePacket::SeekEnd(int offset)
{
    return SeekSet(size()+offset);
}

// this is read all data from source Packet.
BasePacket & BasePacket::operator<<( BasePacket &p )
{
	if (p.size() > 0)
	{
		unsigned int newpos = m_Position + p.size();
		if(newpos > size()) resize(newpos);

		memcpy(&(*this)[m_Position], &p[0], p.size());
		m_Position = newpos;
	}
	return (*this);
}

// this is only read data from source Packet which is begin m_Position, must be careful
BasePacket& BasePacket::read( BasePacket &psrc )
{
	int iCopySize = psrc.size() - psrc.Tell();
	if ( iCopySize > 0)
	{
		unsigned int newpos = m_Position + iCopySize;
		if(newpos > size()) resize(newpos);

		memcpy(&(*this)[m_Position], &psrc[0]+psrc.Tell(), iCopySize);
		m_Position = newpos;
	}
	return (*this);
}
//=======================================================
// class DeltaPacket
//=======================================================
// Overload operator<< for BasePacket
BasePacket &operator<<(BasePacket &p, const DeltaPacket &diff)
{
    unsigned int newpos = p.Tell();
    unsigned short len;
    
    if(diff.m_bEncoded)
    {
    	newpos += diff.size() + 2;
        if(newpos> p.size()) p.resize(newpos);
        len = diff.size() | 0x8000;
        memcpy(&p[p.Tell()+2], &diff[0], diff.size());
    }
    else
    {
    	len = diff.m_LatestVersion.size();
    	newpos += len + 2;
    	if(newpos> p.size()) p.resize(newpos);
        memcpy(&p[p.Tell()+2], &diff.m_LatestVersion[0], len);
    }
    
    p[p.Tell()] = len & 0xff;
    p[p.Tell()+1] = len >> 8;
    p.SeekSet(newpos);
    	
    return p;
}

// Overload operator>> for BasePacket
BasePacket &operator>>(BasePacket &p, DeltaPacket &diff)
{
    unsigned short len = 0;
    p >> len;
    bool encoded = (len & 0x8000)!=0;
    len &= 0x7fff;

    if((p.size() - p.Tell())>= len && p.size()!=0)
	{
       if(encoded)
       {
       	   diff.resize(len);
       	   memcpy(&diff[0], &p[p.Tell()], len);
       	   diff.m_bEncoded = true;
       }
       else
       {
       	   diff.m_LatestVersion.resize(len);
       	   memcpy(&diff.m_LatestVersion[0], &p[p.Tell()], len);
       	   diff.m_bEncoded = false;
       }
       	
       p.SeekCur(len);
	}

    return p;	
}

// Assign another DeltaPacket object : restore differential data(encoded) or just copy
// the latest version(not encoded)
bool DeltaPacket::SetLatestVersion(const DeltaPacket &diff)
{
   unsigned int i =0, pos = 0;
   
   if(diff.m_bEncoded)
   {
      while(i< diff.size())
      {
         char tag = diff[i++];
   
         for(int bit=0; bit< 8; bit++)
         {
            if(tag & 1)
            {
               m_LatestVersion[pos] = diff[i++];
            }
         
            tag >>= 1;
            pos ++;
         }
      }
   }
   else
   {
   	  m_LatestVersion = diff.m_LatestVersion;
   	  m_LatestVersion.SeekSet(0);
   }
   
   return diff.m_bEncoded;
}

// Assign newer version of BasePacket. It will make differential data if the two versions of
// packet have the same data size.
bool DeltaPacket::SetLatestVersion(const BasePacket &NewVersion)
{
   if(m_LatestVersion.size()==NewVersion.size())
   {
      unsigned int tag_pos = 0, pos = 1;
      char tag = 0;
      int bit = 0;
      
      resize(m_LatestVersion.size()+m_LatestVersion.size()/8+1);
      
      for(unsigned int i=0; i< m_LatestVersion.size(); i++)
      {
         if(bit>7)
         {
            *(begin()+tag_pos) = tag;
            tag_pos = pos ++;
            tag = 0;
            bit = 0;
         }
         
         if(m_LatestVersion[i]!=NewVersion[i])
         {
             tag |= (1 << bit);
             *(begin()+ pos) = NewVersion[i];
             pos++;
         }
            
         bit++;
      }
      
      if(tag!=0) *(begin()+tag_pos) = tag;
      resize(pos);
      
      m_bEncoded = true;
   }
   else
   {
   	  m_bEncoded = false;
   }

   m_LatestVersion = NewVersion;
   m_LatestVersion.SeekSet(0);
   
   return m_bEncoded;
}

