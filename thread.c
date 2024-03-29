#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

typedef unsigned long u32;
pthread_t ntid;/*线程标识*/

void printids(const char *);
void *run(void *);

int main()
{
	int err;
	err = pthread_create(&ntid, NULL, run, NULL);
	if(err != 0)
	{
		printf("can't create thread: %s!", strerror(err));
		exit(1);
	}

	printids("main thread: ");
	sleep(1);

	return 0;


}

void printids(const char* s){
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();

	printf("%s pid: %lu tid: %lu (0x%lx)\n", s, (u32)pid, (u32)tid, (u32)tid);
}

void *run(void *arg)
{
	printids("new thread:");
	return ((void*)0);
}





