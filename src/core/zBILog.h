/************************************************************************/
/* 此类专为BI日志打印类，重新封装了Log4XX，日志格式以逗号作为分隔           */
/************************************************************************/

#ifndef _zBILog_h_
#define _zBILog_h_

#include <string>
#include "zLogger.h"

class zBILog 
{

public:
	zBILog(const std::string& name);
	void Log(const char* fmt, ...) __attribute__((format(printf,2,3)));

	~zBILog();

private:
	bool addLocalFileLog(const std::string& filename);
	log4cxx::XLoggerPtr _logger;

	zMutex msgMut;

};

	

	

#endif


