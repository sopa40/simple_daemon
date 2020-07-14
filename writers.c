#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "writers.h"
#include "logging.h"

/** creates new file named sys_info in program folder
 *and writes system information obtained from $ inxi -Fxz
 */
int write_sys_info(void)
{
    FILE *sys_log = fopen(SYS_INFO_FILE,"w");
    if (!sys_log)
        exit(EXIT_FAILURE);
    time_t now;
    time(&now);
    fprintf(sys_log, "Logs obtained at %s\n", ctime(&now));
    fflush(sys_log);
    return system(GET_SYS_INFO);
}

//TODO: implement log options
/** writes information according to log options */
void write_current_state(void)
{
    struct sysinfo info;
    if (sysinfo(&info)) {
        LOGI("Error obtaining info");
        exit(EXIT_FAILURE);
    }
    LOGI("%lu", info.freeram);
}
