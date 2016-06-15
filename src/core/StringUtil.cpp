/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   14:02
	filename: 	E:\work\winne\Shooting\src\Common\include\string\StringUtil.cpp
	file path:	E:\work\winne\Shooting\src\Common\include\string
	file base:	StringUtil
	file ext:	cpp
	author:		piyejun
	
	purpose:	
*********************************************************************/

#include "StringUtil.h"
#include <zlib.h>
#include <math.h>
#ifdef WIN32
#include <io.h>
#else
#include <stdio.h>
#endif


	std::string StringUtil::Format( const char* format, ... )
	{
		char buffer[STR_BUF_SIZE] = {0};
		va_list pa;
		va_start(pa, format);
		int iSize = vsnprintf(buffer, STR_BUF_SIZE, format, pa);
		va_end(pa);
		if(-1 == iSize)
			return "";

		return buffer;
	}

	// millisecond
	unsigned long StringUtil::GetTickCount()
	{
		uint64_t tc;
#ifdef WIN32
		tc = ::GetTickCount();
#else
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		tc = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
		return tc;
	}

	char* StringUtil::ITOA( int i, char* buf, unsigned int size )
	{
#ifndef WIN32
		sprintf(buf, "%d", i);
#else
		sprintf_s(buf, size, "%d", i);
#endif
		return buf;
	}

	char* StringUtil::ITOAX( int i, char* buf, unsigned int size )
	{
#ifndef WIN32
		sprintf(buf, "%x", i);
#else
		sprintf_s(buf, size, "%x", i);
#endif
		return buf;
	}

	void StringUtil::Srand()
	{
		::srand((DWORD)GetTickCount());
	}

	int StringUtil::Random( int Start, int End )
	{
		int iVal = End - Start;
		if(iVal < 0) return -1;
		else if(0== iVal) return Start;
		return Start + ::rand() % (iVal);
	}

	int StringUtil::RandomVec( int Start, int End, std::set<int>& vec, int num )
	{
		if (End - Start <= num)
			return -1;

		while(vec.size() < (DWORD)num)
		{
			Srand();
			int iRet = Random(Start, End);
			vec.insert(iRet);
		}

		return num;
	}

	int StringUtil::Atoint( const char* pChar )
	{
		return ::atoi(pChar);
	}

	bool StringUtil::StringToWString( const std::string& s, std::wstring& ws, DWORD uiCodePage /*= 0*/ )
	{
#ifdef WIN32
		return StringToWStringWin(s, ws, uiCodePage);
#else
		return StringToWStringLinux(s, ws, uiCodePage);
#endif
	}

	bool StringUtil::WStringToString( const std::wstring& ws, std::string& s, DWORD uiCodePage /*= 0*/ )
	{
#ifdef WIN32
		return WStringToStringWin(ws, s, uiCodePage);
#else
		return WStringToStringLinux(ws, s, uiCodePage);
#endif
	}

#ifdef WIN32
	bool StringUtil::StringToWStringWin( const std::string& s, std::wstring& ws, DWORD uiCodePage /*= 0*/ )
	{
		int iConvertedCount = 0;
		int iSize = ::MultiByteToWideChar(
			uiCodePage,0,s.c_str(),-1,NULL,0);

		wchar_t pkBuffer[STR_BUF_SIZE] = {0};

		iConvertedCount = ::MultiByteToWideChar(
			uiCodePage,0,s.c_str(),-1,pkBuffer,iSize);

		if (iConvertedCount != iSize)
		{
			return false;
		}
		ws = pkBuffer;
		return true;
	}

	bool StringUtil::WStringToStringWin( const std::wstring& ws, std::string& s, DWORD uiCodePage /*= 0*/ )
	{
		int iConvertedCount = 0;
		int iSize = WideCharToMultiByte(
			uiCodePage,0,ws.c_str(),-1,NULL,0,NULL,NULL);

		char pkBuffer[STR_BUF_SIZE] = {0};

		iConvertedCount = WideCharToMultiByte(
			uiCodePage,0,ws.c_str(),-1,pkBuffer,iSize,NULL,NULL);

		if (iConvertedCount != iSize)
		{
			return false;
		}
		s = pkBuffer;
		return true;
	}
