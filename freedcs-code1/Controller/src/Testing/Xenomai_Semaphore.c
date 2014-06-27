/*
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
 
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
 
#include  <rtdk.h>
*/ 


#include "../../include/Controller/statcom.h"
#define ITER 10
 
static RT_TASK  t1;
static RT_TASK  t2;

#define HIGH 99 /* high priority */
#define MID 60 /* medium priority */
#define LOW 50  /* low priority */
 
RT_SEM mysync;

int global = 0;
 
void taskOne(void *arg)
{
    int i;
    for (i=0; i < ITER; i++)
    {
		rt_sem_p(&mysync,TM_INFINITE);
        rt_printf("I am taskOne and global = %d................\n", ++global);
    }
}
 
void taskTwo(void *arg)
{
    int i;
    for (i=0; i < ITER; i++)
    {
        rt_printf("I am taskTwo and global = %d----------------\n", --global);
    }
}
 
int main(int argc, char* argv[])
{
    /* Perform auto-init of rt_print buffers if the task doesn't do so */
    rt_print_auto_init(1);
 
    /* Avoids memory swapping for this program */
    mlockall(MCL_CURRENT|MCL_FUTURE);

	// semaphore to sync task startup on
	rt_sem_create(&mysync,"MySemaphore",0,S_PRIO);
	
	
    /* create the two tasks */
    rt_task_create(&t1, "task1", 0, 1, 0);
    rt_task_create(&t2, "task2", 0, 1, 0);
 
    /* start the two tasks */
    rt_task_start(&t1, &taskOne, 0);
    rt_task_start(&t2, &taskTwo, 0);
    
    rt_task_set_priority(&t1,HIGH);
    rt_task_set_priority(&t2,LOW);
    
    rt_printf("wake up all tasks\n");
	rt_sem_broadcast(&mysync);
 
    return 0;
}
