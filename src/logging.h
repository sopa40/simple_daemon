#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#define LOGGING_TAG "[daemon]"
#define LOG_LEVEL PLOG_INFO
#define LOGI(fmt, ...)  plog(LOG_LEVEL, "%s: " LOGGING_TAG " [I] " fmt, "\n" __TIME__, ## __VA_ARGS__)

//#define LOG_SYSLOG

#ifdef LOG_SYSLOG
#include <syslog.h>
#endif

#define BUF_SIZE 1024

typedef enum LogLevel {
    PLOG_INFO = 1,
    PLOG_DEBUG,
    PLOG_WARN,
    PLOG_ERROR,
    PLOG_FATAL,

    PLOG_MAX
} LogLevel;

#define LOG_LEVEL PLOG_INFO

#ifndef LOG_SYSLOG

/**
     * \brief               Formats buffer to write it in log later
     *
     * \warning                It might have 0 or more arguments args
     *
     * \param[to]            Destination buffer.
     * @param[size]          Size of arguments
     * @param[fmt]           Format string
     * @param[args]          Arguments to insert in format string
     *
     * @return              Size of string on success, -1 on failure
     */
int form_log(char *to, size_t size,
             char *fmt, va_list args);

#endif //#ifndef LOG_SYSLOG


/**
     * \brief               Prints message in custom log file or in syslog
     *
     * \warning                Currently log level is only a measure whether to print log or not
     *
     * \param[log_level]     Level of logging
     * @param[format]        Format string
     *
     */
void plog(LogLevel log_level, char *format, ...);

#endif // LOGGING_H
