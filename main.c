#include <argp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
//for defines e.g. LOG_INFO
#include <sys/syslog.h>

#include "logging.h"
#include "daemonize.h"


#define LOG_FILE_NAME "/tmp/log"
#define SYS_INFO_FILE "sys_info"
#define GET_SYS_INFO "inxi -Fxz > " SYS_INFO_FILE
#define LOGGING_TAG "[daemon]"
#define TOO_BIG_MSG "Message is too big. Can not be written\n"
#define WRONG_MSG_TYPE "This message is not informative. Skipped... \n"

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

/** creates new file named SYS_INFO_FILE
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
        LOGI("Error obtaining info\n");
        exit(EXIT_FAILURE);
    }
    LOGI("%lu\n", info.freeram);
}

int main(int argc, char **argv)
{
    LOGI("TEST");
    struct arguments arguments;
    short wait_time;

    /** Set argument defaults */
    arguments.frequency_level = 0;
    arguments.verbose = 0;
    arguments.automatic = 0;
    arguments.info = 0;
    arguments.status = 0;

    /** parce arguments */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);


    switch (arguments.frequency_level) {
        case 0:
            wait_time = 3;
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
            LOGI("smth went wrong writing system info\n");
        }
    }

    daemonize();

    /* Daemon-specific initialization */
    while (1) {
        write_current_state();
        sleep(wait_time);
    }

    exit(EXIT_SUCCESS);
}
