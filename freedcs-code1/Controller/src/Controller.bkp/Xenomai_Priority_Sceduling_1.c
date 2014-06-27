/*
#include <stdio.h>
#include <signal.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>
*/
#include "../../include/Controller/statcom.h"

#define TASK_PRIO  99 /* Highest RT priority */
#define TASK_MODE  0  /* No flags */
#define TASK_STKSZ 0  /* Stack size (use default one) */


typedef void (*TASK_EP)(void*);

RT_TASK task_desc, task_desc1;
RT_SEM  sem_desc, sem_desc1;

int cleanup (RT_TASK*);
void catch_signal(int);
static char* unique_name_create(void* tid, const char* name);
static RT_TASK* task_id_self(void);

static void my_exit(void* dum)
{
    char* name;
    RT_TASK_INFO info;
    RT_TASK *tid;

    tid = task_id_self();
    rt_task_inquire(tid, &info);
    name = info.name;
    printf("%s: exit\n", name);
    if(cleanup(tid))
        printf("Cleanup Error!\n");
    else
        printf("Cleanup successfull!\n");
}

typedef struct {
    TASK_EP break_routine;
    RT_SEM* sem;
} my_brk;

void break_receiver(my_brk* pbrk)
{
    rt_sem_p(pbrk->sem, TM_INFINITE);
    pbrk->break_routine(NULL);
    rt_sem_delete(pbrk->sem);
    free(pbrk);
}

int set_break_receiver(TASK_EP break_routine, RT_SEM* sem)
{
    const char *brktask_name;
    RT_TASK *tid, *tid_br;
    RT_TASK_INFO info;
    my_brk* pbrk;
    int err;

    pbrk = malloc(sizeof(my_brk));

    tid = rt_task_self();
    err = rt_task_inquire(tid, &info);
    if(err)
        printf("ERROR: rt_task_inquire. error:%d \n", err);


    pbrk->break_routine = break_routine;
    pbrk->sem = sem;

    brktask_name = unique_name_create(tid, "BRK_RCV");
    tid_br = malloc(sizeof(*tid));
    err = rt_task_spawn(tid_br, brktask_name, 20000, info.bprio, 0,
                (TASK_EP)break_receiver, pbrk);
    if(err)
        printf("ERROR: rt_task_spawn. error:%d \n", err);
    return(err);
}

void task_body (void *cookie)
{
    int i=0;

    set_break_receiver(my_exit, &sem_desc);
    rt_task_set_periodic(NULL,TM_NOW,20000);

    for (;;) {
        if(!i){
        i++;
            printf("Pippo task RUNNING!\n");
    }
        rt_task_wait_period(NULL);
    }
}

void task_body_1 (void *cookie)
{
    int i=0;

    set_break_receiver(my_exit, &sem_desc1);
    rt_task_set_periodic(NULL,TM_NOW,20000);

    for (;;) {
        if(!i) {
           printf("CruelPippo task RUNNING!\n");
       i++;
    }
        rt_task_wait_period(NULL);
    }
}

int send_break(void)
{
   int err;
   /*  char *sem_name, *sem_name1;
    RT_SEM sem, sem1;
    RT_SEM_INFO info, info1;


    rt_sem_inquire(&sem_desc, &info);
    sem_name = info.name;
    rt_sem_inquire(&sem_desc1, &info1);
    sem_name1 = info1.name;
    err = rt_sem_bind(&sem, sem_name, TM_NONBLOCK) ||
          rt_sem_bind(&sem1, sem_name1, TM_NONBLOCK);
    if(err) return(err); */
    err = rt_sem_v(&sem_desc) ||
          rt_sem_v(&sem_desc1);
    return(err);
}

int main (int argc, char *argv[])
{
    int err;

    signal(SIGTERM, catch_signal);
    signal(SIGINT, catch_signal);

    mlockall(MCL_CURRENT|MCL_FUTURE);

    err = rt_sem_create(&sem_desc, "PippoBRK_SEM", 0, S_FIFO) ||
          rt_sem_create(&sem_desc1,"CruelPippoBRK_SEM", 0, S_FIFO);
    if (err)
       printf("ERROR: rt_sem_create fail.\n");

    err = rt_task_create(&task_desc,
             "Pippo",
             TASK_STKSZ,
             TASK_PRIO,
             TASK_MODE)  ||
      rt_task_create(&task_desc1,
             "CruelPippo",
             TASK_STKSZ,
             TASK_PRIO,
             TASK_MODE);
    if (!err){
    rt_task_start(&task_desc,&task_body,NULL);
    rt_task_start(&task_desc1,&task_body_1,NULL);
    }
    else
       printf("ERROR: rt_task_create fail.\n");

    pause();
    printf("MAIN exit\n");
    return(err);
}

void catch_signal(int sig)
{
    int err;
    err = send_break();
    if(err)
        printf("Error Sending Break!\n");
    return;
}

int cleanup (RT_TASK* tid)
{
    int err;
    RT_TASK_INFO info;

    rt_task_inquire(tid, &info);
    printf("rt_task_delete(%s) calling...\n", info.name);
    err = rt_task_delete(tid);
    if(!err)
        printf("rt_task_delete done!\n");
    return(err);
}

static char* unique_name_create(void* tid, const char* name)
{
    static char unique_name[XNOBJECT_NAME_LEN];
    unsigned int utid;

    if (strlen(name) + 11 > XNOBJECT_NAME_LEN)
        return(NULL);

    utid = (unsigned int)tid;
    sprintf(unique_name, "0x%8.8x", utid);
    strcpy(unique_name + strlen(unique_name), name);
    return(unique_name);
}

static RT_TASK* task_id_self(void)
{
    RT_TASK *tid;
    unsigned int utid;
    RT_TASK_INFO info;
    char* fullname;

    rt_task_inquire(rt_task_self(), &info);
    fullname = info.name;
    fullname[10] = 0;
    sscanf(fullname,"0x%x", &utid);

    tid = (RT_TASK*)utid;
    return(tid);
}
