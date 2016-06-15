/**
 * \file
 * \version  $Id: zMisc.h 92 2013-04-26 05:17:24Z  $
 * \author  ,@163.com
 * \date 2005年01月19日 21时24分52秒 CST
 * \brief 封装一些常用函数
 *
 * 
 */

#ifndef _ZMISC_H_
#define _ZMISC_H_

#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <ctime>
#include <vector>
#include <map>
#include <dirent.h>
#include <unistd.h>
#include <iconv.h>
#include "zType.h"
#include "zMutex.h"
#include <set>
#include <sstream>
#include "zLogger.h"
#include "zRegex.h"
#include <math.h>
#include <float.h>
#include "Fir.h"

/**
 * 计算数组中个体的个数
 */
#define count_of(entry_v) (sizeof(entry_v) / sizeof((entry_v)[0]))

/**
 * 配置文件所在目录
 */
#define CONFIGDIR "config/"

/**
 * 62进制转换函数使用宏
 */
#define SYSTEM_62 62
// 62进制表现字符
const char k62Chars[SYSTEM_62] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
								  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
								  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
								  'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 
								  'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
								  'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 
								  'y', 'z'};

struct odds_t
{
	unsigned int upNum;
	unsigned int downNum;
};

enum ServerType {
	UNKNOWNSERVER	=	0,		/**< 未知服务器类型 */
	SUPERSERVER	=	1,		/**< 服务器管理器 */
	LOGINSERVER	=	10,		/**< 登陆服务器 */
	RECORDSERVER	=	11,		/**< 档案服务器 */
	BILLSERVER	=	12,		/**< 计费服务器 */
	SESSIONSERVER	=	20,		/**< Session服务器 */
	SCENESSERVER	=	21,		/**< 场景服务器 */
	GATEWAYSERVER	=	22,		/**< 网关服务器 */
	QUEUESERVER = 31,			//排队服务器
	MAX_SERVERTYPE				/**< 服务器类型的最大编号 */
};

namespace Misc
{
	template <typename V>
		class Parse
		{
			public:
				V* operator () (const std::string& down, const std::string& separator_down)
				{
					std::string::size_type pos = 0;
					if  ( (pos = down.find(separator_down)) != std::string::npos ) {

						std::string first_element = down.substr(0, pos);
						std::string second_element = down.substr(pos+separator_down.length());
						return FIR_NEW V(first_element, second_element);
					}

					return NULL;
				}
		};

	template <typename V>
		class Parse3
		{
			public:
				V* operator () (const std::string& down, const std::string& separator_down)
				{
					std::string::size_type pos = 0;
					if  ( (pos = down.find(separator_down)) != std::string::npos ) {

						std::string first_element = down.substr(0, pos);
						std::string::size_type npos = 0;
						if ( (npos = down.find(separator_down, pos+separator_down.length())) != std::string::npos) {
							std::string second_element = down.substr(pos+separator_down.length(), npos-pos);
							std::string third_element = down.substr(npos+separator_down.length());
							return FIR_NEW V(first_element, second_element, third_element);
						}
					}

					return NULL;
				}
		};

	/**
	 * \brief  分隔由二级分隔符分隔的字符串
	 * \param list 待分隔的字符串
	 * \param dest 存储分隔结果，必须满足特定的语义要求
	 * \param separator_up 一级分隔符
	 * \param separator_down 二级分隔符		 
	 * \author liqingyu 
	 */
	template <template <typename> class P = Parse>
		class Split
		{
			public:

				template <typename T>
					void operator() (const std::string& list, T& dest, const std::string& separator_up = ";", const std::string& separator_down = ",")
					{	
						typedef typename T::value_type value_type;
						typedef typename T::pointer pointer;

						std::string::size_type lpos = 0;
						std::string::size_type pos = 0;
						P<value_type> p;


						while ( ( lpos = list.find(separator_up, pos)) != std::string::npos) {
							/*
							   std::string down = list.substr(pos, lpos - pos);
							   std::string::size_type dpos = 0;
							   if  ( (dpos = down.find(separator_down)) != std::string::npos ) {

							   std::string first_element = down.substr(0, dpos);
							   std::string second_element = down.substr(dpos+separator_down.length());
							   dest.push_back(typename T::value_type(first_element, second_element));
							   }
							   pos = lpos+1;
							   */
							std::string down = list.substr(pos, lpos - pos);
							pointer v = p(down, separator_down);
							if (v) {
								dest.push_back(*v);
								SAFE_DELETE(v);
							}
							pos = lpos+1;
						}

						std::string down = list.substr(pos, lpos - pos);
						pointer v = p(down, separator_down);
						if (v) {
							dest.push_back(*v);
							SAFE_DELETE(v);
						}
					}
		};
}

