#include <stdio.h>
#include <pthread.h>

void thread_one_print(){
	while(1){
		printf("Hello from thread 1 - A\n");
		pthread_yield();
		printf("Hello from thread 1 - B\n");
	}
}

void thread_two_print(){
	while(1){
		printf("Hello from thread 2 - A\n");
		pthread_yield();
		printf("Hello from thread 2 - B\n");
	}
}

int main(){
	pthread_t thread;
	int thread_one_id = pthread_create(&thread, NULL, thread_one_print, NULL);
	int thread_two_id = pthread_create(&thread, NULL, thread_two_print, NULL);
	pthread_join(thread, NULL);
	return 0;
}
