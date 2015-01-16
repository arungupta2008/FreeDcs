#include "../../include/Controller/log_rt.h"
#include <errno.h>
typedef unsigned uint;
typedef unsigned long long uint64;


// Set indentation 
void set_log_ident_rt(const char *ident){
	log_ident_rt = ident;
	}

/*
 * Modified standard because it does not use tm_milisec 
 * */
struct tm_rt {
   int tm_milisec;   /*Mili seconds ranges from 0 1000   */
   int tm_sec;         /* seconds,  range 0 to 59          */
   int tm_min;         /* minutes, range 0 to 59           */
   int tm_hour;        /* hours, range 0 to 23             */
   int tm_mday;        /* day of the month, range 1 to 31  */
   int tm_mon;         /* month, range 0 to 11             */
   int tm_year;        /* The number of years since 1900   */
   int tm_wday;        /* day of the week, range 0 to 6    */
   int tm_yday;        /* day in the year, range 0 to 365  */
   int tm_isdst;       /* daylight saving time             */
};
/*
 * For Realtime LOG file OPEN and CLOSE Only Please don't touch
 * */
int 
open_rt_file(const char *path){
	rt_file = fopen (path,"a+");
	//printf("Opened file for writing %s\t%s\n",path,rt_file);
	//rt_log_message(LG_MSG, "rt_logging working");
	if (rt_file == NULL) {
        rt_printf("Error while opening Log file %s (%d)\n",path,errno);
        return FAIL;
    }
    return SUCCESS;
}

int 
close_rt_file(FILE *rt_log_internal){
	if(fclose(rt_log_internal) == 0)
	return SUCCESS; 
	
	return FAIL;	
}
// Checks Either Log file opened or not.
void log_check(){
	//printf("At Log check \n");
	if (rt_file == NULL) {
		rt_printf("Log File not opened. Opening Log file \n");
			if(! open_rt_file(log_file_rt) == SUCCESS){
				rt_printf("Failed To open Log file\n");
				//printf(" At success rt_logging started\n ");
				//rt_printf("At success rt_logging started\n");
				//rt_log_message(LG_MSG, "rt_logging started");
			}//else{
			//	rt_printf("Failed To open Log file\n");
			//	//printf("rt_logging failed\n ");
			//}
		}
	}
//****************Ended*****************************

char *asctime_rt(const struct tm_rt *timeptr){
    static char wday_name[7][3] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static char mon_name[12][3] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    static char result[26];


    sprintf(result, "%.3s %02d %02d:%02d:%02d:%03d ",
        mon_name[timeptr->tm_mon],
        timeptr->tm_mday, timeptr->tm_hour,
        timeptr->tm_min, timeptr->tm_sec,timeptr->tm_milisec);
    return result;
}

struct tm_rt* SecondsSinceEpochToDateTime(struct tm_rt* pTm, uint64 SecondsSinceEpoch , uint64 NanoSecSinceEpoc){
  uint64 sec,miliSec,microSec;
  uint quadricentennials, centennials, quadrennials, annuals/*1-ennial?*/;
  uint year, leap;
  uint yday, hour, min;
  uint month, mday, wday;
  static const uint daysSinceJan1st[2][13]=
  {
    {0,31,59,90,120,151,181,212,243,273,304,334,365}, // 365 days, non-leap
    {0,31,60,91,121,152,182,213,244,274,305,335,366}  // 366 days, leap
  };
/*
  400 years:

  1st hundred, starting immediately after a leap year that's a multiple of 400:
  n n n l  \
  n n n l   } 24 times
  ...      /
  n n n l /
  n n n n

  2nd hundred:
  n n n l  \
  n n n l   } 24 times
  ...      /
  n n n l /
  n n n n

  3rd hundred:
  n n n l  \
  n n n l   } 24 times
  ...      /
  n n n l /
  n n n n

  4th hundred:
  n n n l  \
  n n n l   } 24 times
  ...      /
  n n n l /
  n n n L <- 97'th leap year every 400 years
*/

  // Re-bias from 1970 to 1601:
  // 1970 - 1601 = 369 = 3*100 + 17*4 + 1 years (incl. 89 leap days) =
  // (3*100*(365+24/100) + 17*4*(365+1/4) + 1*365)*24*3600 seconds
  // Calculating Mili Seconds
  miliSec = (NanoSecSinceEpoc / 1000000) % 1000;
  // Calculating  Micro Seconds 
  //microSec = (SecondsSinceEpoch / 1000) % 1000;
  sec = SecondsSinceEpoch + 11644473600;

  wday = (uint)((sec / 86400 + 1) % 7); // day of week

  // Remove multiples of 400 years (incl. 97 leap days)
  quadricentennials = (uint)(sec / 12622780800ULL); // 400*365.2425*24*3600
  sec %= 12622780800ULL;

  // Remove multiples of 100 years (incl. 24 leap days), can't be more than 3
  // (because multiples of 4*100=400 years (incl. leap days) have been removed)
  centennials = (uint)(sec / 3155673600ULL); // 100*(365+24/100)*24*3600
  if (centennials > 3)
  {
    centennials = 3;
  }
  sec -= centennials * 3155673600ULL;

  // Remove multiples of 4 years (incl. 1 leap day), can't be more than 24
  // (because multiples of 25*4=100 years (incl. leap days) have been removed)
  quadrennials = (uint)(sec / 126230400); // 4*(365+1/4)*24*3600
  if (quadrennials > 24)
  {
    quadrennials = 24;
  }
  sec -= quadrennials * 126230400ULL;

