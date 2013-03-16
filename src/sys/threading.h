#ifndef _YAFE2_THREADING_H_
#define _YAFE2_THREADING_H_

/*
 *	pthread-ish bare minimum threading api
 */
typedef struct thread_ctx * thread_t;

int thread_create(thread_t * thread, void * (* start_routine)(void*), void * arg);
int thread_join  (thread_t   thread, void ** rc_ptr);
int thread_detach(thread_t   thread);

/*
 *
 */
typedef struct mutex_ctx * mutex_t;

int mutex_create(mutex_t * mutex);
int mutex_delete(mutex_t   mutex);

int mutex_acquire(mutex_t mutex);
int mutex_release(mutex_t mutex);

/*
 *
 */
void yield();

#endif
