#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>
#include "shm_com.h"

int main()
{
	int running = 1;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;
	int shmid;
	
	srand((unsigned int)getpid());
	
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	
	printf("\nshmid: %d", shmid);
	
	if(shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	
	/*now make shared memory accessible to the program*/
	
	shared_memory = shmat(shmid, (void *)0, SHM_RND);
	
	printf("\nShared memory: %d", (int) shared_memory);
	
	if(shared_memory == (void *)-1) {
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	
	printf("\nMemory attached at %X\n", (int)shared_memory);
	
	/*Assign the shared_memory segment to shared_stuff,
	  which then prints out any text in written_by_you.
	  Loop continues till end is found in written_by_you.
	  Call to sleep forces the consumer to sit in it's critical section, which makes the producer wait.
	*/
	
	shared_stuff = (struct shared_use_st *)shared_memory;
	shared_stuff->written_by_you = 0;
	while(running) {
		if(shared_stuff->written_by_you){
			printf("You wrotte: %s", shared_stuff->some_text);
			sleep(rand() % 4);/*make the other process wait for us!*/
			shared_stuff->written_by_you = 0;
			if(strncmp(shared_stuff->some_text, "end", 3) == 0){
				running = 0;
			}
		}
	}
	
	/*finally, shared memory is detached and deleted*/
	if(shmdt(shared_memory) == -1){
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	
	if(shmctl(shmid, IPC_RMID, 0) == -1) {
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
