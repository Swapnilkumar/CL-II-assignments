//clock synchronization - Lamport's clock
#include<stdio.h>
#include<pthread.h>
#include<sys/time.h>

pthread_mutex_t proc[2];
pthread_t id[2];
pthread_cond_t condn[2];
int clock_t_1, clock_t_2=0, clock_t_3=0;

void * increment_1(void * arg)
{
	int * interval;
	interval=(int *)arg;
	while(1)
	{
		sleep(1);
		clock_t_1 = clock_t_1 + (*interval);
		printf("\nclock 1 = %d", clock_t_1);
	}
}

void * increment_2(void * arg)
{
	int * interval;
	interval=(int *)arg;
	while(1)
	{
		sleep(1);
		clock_t_2 = clock_t_2 + (*interval);
		printf("\nclock 2 = %d", clock_t_2);
	}
}

void * increment_3(void * arg)
{
	int * interval;
	interval=(int *)arg;
	while(1)
	{
		sleep(1);
		clock_t_3 = clock_t_3 + (*interval);
		printf("\nclock 3 = %d", clock_t_3);
	}
}

void * clock_1(void * arg)
{
	int * interval;
	interval = (int *)arg;
	pthread_t id1;
	pthread_create(&id1, NULL, &increment_1, (void *)interval);
	pthread_mutex_lock(&proc[0]);
	pthread_cond_wait(&condn[0], &proc[0]);
	pthread_mutex_unlock(&proc[0]);
	printf("\nThread 1 received message at %d", clock_t_1);
	clock_t_1 = clock_t_2 + 1;
	printf("\nThread 1 updated it's clock to %d", clock_t_1);
}

void * clock_2(void * arg)
{
	int * interval;
	interval = (int *)arg;
	pthread_t id2;
	pthread_create(&id2, NULL, &increment_2, (void *)interval);
	pthread_mutex_lock(&proc[1]);
	pthread_cond_wait(&condn[1], &proc[1]);
	pthread_mutex_unlock(&proc[1]);
	printf("\nThread 2 received message at %d", clock_t_2);
	clock_t_2 = clock_t_3 + 1;
	printf("\nThread 2 updated it's clock to %d", clock_t_2);

	while(clock_t_2 < 20);
	printf("\nThread 2 sent message at %d", clock_t_2);

	pthread_mutex_lock(&proc[0]);
	pthread_cond_signal(&condn[0]);
	pthread_mutex_unlock(&proc[0]);
	pthread_join(id2, NULL);
}

void * clock_3(void * arg)
{
	int * interval;
	interval = (int *)arg;
	pthread_t id3;
	pthread_create(&id3, NULL, &increment_3, (void *)interval);

	while(clock_t_3 < 30);
	printf("\nThread 3 sent message at %d", clock_t_3);

	pthread_mutex_lock(&proc[1]);
	pthread_cond_signal(&condn[1]);
	pthread_mutex_unlock(&proc[1]);
	pthread_join(id3, NULL);
}

int main()
{
	int interval[3];
	interval[0]=6;
	printf("\nClock Synchronization using Lamport's Clock");

	pthread_create(&id[0], NULL, &clock_1, (void *)&interval[0]);
	interval[1]=8;
	pthread_create(&id[1], NULL, &clock_2, (void *)&interval[1]);
	interval[2]=10;
	pthread_create(&id[2], NULL, &clock_3, (void *)&interval[2]);
	pthread_join(id[0], NULL);

	printf("\n\n");
	return 0;
}

/*
[oslab@ws203-016 9]$ cc lamportClockSynchronization.c -lpthread
[oslab@ws203-016 9]$ ./a.out 

Clock Synchronization using Lamport's Clock
clock 1 = 6
clock 2 = 8
clock 3 = 10
clock 3 = 20
clock 1 = 12
clock 2 = 16
clock 3 = 30
clock 1 = 18
clock 2 = 24
Thread 3 sent message at 30
Thread 2 received message at 24
Thread 2 updated it's clock to 31
Thread 2 sent message at 31
Thread 1 received message at 18
Thread 1 updated it's clock to 32

[oslab@ws203-016 9]$ ./a.out 

Clock Synchronization using Lamport's Clock
clock 1 = 6
clock 2 = 8
clock 3 = 10
clock 1 = 12
clock 2 = 16
clock 3 = 20
clock 1 = 18
clock 2 = 24
clock 3 = 30
Thread 3 sent message at 30
Thread 2 received message at 24
Thread 2 updated it's clock to 31
Thread 2 sent message at 31
Thread 1 received message at 18
Thread 1 updated it's clock to 32

[oslab@ws203-016 9]$ ./a.out 

Clock Synchronization using Lamport's Clock
clock 3 = 10
clock 1 = 6
clock 2 = 8
clock 3 = 20
clock 1 = 12
clock 2 = 16
clock 3 = 30
clock 1 = 18
clock 2 = 24
Thread 3 sent message at 30
Thread 2 received message at 24
Thread 2 updated it's clock to 31
Thread 2 sent message at 31
Thread 1 received message at 18
Thread 1 updated it's clock to 32

[oslab@ws203-016 9]$
*/
