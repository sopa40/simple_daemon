#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <argp.h>


#define LOG_FILE_NAME "log"
#define SYS_INFO_FILE "sys_info"
#define GET_SYS_INFO "inxi -Fxz >> " SYS_INFO_FILE



#define MAX_FRQNC_LVL 3
#define MIN_FRQNC_LVL 1

const char *argp_program_version = "simple_daemon 1.0";
const char *argp_program_bug_address = "<hazik991@gmail.com>";

//for parcing arguments
struct arguments {
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
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"automatic", 'a', 0, 0, "Set automatic settings for logging format"},
  {"info", 'i', 0, 0, "Write system information in " SYS_INFO_FILE},
  {"status", 's', 0, 0, "Write log in "},
  {"frequency", 'f', "LEVEL", 0, "Frequency of logging 1-3"},
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
            if (atoi(arg) > MAX_FRQNC_LVL || atoi(arg) < MIN_FRQNC_LVL)
                argp_usage(state);
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
   ARGS_DOC. Field 3 in ARGP.
   A description of the non-option command-line arguments
     that we accept.
*/

/*
  DOC.  Field 4 in ARGP.
  Program documentation.
*/
static char doc[] = "simple_daemon -- A program to log different system info.\vby Nazar Sopiha";

/*
   The ARGP structure itself.
*/
static struct argp argp = {options, parse_opt, 0, doc};



//creates new file named SYS_INFO_FILE and writes system information obtained from $ inxi -Fxz
int write_sys_info(void)
{
    FILE *sys_log = fopen(SYS_INFO_FILE, "w");
    if(!sys_log)
        exit(EXIT_FAILURE);
    time_t now;
    time(&now);
    fprintf(sys_log, "Logs obtained at %s\n", ctime(&now));
    fclose(sys_log);
    return 0;//system(GET_SYS_INFO);
}

//write information according to log options
//TODO: implement log options
void write_current_state(FILE *log_file)
{
    struct sysinfo info;
    if (sysinfo(&info)) {
        fprintf(log_file, "Error obtaining info\n");
        fflush(log_file);
        exit(EXIT_FAILURE);
    }
    time_t now;
    time(&now);
    fprintf(log_file, "Logs obtained at %s\n", ctime(&now));
    fprintf(log_file, "%lu\n", info.freeram);
    fflush(log_file);
}

int main(int argc, char **argv)
{
    struct arguments arguments;

    /* Set argument defaults */
    arguments.frequency_level = "";
    arguments.verbose = 0;
    arguments.automatic = 0;
    arguments.info = 0;
    arguments.status = 0;

    /* parsing arguments */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    /* Print argument values */
    //printf ("frequency = %s\n\n", arguments.frequency_level);

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
    FILE *log_file = fopen(LOG_FILE_NAME,"w");
    if (!log_file)
        exit(EXIT_FAILURE);

    time_t now;
    time(&now);
    fprintf(log_file, "Logs obtained at %s\n", ctime(&now));
    fflush(log_file);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        fprintf(log_file, "Error setting own session id. Terminating...\n");
        fflush(log_file);
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        fprintf(log_file, "Error changing directory. Terminating...\n");
        fflush(log_file);
        exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    fprintf(log_file, "before writing sys info\n");
    fflush(log_file);

    int test = write_sys_info();
    if (test) {
        fprintf(log_file, "smth went wrong writing system info\n");
        fflush(log_file);
        exit(EXIT_FAILURE);
    }
    else {
        fprintf(log_file, "else\n");
        fflush(log_file);
    }

    /* Daemon-specific initialization */
    while (1) {
        write_current_state(log_file);
        //TODO: sleep according to frequency level
        sleep(10);
    }

    fclose(log_file);
    exit(EXIT_SUCCESS);
}
