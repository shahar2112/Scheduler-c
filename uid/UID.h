#ifndef __PRQUEUE_H__ 
#define __PRQUEUE_H__

 #include <stddef.h>  /*size_t*/

typedef struct UID 
{
	size_t id;
} ilrd_uid_t;

/* returns task id */
ilrd_uid_t UIDGet();

/* returns uid 0 */
ilrd_uid_t GetBadUid();

/*checks if one uid is equal to another */
int UIDIsEqual(ilrd_uid_t uid1, ilrd_uid_t uid2);

/* returns success or fail */
int UIDIsBad(ilrd_uid_t uid);

/* returns current uid */
size_t GetCurrentUid(ilrd_uid_t uid);

#endif 
