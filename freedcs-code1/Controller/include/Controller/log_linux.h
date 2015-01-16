#ifndef SKEIN_LOG_H_LINUX__
#define SKEIN_LOG_H_LINUX__


#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/types.h>
//#include <syslog.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <errno.h>
#include <native/timer.h>

#include "log.h"
/*
 * Okay, I have modelled the levels for logging off the syslog() interface.
 * However, I would really prefer if only six of the levels are used. You
 * can see them below and I'll describe what each level should be for.
 * Hopefully webcat will remain consistent with these levels.
 *	-- vinod
 */
#define FAIL 1
#define SUCCESS 0
 
#define SLEEP_TIME 20
#define LOG_TRUNCATE_LIMIT 20971520 /* 20 MB */
//Default Log limit
//#define SLEEP_TIME 1
// For Testing 
/// For Testing 
// Indent

/*
 * Store the log level setting to MSG, which is the least level.
 */
extern int log_level;


extern const char *log_file;

extern const char *log_ident;

extern int open_log_file(const char* file);
extern void close_log_file(int fd);
extern void monitor_logsize(void);

extern void toggle_log_level();

extern int log_level;
extern void _log_message(int, char*, ...) __attribute__((format(printf, 2, 3)));
extern void __log_message(int log_file_fd_, int level, char *fmt, va_list args);
#define log_message(level, arg...) \
	(level) > log_level ? (void)0: _log_message(level, arg);	

extern void _log_dump(char*, ...) __attribute__((format(printf, 1, 2)));
#define log_dump(level, arg...) \
	(level) > log_level ? (void)0: _log_dump(arg);

extern void log_io(int level, const char *peername, char inout, const char *buffer, int buflen);
extern void set_log_level(int level);
extern void set_log_ident(const char *ident);
extern const char* get_log_ident();
void dup_log_fd_to_std(void);
int fill_time_string(char *time_string, int len);

void _safeclose(int *fd, int *b_fd);

extern int csc_open(const char *pathname, int flags, mode_t mode);

void set_log_size(const long int size);
long int get_log_size();

#define assertsyslog(cond) if(!(cond)) {log_message(LG_ERR, "assertion failed (%s) in file %s line %d\n", #cond, __FILE__, __LINE__); exit(1);}


#endif /* SKEIN_LOG_H_LINUX__ */