namespace Fir
{
	// rand number generator's seed
	extern __thread unsigned int seedp;
	//	extern __thread struct drand48_data nrand48_buffer;
	//	extern __thread unsigned short int rand48_seedp[3];
};

#define rand_initialize() \
	do { \
		srand(time(NULL)); \
		Fir::seedp = time(NULL); \
		bzero(&Fir::nrand48_buffer, sizeof(Fir::nrand48_buffer)); \
		Fir::rand48_seedp[0] = (unsigned short int)rand_r(&Fir::seedp); \
		Fir::rand48_seedp[1] = (unsigned short int)rand_r(&Fir::seedp); \
		Fir::rand48_seedp[2] = (unsigned short int)rand_r(&Fir::seedp); \
		seed48_r(Fir::rand48_seedp, &Fir::nrand48_buffer); \
	} while(false)

class zMisc
{
	public:	
		/**
		 * \brief 三元组结构
		 * 每个元素可以代表一种含义
		 */
		struct TripleArray
		{       
			DWORD first;
			DWORD second; 
			DWORD third;
		};      

		/**
		 * \brief 三元组容器类型
		 *
		 */
		typedef std::vector<TripleArray> TripleVec;


		/**
		 * \brief 解析三元组的方法
		 * \param con: 存储三元组的容器
		 * \param in: 源字符串
		 * \param delimiter1: 三元组数据的外部分隔符,依据该分隔符生成CON的数据
		 * \param delimiter2: 三元组数据的内部分隔符,依据该分隔符生成TripleArray的数据
		 *
		 */
		static void parseString3(TripleVec& con, std::string &in, const char* delimiter1, const char* delimiter2)
		{       
			std::vector<std::string> tmpVec; 
			Fir::stringtok(tmpVec, in, delimiter1);

			zRegex regex;
			std::string expr="(.*)["+std::string(delimiter2)+"](.*)["+std::string(delimiter2)+"](.*)", s;
			TripleArray tp;
			for(std::vector<std::string>::iterator iter=tmpVec.begin(); iter!=tmpVec.end(); ++iter) 
			{       
				if(regex.compile(expr.c_str()) && regex.match((*iter).c_str()))
				{       
					regex.getSub(s, 1);
					tp.first=atoi(s.c_str());
					regex.getSub(s, 2);
					tp.second=atoi(s.c_str());
					regex.getSub(s, 3);
					tp.third=atoi(s.c_str());
#ifdef _DZL_DEBUG
					Fir::logger->trace("%s: %u, %u, %u", __PRETTY_FUNCTION__, tp.first, tp.second, tp.third);
#endif
					con.push_back(tp);
				}       
			}       
		}       

		//从字符串中查找第pos(从零开始)个数字，如果未找到返回defValue
		template <typename T>
			static WORD getAllNum(const char *s,std::vector<T> & data)
			{
				size_t i;
				int count = 0;
				if (s == NULL) return count;
				bool preIsD = false;
				for (i = 0; i < strlen(s); i++)
				{
					if (isdigit(*(s + i)))
					{
						if (!preIsD)
						{
							count++;
							data.push_back(atoi(s+i));
						}
						preIsD = true;
					}
					else
						preIsD = false;
				}
				return count;
			}

		//随机产生min~max之间的数字，包裹min和max
		static int randBetween(int min,int max)
		{
			if(min==max)
				return min;
			else if (min > max)
				return max + (int) (((double) min - (double)max + 1.0) * rand_r(&Fir::seedp) / (RAND_MAX + 1.0));
			else
				return min + (int) (((double) max - (double)min + 1.0) * rand_r(&Fir::seedp) / (RAND_MAX + 1.0));
		}

