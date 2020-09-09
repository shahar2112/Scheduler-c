#include <stdio.h> /*printf */
#include <pthread.h>
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC */

#include "UID.h" /* forward decleration */

#define NUM_OF_THREADS (11300)

sem_t *newsem_g = NULL;
int counter_g = 0;

void *GetUIDtest(void *array);

#define RED	printf("\033[1;31m")
#define BLUE printf("\033[1;36m")
#define MAGENTA	printf("\033[1;35m")
#define YELLOW printf("\033[1;33m")
#define GREEN printf("\033[1;32m")
#define DEFAULT	printf("\033[0m")

void Barrier();

int main()
{
	size_t i = 0;
	int ret_val = 0;
	int check = 0;
	size_t array[NUM_OF_THREADS];
	pthread_t thread[NUM_OF_THREADS];
	

	for(i=0; i < NUM_OF_THREADS; i++)
    {
        ret_val = pthread_create(&thread[i], NULL, GetUIDtest, (void *)array);
        if (0 != ret_val)
        {
            RED;
            printf("failed to create , ret_val is %d\n", ret_val);
            DEFAULT;
        }
    }


    for(i = 0 ; i < NUM_OF_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

	for(i = 0; i < NUM_OF_THREADS; i++)
	{
		if(array[i] != i)
		{
			check = 1;
		}
	}

	if(check == 0)
	{
		GREEN;
		printf("success\n");
		DEFAULT;
	}
	else
	{
		RED;
		printf("fail\n");
		DEFAULT;
	}
	

	return 0;
}


void *GetUIDtest(void *array)
{
	ilrd_uid_t uid;
	size_t *tmp_array = array;
	
	Barrier();

	uid = UIDGet();

	tmp_array[GetCurrentUid(uid)] = GetCurrentUid(uid);

	return NULL;
}


void Barrier()
{
	static size_t counter = 0;
	static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	static pthread_mutex_t lock1  = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&lock1);

    ++counter;

	if(counter < NUM_OF_THREADS)
	{
   		pthread_cond_wait(&cond, &lock1);
	}
	else
	{
		pthread_cond_broadcast(&cond);
	}
	
    pthread_mutex_unlock(&lock1);

}