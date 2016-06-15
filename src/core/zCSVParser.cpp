#include "zCSVParser.h"
#include <stdio.h>
#include <time.h>

zCSVParser::zCSVParser()
{
	m_CSVData.clear();
	m_titleMap.clear();
	m_currLineNum = -1;
	m_strErrorInfo = "";
}

zCSVParser::~zCSVParser()
{
}

// 读取文件到字符串
bool zCSVParser::ReadAll(const string& strFileName, string& Data)
{
	//读取文件数据
	FILE* pFile = fopen(strFileName.c_str(), "rb");
	if (pFile == 0)
	{
		m_strErrorInfo = "[zCSVParser::ReadAll] read  file error!";
		return false;
	}

	fseek(pFile, 0, SEEK_END);
	long len = ftell(pFile);

	char *pBuffer = new char[len + 1];

	fseek(pFile, 0, SEEK_SET);
	fread(pBuffer, 1, len, pFile);
	fclose(pFile);
	pBuffer[len] = 0;
	int headLen = 0;
	char *pNewBuffer = pBuffer;
	if (len >= 3 && (unsigned char)pBuffer[0] == 0xEF && (unsigned char)pBuffer[1] == 0xBB && (unsigned char)pBuffer[2] == 0xBF) // 去掉bom头
	{
		pNewBuffer += 3;
		headLen = 3;
	}
		
	Data.assign(pNewBuffer, len - headLen);

	delete[] pBuffer;
	return true;
}

// 加载
bool zCSVParser::Load(const string& strFileName)
{
	m_strData = "";
	return (ReadAll(strFileName, m_strData) && Parse(m_strData) && ParseTitle());
}

// 解析
bool zCSVParser::Parse(const string& strText)
{
	// 清除之前的数据
	m_CSVData.clear();
	m_strErrorInfo.clear();

	if (strText.size() == 0)
	{
		m_strErrorInfo = "[zCSVParser::Parse] strText is empty!";
		return false;
	}

	// 定义状态
	enum StateType
	{
		// 新字段开始
		NewFieldStart,

		// 非引号字段
		NonQuotesField,

		// 引号字段
		QuotesField,

		// 字段分隔
		FieldSeparator,

		// 引号字段中的引号
		QuoteInQuotesField,

		// 行分隔符字符1，回车
		RowSeparator,

		// 语法错误
		Error,
	};

	vector< vector< string > >& Ret = m_CSVData;

	vector< string > Fields;
	string strField;

	// 设置初始状态
	StateType State = NewFieldStart;

	for (int i = 0, size = strText.size(); i < size; ++i)
	{
		const char& ch = strText[i];

		switch (State)
		{
			case NewFieldStart:
			{
				if (ch == '"')
				{
					State = QuotesField;
				}
				else if (ch == ',')
				{
					Fields.push_back("");
					State = FieldSeparator;
				}
				else if (ch == '\r' || ch == '\n')
				{
					m_strErrorInfo = "[zCSVParser::Parse] 语法错误：有空行";
					State = Error;
					return false;
				}
				else
				{
					strField.push_back(ch);
					State = NonQuotesField;
				}
			}
			break;

			case NonQuotesField:
			{
				if (ch == ',')
				{
					Fields.push_back(strField);
					strField.clear();
					State = FieldSeparator;
				}
				else if (ch == '\r')
				{
					Fields.push_back(strField);
					State = RowSeparator;
				}
				else if (ch == '\n')
				{
					Fields.push_back(strField);
					State = RowSeparator;
					i = i - 1;
				}
				else
				{
					strField.push_back(ch);
				}
			}
			break;

			case QuotesField:
			{
				if (ch == '"')
				{
					State = QuoteInQuotesField;
				}
				else
				{
					strField.push_back(ch);
				}
			}
			break;

			case FieldSeparator:
			{
				if (ch == ',')
				{
					Fields.push_back("");
				}
				else if (ch == '"')
				{
					strField.clear();
					State = QuotesField;
				}
				else if (ch == '\r')
				{
					Fields.push_back("");
					State = RowSeparator;
				}
				else if (ch == '\n')
				{
					Fields.push_back("");
					State = RowSeparator;
					i = i - 1;
				}
				else
				{
					strField.push_back(ch);
					State = NonQuotesField;
				}
			}
			break;

			case QuoteInQuotesField:
			{
				if (ch == ',')
				{
					// 引号字段闭合
					Fields.push_back(strField);
					strField.clear();
					State = FieldSeparator;
				}
				else if (ch == '\r')
				{
					// 引号字段闭合
					Fields.push_back(strField);
					State = RowSeparator;
				}
				else if (ch == '\n')
				{
					// 引号字段闭合
					Fields.push_back(strField);
					State = RowSeparator;
					i = i - 1;
				}
				else if (ch == '"')
				{
					// 转义
					strField.push_back(ch);
					State = QuotesField;
				}
				else
				{
					m_strErrorInfo = "[zCSVParser::Parse] 语法错误： 转义字符 \" 不能完成转义 或 引号字段结尾引号没有紧贴字段分隔符";
					State = Error;
					return false;
				}
			}
			break;

			case RowSeparator:
			{
				if (ch == '\n')
				{
					Ret.push_back(Fields);
					Fields.clear();
					strField.clear();
					State = NewFieldStart;
				}
				else
				{
					m_strErrorInfo = "[zCSVParser::Parse] 语法错误： 行分隔用了回车 \\r。但未使用回车换行 \\r\\n ";
					State = Error;
					return false;
				}
			}
			break;

			case Error:
			{
				return false;
			}
			break;

			default:
				break;
		}
	}
	// end for

	switch (State)
	{
		case NewFieldStart:
		{
			// Excel导出的CSV每行都以/r/n结尾。包括最后一行
		}
		break;

		case NonQuotesField:
		{
			Fields.push_back(strField);
			Ret.push_back(Fields);
		}
		break;

		case QuotesField:
		{
			m_strErrorInfo = "[zCSVParser::Parse] 语法错误： 引号字段未闭合";
			return false;
		}
		break;

		case FieldSeparator:
		{
			Fields.push_back("");
			Ret.push_back(Fields);
		}
		break;

		case QuoteInQuotesField:
		{
			Fields.push_back(strField);
			Ret.push_back(Fields);
		}
		break;

		case RowSeparator:
		{

		}
		break;

		case Error:
		{

		}
		break;
	default:
		break;
	}

	if (m_CSVData.size() > 0)
	{
		//默认指向标题行
		m_currLineNum = 0;
	}
	return true;
}

