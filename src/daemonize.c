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
