#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "zBILog.h"


#include <log4cxx/level.h>
#include <log4cxx/helpers/loglog.h>
#include <log4cxx/logger.h>
#include <log4cxx/rollingfileappender.h>
#include <log4cxx/dailyrollingfileappender.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/rolling/rolloverdescription.h>
#include <log4cxx/helpers/synchronized.h>
#include <log4cxx/helpers/fileoutputstream.h>
#include <log4cxx/helpers/date.h>
#include <log4cxx/logstring.h>
#include <log4cxx/spi/loggerfactory.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/spi/location/locationinfo.h>



const static int MaxLoggerSize = 10*1024+1;

#define getMessage() \
	char   message[MaxLoggerSize];\
	bzero(message, sizeof(message)); \
	va_list   va; \
	va_start(va, fmt); \
	vsnprintf(message, MaxLoggerSize-1, fmt, va); \
	va_end(va);


zBILog::zBILog(const std::string& name)
{
	_logger = log4cxx::XLogger::getLogger(name);
	addLocalFileLog(name);
	_logger->setLevel(log4cxx::Level::getInfo());
	
}

zBILog::~zBILog()
{

}


bool zBILog::addLocalFileLog(const std::string& filename)
{
	log4cxx::PatternLayoutPtr fpl = new log4cxx::PatternLayout("\%d{yyyyMMdd-HH:mm:ss.SSS},%m%n");
	log4cxx::helpers::Pool pool;
	fpl->activateOptions(pool);

	LocalDailyRollingFileAppender* appender = new  LocalDailyRollingFileAppender();
	appender->setDatePattern(".\%Y\%m\%d-\%H");
	appender->setLayout(fpl);
	appender->setName(filename);
	appender->setFile(filename);
	appender->activateOptions(pool);
	_logger->addAppender(appender);

	return true;
}

void zBILog::Log(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->info(message);
	msgMut.unlock();
}