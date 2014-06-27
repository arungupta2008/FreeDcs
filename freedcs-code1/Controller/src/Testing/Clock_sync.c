#include <pthread.h>
#include <native/timer.h>
#include <rtdk.h>
#include <sys/mman.h>
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int __real_clock_gettime(clockid_t clock_id,
                   struct timespec *tp);


// Interval to sync clocks, 50 msec.
#define CLOCK_SYNC_NSEC_INTERVAL 50000000

// Maximum clock drift tolerated, 2 msec.
#define MAX_NSEC_DRIFT 2000000


void
make_rt_thread()
{
    int retval = 0;
    struct sched_param sparam;

    sparam.sched_priority = 1;

    if ((retval = pthread_setschedparam(pthread_self(),
                              SCHED_FIFO, &sparam)) != 0) {

        rt_printf("pthread_setschedparam: %s\n", strerror(retval));

        exit(retval);
    }
}

// Get Xenomai and Linux realtime clocks.
//
// return 0 on success and -1 otherwise.
int
get_realtime_clocks(struct timespec *pxeno,
                struct timespec *pposix)
{
    if (clock_gettime(CLOCK_REALTIME, pxeno) != 0) {
      rt_printf("%s: error clock_gettime\n", __FUNCTION__);
      return -1;
    }
	rt_printf(" Realtime : %lld", clock_gettime(CLOCK_REALTIME, pxeno));
    if (__real_clock_gettime(CLOCK_REALTIME, pposix) != 0) {
      rt_printf("%s: error __real_clock_gettime\n", __FUNCTION__);

      return -1;
    }

    return 0;
}


// Synchronize Xenomai clock to Linux's.
int
sync_xeno_clock(struct timespec *pposix)
{
    int retval;

    retval = clock_settime(CLOCK_REALTIME, pposix);
    if (retval != 0) {
      return -1;
    }

    return 0;
}

int
main (int argc, char** argv)
{
    struct timespec xeno_real, posix_real;
    struct timespec sync_interval;
    int retval;

    mlockall(MCL_CURRENT | MCL_FUTURE);
    make_rt_thread();

    //rt_print_auto_init(1);
    //rt_print_init(16384, "SYNC_RT");

    // sync clocks.
    retval = get_realtime_clocks(&xeno_real, &posix_real);
    if (retval != 0) {
      rt_printf("%s: cannot read clocks\n", __FUNCTION__);

      exit(1);
    }

    retval = sync_xeno_clock(&posix_real);
    if (retval != 0) {
      rt_printf("%s: cannot sync clock\n", __FUNCTION__);

      exit(1);
    }

    sync_interval.tv_sec = 0;
    sync_interval.tv_nsec = CLOCK_SYNC_NSEC_INTERVAL;

    // loop to keep clocks in sync.
    while (1) {
      nanosleep(&sync_interval, NULL);

      retval = get_realtime_clocks(&xeno_real, &posix_real);
      if (retval != 0) {
          continue;
      }
      if (xeno_real.tv_sec != posix_real.tv_sec) {
          // clocks drift, resync.
          retval = sync_xeno_clock(&posix_real);
          if (retval != 0) {
            rt_printf("%s: cannot resync clock\n", __FUNCTION__);

            continue;
          }
      } else if (labs(xeno_real.tv_nsec - posix_real.tv_nsec) >
               MAX_NSEC_DRIFT) {
          // clocks drift, resync.
          retval = sync_xeno_clock(&posix_real);
          if (retval != 0) {
            rt_printf("%s: cannot resync clock\n", __FUNCTION__);

            continue;
          }
      }
    }

    exit(1);
}