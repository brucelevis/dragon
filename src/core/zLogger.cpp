#include "zLogger.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

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

#include <iostream>
#include <locale.h>

#include "zTime.h"
#include <stdarg.h>
#include <sys/stat.h>
#include <time.h>
#include <string>


LocalDailyRollingFileAppender::LocalDailyRollingFileAppender()
{
}
LocalDailyRollingFileAppender::~LocalDailyRollingFileAppender()
{
	close();
}
bool LocalDailyRollingFileAppender::relinkfile(log4cxx::helpers::Pool& pool)
{
	using namespace log4cxx;
	using namespace log4cxx::rolling;
	using namespace log4cxx::helpers;
	using namespace log4cxx::pattern;
	
	LocalTimeBasedRollingPolicy policy;
	policy.setFileNamePattern(filenamepattern);
	policy.activateOptions(pool);
	std::string symlinkname = getFile();
	std::string filename = policy.getCurFileName(pool);
	
	struct stat statbuf;
	if(::lstat(symlinkname.c_str(), &statbuf) >= 0)
	{
		if(!S_ISLNK(statbuf.st_mode))
		{ //该文件已经存在了但是不是符号链接
			std::string msg = symlinkname;
			msg += "文件已经存在，但不是符号链接，请检查";
			log4cxx::helpers::LogLog::warn(LOG4CXX_STR(msg));
			return false;
		}
	}
	if(-1 == ::unlink(symlinkname.c_str()))
	{
	}
	//printf("创建%s--->>%s的符号链接\n", symlinkname.c_str(), filename.c_str());
	if( -1 == ::symlink(filename.c_str(), symlinkname.c_str()))
	{
		std::string msg = symlinkname;
		msg += "创建符号链接失败，请检查";
		msg += symlinkname;
		msg += "-->>";
		msg += filename;
		log4cxx::helpers::LogLog::warn(LOG4CXX_STR(msg));
		return false;
	}
	return true;
}
bool LocalDailyRollingFileAppender::my_rollover(log4cxx::helpers::Pool& p)
{
	using namespace log4cxx;
	using namespace log4cxx::rolling;
	using namespace log4cxx::helpers;
	using namespace log4cxx::spi;
	
	if (rollingPolicy != NULL) 
	{
		{
			synchronized sync(mutex);
			try {
				RolloverDescriptionPtr rollover1(rollingPolicy->rollover(getFile(), p));

				if (rollover1 != NULL) {
					if (rollover1->getActiveFileName() == getFile()) {
						closeWriter();

						bool success = true;

#if 0 //不用rename了
						if (rollover1->getSynchronous() != NULL) {
							success = false;

							try {
								success = rollover1->getSynchronous()->execute(p);
							} catch (std::exception& ex) {
								LogLog::warn(LOG4CXX_STR("Exception on rollover"));
							}
						}
#endif
						this->relinkfile(p);

						if (success) {
							if (rollover1->getAppend()) {
								fileLength = File().setPath(rollover1->getActiveFileName()).length(p);
							} else {
								fileLength = 0;
							}

							//
							//  async action not yet implemented
							//
							ActionPtr asyncAction(rollover1->getAsynchronous());
							if (asyncAction != NULL) {
								asyncAction->execute(p);
							}

							setFile(
									rollover1->getActiveFileName(), rollover1->getAppend(),
									bufferedIO, bufferSize, p);
						} else {
							setFile(
									rollover1->getActiveFileName(), true, bufferedIO, bufferSize, p);
						}
					} else {
						OutputStreamPtr os(new FileOutputStream(
									rollover1->getActiveFileName(), rollover1->getAppend()));
						WriterPtr newWriter(createWriter(os));
						closeWriter();
						setFile(rollover1->getActiveFileName());
						setWriter(newWriter);

						bool success = true;
#if 0 //不用rename了
						if (rollover1->getSynchronous() != NULL) {
							success = false;

							try {
								success = rollover1->getSynchronous()->execute(p);
							} catch (std::exception& ex) {
								LogLog::warn(LOG4CXX_STR("Exception during rollover"));
							}
						}
#endif		
						this->relinkfile(p);

						if (success) {
							if (rollover1->getAppend()) {
								fileLength = File().setPath(rollover1->getActiveFileName()).length(p);
							} else {
								fileLength = 0;
							}

							//
							//   async action not yet implemented
							//
							ActionPtr asyncAction(rollover1->getAsynchronous());
							if (asyncAction != NULL) {
								asyncAction->execute(p);
							}
						}

						writeHeader(p);
					}

					return true;
				}
			} catch (std::exception& ex) {
				LogLog::warn(LOG4CXX_STR("Exception during rollover"));
			}
		}

	}

	return false;
}

