/**************************************/
/* author: Shahar maimon              */
/* date: 03.10.20                     */
/**************************************/

/*
alias gdd='gd -I ../dlist -I ../sorted_list -I ../priqueue -I ../uid sched.c sched_test.c task.c ../dlist/dlist.c  ../sorted_list/slist.c ../priqueue/priQ.c ../uid/UID.c' 
*/
#include <stdio.h> /*printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /*assert */
#include <time.h> /*time */

#include <unistd.h> /* sleep */

#include "sched.h" /* forward decleration */
#include "priQ.h"  /* forward decleration */
#include "task.h"  /* forward decleration */

static int CmpFunc(const void *current_task, const void *new_task ,const void * param);
static void WaitToNextApperance(task_t *next_task);
static int IsMatch(const void *list_data, void *param_from_user);

struct scheduler
{
	pr_queue_t *pr_q;
	int to_stop;
}; /* sched_t */


/**************************************************************************/
sched_t *SchedCreate()
{
	sched_t *scheduler = NULL;
	
	scheduler = (sched_t*)malloc(sizeof(sched_t));
	if(NULL == scheduler)
	{
		fprintf(stderr, "allocation failed\n");
		return NULL;
	}

	 scheduler->pr_q = PQCreate(CmpFunc, NULL);
	 if(NULL == scheduler->pr_q)
	{
		fprintf(stderr, "allocation failed\n");
		SchedDestroy(scheduler);
		return NULL;

	}
	
	return scheduler;
}



/**************************************************************************/
void SchedDestroy(sched_t *scheduler)
{
	task_t *task = NULL;
	
	assert(scheduler);
	
	if(scheduler->pr_q != NULL)
	{
		while(!PQIsEmpty(scheduler->pr_q))
		{
			/*first peek to see the next task and free task while sched is not empty*/
			task = (task_t*)PQPeek(scheduler->pr_q);	
			TaskDestroy(task);
			PQDequeue(scheduler->pr_q);
		}
		PQDestroy(scheduler->pr_q);
	}
	
	free(scheduler);
	scheduler = NULL;
}



/**************************************************************************/
/* freq in sec */
ilrd_uid_t SchedAdd(sched_t *scheduler, action_t Action, unsigned int freq, void *param)
{
	task_t *task = NULL;
	int status = 0;

	ilrd_uid_t uid = UIDGet();
	
	assert(scheduler);

	/* creating a new task */
	task = TaskCreate(Action, freq, uid, param);
	if(	NULL == task)
	{
		uid = GetBadUid();
		return uid;
	}
	
	/* adding new task to queue */
	status = PQEnqueue(scheduler->pr_q, (void*)task);
	/* if enqueue failed so destroy task and turn uid to 0*/
	if(status == 0)
	{
		TaskDestroy(task);
		uid = GetBadUid();
	}
	
	return uid;
}

/**************************************************************************/
void SchedRemove(sched_t *scheduler, ilrd_uid_t uid)
{
	task_t *task = NULL;

	assert(scheduler);

	/* erase specific data using IsMatch function (Find & Remove) */
	task = PQErase(scheduler->pr_q, IsMatch, &uid);
	
	TaskDestroy(task);
}

/* run all the tasks */
/**************************************************************************/
void SchedRun(sched_t *scheduler)
{
	int status = 0;
	task_t *next_task = NULL;
	
	assert(scheduler);
	
	scheduler->to_stop = 0;
	
	/*if the schedular is not empty so continue tasks (empty returns 1) */
	while(!PQIsEmpty(scheduler->pr_q) && (scheduler->to_stop == 0))
	{
		/* first peek to see time of next task */
		next_task = PQPeek(scheduler->pr_q);
		/* then sleep until next task needs to start */
		WaitToNextApperance(next_task);
		/* then remove the task from the queue */
		PQDequeue(scheduler->pr_q);
		/* now run the task, if status is != 0 
		set time again and add task again to queue 
		if return value from action is 0 so stop */
		status = TaskRun(next_task);
		if(status != 0)
		{
			TaskSetTime2Run(next_task);
			PQEnqueue(scheduler->pr_q, next_task);
		}
		else
		{
			/* if the function stopped so free task */
			TaskDestroy(next_task);
		}
	}
}


/* stop all the tasks */
/**************************************************************************/
void SchedStop(sched_t *scheduler)
{
	assert(scheduler);
	
	scheduler->to_stop = 1;
}




/*************************more functions***********************************/
/**************************************************************************/
static int CmpFunc(const void *current_task, const void *new_task ,const void * param)
{
	int answer;
	
	param = param;
	
	/* compare current task to new task added. if true return 1?*/
	answer = TaskGetTime2Run((task_t*)current_task) <
			 TaskGetTime2Run((task_t*)new_task);
			 
	return answer;
}

/**************************************************************************/
static void WaitToNextApperance(task_t *next_task)
{
	/* if current time is smaller than time2run so
	 sleep more until time equals time to run */
	while(time(NULL) < (TaskGetTime2Run(next_task)))
	{
		sleep(TaskGetTime2Run(next_task)-time(NULL));
	}
}


/**************************************************************************/
/*returns 0 if match or 1 if not */
/* int IsMatch(const void *current_uid, const void *uid_from_user) */
static int IsMatch(const void *list_data, void *param_from_user)
{
	ilrd_uid_t uid;	
	
 	uid = TaskGetUid((task_t *)list_data); 
/* 	return !(GetCurrentUid(uid) == *(size_t *)param_from_user); */
return UIDIsEqual(uid, *(ilrd_uid_t*)param_from_user);
}


