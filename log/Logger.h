#pragma once

#include <log4cplus/configurator.h>

#include <log4cplus/helpers/loglog.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#define IMPLEMENT_LOGGER(loggerName) static log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(#loggerName));

#define LOG_TRACE(msg) LOG4CPLUS_TRACE(logger, LOG4CPLUS_STRING_TO_TSTRING(msg))
#define LOG_DEBUG(msg) LOG4CPLUS_DEBUG(logger, LOG4CPLUS_STRING_TO_TSTRING(msg))
#define LOG_INFO(msg) LOG4CPLUS_INFO(logger, LOG4CPLUS_STRING_TO_TSTRING(msg))
#define LOG_WARN(msg) LOG4CPLUS_WARN(logger, LOG4CPLUS_STRING_TO_TSTRING(msg))
#define LOG_ERROR(msg) LOG4CPLUS_ERROR(logger, LOG4CPLUS_STRING_TO_TSTRING(msg))

#define LOG_TRACE_FMT(...) LOG4CPLUS_TRACE_FMT(logger, ##__VA_ARGS__)
#define LOG_DEBUG_FMT(...) LOG4CPLUS_DEBUG_FMT(logger, ##__VA_ARGS__)
#define LOG_INFO_FMT(...) LOG4CPLUS_INFO_FMT(logger, ##__VA_ARGS__)
#define LOG_WARN_FMT(...) LOG4CPLUS_WARN_FMT(logger, ##__VA_ARGS__)
#define LOG_ERROR_FMT(...) LOG4CPLUS_ERROR_FMT(logger, ##__VA_ARGS__)
