#ifndef SKEIN_LOG_H_RT__
#define SKEIN_LOG_H_RT__

#include "log.h"
#include <native/timer.h>
#include <stdio.h>
#include <stdarg.h>

#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
 
#include <native/task.h>
#include <native/timer.h>
#include <rtdk.h>
//#include "../../include/Controller/statcom.h"

#define FAIL 1
#define SUCCESS 0
// Indent
extern const char *log_file_rt;
extern const char *log_ident_rt;
extern int rt_log_level;
extern FILE *rt_file;
int open_rt_file(const char *path);
int close_rt_file(FILE *rt_log_internal);
void _rt_log_message(int, char*, ...) __attribute__((format(printf, 2, 3)));
#define rt_log_message(level, arg...) \
	(level) > rt_log_level ? (void)0: _rt_log_message(level, arg);
#define assertsyslog(cond) if(!(cond)) {rt_log_message(LG_ERR, "assertion failed (%s) in file %s line %d\n", #cond, __FILE__, __LINE__); exit(1);}	


#endif /* SKEIN_LOG_H_RT__ */