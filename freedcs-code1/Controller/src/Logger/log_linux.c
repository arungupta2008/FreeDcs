#include "../../include/Controller/log_linux.h"

/*
 * Global file descriptor for the log file
 */
static int duped;
char log_file_name = '/log/Controller.log';
/*
 * log file size 
 * */
static long int log_file_size = LOG_TRUNCATE_LIMIT;
/*
 * Used to toggle the log-level, e.g. webcat -k debug
 */
static int configured_log_level=0;

/* keep track of the biggest fd
 */
extern int biggest_fd;

int
fill_time_string(char *time_string, int len)
{
    time_t nowtime;
    struct tm result;
    
    nowtime = time(NULL);
    /* Format is month day hour:minute:second (24 time) */
    return strftime(time_string, len, "%b %d %H:%M:%S ",
	     localtime_r(&nowtime, &result));
}

/*
 * Truncate log file to a zero length.
 */
static void
truncate_log_file(void)
{
    if(duped) {
	assert(log_file_fd == STDOUT_FILENO || log_file_fd == STDERR_FILENO);
	lseek(STDOUT_FILENO, 0, SEEK_SET);
	lseek(STDERR_FILENO, 0, SEEK_SET);
    } else {
	lseek(log_file_fd, 0, SEEK_SET);
    }
    ftruncate(log_file_fd, 0);
}

void
monitor_logsize(void)
{
//#define LOG_TRUNCATE_LIMIT 131072 /* 125 KB  For Testing*/
//#define LOG_TRUNCATE_LIMIT 10000 /* Bytes  For Testing*/
    struct stat sbuf;
    if(fstat(log_file_fd, &sbuf) == 0) {
	if(sbuf.st_size > LOG_TRUNCATE_LIMIT) {
	    truncate_log_file();
	    log_message(LG_MSG, "-----------------Log file truncated-------------\n");
	}
    } else {
	log_message(LG_CRIT, "monitor_logsize: fstat failure with logfile.");
    }
}

/*
 * Set the log level for writing to the log file.
 */
void
set_log_level(int level)
{
    configured_log_level = log_level = level;
}

void
set_log_ident(const char *ident)
{
    log_ident = ident;
}

const char*
get_log_ident()
{
    return log_file_size;
}

void
set_log_size(const long int size)
{
    log_file_size = size;
}

long int
get_log_size()
{
    return log_ident;
}

/*
 * Get the log level from string
 */
int
get_log_level_from_string(const char *string)
{
    int i;
    // Here 8 means Size of  log_string
    for(i=0; i < 8; i++) {
	if(strcasecmp(string, log_string[i]) == 0) {
	    return i;
	}
    }
    return -1;
}

//Yes to see this one
/*
 * -k debug calls this, to toggle the logging level.
 */
void
toggle_log_level()
{
    if(log_level == LG_MAX)
	log_level = configured_log_level;
    else
	log_level = LG_MAX;

    if(log_file_fd >= 0)
	log_message(LG_MSG, "Toggling log level to: %s\n", log_string[log_level]);
}

void
_log_dump(char *fmt, ...)
{
    int n;
    char str[STRING_LENGTH];
    va_list args;

    va_start(args, fmt);
    n = vsnprintf(str, STRING_LENGTH, fmt, args);
    if(n >= STRING_LENGTH) {
	strcpy(str+STRING_LENGTH-6, " ...\n");
	n = STRING_LENGTH-1;
    }
    write(log_file_fd, str, n);
    va_end(args);
}
// Checks Either Log file opened or not.
void linux_log_check(){
	if (log_file_fd == STDERR_FILENO) {
		//printf("Log File not opened. Opening Log file \nNote: Here Standard stdout file is opened  \n");
			if(open_log_file(log_file) > STDERR_FILENO){
				log_message(LG_MSG, "linux_logging started");
			}else{
				printf("Failed To open Log file\n");
			}
		}
	}
	
