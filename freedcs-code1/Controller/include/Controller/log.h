#ifndef FREEDCS__LOG__
#define FREEDCS__LOG__


#include <unistd.h>
#include <assert.h>


extern char *log_string[];



#ifndef LOG_STRING_ENUM__
#define LOG_STRING_ENUM__
typedef enum {
	LG_MSG,     /* always display this message e.g. webcat start & end */
	LG_CRIT,    /* critical conditions */
	LG_ERR,     /* error conditions */
	LG_WARN,    /* warning */
	LG_NOTICE,  /* normal, but significant, condition */
	LG_INFO,    /* informational message */
	LG_DEBUG,   /* debug-level message */
	LG_MAX	    /* should always come last, has no special meaning */	
} log_level_t;

#endif /* LOG_STRING_ENUM__ */

#ifndef CONSTANTS__
#define CONSTANTS__


#define TIME_LENGTH 26
#define STRING_LENGTH 2048
extern  int log_file_fd;


#endif /* CONSTANTS__ */

/*
 * This File stream for opening the 
 * */
 
//FILE *rt_file;

/*
 * Global file descriptor for the log file
 */
//Remember Please check that logd is running before calling the log_rt.c or not because if it's not running then log_file_fd have the STDERR_FILENO so we won't be able to log in to /log/Controller.log


#endif /* FREEDCS__LOG__ */