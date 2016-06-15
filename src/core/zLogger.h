/**
 * \file
 * \version  $Id: zLogger.h 19808 2008-02-25 08:51:20Z dzl $
 * \author  ,okyhc@263.sina.com
 * \date 2004年11月18日 15时25分57秒 CST
 * 
 * \brief Fir新的日志系统声明文件
 *
 * 本文件是定义了三个类<code>zLogger::zLevel</code>,<code>zLogger</code>,<code>zLogger::zLoggerLocalFileAppender</code>
 */

#ifndef _ZLOGGER_H_
#define _ZLOGGER_H_

#include <log4cxx/logger.h>
#include <log4cxx/dailyrollingfileappender.h>
#include <log4cxx/rolling/timebasedrollingpolicy.h>
#include <log4cxx/helpers/datetimedateformat.h>

#include "zType.h"
#include "zMutex.h"
#include <fstream>
#include <string.h>


/**
 * \def MSGBUF_MAX
 * \brief Fir日志系统定义的最大日志长度 
 */
#define MSGBUF_MAX 4096

/**
 * \brief Fir项目的日志类，以Log4cxx基础构建的。
 *
 * 目前实现了三种写日志方式，即控制台、本地文件和Syslog系统。Syslog的等级为user。
 *
 * 默认日志级别是#DEBUG
 *
 * 此类为线程安全类。
 */

//重载为了创建文件的符号链接
class LocalDailyRollingFileAppender : public log4cxx::DailyRollingFileAppender
{
public:
	LocalDailyRollingFileAppender();
	virtual ~LocalDailyRollingFileAppender();
	std::string filenamepattern;

	bool relinkfile(log4cxx::helpers::Pool& pool);
	bool my_rollover(log4cxx::helpers::Pool& p);

	virtual void subAppend(const log4cxx::spi::LoggingEventPtr& event, log4cxx::helpers::Pool &pool);
	virtual void activateOptions(log4cxx::helpers::Pool& pool);
};

class LocalTimeBasedRollingPolicy : public log4cxx::rolling::TimeBasedRollingPolicy
{
public:
	std::string getCurFileName(log4cxx::helpers::Pool &pool);
	~LocalTimeBasedRollingPolicy(){}
};


//继承一个新的 Logger
namespace log4cxx
{
        namespace spi {
          namespace location {
            class LocationInfo;
          }
        }
        // Any sub-class of Logger must also have its own implementation of
        // LoggerFactory.
        class XFactory :
                public virtual spi::LoggerFactory,
                public virtual helpers::ObjectImpl
        {
        public:
                DECLARE_ABSTRACT_LOG4CXX_OBJECT(XFactory)
                BEGIN_LOG4CXX_CAST_MAP()
                        LOG4CXX_CAST_ENTRY(XFactory)
                        LOG4CXX_CAST_ENTRY(spi::LoggerFactory)
                END_LOG4CXX_CAST_MAP()

                XFactory();
                virtual LoggerPtr makeNewLoggerInstance(
                   log4cxx::helpers::Pool& pool,
                   const LogString& name) const;
        };

        typedef helpers::ObjectPtrT<XFactory> XFactoryPtr;

        /**
        A simple example showing Logger sub-classing. It shows the
        minimum steps necessary to implement one's {@link LoggerFactory}.
        Note that sub-classes follow the hierarchy even if its loggers
        belong to different classes.
        */
        class XLogger : public Logger
        {
        // It's enough to instantiate a factory once and for all.
        static XFactoryPtr factory;

        public:
                DECLARE_ABSTRACT_LOG4CXX_OBJECT(XLogger)
                BEGIN_LOG4CXX_CAST_MAP()
                        LOG4CXX_CAST_ENTRY(XLogger)
                        LOG4CXX_CAST_ENTRY_CHAIN(Logger)
                END_LOG4CXX_CAST_MAP()

                /**
                        Just calls the parent constuctor.
                */
                XLogger(log4cxx::helpers::Pool& pool,
                        const LogString& name1) : Logger(pool, name1) {}

                /**
                        Nothing to activate.
                */
                void activateOptions() {}
				
				void setName(const std::string& setName)
				{
					name = setName;
				}

				static LoggerPtr getLogger(const LogString& name);

				static LoggerPtr getLogger(const helpers::Class& clazz);
        };

        typedef helpers::ObjectPtrT<XLogger> XLoggerPtr;

};

class zLogger
{
	private:
		//log4cxx::LoggerPtr _logger;
		log4cxx::XLoggerPtr _logger;

		bool addConsoleLog();
	public:
		zLogger(const std::string& name);
		~zLogger();
		
		bool setLevel(const std::string& level);
		bool removeConsoleLog();
		bool addLocalFileLog(const std::string& filename);
		bool addBasicFileLog(const std::string& filename);
		const log4cxx::LogString  getName();
		void setName(const std::string& setName);

		bool errorM(const char * fmt, ...) __attribute__((format(printf,2,3))) ;
		bool fatalM(const char * fmt, ...) __attribute__((format(printf,2,3)));
		bool warnM(const char * fmt, ...) __attribute__((format(printf,2,3)));

		void debug(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void trace(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void info(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void warn(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void error(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void fatal(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void iffy(const char * fmt, ...) __attribute__((format(printf,2,3)));
		void alarm(const char * fmt, ...) __attribute__((format(printf,2,3)));

		void sdebug(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void strace(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void sinfo(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void swarn(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void serror(const char* fmt, ...) __attribute__((format(printf,2,3)));
		void sfatal(const char* fmt, ...) __attribute__((format(printf,2,3)));



	private:
		char message[MSGBUF_MAX];
		zMutex msgMut;
};
#endif
