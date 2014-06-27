//#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
 
#include <native/task.h>
#include <native/timer.h>
 
#include  <rtdk.h>
     #include <stdio.h> 
//#include "../../include/Controller/statcom.h"
#include <native/timer.h>
RT_TASK demo_task;
 
typedef unsigned uint;
typedef unsigned long long uint64;


//extern struct tm {
//   int tm_sec;         /* seconds,  range 0 to 59          */
//   int tm_min;         /* minutes, range 0 to 59           */
//   int tm_hour;        /* hours, range 0 to 23             */
//   int tm_mday;        /* day of the month, range 1 to 31  */
//   int tm_mon;         /* month, range 0 to 11             */
//   int tm_year;        /* The number of years since 1900   */
//   int tm_wday;        /* day of the week, range 0 to 6    */
//   int tm_yday;        /* day in the year, range 0 to 365  */
//   int tm_isdst;       /* daylight saving time             */
//};
//
//extern char *asctime(const struct tm *timeptr)
//{
//    static char wday_name[7][3] = {
//        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
//    };
//    static char mon_name[12][3] = {
//        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
//        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
//    };
//    static char result[26];
//
//
//    sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
//        wday_name[timeptr->tm_wday],
//        mon_name[timeptr->tm_mon],
//        timeptr->tm_mday, timeptr->tm_hour,
//        timeptr->tm_min, timeptr->tm_sec,
//        1900 + timeptr->tm_year);
//    return result;
//}


struct tm* SecondsSinceEpochToDateTime(struct tm* pTm, uint64 SecondsSinceEpoch)
{
  uint64 sec;
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
 
 
 
void demo(void *arg){
  RT_TASK *curtask;
  RT_TASK_INFO curtaskinfo;
	rt_printf("\nDemo Called \n");
	RTIME t;
	RTIME period;
	RTIME read;
	char* str_buf;
	uint16_t h, m, s, milli, micro;
  // hello world
  rt_printf("Hello World!\n");
 
  // inquire current task
  curtask=rt_task_self();
  rt_task_inquire(curtask,&curtaskinfo);
  struct timespec pxeno;
  clock_gettime(CLOCK_REALTIME, &pxeno);
  rt_printf(" clock_gettime : %lld :  %lld\n", (long long)pxeno.tv_sec, pxeno.tv_nsec);
  //rt_printf(" clock_gettime %lld\n", pxeno);
	while(1)
	{
		/* get time */
		t = rt_timer_read();
		read = rt_timer_tsc();
		//rt_printf("\nReal Time ::   %s\n",   asctime(SecondsSinceEpochToDateTime(&t, t)));
		/* do something - e.g. print time to pipe */
		micro = (t / 1000) % 1000;
		milli = (t / 1000000) % 1000;
		s = (t / 1000000000) % 60;
		rt_printf("\nReal Time ::   %s\n",   asctime(SecondsSinceEpochToDateTime(&t, (t/1000000000))));
		FILE *file,*file1;
		file = fopen ("res","a+");
		rt_fprintf(file,"\nReal Time ::   %s\n",   asctime(SecondsSinceEpochToDateTime(&t, (t/1000000000))));
		char *msg;
		char str[500];
		msg = &str;
		int a=1;
		file1 = fmemopen (str, strlen (str), "a+");
		rt_fprintf(file1, "\nRealTime\n");
		fclose (file1);
		printf("\n\nPrinted to String |%s|\n\n\n", str);
		rt_fprintf(file,"\nPrinted from String ::   %s\n",str);
		
	char *bp;
       size_t size;
       FILE *stream;
     
       stream = open_memstream (&bp, &size);
       rt_fprintf (stream, "hello");
       fflush (stream);
       printf ("buf = `%s', size = %d\n", bp, size);
       rt_fprintf (stream, ", world");
       fclose (stream);
       printf ("buf = `%s', size = %d\n", bp, size);
     
     rt_fprintf(file,"\nPrinted from String2 ::   %s\n",bp);
		
		
		
		
		
		
		
		m = (t / 1000000000 / 60) % 60;
		h = (t / 1000000000 / 60 / 60) % 24;
		rt_printf("rt_timer_read : %lld\n", t);
		rt_printf("%02d:%02d:%02d.%03d.%03d\n", h, m, s, milli, micro);
		rt_printf(" rt_timer_tsc %lld\n", read);
		//rt_pipe_write(&msg_pipe, str_buf, strlen(str_buf), P_NORMAL);
		
		//rt_printf("Time : %s",str_buf);

		/* sleep till next step */
		//rt_task_wait_period(NULL);
		break ;
	}	
  // print task name
  rt_printf("Task name : %s \n", curtaskinfo.name);
}
 
int main(void)
{
  char  str[10] ;
 
  // Perform auto-init of rt_print buffers if the task doesn't do so
  rt_print_auto_init(1);
 
  // Lock memory : avoid memory swapping for this program
  mlockall(MCL_CURRENT|MCL_FUTURE);
 
  rt_printf("start task\n");
 
  /*
   * Arguments: &task,
   *            name,
   *            stack size (0=default),
   *            priority,
   *            mode (FPU, start suspended, ...)
   */
  sprintf(str,"hello");
  rt_task_create(&demo_task, str, 0, 50, 0);
 
  /*
   * Arguments: &task,
   *            task function,
   *            function argument
   */
  rt_task_start(&demo_task, &demo, 0);
  return 0;
}
