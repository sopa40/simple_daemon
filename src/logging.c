/*-----------------------------------------------------------------------
A simple daemon project that monitors the system information and logs some information.
Copyright (C) 2020  Nazar Sopiha

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
------------------------------------------------------------------------*/

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

char* get_time(void)
{
    time_t mytime = time(NULL);
    char* time_str = ctime(&mytime);
    time_str[strlen(time_str) - 1] = ' ';
    return time_str;
}

/*!
* \brief                Formats buffer to write it in log later
*
* \warning              It might have 0 or more arguments args
*
* \param[to]            Destination buffer.
* \param[size]          Size of arguments
* \param[fmt]           Format string
* \param[args]          Arguments to insert in format string
*
* \return               Size of string on success, -1 on failure
*/
static int form_log(char *to, size_t size,
                    char *fmt, va_list args)
 {
    int tsize;

    tsize = vsnprintf(to, size, fmt, args);

    /** Let the caller know that incoming message has been cut */
    if (tsize >= (int)size)
        return -1;

    return tsize;
}


/**
* \brief               Prints buffer into a log file
*
*
* \param[buf]           Source buffer
* \param[buf_len]       Length of buffer
* \param[msg_len]      Length of message
*
*/
void put_log(char *buf, size_t buf_len, int msg_len)
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
    if (msg_len < 0)
        fwrite(overflow, sizeof(char), strlen(overflow), pFile);

    fflush(pFile);
}
#endif //#ifndef LOG_SYSLOG

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
