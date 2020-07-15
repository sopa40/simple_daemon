# simple_daemon
A simple daemon project that monitors the system information and logs some information.

Current setup requires launching the daemon with a command line for simple demonstration. Console app will configure all required parameters and launch the daemon. Daemon logs can be found in syslog:
- Ubuntu:  `$tail -f /var/log/syslog`
- Majnaro: `$journalctl -a _PID=id`, where id of process can be obtained with `$pidof simple_daemon`.

About the daemon: The simple_daemon is a daemon that can log some system information into files and syslog.
To launch simple_daemon clone the project and just use `make` in source folder. In .build folder you can find build artefacts and target file.
Copy of the target file you can find in source folder as well. Just launch it with `$ ./simple_daemon`

With additional options you can configure types of logs, its frequency and verbosity.
Just run `$ ./simple_daemon --help` or `$ ./simple_daemon --usage` to read more about options.

To enable logging into syslog uncomment `#DEFINES := LOG_SYSLOG` (line 23) in the Makefile.