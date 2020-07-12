#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <argp.h>



#define SYS_INFO_FILE "sys_info"
#define GET_SYS_INFO "inxi -Fxz >> " SYS_INFO_FILE

#define LOG_FILE_NAME "log"

const char *argp_program_version = "simple_daemon 1.0";
const char *argp_program_bug_address = "<hazik991@gmail.com>";

//for parcing arguments
struct arguments {
  char *args[2];            /* ARG1 and ARG2 */
  int verbose;              /* The -v flag */
  int automatic;                 /* The -a flag */
  int info;                 /* The -i flag */
  int status;               /* The -s flag */
  char *frequency_level;  /* Argument for -f */
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
  {"frequency", 'f', "FRQNC_LVL", 0, "Frequency of logging"},
  // {"output",  'o', "OUTFILE", 0,
  // "Output to OUTFILE instead of to standard output"},
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
        case ARGP_KEY_ARG:
            if (state->arg_num >= 2)
                argp_usage(state);
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 2)
                argp_usage (state);
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
static char args_doc[] = "ARG1 ARG2";


/*
  DOC.  Field 4 in ARGP.
  Program documentation.
*/
static char doc[] =
"simple_daemon -- A program to log different system info.\vby Nazar Sopiha";

/*
   The ARGP structure itself.
*/
static struct argp argp = {options, parse_opt, args_doc, doc};



//creates new file named SYS_INFO_FILE and writes system information obtained from $ inxi -Fxz
int write_sys_info(void)
{
    FILE *sys_log = fopen(SYS_INFO_FILE, "w");
    if(!sys_log)
        exit(EXIT_FAILURE);

    time_t now;
    time(&now);
    fprintf(sys_log, "Logs obtained at %s\n", ctime(&now));
    fflush(sys_log);
    return system(GET_SYS_INFO);
}

//writes information according to log options
//TODO: implement log options
int write_current_state(FILE *log_file) {
    struct sysinfo info;
    int test = sysinfo(&info);
    if(test){
        fprintf(log_file, "Error obtaining info\n");
        fflush(log_file);
        exit(EXIT_FAILURE);
    }
    fprintf(log_file, "%lu\n", info.freeram);
    fflush(log_file);
}

int main(int argc, char **argv)
{
    struct arguments arguments;
    FILE *outstream = stdout;

    /* Set argument defaults */
    // arguments.outfile = NULL;
    arguments.frequency_level = "";
    arguments.verbose = 0;
    arguments.automatic = 0;
    arguments.info = 0;
    arguments.status = 0;

    /* Where the magic happens */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    /* Print argument values */
    fprintf (outstream, "frequency = %s\n\n",
       arguments.frequency_level);
    fprintf (outstream, "ARG1 = %s\nARG2 = %s\n\n",
       arguments.args[0],
       arguments.args[1]);


    if(write_sys_info()){
        printf("smth went wrong writing system info\n");
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
    FILE *log_file = fopen(LOG_FILE_NAME,"w");
    if(!log_file)
        exit(EXIT_FAILURE);

    fprintf(log_file, "Log started\n");
    fflush(log_file);

    write_current_state(log_file);


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
