#ifndef __SCHED_HPQ_H__ 
#define __SCHED_HPQ_H__

#include "UID.h"

typedef struct scheduler sched_t; 
typedef int(*action_t)(void *param);

sched_t *SchedCreate();

void SchedDestroy(sched_t *scheduler);

/* freq in sec */
ilrd_uid_t SchedAdd(sched_t *scheduler, action_t Action, unsigned int freq, void *param);

void SchedRemove(sched_t *scheduler, ilrd_uid_t uid);

/* starts the event loop (control moves to the sched */
void SchedRun(sched_t *scheduler);

void SchedStop(sched_t *scheduler);


#endif

