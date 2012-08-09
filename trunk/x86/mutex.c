#include "mutex.h"
void init_mutex(mutex_t *mutex)
{
	init_lock(&mutex->lock); // sets lock = 0
}
void destroy_mutex(mutex_t *mutex)
{
	test_and_set(0,&mutex->lock);
}
void mutex_try_lock(mutex_t *mutex)
{
	// test_and_set returns old value of lock until it sets 
	while(test_and_set(1,&mutex->lock)==1); //loop here indefinitely
}

void mutex_unlock(mutex_t *mutex) // unlock , lock=0
{
	test_and_set(0,&mutex->lock);
}
