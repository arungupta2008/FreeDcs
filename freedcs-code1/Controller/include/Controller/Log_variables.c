#include "log.h"
#include "log_linux.h"
#include "log_rt.h"

#ifndef LOG_STRING__
#define LOG_STRING__
char *log_string[] = {
	"MESSAGE",
	"CRITICAL",
	"ERROR",
	"WARNING",
	"NOTICE",
	"INFO",
	"DEBUG",
	"MAX"
};
#endif /* LOG_STRING__ */

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

 int log_file_fd=STDERR_FILENO;
 
 
 int log_level=LG_MAX;
 
 const char *log_file="/log/Controller.log";
 
 
 const char *log_file_rt="/log/Controller.log";
const char *log_ident_rt ="FreeDcs_RT";

int rt_log_level = LG_MAX;

FILE *rt_file;
const char *log_ident="FreeDcs";