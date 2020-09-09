#include <stddef.h> /*size_t*/
#include <pthread.h>
#include <stdio.h>

#include "UID.h" /* forward decleration */

pthread_mutex_t lock;

/* returns task id */
ilrd_uid_t UIDGet()
{
	static size_t count = 1;
	ilrd_uid_t uid;

	if(pthread_mutex_init(&lock, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
        return GetBadUid(); 
    } 

	pthread_mutex_lock(&lock); 
	
	uid.id = count;
	count++;
	
    pthread_mutex_unlock(&lock);

	return uid;
}

/* returns uid 0 */
ilrd_uid_t GetBadUid()
{
	ilrd_uid_t uid;
	uid.id = 0;
	return uid;
}


int UIDIsEqual(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return uid1.id == uid2.id;
}

/* returns success or fail , if uid is 0 so uid is bad and return 1, */
int UIDIsBad(ilrd_uid_t uid)
{
	return  uid.id == 0;
} 

/* returns current uid */
size_t GetCurrentUid(ilrd_uid_t uid)
{
	return uid.id;
}
