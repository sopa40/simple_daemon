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
    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    /** Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        //LOGI("Failed to set session ID. Exit...");
        exit(EXIT_FAILURE);
    }

    /** Change the current working directory */
    if ((chdir("/")) < 0) {
        //LOGI("Error changing directory. Terminating...\n");
        exit(EXIT_FAILURE);
    }

    /** close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
