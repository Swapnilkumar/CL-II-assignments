//SERVER PROGRAM

#include <rpc/rpc.h>
#include "DBHandle.h"
#include <stdio.h>
#define FILENAME "db.txt"

FILE *fp;

int* insert_1(record *r,CLIENT *client)
{
 	int *p=(int*)malloc(sizeof(int));
	fp= fopen(FILENAME,"a");
	
	fwrite((char*)r,sizeof(record),1,fp);
		
	fclose(fp);
	*p=1;
	return p;
}
int * insert_1_svc(record *r,   struct svc_req *svc) {
  CLIENT *client;
  return(insert_1(r,client));
}

int * delete_1(int * rollno,CLIENT *client)
{
 	FILE *tmp;
	int cnt =0;
	int *cntt=malloc(sizeof(int));

	record r;
	//file -> tmp //skip the record to be deleted
	fp = fopen(FILENAME,"r"); 
	tmp = fopen("tmp.bin","w");
	printf("Record to  be Deleted : %d",*rollno);
	
	while(fread((char*)&r,sizeof(record),1,fp))
	{	if(r.rollno!=*rollno)
		{
			fwrite((char*)&r,sizeof(record),1,tmp);
		}
		else
		{
			cnt ++;
		}
	}

	fclose(tmp);
	fclose(fp);

	//tmp -> file //entire
	fp = fopen(FILENAME,"w"); 
	tmp = fopen("tmp.bin","r");

	while(fread((char*)&r,sizeof(record),1,tmp))
	{
		fwrite((char*)&r,sizeof(record),1,fp);
	}
	
	fclose(tmp);
	fclose(fp);
	*cntt = cnt;
	//
	return (cntt); //no of records deleted
}
int * delete_1_svc(int * rollno,   struct svc_req *svc) {
  CLIENT *client;
  return(delete_1(rollno,client));
}


int *  update_1(record *nr,CLIENT *client)
{
    FILE *tmp;
	int cnt =0;
	int *cntt=malloc(sizeof(int));
	record r;
	//file -> tmp //replace the record to be updated
	fp = fopen(FILENAME,"r"); 
	tmp = fopen("tmp.bin","w");
	
	
	while(fread((char*)&r,sizeof(record),1,fp))
	{	if(r.rollno!=nr->rollno)
		{
			fwrite((char*)&r,sizeof(record),1,tmp);
		}
		else
		{
			fwrite((char*)nr,sizeof(record),1,tmp);
			cnt ++;
		}
	}

	fclose(tmp);
	fclose(fp);

	//tmp -> file //entire
	fp = fopen(FILENAME,"w"); 
	tmp = fopen("tmp.bin","r");

	while(fread((char*)&r,sizeof(record),1,tmp))
	{
		fwrite((char*)&r,sizeof(record),1,fp);
	}
	
	fclose(tmp);
	fclose(fp);
	*cntt =cnt;
	//
	return (cntt); //no of records updated
}
int * update_1_svc(record *nr,struct svc_req *svc) {
  CLIENT *client;
  return(update_1(nr,client));
}
record * retrive_1(int * rollno,CLIENT *client)
{
	record *r =  (record*) malloc (sizeof(record));
	fp = fopen(FILENAME,"r"); 
	if(fp == NULL) 
	{
		printf("File NOT Found\n");
		return NULL;
	}
	while(fread(r,sizeof(record),1,fp))
	{	if(r->rollno==*rollno)
		{
			fclose(fp);
			return r;
	
		}		
	}
	r->rollno=0;
	fclose(fp);
	return r;
}

record * retrive_1_svc(int * rollno,struct svc_req *svc) {
  CLIENT *client;
  return(retrive_1(rollno,client));
}






