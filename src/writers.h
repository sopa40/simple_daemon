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

#ifndef WRITERS_H
#define WRITERS_H


#define LOG_FILE_NAME "/tmp/log"
#define SYS_INFO_FILE "/tmp/sys_info"
#define GET_SYS_INFO "inxi -Fxz > " SYS_INFO_FILE

/*!
    * \brief                Writes system info in sys_info file in program directory
    *
    * \warning              Information is obtained via console command # inxi -Fxz
    *
    * \return               0 on success, -1 on failure obtaining system info, -2 on failure opening file
    */
int write_sys_info(void);


/*!
    * \brief                Writes system info in /tmp/log file with time tracking
    *
    * \warning                 Information is saved in struct sysinfo. Failure is logged.ch
    *
    * \return               0 on success, -1 on failure
    */
int write_current_state(int status_level);


#endif // WRITERS_H