void LocalDailyRollingFileAppender::subAppend(const log4cxx::spi::LoggingEventPtr& event, log4cxx::helpers::Pool &p)
{
	std::string filename;
	std::string filename2 ;
	if(0)
	{
		filename = getFile();
		log4cxx::rolling::RolloverDescriptionPtr rollover1(rollingPolicy->rollover(getFile(), p));
		if(rollover1)
		{
			filename2 =  rollover1->getActiveFileName();
		}
	}
	//printf("开始change files filename:%s, filename2:%s\n", filename.c_str(), filename2.c_str());
	
	// The rollover check must precede actual writing. This is the
	// only correct behavior for time driven triggers.
	if (triggeringPolicy->isTriggeringEvent(this, event, getFile(), getFileLength())) 
	{
		//   wrap rollover request in try block since
		//    rollover may fail in case read access to directory
		//    is not provided.  However appender should still be in good
		//     condition and the append should still happen.
		try {
			my_rollover(p);
		} catch (std::exception& ex) {
			log4cxx::helpers::LogLog::warn(LOG4CXX_STR("Exception during rollover attempt."));
		}   
	}
	FileAppender::subAppend(event, p); 
}

void LocalDailyRollingFileAppender::activateOptions(log4cxx::helpers::Pool& pool) 
{
	using namespace log4cxx;
	using namespace log4cxx::rolling;
	using namespace log4cxx::helpers;
	using namespace log4cxx::pattern;

	LocalTimeBasedRollingPolicy* policy = new LocalTimeBasedRollingPolicy();
	LogString pattern(getFile());
	bool inLiteral = false;
	bool inPattern = false;

	for (size_t i = 0; i < datePattern.length(); i++) {
		if (datePattern[i] == 0x27 /* '\'' */) {
			inLiteral = !inLiteral;

			if (inLiteral && inPattern) {
				pattern.append(1, (logchar) 0x7D /* '}' */);
				inPattern = false;
			}
		} else {
			if (!inLiteral && !inPattern) {
				const logchar dbrace[] = { 0x25, 0x64, 0x7B, 0 }; // "%d{"
				pattern.append(dbrace);
				inPattern = true;
			}

			pattern.append(1, datePattern[i]);
		}
	}

	if (inPattern) {
		pattern.append(1, (logchar) 0x7D /* '}' */);
	}

	policy->setFileNamePattern(pattern);
	//policy->activateOptions(pool);
	filenamepattern = pattern;
	setTriggeringPolicy(policy);
	setRollingPolicy(policy);
	if(this->relinkfile(pool))
	{
		RollingFileAppenderSkeleton::activateOptions(pool);
	}
}

std::string LocalTimeBasedRollingPolicy::getCurFileName(log4cxx::helpers::Pool &pool)
{
	using namespace log4cxx;
	using namespace log4cxx::rolling;
	using namespace log4cxx::helpers;
	using namespace log4cxx::pattern;
	LogString buf; 
	ObjectPtr obj(new Date());  
	formatFileName(obj, buf, pool);  
	return buf;
}

using namespace log4cxx;
using namespace log4cxx::spi;
IMPLEMENT_LOG4CXX_OBJECT(XLogger)
IMPLEMENT_LOG4CXX_OBJECT(XFactory)

namespace log4cxx
{
	XFactoryPtr XLogger::factory = new XFactory();

	LoggerPtr XLogger::getLogger(const LogString& name)
	{
		return LogManager::getLogger(name, factory);
	}

	LoggerPtr XLogger::getLogger(const helpers::Class& clazz)
	{
		return XLogger::getLogger(clazz.getName());
	}

	XFactory::XFactory()
	{
	}

	LoggerPtr XFactory::makeNewLoggerInstance(log4cxx::helpers::Pool& pool, 
			const LogString& name) const
	{
		return new XLogger(pool, name);
	}
};

zLogger::zLogger(const std::string& name)
{
	//_logger = log4cxx::Logger::getLogger(name.c_str());
	_logger = log4cxx::XLogger::getLogger(name);
	addConsoleLog();
	setLevel("trace");
}

zLogger::~zLogger()
{
//	_logger->getLoggerRepository()->shutdown();
}

bool zLogger::addConsoleLog()
{
	log4cxx::PatternLayoutPtr cpl = new log4cxx::PatternLayout("\%d{yyyyMMdd-HH:mm:ss.SSS }%c %5p: %m%n");

	log4cxx::helpers::Pool pool;
	cpl->activateOptions(pool);
	log4cxx::ConsoleAppenderPtr appender = new log4cxx::ConsoleAppender(cpl);
	appender->setName("console");

	_logger->addAppender(appender);
	return true;
}

bool zLogger::removeConsoleLog()
{
	log4cxx::ConsoleAppenderPtr appender = _logger->getAppender("console");
	_logger->removeAppender(appender);
	appender->close();
	return true;
}

bool zLogger::addLocalFileLog(const std::string& filename)
{
	log4cxx::PatternLayoutPtr fpl = new log4cxx::PatternLayout("\%d{yyyyMMdd-HH:mm:ss.SSS }%c %5p: %m%n");
	log4cxx::helpers::Pool pool;
	fpl->activateOptions(pool);

	LocalDailyRollingFileAppender* appender = new  LocalDailyRollingFileAppender();
	//appender->setDatePattern("'.'yyyy-MM-dd-HH");
	appender->setDatePattern(".\%Y\%m\%d-\%H");
	appender->setLayout(fpl);
	appender->setName(filename);
	appender->setFile(filename);
	appender->activateOptions(pool);
	_logger->addAppender(appender);

	return true;
}

