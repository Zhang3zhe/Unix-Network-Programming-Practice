// ∞¥≤ªÕ¨À≥–Ú∑√Œ ª•≥‚À¯µº÷¬À¿À¯
#include<pthread.h>
#include<unistd.h>
#include<stdio.h>

int a = 0;
int b = 0;

pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;

void* another(void* arg){
	pthread_mutex_lock(&mutex_b);
	printf("In child thread, got mutex b, waiting for mutex a.\n");
	sleep(5);
	b = 2;
	pthread_mutex_lock(&mutex_a);
	a = 2;
	pthread_mutex_unlock(&mutex_a);
	pthread_mutex_unlock(&mutex_b);
	pthread_exit(NULL);
}

int main(){
	sleep(20);
	pthread_t id;
	pthread_mutex_init(&mutex_a, NULL);
	pthread_mutex_init(&mutex_b, NULL);
	pthread_create(&id, NULL, another, NULL);
	
	pthread_mutex_lock(&mutex_a);
	printf("In parent thread, got mutex a, waiting for mutex b.\n");
	sleep(5);
	a = 1;
	pthread_mutex_lock(&mutex_b);
	b = 1;
	pthread_mutex_unlock(&mutex_b);
	pthread_mutex_unlock(&mutex_a);
	
	pthread_join(id, NULL);
	pthread_mutex_destroy(&mutex_a);
	pthread_mutex_destroy(&mutex_b);
	
	return 0;
}
