// 14.2 创建线程和结束线程
#include<pthread.h>
int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void*(*start_routine)(void*),void* arg);
void pthread_exit(void* retval);
int pthread_join(pthread_t thread, void** retval);
int pthread_cancel(pthread_t thread);

// 14.4 POSIX信号量
#include<semaphore.h>
int sem_init(sem_t* sem, int pshared, unsigned int value);
int sem_destroy(sem_t* sem);
int sem_wait(sem_t* sem);
int sem_trywait(sem_t* sem);
int sem_post(sem_t* sem);

// 14.5.1 互斥锁基础API
#include<pthread.h>
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* mutexattr);
int pthread_mutex_destroy(pthread_mutex_t* mutex);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_trylock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);

// 14.6 条件变量
#include<pthread.h>
int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* cond_attr);
int pthread_cond_destroy(pthread_cond_t* cond);
int pthread_broadcast(pthread_cond_t* cond);
int pthread_cond_signal(pthread_cond_t* cond);
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);