bool zLogger::addBasicFileLog(const std::string& filename)
{
	log4cxx::PatternLayoutPtr fpl = new log4cxx::PatternLayout("%m%n");
	log4cxx::helpers::Pool pool;
	fpl->activateOptions(pool);

	LocalDailyRollingFileAppender* appender = new  LocalDailyRollingFileAppender();
	//appender->setDatePattern("'.'yyyy-MM-dd-HH");
	appender->setDatePattern(".\%y\%m\%d-\%H");
	appender->setLayout(fpl);
	appender->setName(filename);
	appender->setFile(filename);
	appender->activateOptions(pool);
	_logger->addAppender(appender);

	return true;
}

/**
 * \brief 设置Logger的名字，它出现在每条日志信息中
 * \param setName 要被设置的名字
 */
void zLogger::setName(const std::string& setName)
{
    _logger->setName(setName);
}

//志等级 trace < debug < info < warn < error < fatal 
bool zLogger::setLevel(const std::string& level)
{
	if(level == "trace")
	{
		_logger->setLevel(log4cxx::Level::getTrace());
	}
	else if(level == "debug")
	{
		_logger->setLevel(log4cxx::Level::getDebug());
	}
	else if(level == "info")
	{
		_logger->setLevel(log4cxx::Level::getInfo());
	}
	else if(level == "warn")
	{
		_logger->setLevel(log4cxx::Level::getWarn());
	}
	else if(level == "error")
	{
		_logger->setLevel(log4cxx::Level::getError());
	}
	else if(level == "fatal")
	{
		_logger->setLevel(log4cxx::Level::getFatal());
	}
	else if(level == "off")
	{
		_logger->setLevel(log4cxx::Level::getOff());
	}
	else
	{
		_logger->setLevel(log4cxx::Level::getTrace());
	}
	return true;
}

const static int MaxLoggerSize = 1024;
//const static int MaxLoggerSize = 10*1024;

#define getMessage() \
	bzero(message, sizeof(message)); \
	va_list   va; \
	va_start(va, fmt); \
	vsnprintf(message, sizeof(message)-1, fmt, va); \
	va_end(va);

#define processLine()\
	std::string tmp(message);\
	bzero(message, sizeof(message)); \
	unsigned int index = 0;\
	for(unsigned int i = 0; i < tmp.size(); i++)\
	{\
		message[index++] = tmp[i];\
		break;\
		if(tmp[i] == '\t') \
		{\
			message[index++] = '\\'; \
			message[index++] = 't';\
		}\
		else if(tmp[i] == '\n') \
		{\
			message[index++] = '\\'; \
			message[index++] = 'n';\
		}\
		else\
		{\
			message[index++] = tmp[i];\
		}\
	}


void zLogger::debug(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->debug(message);
	msgMut.unlock();
}
void zLogger::trace(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->trace(message);
	msgMut.unlock();
}
void zLogger::info(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->info(message);
	msgMut.unlock();
}
void zLogger::warn(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->warn(message);
	msgMut.unlock();
}
void zLogger::error(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->error(message);
	msgMut.unlock();
}
void zLogger::fatal(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->fatal(message);
	msgMut.unlock();
}
void zLogger::iffy(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->info(message);
	msgMut.unlock();
}
void zLogger::alarm(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->info(message);
	msgMut.unlock();
}


void zLogger::sdebug(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->debug(message);
	msgMut.unlock();
}
void zLogger::strace(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->trace(message);
	msgMut.unlock();
}
void zLogger::sinfo(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->info(message);
	msgMut.unlock();
}
void zLogger::swarn(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->warn(message);
	msgMut.unlock();
}
void zLogger::serror(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->error(message);
	msgMut.unlock();
}
void zLogger::sfatal(const char* fmt, ...)
{
	msgMut.lock();
	getMessage();
	_logger->fatal(message);
	msgMut.unlock();
}

/**
 * \brief 写fatal程序日志,并发送到监控服务器
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::fatalM(const char * fmt, ...)
{
    msgMut.lock();
    getMessage();

    _logger->fatal(message);
    msgMut.unlock();
    return true;
}

/**
 * \brief 写error程序日志,并发送到监控服务器
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::errorM(const char * fmt, ...)
{
    msgMut.lock();
    getMessage();

    _logger->error(message);
    msgMut.unlock();
    return true;
}
/**
 * \brief 写warn程序日志,并发送到监控服务器
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::warnM(const char * fmt, ...)
{
    msgMut.lock();
    getMessage();

    _logger->warn(message);
    msgMut.unlock();
    return true;
}
/**
 * \brief 得到Logger的名字，它出现在每条日志信息中
 * \return  Logger名字
 */
const log4cxx::LogString zLogger::getName()
{
    return _logger->getName();
}