void
_log_message(int level, char *fmt, ...)
{
	linux_log_check();
	//Opening File for Logging !!!
	//const char *log_file="/log/Controller.log";
	//int fd = open_log_file(log_file);
	// printf("at Log message Printing Level %d\n",level);
    int n;
    char *msg;
    va_list args;
    char time_string[TIME_LENGTH];
    char str[STRING_LENGTH];

    assert(level <= log_level);
    va_start(args, fmt);

    fill_time_string(time_string, TIME_LENGTH);

    n = snprintf(str, STRING_LENGTH, "%-9s %s [%s:%lu]: ",
	    log_string[level], time_string, log_ident, (long int) getpid());

    msg = str + n;
    n  += vsnprintf(msg, STRING_LENGTH-n, fmt, args);
    //printf("\n  New Size %d\n" , snprintf(str, STRING_LENGTH, "\n "));
    
    /* maximum size that could be filled in str will be STRING_LENGTH,
     * now if vsnprintf has overflown, it will return a value greater
     * than this. So make it proper.
     */
    if(n >= STRING_LENGTH) {
	/* last char is '\0', we need not write it,
	 * we add ... at the end to indicate an overflow
	 */
	strcpy(str+STRING_LENGTH-6, " ...\n");
	n = STRING_LENGTH-1;
    }
    // printf("Final Dtring going to log with byte Size %d at FD :%d  \n%s\n",n,log_file_fd,str);
    write(log_file_fd, str, n);
    char new_line[1];
    new_line[0]='\n';
    write(log_file_fd, new_line, 1);

    va_end(args);
}


void
log_io(int level, const char *peername, char inout, const char *buffer, int buflen)
{
    int n;

    char time_string[TIME_LENGTH];
    char str[STRING_LENGTH];

    if(level > log_level) {
	return;
    }

    fill_time_string(time_string, TIME_LENGTH);

    n = snprintf(str, STRING_LENGTH, "%-9s %s [%lu]: %s %c ",
	    log_string[level], time_string, (long int) getpid(), peername, inout);

    write(log_file_fd, str, n);
    write(log_file_fd, buffer, buflen);
}

int
csc_open(const char *pathname, int flags, mode_t mode)
{
    int fd;
    if ((fd = open(pathname, flags, mode)) == -1) {
	log_message(LG_ERR, "csc_open: open(%s) error '%s'\n", pathname, strerror(errno));
	return FAIL;
    }
    return fd;
}

/*
 * Open the log file and store the file descriptor in a global location.
 */
int
open_log_file(const char* log_file_name)
{
	// printf("trying to Open a Log file for Logging\n");
	// printf("Initinal File Discriptor Info : %d\n",log_file_fd);
    //log_file_fd = csc_open(log_file_name,O_WRONLY | O_CREAT | O_TRUNC | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH , 0777);
    log_file_fd = csc_open(log_file_name, O_CREAT|O_WRONLY|O_APPEND , 0666);
    //log_file_fd = csc_open(log_file_name ,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    // 777(octal) || 111111111 || 511 (decimal)
    //666(octal) || 110110110 || 438(decimal)
    // O_WRONLY | O_CREAT | O_TRUNC | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
    // O_CREAT|O_WRONLY|O_APPEND
    // printf("File Discriptor Info : %d\n",log_file_fd);
    return log_file_fd;
}

/*
 * Close the log file
 */
void
close_log_file(int fd)
{
    if (close(fd) == -1) {
	log_message(LG_ERR, "safeclose: fd (%d) close error '%s'\n", fd, strerror(errno));
	return;
    }
    fd = -1;
}

//void
//dup_log_fd_to_std(void)
//{
//    int old_fd;
//    if(log_file_fd == STDOUT_FILENO || log_file_fd == STDERR_FILENO)
//	return;
//    duped = 1;
//
//    csc_dup2(log_file_fd, STDOUT_FILENO, &biggest_fd);
//    csc_dup2(log_file_fd, STDERR_FILENO, &biggest_fd);
//    /* now we have stderr and stdout pointing to our log */
//    /* close the original log file */
//    old_fd = log_file_fd;
//    log_file_fd = STDOUT_FILENO;
//    close(old_fd);
//}

//int main(){
//	printf("Here");
//	const char *log_file="/log/Controller.log";
//	printf("Here %s\n",log_file);
//	printf("Opening Log file\n");
//	int fd = open_log_file(log_file);
//	printf("putting LG_MSG\n");
//	log_message(LG_MSG, "LG_MSG");
//	printf("putting LG_CRTI\n");
//	log_message(LG_CRIT, "LG_CRIT");
//	printf("putting LG_ERR\n");
//	log_message(LG_ERR, "LG_ERR");
//	printf("putting LG_NOTICE\n");
//	log_message(LG_WARN, "LG_NOTICE");
//	printf("putting LG_INFOR\n");
//	log_message(LG_INFO, "LG_INFO");
//	printf("putting LG_DEBUG\n");
//	log_message(LG_DEBUG, "LG_DEBUG");
//	printf("putting LG_MAX\n");
//	log_message(LG_MAX, "LG_MAX");
//	for(;;){
//			int arun =10000;
//		 sleep(SLEEP_TIME);
//		 printf("Going to Sleep\n");
//		 printf("Checking Log file Size %d\n" ,arun);
//		 log_message(LG_MSG, "Checking Log file Size %d" ,arun);
//		 monitor_logsize();
//		}
//	return 0;
//	}