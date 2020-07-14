#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>


#define LOG_LEVEL PLOG_INFO
#define LOGI(fmt, ...)  plog(LOG_LEVEL, "%s: " LOGGING_TAG " [I] " fmt "\n", __TIME__, ## __VA_ARGS__)

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
int form_log(char *to, size_t size,
             char *fmt, va_list args);

#endif //#ifndef LOG_SYSLOG

void plog(LogLevel log_level, char *format, ...);
