#ifndef WRITERS_H
#define WRITERS_H


#define LOG_FILE_NAME "/tmp/log"
#define SYS_INFO_FILE "/tmp/sys_info"
#define GET_SYS_INFO "inxi -Fxz > " SYS_INFO_FILE

/**
    * \brief                Writes system info in sys_info file in program directory
    *
    * \warning                 Information is obtained via console command # inxi -Fxz
    *
    * \return               0 on success, -1 on failure obtaining system info, -2 on failure opening file
    */
int write_sys_info(void);


/**
    * \brief                Writes system info in /tmp/log file with time tracking
    *
    * \warning                 Information is saved in struct sysinfo. Failure is logged.ch
    *
    * \return               0 on success, -1 on failure
    */
void write_current_state(void);


#endif // WRITERS_H
