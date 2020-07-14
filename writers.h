#define LOG_FILE_NAME "/tmp/log"
#define SYS_INFO_FILE "sys_info"
#define GET_SYS_INFO "inxi -Fxz > " SYS_INFO_FILE

int write_sys_info(void);

void write_current_state(void);
