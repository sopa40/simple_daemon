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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "writers.h"
#include "logging.h"



int write_sys_info(void)
{
    FILE *sys_log = fopen(SYS_INFO_FILE,"w");
    if (!sys_log)
        return -2;
    time_t now;
    time(&now);
    fprintf(sys_log, "Logs obtained at %s\n", ctime(&now));
    fflush(sys_log);
    int res = system(GET_SYS_INFO);
    fclose(sys_log);
    return res;
}

//TODO: implement log options
int write_current_state(void)
{
    struct sysinfo info;
    if (sysinfo(&info)) {
        LOGI("Error obtaining info");
        return -1;
    }
    LOGI("%lu", info.freeram);
    return 0;
}