#else
	bool StringUtil::StringToWStringLinux( const std::string& s, std::wstring& ws, unsigned int uiCodePage /*= 0*/ )
	{
		if (NULL==setlocale(LC_ALL, "zh_CN.utf8"))
		{
			//Fir::logger->info(("StringToWStringLinux::setlocale() fail..");
			return false;
		}
		int iConvertedCount = 0;
		int iSize = ::mbstowcs(NULL, s.c_str(), 0);    

		wchar_t pkBuffer[STR_BUF_SIZE] = {0};

		iConvertedCount = ::mbstowcs(pkBuffer, s.c_str(), iSize);

		if (iConvertedCount != iSize)
		{
			//Fir::logger->info(("size not equal!");
			return false;
		}
		ws = pkBuffer;
		return true;
	}

	bool StringUtil::WStringToStringLinux( const std::wstring& ws, std::string& s, unsigned int uiCodePage /*= 0*/ )
	{
		if (NULL==setlocale(LC_ALL, "zh_CN.utf8"))
		{
			//Fir::logger->info(("WStringToStringLinux:: setlocale() fail..");
			return false;
		}
		int iConvertedCount = 0;
		int iSize = ::wcstombs(NULL, ws.c_str(), 0);    

		char pkBuffer[STR_BUF_SIZE] = {0};

		iConvertedCount = ::wcstombs(pkBuffer, ws.c_str(), iSize);

		if (iConvertedCount != iSize)
		{
			//Fir::logger->info(("size not equal!");
			return false;
		}
		s = pkBuffer;
		return true;
	}

