/*
	Xenomai demo - real time daemon
	Author    : M.Langer
	Copyright : taskit GmbH
	www.taskit.de

	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

		1. Redistributions of source code must retain the above copyright
		   notice, this list of conditions and the following disclaimer.
		2. Redistributions in binary form must reproduce the above copyright
		   notice, this list of conditions and the following disclaimer in the
		   documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT OWNER AND CONTRIBUTORS AS IS
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

/* xenomai headers */
#include <native/task.h>
#include <native/timer.h>
#include <native/pipe.h>

#define ALLOC(p, n)  p = malloc(n); while(p == NULL) {sleep(1); p = malloc(n);};

/* global structures */
RT_TASK demo_task;
//RT_PIPE msg_pipe;

/* default settings */
uint16_t freq = 10;

void demo_task_func(void* args)
{
	rt_printf("\nTask Called \n");
	RTIME t;
	RTIME period;
	char* str_buf;
	uint16_t h, m, s, milli, micro;

	/* allocate string buffer */
	ALLOC(str_buf, 256);

	/* setup periodic timer */
	period = 10000 / freq;
	rt_task_set_periodic(NULL, TM_NOW, period);

	/* real-time application loop */
	while(1)
	{
		/* get time */
		t = rt_timer_read();

		/* do something - e.g. print time to pipe */
		micro = (t / 1000) % 1000;
		milli = (t / 1000000) % 1000;
		s = (t / 1000000000) % 60;
		m = (t / 1000000000 / 60) % 60;
		h = (t / 1000000000 / 60 / 60) % 24;
		rt_printf("%02d:%02d:%02d.%03d.%03d\n", h, m, s, milli, micro);
		//rt_pipe_write(&msg_pipe, str_buf, strlen(str_buf), P_NORMAL);
		
		//rt_printf("Time : %s",str_buf);

		/* sleep till next step */
		rt_task_wait_period(NULL);
	}

	/* clean up */
	free(str_buf);
}

void catch_signal(int sig)
{
	/* nothing to handle, yet */
}

/* main loop */
int main(int argc, char** argv)
{
	/* catch signals */
	signal(SIGTERM, catch_signal);
	signal(SIGINT, catch_signal);

	/* disable paging */
	mlockall(MCL_CURRENT | MCL_FUTURE);

	/* create real time pipe */
	//rt_pipe_create(&msg_pipe, "msg_pipe", P_MINOR_AUTO, 0);

	/* launch real time task */
	rt_task_create(&demo_task, "demo", 0, 99, 0);
	rt_task_start(&demo_task, &demo_task_func, NULL);

	/* wait for terminating signal */
	pause();

	/* clean up */
	rt_task_delete(&demo_task);
	//rt_pipe_delete(&msg_pipe);

	return(0);
}

