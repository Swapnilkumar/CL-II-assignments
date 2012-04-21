// client program

#include<stdio.h>
#include<stdlib.h>
#include "idle.h"
#include <rpc/rpc.h>

#define MMAX 20



int max1(float *val,int no){
	int idx=0;
	float mval = val[0];
	int i;
	
	for(i=0;i<no;i++){
		if(val[i] > mval){
			mval = val[i];
			idx = i;
		}
	}	
		
	return idx;
}

int main(int argc,char* argv[] ){

int i,minserver,*res;
float cpu[MMAX],*pcpu;
char **server=argv +1;
int no_server=argc - 1;
CLIENT *cl[MMAX];
twono *v = malloc (sizeof( twono));


for(i=0;i<no_server;i++){	
	cl[i] = clnt_create(server[i], IDLEWORK, IDLEWORK1, "tcp");
	if (cl[i] == NULL) {
		/*
		* Couldn’t establish connection with server.
		* Print error message and die.
		*/
		clnt_pcreateerror(server[i]);
		
	}else{
		pcpu=getcpu_1(NULL,cl[i]);
		cpu[i]=*pcpu;

		printf("CPU at IP = %s is %.2f%% free\n",server[i],cpu[i]);
	}
}
minserver = max1(cpu,no_server);
printf("Selecting the CPU at IP = %s\n",server[minserver]);

//perform the task : addition from the idle machine

v->no1 = 5;
v->no2 = 29;

res = add_1(v,cl[minserver]);


printf("Task performed from the server IP=%s\n",server[minserver]);
return 0;
}
