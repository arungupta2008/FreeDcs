/*#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
 
#include <native/task.h>
#include <native/timer.h>
 
#include  <rtdk.h>
*/
#include "../../include/Controller/statcom.h"

RT_TASK demo_task;
period_ns = 10000.0 ;
void task_function(int *arg)
{
int i =0 ;
  rt_task_set_periodic(NULL, TM_NOW, period_ns); 
  while (i<30) {
   rt_printf("Hello World! : %d\n", i);
   printf("Let's see they prints me or not : %d\n", i);
    rt_task_wait_period(NULL);
    rt_printf("Waiting Period is ended : %d\n", i);
    printf("Waiting Period is ended let's see the prints me or not! : %d\n", i);
    ++i;
  }
  return;
}
 
void demo(void *arg){
  RT_TASK *curtask;
  RT_TASK_INFO curtaskinfo;
 
  // hello world
  rt_printf("Hello World!\n");
 
  // inquire current task
  curtask=rt_task_self();
  rt_task_inquire(curtask,&curtaskinfo);
 
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
 
  rt_printf("\nstart task\n");
 
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
  rt_task_start(&demo_task, &task_function, 1);
  //rt_task_start(&demo_task, &demo, 0);
  return 0;
}