/*#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
 
#include <native/task.h>
#include <native/timer.h>
 
#include  <rtdk.h>
*/
#include "../../include/Controller/statcom.h"

RT_TASK demo_task1;
RT_TASK demo_task2;
RT_TASK demo_task3;
RT_TASK demo_task4;
RT_TASK demo_task5;
RT_TASK demo_task6;
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
 
void demo(void *arg) {
    RT_TASK *curtask;
    RT_TASK_INFO curtaskinfo;
    curtask=rt_task_self();
    rt_task_inquire(curtask,&curtaskinfo);
    rt_printf("Task name: %s ", curtaskinfo.name);
}
 
int main(void)
{
  char  str[10] ;
  char  str1[10] ;
 
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
  /* 
  sprintf(str,"hello1\n");
  rt_task_create(&demo_task1, str, 0, 50, 0);
  sprintf(str,"hello2\n");
  rt_task_create(&demo_task2, str, 0, 50, 0);
  sprintf(str,"hello3\n");
  rt_task_create(&demo_task3, str, 0, 50, 0);
  sprintf(str,"hello\n");
  rt_task_create(&demo_task4, str, 0, 50, 0);
  sprintf(str,"hello5\n");
  rt_task_create(&demo_task5, str, 0, 50, 0);
 */
 sprintf(str,"hello1\n");
 rt_printf("Task 1\n");
  rt_task_create(&demo_task1, str, 0, 50, 0);
  rt_printf("Task 2\n");
  //sprintf(str,"hello2\n");
  rt_task_create(&demo_task2, str, 0, 50, 0);
  rt_printf("Task 3\n");
  //sprintf(str,"hello3\n");
  rt_task_create(&demo_task3, str, 0, 50, 0);
  rt_printf("Task 4\n");
  //sprintf(str,"hello\n");
  rt_task_create(&demo_task4, str, 0, 50, 0);
  rt_printf("Task 5\n");
  //sprintf(str,"hello5\n");
  rt_task_create(&demo_task5, str, 0, 50, 0);
  /*
   * Arguments: &task,
   *            task function,
   *            function argument
   */
  rt_task_start(&demo_task1, &demo, 1);
  rt_task_start(&demo_task2, &demo, 1);
  rt_task_start(&demo_task3, &demo, 1);
  rt_task_start(&demo_task4, &demo, 1);
  rt_task_start(&demo_task5, &demo, 1);
  //rt_task_start(&demo_task, &demo, 0);
  return 0;
}