// 解析标题
bool zCSVParser::ParseTitle()
{
	m_titleMap.clear();
	if (m_CSVData.size() <= 0)
	{
		m_strErrorInfo = "[zCSVParser::ParseTitle] csv data is empty!";
		return false;
	}
	vector<string>& titelVec = m_CSVData[0];
	for (unsigned int i = 0; i < titelVec.size(); ++i)
	{
		string titile = titelVec[i];
		if (m_titleMap.find(titile) != m_titleMap.end())
		{
			m_strErrorInfo = "[zCSVParser::ParseTitle] csv title repeated!";
			return false;
		}
		m_titleMap.insert(make_pair(titelVec[i], i));
	}
	return true;
}

// 字符串转时间
time_t zCSVParser::StringToDatetime(const string& str)
{
	tm tm_;
	int year, month, day, hour, minute, second;
	sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	time_t t_ = mktime(&tm_); //已经减了8个时区  
	return t_; //秒时间  
}

// 移至标题行
bool zCSVParser::MoveToTitleLine()
{
	if (m_CSVData.size() <= 0)
	{
		m_strErrorInfo = "[zCSVParser::MoveToTitleLine] csv data is empty!";
		return false;
	}
	//标题行为第0行
	m_currLineNum = 0;
	return true;
}

// 移至下一行
bool zCSVParser::MoveToNextLine()
{
	if (-1==m_currLineNum || (unsigned int)m_currLineNum >= m_CSVData.size() - 1)
	{
		m_strErrorInfo = "[zCSVParser::MoveToNextLine] there is no more line!";
		return false;
	}
	m_currLineNum++;
	return true;
}

// 移至某一行
bool zCSVParser::MoveToLine(unsigned int lineNum)
{
	if (lineNum > m_CSVData.size()-1)
	{
		m_strErrorInfo = "[zCSVParser::MoveToLine] linenum is out of csv data!";
		return false;
	}
	m_currLineNum = lineNum;
	return true;
}

