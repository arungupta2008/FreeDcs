/*
 * Dump CLOCK_HOST_REALTIME data in the vdso page
 * Written by Wolfgang Mauerer <wolfgang.mauerer at domain.hid>
 */

#include <stdio.h>
#include <signal.h>
#include <sys/mman.h>
#include <nucleus/vdso.h>
#include <nucleus/types.h>
//#include <nucleus/seqlock_user.h>
#include <native/task.h>
#include "../../include/Controller/engstatcom.h"
#define CLOCK_HOST_REALTIME 42

extern unsigned long xeno_sem_heap[2];
static unsigned modeswitches = 0;

void count_modeswitches(int sig __attribute__((unused)))
{
	modeswitches++;
}

int main(int argc, char **argv)
{
	int res;
	struct timespec ts1, ts2;

	mlockall(MCL_CURRENT|MCL_FUTURE);

	if (!xeno_sem_heap[1]) {
		fprintf(stderr, "Could not determine position of the "
			"global semaphore heap\n");
		return 1;
	}

	if (!xnvdso_test_feature(XNVDSO_FEAT_HOST_REALTIME)) {
		printf("XNVDSO_FEAT_HOST_REALTIME not available\n");
		return 1;
	}

	if (nkvdso->hostrt_data.live)
		printf("hostrt data area is live\n");
	else {
		printf("hostrt data area is not live\n");
		return 2;
	}

	printf("Sequence counter : %u\n",
	       nkvdso->hostrt_data.seqcount.sequence);
	printf("wall_time_sec    : %ld\n", nkvdso->hostrt_data.wall_time_sec);
	printf("wall_time_nsec   : %u\n", nkvdso->hostrt_data.wall_time_nsec);
	printf("wall_to_monotonic\n");
	printf("          tv_sec : %jd\n",
	       (intmax_t)nkvdso->hostrt_data.wall_to_monotonic.tv_sec);
	printf("         tv_nsec : %ld\n",
	       nkvdso->hostrt_data.wall_to_monotonic.tv_nsec);
	printf("cycle_last       : %lu\n", nkvdso->hostrt_data.cycle_last);
	printf("mask             : 0x%lx\n", nkvdso->hostrt_data.mask);
	printf("mult             : %u\n", nkvdso->hostrt_data.mult);
	printf("shift            : %u\n\n", nkvdso->hostrt_data.shift);

	res = clock_gettime(CLOCK_REALTIME, &ts1);
	if (res)
		printf("clock_gettime(CLOCK_REALTIME) failed!\n");

	signal(SIGXCPU, count_modeswitches);
	rt_task_set_mode(0, T_WARNSW, NULL);
	modeswitches = 0;
	res = clock_gettime(CLOCK_HOST_REALTIME, &ts2);
	if (res)
		printf("clock_gettime(CLOCK_HOST_REALTIME) failed!\n");

	if (modeswitches == 1) {
		printf("CLOCK_HOST_REALTIME caused a mode switch.\n");
		return 3;
	}

	rt_task_set_mode(T_CONFORMING, 0, NULL);
	printf("CLOCK_REALTIME     : tv_sec = %jd, tv_nsec = %llu\n",
	       ts1.tv_sec, ts1.tv_nsec);
	printf("CLOCK_HOST_REALTIME: tv_sec = %jd, tv_nsec = %llu\n",
	       ts2.tv_sec, ts2.tv_nsec);
	return 0;
}