#endif

	bool StringUtil::AnsiToUTF8( const std::string& ansiString, std::string& utf8String )
	{
#ifdef WIN32

		std::wstring ws;
		if (!StringToWString(ansiString,ws,CP_ACP)) 
		{
			return false;
		}

		if (!WStringToString(ws,utf8String,CP_UTF8)) 
		{
			return false;
		}
		return true;
#else
		return false;
#endif
	}

	bool StringUtil::UTF8ToAnsi( const std::string& utf8String, std::string& ansiString )
	{
		std::wstring ws;
#ifdef WIN32
		if (!StringToWString(utf8String,ws,CP_UTF8)) 
#else
		if (!StringToWString(utf8String,ws,0)) 
#endif
		{
			return false;
		}

#ifdef WIN32
		if (!WStringToString(ws,ansiString,CP_ACP)) 
#else
		if (!WStringToString(ws,ansiString,0)) 
#endif
		{
			return false;
		}
		return true;
	}

	bool StringUtil::SplitStr( const std::string& szStr, const char szMark, std::vector<std::string>& strArray )
	{
		if (szStr.length()==0)
		{
			return false;
		}
		std::string::size_type pos1,pos2;
		pos2 = szStr.find(szMark);

		if (std::string::npos == pos2)
		{
			strArray.push_back(szStr);
			return true;
		}
		else
		{
			pos1 = 0;
			if (0==pos2)
			{
				pos1 = pos2 + 1;
				pos2 = szStr.find(szMark, pos1);
			}
			while(std::string::npos != pos2)
			{
				strArray.push_back(szStr.substr(pos1, pos2-pos1));
				pos1 = pos2 + 1;
				pos2 = szStr.find(szMark, pos1);
			}
			if (pos1 != szStr.length())
			{
				strArray.push_back(szStr.substr(pos1));
			}
		}

		return true;
	}

	bool StringUtil::SplitStr( const std::string& szStr, const std::string& szMark, std::vector<std::string>& strArray )
	{
		if (szStr.length()==0)
		{
			return false;
		}
		std::string::size_type MkLen = szMark.length();
		std::string::size_type pos1,pos2;
		pos2 = szStr.find(szMark);

		if (std::string::npos == pos2)
		{
			strArray.push_back(szStr);
			return true;
		}
		else
		{
			pos1 = 0;
			if (0==pos2)
			{
				pos1 = pos2 + MkLen;
				pos2 = szStr.find(szMark, pos1);
			}
			while(std::string::npos != pos2)
			{
				strArray.push_back(szStr.substr(pos1, pos2-pos1));
				pos1 = pos2 + MkLen;
				pos2 = szStr.find(szMark, pos1);
			}
			if ( pos1 < szStr.length() )
			{
				strArray.push_back(szStr.substr(pos1));
			}
		}

		return true;
	}

	bool StringUtil::ReplaceSpecialString(std::string& Target, const char& level1, int idx1, const char& level2, int idx2, const std::string& repStr)
	{
		if(Target.empty()) return false;
		std::string FirstStr, NewStr;
		int iCount = 0, LastPos1=0, LastPos2 =0;
		bool IsFind = false;
		if (Target[0]==level1)
		{
			iCount = -1;
		}
		// 1. find the first char
		for (int i=0; i < (int)Target.length(); ++i)
		{
			if (Target[i] == level1)
			{
				++iCount;
				if (iCount==idx1+1)
				{
					LastPos1+=1;
					FirstStr = Target.substr(LastPos1, i-LastPos1);
					IsFind = true;
					break;
				}
				LastPos1 = i;
			}

		}
		if(!IsFind) return false;

		// 2. find the second char
		iCount = 0;
		IsFind = false;
		for (int i=LastPos1; i < LastPos1+ (int)FirstStr.length(); ++i)
		{
			if (Target[i] == level2)
			{
				++iCount;
				if (iCount==idx2+1)
				{
					NewStr = Target.substr(0, LastPos2+1);
					IsFind = true;
					LastPos2 = i;
					break;
				}
				LastPos2 = i;
			}
		}
		if(!IsFind) return false;

		// 3. merge the target string
		NewStr.append(repStr);  // the middle string
		std::string TailStr = Target.substr(LastPos2, std::string::npos);
		NewStr.append(TailStr);
		Target = NewStr;

		return true;
	}

	std::string& StringUtil::StrReplace(std::string& srcStr, const std::string& oldStr, const std::string& newStr)
	{
		for (std::string::size_type pos(0); pos != std::string::npos; pos += newStr.length())
		{
			pos = srcStr.find(oldStr);

			if (pos != std::string::npos)
			{
				srcStr.replace(pos, oldStr.length(), newStr);
			}
			else
			{
				break;
			}
		}
		return srcStr;
	}

	DWORD  StringUtil::Compress(BYTE *source, DWORD source_size, BYTE *target, DWORD target_size) 
	{
		uLongf dest_len = (uLongf)target_size;

		int zerr = compress(target, &dest_len, source, source_size);
		switch(zerr) {
		case Z_MEM_ERROR:	// not enough memory
		case Z_BUF_ERROR:	// not enough room in the output buffer
			printf("Zlib error : %s", zError(zerr));
			return 0;
		case Z_OK:
			return dest_len;
		}
		return 0;
	}

	DWORD StringUtil::Uncompress( BYTE *source, DWORD source_size, BYTE *target, DWORD target_size )
	{
		uLongf dest_len = (uLongf)target_size;

		int zerr = uncompress(target, &dest_len, source, source_size);
		switch(zerr) {
		case Z_MEM_ERROR:	// not enough memory
		case Z_BUF_ERROR:	// not enough room in the output buffer
		case Z_DATA_ERROR:	// input data was corrupted
			printf("Zlib error : %s", zError(zerr));
			return 0;
		case Z_OK:
			return dest_len;
		}
		return 0;
	}

	std::string StringUtil::GetFilePath(const char* _filename)
	{
		int ilen = ::strlen(_filename);
		char* pChar = (char*)_filename + ilen;
		int i=0;
		for (; i<ilen;++i)
		{
			if ((*(pChar-i) == '/') || (*(pChar-i) == '\\'))
			{
				--i;
				break;
			}
		}
		int copyLen = ilen - i;

		return std::string(_filename, copyLen);
	}

	std::string StringUtil::GetFileName(const char* _filename)
	{
		int ilen = ::strlen(_filename);
		char* pChar = (char*)_filename + ilen;
		int i=0;
		for (; i<ilen;++i)
		{
			if ((*(pChar-i) == '/') || (*(pChar-i) == '\\'))
			{
				--i;
				break;
			}
		}
		int startPos = ilen - i;

		return std::string(_filename + startPos, i);
	}

	void StringUtil::SetProgramCurrentPath()
	{
#ifdef WIN32
		// for taskschd.msc call
		std::string fullpath;
		char buf[128] = {0};
		::GetCurrentDirectoryA(128, buf);
		fullpath = buf;
		if( std::string::npos == fullpath.find("server_run") )
		{
			::GetModuleFileNameA(NULL, buf, 128);
			std::string curPath = StringUtil::GetFilePath(buf);
			::SetCurrentDirectoryA(curPath.c_str());
		}
#endif
	}

	std::string StringUtil::GetNow()
	{
		time_t now = time(NULL);
		return Date2String(now);
	}

	std::string StringUtil::Date2String( time_t tme )
	{
		struct tm ttm;
		int error = 0;
#ifdef WIN32
		error = localtime_s(&ttm, &tme);
#else
		localtime_r(&tme, &ttm);
#endif
		
		char buf[32]={0};
		if (0 == error)
		{
			strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ttm);
		}
		return buf;
	}

	time_t StringUtil::String2Data(const char* timeStr)
	{
		struct tm tm1;
		time_t time1;
		sscanf(timeStr, "%4d-%2d-%2d %2d:%2d:%2d",
			&tm1.tm_year,
			&tm1.tm_mon,
			&tm1.tm_mday,
			&tm1.tm_hour,
			&tm1.tm_min,
			&tm1.tm_sec);
		tm1.tm_year -= 1900;
		tm1.tm_mon--;
		tm1.tm_isdst = -1;
		time1 = mktime(&tm1);
		return time1;
	}

	time_t StringUtil::String2LocalData(const char* timeStr)
	{
		struct tm tmTime = { 0 };
		time_t time_t;
		strptime(timeStr, "%Y-%m-%d %H:%M:%S", &tmTime);
		time_t = timegm(&tmTime);
		return time_t;
	}

	
	bool StringUtil::DifferenceOfTwoTimes( DWORD old_t, DWORD cur_t, int& out_day )
	{
		DWORD day_old = old_t / 86400;  // 24*60*60
		DWORD day_cur = cur_t / 86400;  // 24*60*60
		out_day = day_cur - day_old;
		return true;
	}

	int StringUtil::GetTimeZone()
	{
		int iZone = 0;
#ifdef WIN32
		_tzset();
		long diff_secs;  
		_get_timezone(&diff_secs);
		iZone = diff_secs;
#else
		tzset();
		iZone = (int)timezone;
#endif
		return iZone;
	}

	int StringUtil::GenerateID( const char* str )
	{
		if(NULL==str) return 0;
#ifdef WIN32
		static unsigned int m[32];
		int i = COPY_STRING_LENGTH;

#pragma warning(push)
#pragma warning(disable:4996)
		// must use strncpy because strncpy do full zero for last char , but strncpy_s not done. by YWG 20110612
		strncpy( (char *)m, str, i * 4 );  
#pragma warning(pop)

		unsigned int v = 0;
		m[i++]=0x9BE74448,m[i++]=0x66F42C48;
		v=0xF4FA8928;

		__asm {
			mov esi,0x37A8470E  ;x0=0x37A8470E
				mov edi,0x7758B42B  ;y0=0x7758B42B
				xor ecx,ecx
_loop:
			mov ebx,0x267B0B11  ;w=0x267B0B11
				rol v,1
				lea eax,m
				xor ebx,v

				mov eax,[eax+ecx*4]
			mov edx,ebx
				xor esi,eax
				xor edi,eax

				add edx,edi
				or edx,0x2040801  ;a=0x2040801
				and edx,0xBFEF7FDF  ;c=0xBFEF7FDF

				mov eax,esi
				mul edx
				adc eax,edx
				mov edx,ebx
				adc eax,0

				add edx,esi
				or edx,0x804021   ;b=0x804021
				and edx,0x7DFEFBFF  ;d=0x7DFEFBFF

				mov esi,eax
				mov eax,edi
				mul edx

				add edx,edx
				adc eax,edx
				jnc _skip
				add eax,2
_skip:
			inc ecx;
			mov edi,eax
				cmp ecx,i
				jnz _loop
				xor esi,edi
				mov v,esi
		}

		return (int)v;
#else
		int seed = 131; // 31 131 1313 13131 131313 etc..
		int hash = 0;
		while (*str)
		{
			hash = hash * seed + (*str++);
		}  
		return (hash & 0xFFFFFFFF);
#endif
	}

	int StringUtil::GenerateID( const std::string& str )
	{
		if(str.empty()) return 0;
		return GenerateID(str.c_str());
	}

	float StringUtil::Pow( float _x, int _y )
	{
		return pow(_x, _y);
	}

	bool StringUtil::BitSet2DWORD( const BitSet& b, DWORD& u )
	{
		u = (DWORD)b.to_ulong();
		return true;
	}

	bool StringUtil::DWORDtoBitSet( const DWORD& u, BitSet& b )
	{
		for (int i=0; i<32; ++i)
		{
			(u & (1 << i)) ? b.set(i, true) : b.set(i, false);
		}
		return true;
	}

	SQWORD StringUtil::Atoi64( const char* pChar )
	{
#ifdef WIN32
		return ::_atoi64(pChar);
#else
		return ::atoll(pChar);
#endif
	}

	bool StringUtil::IsNumber( const std::string& str )
	{
		if(str.empty()) return false;
		for (int i=0; i<(int)str.length(); ++i)
		{
			char c = str.at(i);
			if (c < 48 || c > 57) return false;
		}
		return true;
	}

	bool StringUtil::IsExistFile( const std::string& str )
	{
		return (0==::access(str.c_str(), 0));
	}

	bool StringUtil::IsExistDateFile()
	{
		std::string name = "";
#ifdef WIN32
		name = "C:\\Users\\Administrator\\temp01";
#else
		name = "/tmp/temp01";
#endif
		return IsExistFile(name);
	}

	bool StringUtil::CreateFile( const std::string& str )
	{
		FILE *fp = NULL;
		if((fp = ::fopen(str.c_str(), "w+b")) == NULL) return false;
		return true;
	}

	void StringUtil::CreateDateFile()
	{
		std::string name = "";
#ifdef WIN32
		name = "C:\\Users\\Administrator\\temp01";
#else
		name = "/tmp/temp01";
#endif
		CreateFile(name);
	}


	// type,itemId,amount(type 1:实物 2:虚拟物品 体力 元宝等)
	void StringUtil::SplitItemStr(const std::string& srcStr, std::string& itemStr, std::string& moneyStr)
	{
		std::vector<std::string> itemVec;
		if (!StringUtil::SplitStr(srcStr, ';', itemVec))
		{
			return;
		}
		for (auto iter = itemVec.begin(); iter != itemVec.end(); ++iter)
		{
			std::vector<std::string> vec2;
			if (StringUtil::SplitStr(*iter, ',', vec2))
			{
				if (vec2.size() != 3) continue;
				if ((!StringUtil::IsNumber(vec2[0])) || (!StringUtil::IsNumber(vec2[1])) || (!StringUtil::IsNumber(vec2[2])))
				{
					continue;
				}
				int itemType = StringUtil::Atoint(vec2[0].c_str());
				if (1 == itemType)
				{
					itemStr += StringUtil::Format("%s,%s;", vec2[1].c_str(), vec2[2].c_str());
				}
				if (2 == itemType)
				{
					moneyStr += StringUtil::Format("%s,%s;", vec2[1].c_str(), vec2[2].c_str());
				}
			}
		}
	}

	bool StringUtil::SplitItemStr(const std::string& srcStr, std::vector<obj_common>& itemVec)
	{
		std::vector<std::string> temVec;
		if (!StringUtil::SplitStr(srcStr, ';', temVec))
		{
			return false;
		}
		for (auto iter = temVec.begin(); iter != temVec.end(); ++iter)
		{
			std::vector<std::string> vec2;
			if (StringUtil::SplitStr(*iter, ',', vec2))
			{
				if (vec2.size() != 2) continue;
				if ((!StringUtil::IsNumber(vec2[0])) || (!StringUtil::IsNumber(vec2[1])))
				{
					continue;
				}
				
				obj_common oneItem;
				oneItem.item_id = StringUtil::Atoint(vec2[0].c_str());
				oneItem.count = StringUtil::Atoint(vec2[1].c_str());
				itemVec.push_back(oneItem);
			}
		}
		return true;
	}

	bool StringUtil::SplitMoneyStr(const std::string& srcStr, std::vector<mail_money>& moneyVec)
	{
		std::vector<std::string> temVec;
		if (!StringUtil::SplitStr(srcStr, ';', temVec))
		{
			return false;
		}
		for (auto iter = temVec.begin(); iter != temVec.end(); ++iter)
		{
			std::vector<std::string> vec2;
			if (StringUtil::SplitStr(*iter, ',', vec2))
			{
				if (vec2.size() != 2) continue;
				if ((!StringUtil::IsNumber(vec2[0])) || (!StringUtil::IsNumber(vec2[1])))
				{
					continue;
				}

				mail_money oneItem;
				oneItem.type = StringUtil::Atoint(vec2[0].c_str());
				oneItem.value = StringUtil::Atoint(vec2[1].c_str());
				moneyVec.push_back(oneItem);
			}
		}
		return true;
	}

	bool StringUtil::SplitSkillStr(const std::string& srcStr, std::vector<obj_skill>& skillVec)
	{
		std::vector<std::string> temVec;
		if (!StringUtil::SplitStr(srcStr, ';', temVec))
		{
			return false;
		}
		for (auto iter = temVec.begin(); iter != temVec.end(); ++iter)
		{
			std::vector<std::string> vec2;
			if (StringUtil::SplitStr(*iter, ',', vec2))
			{
				if (vec2.size() != 2) continue;
				if ((!StringUtil::IsNumber(vec2[0])) || (!StringUtil::IsNumber(vec2[1])))
				{
					continue;
				}

				obj_skill skill;
				skill.skill_id = StringUtil::Atoint(vec2[0].c_str());
				skill.level = StringUtil::Atoint(vec2[1].c_str());
				skillVec.push_back(skill);
			}
		}
		return true;
	}

	
	bool StringUtil::ItemStrCheck(const std::string& srcStr)
	{
		std::vector<std::string> temVec;
		if (!StringUtil::SplitStr(srcStr, ';', temVec))// 首次拆分失败
		{
			return false;
		}
		for (auto iter = temVec.begin(); iter != temVec.end(); ++iter)
		{
			std::vector<std::string> vec2;
			if (StringUtil::SplitStr(*iter, ',', vec2))
			{
				if (vec2.size() != 2)// 拆分后的单个长度必须为2
				{
					return false;
				}
				if ((!StringUtil::IsNumber(vec2[0])) || (!StringUtil::IsNumber(vec2[1])))// id和数量必须为数字
				{
					return false;
				}
				int item_id = StringUtil::Atoint(vec2[0].c_str());// id 不能为负值
				if (item_id <= 0)
				{
					return false;
				}
				int count = StringUtil::Atoint(vec2[1].c_str());// 数量必须大于0
				if (count <= 0)
				{
					return false;
				}
			}
		}
		return true;
	}



    int StringUtil::StrlenUtf8(const char *s)
	{
		int i = 0, j = 0;
		while (s[i]) {
			if ((s[i] & 0xc0) != 0x80) j++;
			i++;
		}
		return j;
	}

	std::string& StringUtil::trim(std::string &s)
	{
		if (s.empty())
		{
			return s;
		}

		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);

		return s;
	}


	void StringUtil::CheckIntMax(int currValue, int &addValue)
	{
		/*if (addValue < 0)
		{
		return;
		}
		value += addValue;

		if (value > HELL_INT_MAX)
		{
		value = HELL_INT_MAX;
		}

		if (value < 0)
		{
		value = HELL_INT_MAX;
		}*/
		
		SQWORD totalValue = currValue + addValue;
		if (totalValue > HELL_INT_MAX)
		{
			addValue = HELL_INT_MAX - currValue;
		}

		if (totalValue < -HELL_INT_MAX)
		{
			addValue = -HELL_INT_MAX - currValue;
		}
	}

	bool StringUtil::SplitStr2IntVec(const std::string& szStr, const std::string& szMark, std::vector<int>& intArray)
	{
		if (szStr.length() == 0)
		{
			return false;
		}
		std::string::size_type MkLen = szMark.length();
		std::string::size_type pos1, pos2;
		pos2 = szStr.find(szMark);

		if (std::string::npos == pos2)
		{
			intArray.push_back(StringUtil::Atoint(szStr.c_str()));
			return true;
		}
		else
		{
			pos1 = 0;
			if (0 == pos2)
			{
				pos1 = pos2 + MkLen;
				pos2 = szStr.find(szMark, pos1);
			}
			while (std::string::npos != pos2)
			{
				intArray.push_back(StringUtil::Atoint(szStr.substr(pos1, pos2 - pos1).c_str()));
				pos1 = pos2 + MkLen;
				pos2 = szStr.find(szMark, pos1);
			}
			if (pos1 < szStr.length())
			{
				intArray.push_back(StringUtil::Atoint(szStr.substr(pos1).c_str()));
			}
		}

		return true;
	}

