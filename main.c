#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>



#define SYS_INFO_FILE sys_info

//creates new file named SYS_INFO_FILE and writes system information obtained from $ inxi -Fxz
int write_sys_info(void)
{
    FILE *sys_log = fopen("SYS_INFO_FILE", "w");
    if(!sys_log)
        exit(EXIT_FAILURE);

    time_t now;
    time(&now);
    fprintf(sys_log, "Logs obtained at %s\n", ctime(&now));
    fflush(sys_log);
    return system("inxi -Fxz >> SYS_INFO_FILE");
}

//writes information according to log options
//TODO: implement log options
int write_current_state(FILE *log_file) {
    struct sysinfo info;
    int test = sysinfo(&info);
    fprintf(log_file, "%lu\n", info.freeram);
    fflush(log_file);
}

int main(void) {

    if(write_sys_info()){
        printf("smth went wrong\n");
        exit(EXIT_FAILURE);
    }

    /* Process ID and Session ID */
    pid_t pid, sid;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* If we got a good PID, then
       we can exit the parent process. */
    if (pid > 0)
        exit(EXIT_SUCCESS);


    /* Change the file mode mask */
    umask(0);

    /* Open log */
    FILE *log_file = fopen("log","w");
    if(!log_file)
        exit(EXIT_FAILURE);

    fprintf(log_file, "Log started\n");
    fflush(log_file);

    write_current_state(log_file);


    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
        /* TODO: Log the failure */
        exit(EXIT_FAILURE);

    fprintf(log_file, "After sid\n");
    fflush(log_file);

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        /* TODO: Log the failure */
        exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* Daemon-specific initialization goes here */
    /* The Big Loop */
    while (1) {
        /* Do some task here ... */
        fprintf(log_file, "Logging\n");
        fflush(log_file);
        sleep(10);
    }

    fclose(log_file);
    exit(EXIT_SUCCESS);
}
