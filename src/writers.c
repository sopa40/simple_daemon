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
