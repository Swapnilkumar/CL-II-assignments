
/*
Note : here three threads are switching control from one to another by using mutex. We assume that gaining mutex lock is receving message. And process which released the lock is sender of message. So each time, we set 'sender' before message is sent and then lock is released. 
After locking the mutex and receiving message, we check sender time and receiver time.. if receiver_time < sender time then we update receiver time by sender_time+1
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


//Global variables
pthread_t id[3];
int clock_t_1=0,clock_t_2=0,clock_t_3=0;
int sender = 1;
pthread_mutex_t mutex_var;

void *increment1(void *interval)
{
	int *intr= (int*)interval;
	while(1)
	{
	
		sleep(2);
		clock_t_1 = clock_t_1 + *intr;
		printf("\nClock 1 = %d",clock_t_1);
	}
}

void *increment2(void *interval)
{
	int *intr=(int*)interval;
	while(1)
	{
	
		sleep(2);
		clock_t_2 = clock_t_2 + *intr;
		printf("\nClock 2 = %d",clock_t_2);
	}
}

void *increment3(void *interval)
{
	int *intr= (int*)interval;
	while(1)
	{
	
		sleep(2);
		clock_t_3 = clock_t_3 + *intr;
		printf("\nClock 3 = %d",clock_t_3);
	}
}

void* clock1(void *interval)
{
	int *intr = (int*)interval;
	pthread_t id1;

	
	//create thread tp increment
	pthread_create(&id1,NULL,&increment1,interval);
	
	while(1)
	{
		sleep(5);
		pthread_mutex_lock(&mutex_var);
		//lock obtained so message received
		printf("\nThread 1 received message at %d from %d",clock_t_1, sender);
		
		if(sender == 2 && clock_t_1 < clock_t_2)
		{
			clock_t_1 = clock_t_2 + 1;
			printf("\nThread 1 updates its clock at %d",clock_t_1);
		
		}
		if(sender==3 && clock_t_1<clock_t_3)
		{
			clock_t_1 = clock_t_3 + 1;
			printf("\nThread 1 updated its clock at %d",clock_t_1);
		}
		
		//releasing lock..i.e.sending mesaage
		sender =1; //to detect who sent the message
		pthread_mutex_unlock(&mutex_var);
		
		printf("\nThread 1 sends message at %d",clock_t_1);
	
	}
 	pthread_join(id1,NULL);


}

void* clock2(void *interval)
{
	int *intr = (int*)interval;
	pthread_t id2;

	
	//create thread to increment
	pthread_create(&id2,NULL,&increment2,interval);
	
	while(1)
	{
	
//		printf("\nM here");
		sleep(5);
		pthread_mutex_lock(&mutex_var);
		//lock obtained so message received
		printf("\nThread 2 received message at %d from %d",clock_t_2, sender);
		
		if(sender == 1 && clock_t_2 < clock_t_1)
		{
			clock_t_2 = clock_t_1 + 1;
			printf("\nThread 2 updates its clock at %d",clock_t_2);
		
		}
		if(sender==3 && clock_t_2<clock_t_3)
		{
			clock_t_2 = clock_t_3 + 1;
			printf("\nThread 2 updated its clock at %d",clock_t_2);
		}
		
		//releasing lock..i.e.sending mesaage
		sender =2; //to detect who sent the message
		pthread_mutex_unlock(&mutex_var);
		
		printf("\nThread 2 sends message at %d",clock_t_2);
	
	}
 	pthread_join(id2,NULL);


}
void* clock3(void *interval)
{
	int *intr = (int*)interval;
	pthread_t id3;

	
	//create thread tp increment
	pthread_create(&id3,NULL,&increment3,interval);
	
	while(1)
	{
		sleep(5);
		pthread_mutex_lock(&mutex_var);
		//lock obtained so message received
		printf("\nThread 3 received message at %d from %d",clock_t_3, sender);
		
		if(sender == 1 && clock_t_3 < clock_t_1)
		{
			clock_t_3 = clock_t_1 + 1;
			printf("\nThread 3 updates its clock at %d",clock_t_3);
		
		}
		if(sender==2 && clock_t_3<clock_t_2)
		{
			clock_t_3 = clock_t_2 + 1;
			printf("\nThread 3 updated its clock at %d",clock_t_3);
		}
		
		//releasing lock..i.e.sending mesaage
		sender =3; //to detect who sent the message
		pthread_mutex_unlock(&mutex_var);
		
		printf("\nThread 3 sends message at %d",clock_t_3);
	
	}
 	pthread_join(id3,NULL);


}

int main()
{
	int interval[3];
	interval[0]=6;
	interval[1]=8;
	interval[2]=10;
	
	//create threads
	pthread_create(&id[0],NULL,&clock1,(void*)&interval[0]);
	pthread_create(&id[1],NULL,&clock2,(void*)&interval[1]);
	pthread_create(&id[2],NULL,&clock3,(void*)&interval[2]);
	
	printf("m before join")	;
	//Join threads one by one
	pthread_join(id[0],NULL);
	pthread_join(id[1],NULL);
	pthread_join(id[2],NULL);
}