// 获取某个字段的值(string)
bool zCSVParser::GetFieldValue(const string& fieldName, string& strValue)
{
	strValue = "";
	if (m_titleMap.find(fieldName) == m_titleMap.end())
	{
		m_strErrorInfo = "[zCSVParser::GetFieldValue] cant find the given field!";
		return false;
	}
	int fieldNum = m_titleMap[fieldName];
	strValue = (m_CSVData[m_currLineNum])[fieldNum];
	return true;
}

bool zCSVParser::GetFieldValue(unsigned int fieldNum, string& strValue)
{
	strValue = "";
	if (fieldNum <= 0 || fieldNum > m_titleMap.size())
	{
		m_strErrorInfo = "[zCSVParser::GetFieldValue] fieldNum out of fields!";
		return false;
	}
	strValue = (m_CSVData[m_currLineNum])[fieldNum-1];
	return true;
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, string& strValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, strValue));
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, string& strValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldName, strValue));
}


// 获取某个字段的值(int)
bool zCSVParser::GetFieldValue(const string& fieldName, int& numValue)
{
	numValue = 0;
	if (m_titleMap.find(fieldName) == m_titleMap.end())
	{
		m_strErrorInfo = "[zCSVParser::GetFieldValue] cant find the given field!";
		return false;
	}
	int fieldNum = m_titleMap[fieldName];
	numValue = atoi((m_CSVData[m_currLineNum])[fieldNum].c_str());
	return true;
}

bool zCSVParser::GetFieldValue(unsigned int fieldNum, int& numValue)
{
	numValue = 0;
	if (fieldNum <= 0 || fieldNum > m_titleMap.size())
	{
		m_strErrorInfo = "[zCSVParser::GetFieldValue] fieldNum out of fields!";
		return false;
	}
	numValue = atoi((m_CSVData[m_currLineNum])[fieldNum-1].c_str());
	return true;
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, int& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, numValue));
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, int& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldName, numValue));
}

// 获取某个字段的值(long)
bool zCSVParser::GetFieldValue(const string& fieldName, long& numValue)
{
	numValue = 0;
	if (m_titleMap.find(fieldName) == m_titleMap.end())
	{
		m_strErrorInfo = "[zCSVParser::GetFieldValue] cant find the given field!";
		return false;
	}
	int fieldNum = m_titleMap[fieldName];
	numValue = atol((m_CSVData[m_currLineNum])[fieldNum].c_str());
	return true;
}

bool zCSVParser::GetFieldValue(unsigned int fieldNum, long& numValue)
{
	numValue = 0;
	if (fieldNum <= 0 || fieldNum > m_titleMap.size())
	{
		m_strErrorInfo = "[zCSVParser::GetFieldValue] fieldNum out of fields!";
		return false;
	}
	numValue = atol((m_CSVData[m_currLineNum])[fieldNum-1].c_str());
	return true;
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, long& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, numValue));
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, long& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldName, numValue));
}

// 获取某个字段的值(unsigned int)
bool zCSVParser::GetFieldValue(const string& fieldName, unsigned int& numValue)
{
	int val = 0;
	bool res = GetFieldValue(fieldName, val);
	if (!res)
	{
		return res;
	}
	numValue = (unsigned int)val;
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int fieldNum, unsigned int& numValue)
{
	int val = 0;
	bool res = GetFieldValue(fieldNum, val);
	if (!res)
	{
		return res;
	}
	numValue = (unsigned int)val;
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, unsigned int& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, numValue));
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, unsigned int& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldName, numValue));
}

// 获取某个字段的值(unsigned long)
bool zCSVParser::GetFieldValue(const string& fieldName, unsigned long& numValue)
{
	long val = 0;
	bool res = GetFieldValue(fieldName, val);
	if (!res)
	{
		return res;
	}
	numValue = (unsigned long)val;
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int fieldNum, unsigned long& numValue)
{
	long val = 0;
	bool res = GetFieldValue(fieldNum, val);
	if (!res)
	{
		return res;
	}
	numValue = (unsigned long)val;
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, unsigned long& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, numValue));
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, unsigned long& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldName, numValue));
}

