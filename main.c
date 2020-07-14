#include <argp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

#include "logging.h"
#include "daemonize.h"
#include "writers.h"




const char *argp_program_version = "simple_daemon 1.0";
const char *argp_program_bug_address = "<hazik991@gmail.com>";

///for parcing arguments
struct arguments {
  int verbose;              /* The -v flag */
  int automatic;            /* The -a flag */
  int info;                 /* The -i flag */
  int status;               /* The -s flag */
  int frequency_level;      /* The argument for -f option */
};

/**
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output", 0},
  {"automatic", 'a', 0, 0, "Set automatic settings for logging format", 0},
  {"info", 'i', 0, 0, "Write system information in " SYS_INFO_FILE, 0},
  {"status", 's', 0, 0, "Write logs in " LOG_FILE_NAME, 0},
  {"frequency", 'f', "FRQNC_LVL", 0, "Frequency of logging", 0},
  {0}
};

/**
   PARSER. Field 2 in ARGP.
   Order of parameters: KEY, ARG, STATE.
*/
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    int frqnc;
    struct arguments *arguments = state->input;
    switch (key) {
        case 'v':
            arguments->verbose = 1;
            break;
        case 'a':
            arguments->automatic = 1;
            break;
        case 'f':
            frqnc = atoi(arg);
            if (frqnc > 3 || frqnc < 1)
                argp_usage (state);
            arguments->frequency_level = frqnc;
            break;
        case 's':
            arguments->status = 1;
            break;
        case 'i':
            arguments->info = 1;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
        }
    return 0;
}

/**
  DOC.  Field 4 in ARGP.
  Program documentation.
*/
static char doc[] =
"simple_daemon -- A program to log different system info.\vby Nazar Sopiha";

/**
   The ARGP structure itself.
*/
static struct argp argp = {options, parse_opt, 0, doc, 0, 0, 0};

int main(int argc, char **argv)
{
    daemonize();
    struct arguments arguments;
    short wait_time;

    /** Set argument defaults */
    arguments.automatic = 0;
    arguments.verbose = 0;
    arguments.info = 0;
    arguments.status = 0;
    arguments.frequency_level = 0;

    /** parce arguments */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    if (arguments.automatic) {
        arguments.verbose = 0;
        arguments.info = 1;
        arguments.status = 1;
        arguments.frequency_level = 2;
    }

    switch (arguments.frequency_level) {
        case 0:
            wait_time = 5;
            break;
        case 1:
            wait_time = 10;
            break;
        case 2:
            wait_time = 3;
            break;
        case 3:
            wait_time = 1;
            break;
        default:
            exit(EXIT_FAILURE);
    }

    if (arguments.info) {
        if (write_sys_info()) {
            LOGI("smth went wrong writing system info");
        }
    }

    /* Daemon-specific initialization */
    while (1) {
        write_current_state();
        sleep(wait_time);
    }

    exit(EXIT_SUCCESS);
}