		//随机产生min~max之间的数字，包括min和max
		// nrand_r版本
		/*static int randBetween(const int min,const int max)
		  {
		  if(min==max)
		  return min;
		  else
		  {
		  long int m_rand = 0;
		  nrand48_r((unsigned short*)&Fir::rand48_seedp, &Fir::nrand48_buffer, &m_rand);
		  if (min > max)
		  return max + (int) (((double) min - (double)max + 1.0) * (double)m_rand / (double)(1.0 + RAND_MAX));
		  else
		  return min + (int) (((double) max - (double)min + 1.0) * (double)m_rand / (double)(1.0 + RAND_MAX));
		  }
		  }
		//  */

		//获取几分之的几率
		static bool selectByOdds(const unsigned int upNum, const unsigned int downNum)
		{
			unsigned int m_rand;
			if (downNum < 1) return false;
			if (upNum < 1) return false;
			if (upNum > downNum - 1) return true;
			m_rand = 1 + (unsigned int) ((double)downNum * rand_r(&Fir::seedp) / (RAND_MAX + 1.0));
			return (m_rand <= upNum);
		}

		//获取几分之的几率
		// nrand48_r版
		/*static bool selectByOdds(const unsigned int upNum, const unsigned int downNum)
		  {
		  if (downNum < 1) return false;
		  if (upNum < 1) return false;
		  if (upNum > downNum - 1) return true;

		  unsigned long int m_rand = 0;
		  nrand48_r((unsigned short*)&Fir::rand48_seedp, &Fir::nrand48_buffer, (long int*)&m_rand);
		  m_rand = 1 + (unsigned int) ((double)downNum * (double)m_rand / (double)(1.0 + RAND_MAX));
		  return (m_rand <= upNum);
		  }
		// */

		//获取几分之几的几率
		static bool selectByt_Odds(const odds_t &odds)
		{
			return selectByOdds(odds.upNum, odds.downNum);
		}

		//获取百分之的几率
		static bool selectByPercent(const unsigned int percent)
		{
			return selectByOdds(percent, 100);
		}

		//获取万分之的几率
		static bool selectByTenTh(const unsigned int tenth)
		{
			return selectByOdds(tenth, 10000);
		}

		//获取十万分之的几率
		static bool selectByLakh(const unsigned int lakh)
		{
			return selectByOdds(lakh, 100000);
		}

		//获取亿分之之的几率
		static bool selectByOneHM(const unsigned int lakh)
		{
			return selectByOdds(lakh, 100000000);
		}

		/**
		 * \brief 删除目录树
		 * \author fqnewman
		 */
		static bool rmDirTree(std::string path)
		{
			struct dirent *record;
			DIR* tDir = opendir(path.c_str());
			if (tDir == NULL) return false;

			while(NULL != (record = readdir(tDir)))
			{
				if (record->d_type == DT_DIR)
				{
					if (strcmp(record->d_name,".") == 0 ||	strcmp(record->d_name,"..") == 0) continue;

					if (!rmDirTree((path+"/"+record->d_name).c_str())) return false;
				}
				else
				{
					if (unlink((path+"/"+record->d_name).c_str()) != 0) return false;
				}
			}
			closedir(tDir);
			if (rmdir(path.c_str())!=0)return false;
			return true;
		}

		static unsigned char* stringConv(unsigned char* in, const char* fromEncoding, const char* toEncoding)
		{
			unsigned char* out = NULL;
			size_t ret=0, size=0, out_size=0;

			size = strlen((char *)in); 
			out_size = size * 2 + 1; 
			out = FIR_NEW unsigned char[out_size]; 

			bzero(out,out_size);

			if (out)
			{
				if(fromEncoding!=NULL && toEncoding!=NULL)
				{
					iconv_t icv_in = iconv_open(toEncoding, fromEncoding);
					if ((iconv_t)-1 == icv_in)
					{
						SAFE_DELETE_VEC(out);
						out = NULL;
					}
					else
					{
						char *fromtemp = (char *)in;
						char *totemp =(char *)out;
						size_t tempout = out_size-1;
						ret =iconv(icv_in, &fromtemp, &size, &totemp,&tempout);
						if ((size_t)-1 == ret)
						{
							SAFE_DELETE_VEC(out);
							out = NULL;
						}
						iconv_close(icv_in);
					}
				}
				else
					strncpy((char *)out,(char *)in,size);

			}
			return (out);
		}

		static bool equalFloat(float a, float b)
		{
			return fabs(a - b) < FLT_EPSILON;
		}

		static bool equalDouble(double a, double b)
		{
			return fabs(a - b) < DBL_EPSILON;
		}

