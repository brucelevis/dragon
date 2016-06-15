/********************************************************************
	created:	2013/01/22
	created:	22:1:2013   19:31
	filename: 	E:\work\Shooting\src\Common\include\string\StringUtil.h
	file path:	E:\work\Shooting\src\Common\include\string
	file base:	StringUtil
	file ext:	h
	author:		piyejun
	
	purpose:	operate string, note: don't use at multi thread.
*********************************************************************/

#ifndef _StringUtil_h_
#define _StringUtil_h_

#include <string.h>
#include <stdarg.h>
#include <set>
#include <vector>
#include <time.h>
#include "zSingleton.h"
#include "CommonStruct.h"

#ifdef WIN32
#undef WIN32_LEAN_AND_MEAN
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#endif

using namespace FIGHTING;

#define  STR_BUF_SIZE  2048
#define COPY_STRING_LENGTH ( int(int(strnlen_s(str,128)+3)*.25f) )
#define HELL_INT_MAX 2100000000 // 21亿



class StringUtil
{
public:
	static std::string Format( const char* format, ... );

	static unsigned long GetTickCount();

	static char* ITOA( int i, char* buf, unsigned int size );

	// HEX, 16
	static char* ITOAX( int i, char* buf, unsigned int size );

	static void sleep( int millisecond );

	static void Srand();

	static int Random( int Start, int End );

	static int RandomVec( int Start, int End, std::set<int>& vec, int num );

	static int Atoint( const char* pChar );

	static bool StringToWString( const std::string& s, std::wstring& ws, UINT32 uiCodePage /*= 0*/ );

	static bool WStringToString( const std::wstring& ws, std::string& s, UINT32 uiCodePage /*= 0*/ );

#ifdef WIN32
	static bool StringToWStringWin( const std::string& s, std::wstring& ws, UINT32 uiCodePage /*= 0*/ );

	static bool WStringToStringWin( const std::wstring& ws, std::string& s, UINT32 uiCodePage /*= 0*/ );
#else
	static bool StringToWStringLinux( const std::string& s, std::wstring& ws, unsigned int uiCodePage /*= 0*/ );

	static bool WStringToStringLinux( const std::wstring& ws, std::string& s, unsigned int uiCodePage /*= 0*/ );
	
#endif


	static bool AnsiToUTF8( const std::string& ansiString, std::string& utf8String );
	
	static bool UTF8ToAnsi( const std::string& utf8String, std::string& ansiString );

	static bool SplitStr( const std::string& szStr, const char szMark, std::vector<std::string>& strArray );

	static bool SplitStr( const std::string& szStr, const std::string& szMark, std::vector<std::string>& strArray );

	static bool SplitStr2IntVec(const std::string& szStr, const std::string& szMark, std::vector<int>& strArray);

	// idx1: the index of the first string.
	static bool ReplaceSpecialString(std::string& Target, const char& level1, int idx1, const char& level2, int idx2, const std::string& repStr);

	// add by weigk 2015-08-05
	static std::string& StrReplace(std::string& srcStr, const std::string& oldStr, const std::string& newStr);

#ifndef WIN32
	static char* GetLocalIpLinux();
	
#endif

	// ZIP compress.
	// target, target_size , is return value.
	// source, source_size is input value.
	static UINT32  Compress(BYTE *source, UINT32 source_size, BYTE *target, UINT32 target_size); 

	// ZIP uncompress
	static UINT32 Uncompress(BYTE *source, UINT32 source_size, BYTE *target, UINT32 target_size);

	static std::string GetFilePath(const char* _filename);

	static std::string GetFileName(const char* _filename);

	static void SetProgramCurrentPath();
	// datatime to string(add by wgk 20150522)
	static std::string Date2String(time_t tm);

	static time_t String2Data(const char* timeStr);

	static time_t String2LocalData(const char* timeStr);

	static std::string GetNow();
	
	// calculation the two time 's interval measure of days.
	static bool DifferenceOfTwoTimes(UINT32 old_t, UINT32 cur_t, int& out_day);

	// get the time zone different seconds
	static int GetTimeZone();

	// translate string to int(from YANGWENGE)
	static int GenerateID( const char* str );

	static int GenerateID( const std::string& str);

	static float Pow(float _x, int _y);

	static bool BitSet2DWORD(const BitSet& b, UINT32& u);

	static bool DWORDtoBitSet(const UINT32& u, BitSet& b);

	static SQWORD Atoi64(const char* pChar);

	// verify the string is the number.
	static bool IsNumber(const std::string& str);

	static bool IsExistFile(const std::string& str);

	static bool IsExistDateFile();

	static bool CreateFile(const std::string& str);

	static void CreateDateFile();

	static void DoSome();

	// 拆分虚拟道具与元宝混合的物品字符串(type,itemid,count;type,itemid,count;...)(type:1 普通物品，2:虚拟物品（元宝体力之类）)
	static void SplitItemStr(const std::string& srcStr, std::string& itemStr, std::string& moneyStr);
	
	// 拆分物品字符串(itemid,count;itemid,count;...)
	static bool SplitItemStr(const std::string& srcStr, std::vector<obj_common>& itemVec);
	// 拆分金钱字符串（type,value;type,value;...）
	static bool SplitMoneyStr(const std::string& srcStr, std::vector<mail_money>& moneyVec);
	// 拆分 技能字符串（type,value;type,value;...）
	static bool SplitSkillStr(const std::string& srcStr, std::vector<obj_skill>& skillVec);
	
	// 判定物品字符串是否合法(itemid,count;itemid,count;itemid,count;)
	static bool ItemStrCheck(const std::string& srcStr);

	static int StrlenUtf8(const char * s);

	static std::string& trim(std::string &s);

	// 天数
	static int GetDayNum(UINT32 time){ return time / 86400 + 1; }

	template<class _Ty> inline
	static void Swap(_Ty& _Left, _Ty& _Right)
	{	
		_Ty _Tmp = _Left;
		_Left = _Right;
		_Right = _Tmp;
	}

	// int上限检测并修正
	static void CheckIntMax(int currValue, int &addValue);
};

#endif
