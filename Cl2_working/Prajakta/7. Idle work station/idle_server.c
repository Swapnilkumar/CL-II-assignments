//server program

#include<stdio.h>
#include<stdlib.h>
#include<rpc/rpc.h>
#include "idle.h"


float* getcpu_1(void* p1,CLIENT *p2){
	FILE *fp;
	float *val = malloc(sizeof(float));
	system("mpstat  > info.temp");
	fp=fopen("info.temp","r");
	fseek(fp,-6,SEEK_END);
	fscanf(fp,"%f",val);
	fclose(fp);
	return val;
}

float * getcpu_1_svc(void *p1, struct svc_req *p2){
	CLIENT *p;
	return getcpu_1(p1,p);
}

int * add_1(struct twono *p1, CLIENT *p2){
	int *res = malloc(sizeof(int));
	*res = p1->no1 + p1->no2;
	return res;
}	


int * add_1_svc(struct twono *p1, struct svc_req *p2){
	CLIENT *p;
	return add_1(p1,p);
}
