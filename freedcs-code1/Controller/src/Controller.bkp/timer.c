#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/queue.h>
#include <native/intr.h>

#define STACK_SIZE 8192
#define STD_PRIO 1

RT_TASK test_task_ptr;
int int_count = 0;
int end = 0;

//                     --s-ms-us-ns
RTIME task_period_ns =   1000000000llu;

void testtask(void *cookie){
	int count = 0;
	int ret;
	unsigned long overrun;
	ret = rt_task_set_periodic(NULL, TM_NOW, rt_timer_ns2ticks(task_period_ns));
	if (ret) {
		printf("error while set periodic, code %d\n",ret);
		return;
	}

	while(!end){
		ret = rt_task_set_mode(0, T_CONFORMING, NULL);
		//ret = rt_task_set_mode( 0,T_RRB,0 );
		if (ret) {
			printf("error while rt_task_set_mode, code %d\n",ret);
			return;
		}
		ret = rt_task_wait_period(&overrun);
		if (ret) {
			printf("error while rt_task_wait_period, code %d\n",ret);
			return;
		}
		count++;
		printf("message from testtask: count=%d\n", count);
		fflush(NULL);
	}
}
// signal-handler, to ensure clean exit on Ctrl-C
void clean_exit(int dummy) {
	printf("cleanup\n");
	end = 1;
	rt_task_delete(&test_task_ptr);
	printf("end\n");
}

int main(int argc, char *argv[]) {
	int err, ret;
	printf("start\n");
	// install signal handler
	signal(SIGTERM, clean_exit);	
	signal(SIGINT, clean_exit);	
	// start timer -> depricated in Xeno 2.2
	/* ret = rt_timer_start(TM_ONESHOT);
	switch (ret) {
		case 0:       printf("timer started\n");
		              break;
		case -EBUSY:  printf("timer is running\n");
		              break;
		case -ENOSYS: printf("can't start timer\n");
		              return ret;
	}
	*/
	mlockall(MCL_CURRENT | MCL_FUTURE);
	err = rt_task_spawn(&test_task_ptr, "Timer", STACK_SIZE, STD_PRIO, 0, &testtask, NULL);
	if (err) {
		printf("error rt_task_spawn\n");
		return 0;
	}
	// wait for signal & return of signal handler
	pause();
	fflush(NULL);
	return 0;
}
