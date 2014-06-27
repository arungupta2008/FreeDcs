#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

// Application settings # TODO: import these from a .ini file

#define WORKDIR   "/log"
#define LOGDIR    "/log"
#define LOGFILE   "/log/management.log"
#define SCRIPTDIR "/var"
#define PIDFILE   "/log/daemon.pid"

int main(void) {
  printf("Management Daemon\nInitializing...");
  pid_t pid, sid;

  setlogmask(LOG_UPTO (LOG_NOTICE)); // Above all these LOGS will be set to LOG_NOTICE
  openlog(LOGFILE, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  syslog(LOG_NOTICE, "Management Daemon started by User %d", getuid());
  closelog();


  pid = fork();
  printf("Done.\nForking...\n");
  if(pid < 0) {
    printf("Fork failed! Exiting...\n");
    // TODO: syslog facility

    syslog(LOG_EMERG, "Forking failed, exiting.");
    closelog();

    exit(EXIT_FAILURE);
  }
  if(pid > 0) {
    FILE *pidfile;
    pidfile = fopen(PIDFILE, "w");
    fprintf(pidfile, "%d\n", pid);
    fclose(pidfile);
    printf("PID written to %s\nUsing log file %s\nGoing silent...\n", PIDFILE, LOGFILE);
    // TODO: syslog facility

    openlog(LOGFILE, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_NOTICE, "Fork returned with valid PID: %d. PID file: %s", pid, PIDFILE);

    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if(sid < 0) {
    // TODO: syslog facility
    printf("SID is corrupt\n");
    exit(EXIT_FAILURE);
  }
  if(sid > 0) {
    printf("Acquired valid SID!\n");
  }
  if((chdir(WORKDIR)) < 0) {
    // TODO: syslog facility
    //The chdir() function shall cause the directory named by the pathname pointed to by the path argument to become the current working directory; that is, the starting point for path searches for pathnames not beginning with '/'.
    // Just like CD
    printf("Directory change failed. Got permissions?\n");
    exit(EXIT_FAILURE);
  }

  // Going Silent
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  // daemon init here

  FILE *fp;

  // The big loop
  closelog();

  while(1) {
    // Do your thing...
    // TODO: implement daemon executing mechanics.
    int ret; 
    ret = system("ping google.com -c 1");
    fp = fopen("/var/run/management.output", "a");
    if(ret == 0) {
      fprintf(fp, "Success!\n");
      fclose(fp);
    }
    if(ret == 512) {
      fprintf(fp, "Failure!\n");
      fclose(fp);
    }
    // Sleep till the next heartbeat
    // TODO: notice level log about heartbeats if verbosity is set to high
    sleep(30);
  }
 exit(EXIT_SUCCESS);
}