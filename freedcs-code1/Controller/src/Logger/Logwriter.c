#include "../../include/Controller/log_linux.h"
//For Setting up the rt_file 
//#include "../../include/Controller/log_rt.h"
#include <sys/file.h>

#define EXIT_FAILURE_  log_message(LG_MSG, "Incorrect arguments logwriter"); exit(1);
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0


static void skeleton_daemon()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0){
		EXIT_FAILURE_
        exit(EXIT_FAILURE);
	}

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    //int x;
    //for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    //{
    //    close (x);
    //}
   
}


int main( int argc, char *argv[] ){
	skeleton_daemon();
	
	const char *log_file="/log/Controller.log";
	int fd = open_log_file(log_file);
	pid_t get_pid = getpid();
	char str[6]; // Max PID is 5 only
	sprintf(str, "%d", get_pid);
	int pid_file = open("/var/run/logd.pid", O_CREAT | O_RDWR, 0666);
	write(pid_file,str, strlen(str));
	int rc = flock(pid_file, LOCK_EX | LOCK_NB);
	if(rc) {
		if(EWOULDBLOCK == errno)
        printf("logd already running\n"); // another instance is running
	}
	else {
		// this is the first instance
		log_message(LG_MSG, "logd started");
		for(;;){
		 sleep(SLEEP_TIME);
		 log_message(LG_MSG, "Checking Log file Size");
		 monitor_logsize();
		}
	}
	return EXIT_SUCCESS;
	}