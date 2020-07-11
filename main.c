#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void write_message(char *message)
{
    
}

int main(void) {

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
    FILE *logptr = fopen("log","w");
    if(!logptr)
        exit(EXIT_FAILURE);

    fprintf(logptr, "Log started\n");
    fflush(logptr);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
        /* TODO: Log the failure */
        exit(EXIT_FAILURE);

    fprintf(logptr, "After sid\n");
    fflush(logptr);

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
        fprintf(logptr, "Logging\n");
        fflush(logptr);
        sleep(10);
    }

    fclose(logptr);
    exit(EXIT_SUCCESS);
}
