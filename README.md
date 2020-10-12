# simple_daemon
A simple daemon project that monitors the system information and logs some of it.

Current setup requires launching the daemon with a command line for simple demonstration. Console app will configure all required parameters and launch the daemon. Daemon logs can be found depending on the setup either in /tmp/log or in syslog:
- Ubuntu:   `$ tail -f /var/log/syslog`
- Majnaro: `$ journalctl -a _PID=id`, where id of process can be obtained with `$pidof simple_daemon`.

About the daemon: The simple_daemon is a daemon that can log some system information into files (/tmp/log and /tmp/sys_info) and syslog.
To launch simple_daemon clone the project and just use `make` in source folder. In .build folder you can find build artefacts and target file.
Copy of the target file you can find in source folder as well. Just launch it with `$ ./simple_daemon`

With additional options you can configure types of logs, its frequency and verbosity.
Just run `$ ./simple_daemon --help` or `$ ./simple_daemon --usage` to read more about options.

To enable logging into syslog uncomment `#DEFINES := LOG_SYSLOG` in the Makefile.

## Build documentation

To create documentation required Doxygen 1.8.7+. Run in project folder `doxygen dconfig`. Documentation will appear in html folder.

No Latex is created, but you can change it in dconfig file.

## Build simple_daemon

`$ git clone https://github.com/sopa40/simple_daemon/`

`$ cd simple_daemon/src`

`$ make`

Done. Execute with `$ ./simple_daemon` and options that you prefer.

To terminate daemon:

`$ pidof simple_daemon`

and then with obtained pid

`$ kill -TERM pid`

## Build and run test

Testing made with Google testing framework (gtest)

Go to project folder

`$ cd src/`

`$ make test`

`$ ./test`
