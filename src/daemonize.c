#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "daemonize.h"
#include "logging.h"


void daemonize()
{
    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        LOGI("Failed to fork. Exit...!");
        exit(EXIT_FAILURE);
    }

    if (pid > 0){
        exit(EXIT_SUCCESS);
    }
    else {
    }
    umask(0);
    sid = setsid();
    if (sid < 0) {
        LOGI("Failed to set session ID. Exit...");
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
        LOGI("Error changing directory. Terminating...");
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