  // Remove multiples of years (incl. 0 leap days), can't be more than 3
  // (because multiples of 4 years (incl. leap days) have been removed)
  annuals = (uint)(sec / 31536000); // 365*24*3600
  if (annuals > 3)
  {
    annuals = 3;
  }
  sec -= annuals * 31536000ULL;

  // Calculate the year and find out if it's leap
  year = 1601 + quadricentennials * 400 + centennials * 100 + quadrennials * 4 + annuals;
  leap = !(year % 4) && (year % 100 || !(year % 400));

  // Calculate the day of the year and the time
  yday = sec / 86400;
  sec %= 86400;
  hour = sec / 3600;
  sec %= 3600;
  min = sec / 60;
  sec %= 60;

  // Calculate the month
  for (mday = month = 1; month < 13; month++)
  {
    if (yday < daysSinceJan1st[leap][month])
    {
      mday += yday - daysSinceJan1st[leap][month - 1];
      break;
    }
  }

  // Fill in C's "struct tm"
  memset(pTm, 0, sizeof(*pTm));
  pTm->tm_milisec= miliSec;   // [0-999]
  pTm->tm_sec = sec;          // [0,59]
  pTm->tm_min = min;          // [0,59]
  pTm->tm_hour = hour;        // [0,23]
  pTm->tm_mday = mday;        // [1,31]  (day of month)
  pTm->tm_mon = month - 1;    // [0,11]  (month)
  pTm->tm_year = year - 1900; // 70+     (year since 1900)
  pTm->tm_wday = wday;        // [0,6]   (day since Sunday AKA day of week)
  pTm->tm_yday = yday;        // [0,365] (day since January 1st AKA day of year)
  pTm->tm_isdst = -1;         // daylight saving time flag

  return pTm;
} 

// Get Xenomai and Linux realtime clocks.
// return 0 on success and -1 otherwise.
int
get_realtime_clocks(struct timespec *pxeno)
{
    if (clock_gettime(CLOCK_REALTIME, pxeno) != 0) {
      rt_printf("%s: error clock_gettime\n", __FUNCTION__);
      return -1;
    }
    //rt_printf("CASE3.2.1 \n");
    return 0;
}

int
rt_fill_time_string(FILE *rt_file_, int n)
{
	//log_check();
	//printf("rt_fill_time_string\n");
	struct tm_rt t;
    struct timespec posix_real;
    uint64 retval=0;
    //rt_printf("CASE3.1 \n");
    retval = get_realtime_clocks(&posix_real);
    //rt_printf("CASE3.2 \n");
    if (retval != 0) {
      rt_printf("%s: cannot read clocks\n", __FUNCTION__);
      exit(1);
    }
    n += rt_fprintf(rt_file_,"%s",asctime_rt(SecondsSinceEpochToDateTime(&t,posix_real.tv_sec,posix_real.tv_nsec)));
    //rt_printf("%s", asctime(SecondsSinceEpochToDateTime(&t,posix_real.tv_sec)));
    //rt_printf("CASE3.3 \n");
    return n;
    //returns 26-character string
    /* Format is month day hour:minute:second (24 time) */
    //return strftime(time_string, len, "%b %d %H:%M:%S ", localtime_r(&nowtime, &result));
}

void
_rt_log_message(int level, char *fmt, ...){
	va_list argp;
	va_start(argp, fmt);
	log_check();
	__rt_log_message(rt_file ,level ,fmt, argp);
	va_end(argp);
}
	
void
__rt_log_message(FILE *rt_file_ ,int level, char *fmt, va_list args)
{
	//printf("printf at Log message Printing Level %d\n",level);
	//rt_printf("at Log message Printing Level %d\n",level);
    int n=0;
    //char *msg;
    //va_list args;
   // char time_string[TIME_LENGTH];
    //char str[STRING_LENGTH];

    assert(level <= rt_log_level);
    //va_start(args, fmt);
    rt_print_auto_init(1);
	//asctime(SecondsSinceEpochToDateTime(&t, (t/1000000000)))
	//rt_printf("CASE2 \n");
	//rt_fprintf(rt_file,"\nBy Real Real Time by log_rt:: 1 number of bytes : %d \n",a);
    n += rt_fprintf(rt_file_, "%-9s ",   log_string[level]);
	//rt_printf("CASE3 \n");
    n += rt_fill_time_string(rt_file_ , n);
    //rt_printf("CASE4 \n");
    n += rt_fprintf(rt_file_, " [%s:%lu]: ", log_ident_rt, (long int) getpid());

    //msg = str + n;
    n  += rt_vfprintf(rt_file_, fmt, args);
    //printf("\n  New Size %d\n" , snprintf(str, STRING_LENGTH, "\n "));
    n += rt_fprintf(rt_file_, "\n");
    /* maximum size that could be filled in str will be STRING_LENGTH,
     * now if vsnprintf has overflown, it will return a value greater
     * than this. So make it proper.
     */
    if(n >= STRING_LENGTH) {
	/* last char is '\0', we need not write it,
	 * we add ... at the end to indicate an overflow
	 */
	//strcpy(str+STRING_LENGTH-6, " ...\n");
	//n = STRING_LENGTH-1;
		rt_log_message(LG_ERR, "Size of last log entry was greater than 2048");
    }
    // printf("Final Dtring going to log with byte Size %d at FD :%d  \n%s\n",n,log_file_fd,str);
    //rt_fprintf(file, "\nReal Time ::   %s\n",   asctime(SecondsSinceEpochToDateTime(&t, (t/1000000000))));
    //write(log_file_fd, str, n);
    //char new_line[1];
    //new_line[0]='\n';
    //write(log_file_fd, new_line, 1);
	va_end(args);
}