// 获取某个字段的值(short)
bool zCSVParser::GetFieldValue(const string& fieldName, short& numValue)
{
	int val = 0;
	bool res = GetFieldValue(fieldName, val);
	if (!res)
	{
		return res;
	}
	numValue = (short)val;
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int fieldNum, short& numValue)
{
	int val = 0;
	bool res = GetFieldValue(fieldNum, val);
	if (!res)
	{
		return res;
	}
	numValue = (short)val;
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, short& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, numValue));
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, short& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldName, numValue));
}


// 获取某个字段的值(unsigned short)
bool zCSVParser::GetFieldValue(const string& fieldName, unsigned short& numValue)
{
	int val = 0;
	bool res = GetFieldValue(fieldName, val);
	if (!res)
	{
		return res;
	}
	numValue = (unsigned short)val;
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int fieldNum, unsigned short& numValue)
{
	int val = 0;
	bool res = GetFieldValue(fieldNum, val);
	if (!res)
	{
		return res;
	}
	numValue = (unsigned short)val;
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, unsigned short& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, numValue));
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, unsigned short& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldName, numValue));
}


// 获取某个字段的值(float)
bool zCSVParser::GetFieldValue(const string& fieldName, float& numValue)
{
	std::string val = "";
	bool res = GetFieldValue(fieldName, val);
	if (!res)
	{
		return res;
	}
	numValue = atof(val.c_str());
	return res;
}
bool zCSVParser::GetFieldValue(unsigned int fieldNum, float& numValue)
{
	std::string val = "";
	bool res = GetFieldValue(fieldNum, val);
	if (!res)
	{
		return res;
	}
	numValue = atof(val.c_str());
	return res;
}
bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, float& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, numValue));
}
bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, float& numValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldName, numValue));
}


// 获取某个字段的值(bool)
bool zCSVParser::GetFieldValue(const string& fieldName, bool& boolValue)
{
	std::string val = "";
	bool res = GetFieldValue(fieldName, val);
	boolValue = false;
	if ("1" == val || "true" == val || "True" == val)
	{
		boolValue = true;
	}
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int fieldNum, bool& boolValue)
{
	std::string val = "";
	bool res = GetFieldValue(fieldNum, val);
	boolValue = false;
	if ("1" == val || "true" == val || "True" == val)
	{
		boolValue = true;
	}
	return res;
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, bool& boolValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, boolValue));
}

bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, bool& boolValue)
{
	return (MoveToLine(lineNum) && GetFieldValue(fieldName, boolValue));
}



// 获取当前行某个字段的值(time)
//bool zCSVParser::GetFieldValue(const string& fieldName, time_t& timeValue)
//{
//	timeValue = 0;
//	if (m_titleMap.find(fieldName) == m_titleMap.end())
//	{
//		m_strErrorInfo = "[zCSVParser::GetFieldValue] cant find the given field!";
//		return false;
//	}
//	int fieldNum = m_titleMap[fieldName];
//	timeValue = StringToDatetime((m_CSVData[m_currLineNum])[fieldNum].c_str());
//	if (-1 == timeValue)
//	{
//		m_strErrorInfo = "[zCSVParser::GetFieldValue] timestr format error!";
//		return false;
//	}
//	return true;
//}
//
//bool zCSVParser::GetFieldValue(unsigned int fieldNum, time_t& timeValue)
//{
//	timeValue = 0;
//	if (fieldNum > m_titleMap.size())
//	{
//		m_strErrorInfo = "[zCSVParser::GetFieldValue] fieldNum out of fields!";
//		return false;
//	}
//	timeValue = StringToDatetime((m_CSVData[m_currLineNum])[fieldNum].c_str());
//	if (-1 == timeValue)
//	{
//		m_strErrorInfo = "[zCSVParser::GetFieldValue] timestr format error!";
//		return false;
//	}
//	return true;
//}
//
//bool zCSVParser::GetFieldValue(unsigned int lineNum, unsigned int fieldNum, time_t& timeValue)
//{
//	return (MoveToLine(lineNum) && GetFieldValue(fieldNum, timeValue));
//}
//
//bool zCSVParser::GetFieldValue(unsigned int lineNum, const string& fieldName, time_t& timeValue)
//{
//	return (MoveToLine(lineNum) && GetFieldValue(fieldName, timeValue));
//}

