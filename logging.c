#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include "logging.h"

//#define LOG_SYSLOG

#ifdef LOG_SYSLOG
#include <syslog.h>
#endif

#define LOG_LEVEL PLOG_INFO

#ifndef LOG_SYSLOG
int form_log(char *to, size_t size,
             char *fmt, va_list args)
 {
    int tsize;

    tsize = vsnprintf(to, size, fmt, args);

    /** Let the caller know that incoming message has been cut */
    if (tsize >= (int)size)
        return -1;

    return tsize;
}

static void put_log(char *buf, size_t buf_len, int msg_len)
{
    const char *overflow = "*** missing symbols - message too long ***\n";
    size_t to_write = msg_len > 0 ? (size_t) msg_len : buf_len;
    static FILE *pFile = NULL;

    if (!pFile)
        pFile = fopen("/tmp/log", "w+");
    if (!pFile)
        return;

    /*
     * actually don't care if the message was written
     * - there is no other way to print logs
     */
    fwrite(buf, sizeof(char), to_write, pFile);
    fflush(pFile);
    if (msg_len < 0)
        fwrite(overflow, sizeof(char), strlen(overflow), pFile);
}
#endif //#ifndef LOG_SYSLOG

/* currently log level is only a measure whether to print log or not */
void plog(LogLevel log_level, char *format, ...)
{
#ifndef LOG_SYSLOG
    char buf[BUF_SIZE];
    va_list args;
    int len;

    if (log_level >= PLOG_MAX ||
        log_level < LOG_LEVEL)
        return;

    va_start(args, format);
    len = form_log(buf, sizeof(buf), format, args);
    va_end(args);
    put_log(buf, sizeof(buf), len);
#else //#ifndef LOG_SYSLOG
    /* TODO: recalculate log level */
    va_list args;
    openlog(NULL, LOG_PID, LOG_USER);
    va_start(args, format);
    syslog(LOG_INFO, format, args);
    va_end(args);
    closelog();
#endif //#ifndef LOG_SYSLOG

}
