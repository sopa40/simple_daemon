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


#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

#define LOGGING_TAG "[daemon]"
#define LOG_LEVEL PLOG_INFO

/*!
     * \brief                Returns current time
     *
     * \warning              No error handling
     *
     * \return               current time on success
     */
char* get_time(void);

#define LOGI(fmt, ...)  plog(LOG_LEVEL, "%s %s: " LOGGING_TAG "[I] " fmt, "\n", get_time(), ## __VA_ARGS__)

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
int form_log(char *to, size_t size,
             char *fmt, va_list args);

#endif //#ifndef LOG_SYSLOG


/*!
     * \brief                Prints message in custom log file or in syslog
     *
     * \warning              log file /tmp/log will be opened, but not closed properly.
     *
     * \param[log_level]     Level of logging
     * \param[format]        Format string
     *
     */
void plog(LogLevel log_level, char *format, ...);

#endif // LOGGING_H
