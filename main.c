#include <argp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define LOG_FILE_NAME "log"
#define SYS_INFO_FILE "sys_info"
#define GET_SYS_INFO "inxi -Fxz >> " SYS_INFO_FILE

const char *argp_program_version = "simple_daemon 1.0";
const char *argp_program_bug_address = "<hazik991@gmail.com>";

//for parcing arguments
struct arguments {
  char *args[2];            /* ARG1 and ARG2 */
  int verbose;              /* The -v flag */
  int automatic;            /* The -a flag */
  int info;                 /* The -i flag */
  int status;               /* The -s flag */
  char *frequency_level;    /* Argument for -f */
};

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output", 0},
  {"automatic", 'a', 0, 0, "Set automatic settings for logging format", 0},
  {"info", 'i', 0, 0, "Write system information in " SYS_INFO_FILE, 0},
  {"status", 's', 0, 0, "Write log in ", 0},
  {"frequency", 'f', "FRQNC_LVL", 0, "Frequency of logging", 0},
  {0}
};

/*
   PARSER. Field 2 in ARGP.
   Order of parameters: KEY, ARG, STATE.
*/
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    switch (key) {
        case 'a':
            arguments->automatic = 1;
            break;
        case 'v':
            arguments->verbose = 1;
            break;
        case 'f':
            arguments->frequency_level = arg;
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

/*
  DOC.  Field 4 in ARGP.
  Program documentation.
*/

static char doc[] =
"simple_daemon -- A program to log different system info.\vby Nazar Sopiha";

/*
   The ARGP structure itself.
*/
static struct argp argp = {options, parse_opt, 0, doc, 0, 0, 0};



//creates new file named SYS_INFO_FILE and writes system information obtained from $ inxi -Fxz
int write_sys_info(void)
{
    FILE *sys_log = fopen(LOG_FILE_NAME,"w");
    if (!sys_log)
        exit(EXIT_FAILURE);
    time_t now;
    time(&now);
    fprintf(sys_log, "Logs obtained at %s\n", ctime(&now));
    fflush(sys_log);
    return system(GET_SYS_INFO);
}

//writes information according to log options
//TODO: implement log options
void write_current_state(FILE *log_file)
{
    struct sysinfo info;
    if (sysinfo(&info)) {
        fprintf(log_file, "Error obtaining info\n");
        fclose(log_file);
        exit(EXIT_FAILURE);
    }
    fprintf(log_file, "%lu\n", info.freeram);
    fflush(log_file);
}

int main(int argc, char **argv)
{
    struct arguments arguments;
    pid_t pid, sid;
    time_t now;
    /* log journal */
    FILE *log_file;

    /* Set argument defaults */
    arguments.frequency_level = "";
    arguments.verbose = 0;
    arguments.automatic = 0;
    arguments.info = 0;
    arguments.status = 0;

    /* parce arguments */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* parent proccess exits with success */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Change the file mode mask */
    umask(0);

    /* Open log */
    log_file = fopen(LOG_FILE_NAME,"w");
    if (!log_file)
        exit(EXIT_FAILURE);

    time(&now);
    fprintf(log_file, "Logging started at %s\n", ctime(&now));
    fflush(log_file);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        fprintf(log_file, "Error setting own session id. Terminating...\n");
        fclose(log_file);
        exit(EXIT_FAILURE);
    }

    if (write_sys_info()) {
        printf("smth went wrong writing system info\n");
        fclose(log_file);
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        fprintf(log_file, "Error changing directory. Terminating...\n");
        fclose(log_file);
        exit(EXIT_FAILURE);
    }


    /* fclose out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* Daemon-specific initialization */

    while (1) {
        write_current_state(log_file);
        sleep(10);
    }

    fclose(log_file);
    exit(EXIT_SUCCESS);
}