		/**
		 * \brief 62进制字符串转为数值
		 * \param char62 62进制字符
		 * \return 数值
		 */
		static int char62toi(char char62)
		{
			if (char62 >= '0' && char62 <= '9')
			{
				return (char62 - '0');
			}
			else if (char62 >= 'A' && char62 <= 'Z')
			{
				return ('9' - '0' + 1 ) + (char62 - 'A');
			}
			else if (char62 >= 'a' && char62 <= 'z')
			{
				return ('9' - '0' + 1) + ('Z' - 'A' + 1) + (char62 - 'a');
			}
			return 0;
		}

		/**
		 * \brief 转换成62进制字符串
		 * \param value 数字
		 * \param length 62进制字符串长度
		 * \return 62进制字符串
		 */
		static std::string itosystem62(int value, int length)
		{
			std::string system62 = "";
			int temp = value;
			// 转换
			while (temp > 0)
			{
				system62 = k62Chars[temp % SYSTEM_62] + system62;
				temp /= SYSTEM_62;
			}
			// 补足
			while (system62.length() < (size_t)length)
			{
				system62 = "0" + system62;
			}

			return system62;
		}

		/**
		 * \brief 62进制字符串转换成数值
		 * \param system62 62进制字符串
		 * \return 数值
		 */
		static int system62toi(std::string system62)
		{
			int value = 0;
			// 转换
			for (std::string::iterator iter = system62.begin(); iter != system62.end(); ++iter)
			{
				value = (value * SYSTEM_62) + char62toi(*iter);
			}

			return value;
		}
};

template <typename T>
struct singleton_default
{
	private:
		singleton_default();

	public:
		typedef T object_type;

		static object_type & instance()
		{
			return obj;
		}

		static object_type obj;
};
template <typename T>
typename singleton_default<T>::object_type singleton_default<T>::obj;

//手动调用构造函数，不分配内存
	template<class _T1> 
inline	void constructInPlace(_T1  *_Ptr)
{
	new (static_cast<void*>(_Ptr)) _T1();
}
/// 声明变长指令
#define BUFFER_CMD(cmd,name,len) char buffer##name[len]={0};\
														cmd *name=(cmd *)buffer##name;constructInPlace(name);

	template < typename T>
T **make_2d_array(const int x,const int y , T *value=NULL,const int width=0)
{
	T **t;
	t = FIR_NEW T*[x];
	if(value)
	{
		for(int i=0 ; i < x ; i++)
		{
			t[i] = FIR_NEW T[y];
			for(int j=0 ; j < y ; j++)
			{
				t[i][j]=value[j*width + i];
			}
		}
	}
	else
	{
		for(int i=0 ; i < x ; i++)
		{
			*t = FIR_NEW T[y];
		}
	}
	return t;
}
	template < typename T>
void free_2d_array(T **&t ,const int x,const int y)
{
	for(int i=0 ; i < x ; i++)
	{
		SAFE_DELETE_VEC(t[i]);
	}
	SAFE_DELETE_VEC(t);
}


template <typename T>
class zCmdCheck : public zNoncopyable
{
	public:
		zCmdCheck()
		{
		}

		virtual ~zCmdCheck()
		{
			cmdset.clear();
		}

		bool put(const T& t)
		{
			CmdSet_Iter iter = cmdset.find(t);
			if (iter != cmdset.end())
				return false;

			cmdset.insert(t);
			return true;
		}       

		void erase(const T& t)
		{       
			cmdset.erase(t);
		}       

		virtual void list(const char* desc = NULL) 
		{       
			std::ostringstream oss;
			if (desc)
				oss << desc << ": ";
			CmdSet_Iter iter = cmdset.begin();
			for(; iter != cmdset.end(); ++iter)     
			{       
				oss << *iter << " "; 
			}       
			Fir::logger->trace("%s, %s", __PRETTY_FUNCTION__, oss.str().c_str());
		}

		bool check(const T& t)
		{
			CmdSet_Iter iter = cmdset.find(t);
			if (iter == cmdset.end()) return true;
			return false;
		}

		int size()
		{
			return cmdset.size();
		}

	protected:
		typedef std::set<T> CmdSet;
		typedef typename CmdSet::iterator CmdSet_Iter;
		CmdSet cmdset;
};

typedef zCmdCheck<WORD> CmdCheck;


#endif

