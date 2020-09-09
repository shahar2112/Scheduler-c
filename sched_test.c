#include <stdio.h> /*printf */
#include <stdlib.h> /* exit */
#include <time.h> /*time */

#include "sched.h" /* forward decleration */

int Action(void *param);  /* forward decleration */
int Action2(void *param);  /* forward decleration */
int StopAction(void *param);  /* forward decleration */
int RemoveAction(void *param); /* forward decleration */
void CheckUid(ilrd_uid_t UID); /* forward decleration */

typedef struct helper
{
	sched_t *scheduler;
	ilrd_uid_t uid;
}check_t;



int main()
{

	sched_t *scheduler = NULL;
	void *param = 0;
	check_t *check_remove = NULL;
	ilrd_uid_t uid;
	ilrd_uid_t uid2;
	
	check_remove = (check_t*)malloc(sizeof(check_t));
	if(NULL == check_remove)
	{
		printf("Error. Allocation was unsuccessful. \n");
		exit(1);
	}
	
	scheduler = SchedCreate();
	if(NULL == scheduler)
	{
		printf("Error. Allocation was unsuccessful. \n");
		exit(1);
	}
	
	
	uid = SchedAdd(scheduler, Action, 2, NULL);
	printf("uid of first task is %ld\n", uid.id);
	CheckUid(uid);
	
	uid2 = SchedAdd(scheduler, Action2, 3, NULL);
	printf("uid of second task is %ld\n", uid2.id);
	CheckUid(uid);
	
	uid = SchedAdd(scheduler, StopAction, 8, scheduler);
	printf("uid of third task is %ld\n", uid.id);
	CheckUid(uid);
	
	
	check_remove->scheduler = scheduler;
	check_remove->uid = uid2;

	printf("uid of the task to remove %ld\n", (check_remove->uid).id);
	/********/
	SchedAdd(scheduler, RemoveAction, 5, check_remove);
	
	SchedRun(scheduler);
	 
	SchedDestroy(scheduler);
	
	return 0;
}






/**************************************************************************/


int Action(void *param)
{
	static int count = 5;
	
	param = param;
	
		printf("countdown is %d\n", count);
		count--;
		return count;
}



/**************************************************************************/
int Action2(void *param)
{
	param = param;
	
	printf("time(NULL) = %ld\n", time(NULL));
	
	return 1;
}




/**************************************************************************/
int StopAction(void *param) /*the param will be the schedular */
{
	printf("stopping sched\n");
	SchedStop((sched_t *)param);
	
	return 1;
}



/*************************************************************************/
int RemoveAction(void *param) /*the param will be the helper struct */
{
	sched_t *sched = ((check_t *)param)->scheduler;
	ilrd_uid_t uid = ((check_t *)param)->uid;
	
	SchedRemove(sched, uid);

	return 0;
}

/*************************************************************************/
void CheckUid(ilrd_uid_t UID)
{
	if(UIDIsEqual(UID, GetBadUid()))
	{
		printf("didn't add task \n" );
	}
}

