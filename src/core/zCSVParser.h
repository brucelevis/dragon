/**********************************************
*				csv文件解析器				  *
***********************************************/

#ifndef _CSV_PARSER_H_
#define _CSV_PARSER_H_

#include <vector>
#include <map>
#include <string>
using namespace  std;


class zCSVParser
{
public:
	zCSVParser();
	~zCSVParser();

public:
	// 载入CSV文件
	bool Load(const string& strFileName);
	// 从字符串解析
	bool Parse(const string&  strText);
	// 读取文件数据至字符串
	bool ReadAll(const string& strFileName , string& strData );
	
public :
	vector< vector< string > >& GetCSVData(){ return m_CSVData; }
	const string& GetErrorInfo( ) const { return m_strErrorInfo; }
	const std::string& GetDataStr() { return m_strData; };
public:
	// 移至标题行
	bool MoveToTitleLine();
	// 移至下一行
	bool MoveToNextLine();
	// 移至某一行(0为标题行 真实数据行从1开始)
	bool MoveToLine(unsigned int lineNum);
	// 获取当前行号
	int GetCurrLineNum(){ return m_currLineNum; }
	// 获取总行数(不包含标题行)
	unsigned int GetTotalLine(){ return m_CSVData.size()-1; }

	// 获取当前行某个字段的值(string)
	bool GetFieldValue(const string& fieldName, string& strValue);
	bool GetFieldValue(unsigned int fieldNum, string& strValue);
	bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, string& strValue);
	bool GetFieldValue(unsigned int lineNum, const string& fieldName, string& strValue);

	// 获取某个字段的值(int)
	bool GetFieldValue(const string& fieldName, int& numValue);
	bool GetFieldValue(unsigned int fieldNum, int& numValue);
	bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, int& numValue);
	bool GetFieldValue(unsigned int lineNum, const string& fieldName, int& numValue);

	// 获取某个字段的值(long)
	bool GetFieldValue(const string& fieldName, long& numValue);
	bool GetFieldValue(unsigned int fieldNum, long& numValue);
	bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, long& numValue);
	bool GetFieldValue(unsigned int lineNum, const string& fieldName, long& numValue);
	
	// 获取某个字段的值(unsigened int)
	bool GetFieldValue(const string& fieldName, unsigned int& numValue);
	bool GetFieldValue(unsigned int fieldNum, unsigned int& numValue);
	bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, unsigned int& numValue);
	bool GetFieldValue(unsigned int lineNum, const string& fieldName, unsigned int& numValue);

	// 或取某个字段的值(unsgined long)
	bool GetFieldValue(const string& fieldName, unsigned long& numValue);
	bool GetFieldValue(unsigned int fieldNum, unsigned long& numValue);
	bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, unsigned long& numValue);
	bool GetFieldValue(unsigned int lineNum, const string& fieldName, unsigned long& numValue);
	
	// 获取某个字段的值(short)
	bool GetFieldValue(const string& fieldName, short& numValue);
	bool GetFieldValue(unsigned int fieldNum, short& numValue);
	bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, short& numValue);
	bool GetFieldValue(unsigned int lineNum, const string& fieldName, short& numValue);
	
	// 获取某个字段的值(unsigned short)
	bool GetFieldValue(const string& fieldName, unsigned short& numValue);
	bool GetFieldValue(unsigned int fieldNum, unsigned short& numValue);
	bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, unsigned short& numValue);
	bool GetFieldValue(unsigned int lineNum, const string& fieldName, unsigned short& numValue);

	// 获取某个字段的值(float)
	bool GetFieldValue(const string& fieldName, float& numValue);
	bool GetFieldValue(unsigned int fieldNum, float& numValue);
	bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, float& numValue);
	bool GetFieldValue(unsigned int lineNum, const string& fieldName, float& numValue);
	
	// 获取某个字段的值(bool)
	bool GetFieldValue(const string& fieldName, bool& boolValue);
	bool GetFieldValue(unsigned int fieldNum, bool& boolValue);
	bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, bool& boolValue);
	bool GetFieldValue(unsigned int lineNum, const string& fieldName, bool& boolValue);

	// 获取某个字段的值(time)
	//bool GetFieldValue(const string& fieldName, time_t& timeValue);
	//bool GetFieldValue(unsigned int fieldNum, time_t& timeValue);
	//bool GetFieldValue(unsigned int lineNum, unsigned int fieldNum, time_t& timeValue);
	//bool GetFieldValue(unsigned int lineNum, const string& fieldName, time_t& timeValue);
	
private:
	bool ParseTitle();
	static time_t StringToDatetime(const string& str);
private : 
	// 标题索引
	map<string, int> m_titleMap;
	// csv数据
	string m_strData;
	// csv数据
	vector< vector< string > > m_CSVData;
	// 错误信息
	string m_strErrorInfo;
	// 当前行
	int m_currLineNum;
};

#endif