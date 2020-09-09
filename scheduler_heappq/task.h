#ifndef __TASK_H__ 
#define __TASK_H__

/*#include "UID.h"*/
#include "sched_hpq.h" /*includes inside uid.h for ilrd_uit_t*/

/*typedef int(*action_t)(void *param);*/
typedef struct task task_t;

task_t *TaskCreate(action_t Action, unsigned int freq, ilrd_uid_t uid, void *param);

void TaskDestroy(task_t *task);

/*returns retVal of Action*/
int TaskRun(task_t *task); 

void TaskSetTime2Run(task_t *task);

size_t TaskGetTime2Run(task_t *task);

ilrd_uid_t TaskGetUid(task_t *task);

#endif
