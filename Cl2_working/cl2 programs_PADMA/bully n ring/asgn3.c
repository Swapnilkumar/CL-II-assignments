
//page 1
#include<stdio.h>
#include<stdlib.h>
#define max 20

int list[max];
int no_proc, CoOrdinator_id;

void display()
{
	int i;
	printf("\n---------List----------");
	printf("\nProcess: ");
	for(i=1; i<=no_proc;i++)
	{
		printf("%2d", i);
	}
	printf("\nAlive: ");
	for(i=1; i<=no_proc; i++)
	{
		printf("%2d", list[i]);
	}
}

void bully()
{
	int choice=1, crash, activate, genid, i, flag, gen_coOrdinator;
	
	while(choice<4)
	{
		printf("\nCurrent co-ordinator: %d", CoOrdinator_id);
		printf("\n1. Crash a process");
		printf("\n2. Activate a process");
		printf("\n3. Display");
		printf("\n4. Exit\n");
		scanf("%d", &choice);
		switch(choice)
		{
			case 1:
			printf("\nEnter a process id to crash: ");
			scanf("%d", &crash);
			list[crash]=0;
			if(crash==CoOrdinator_id)
			{
			printf("\nEnter generator id :");
		scanf("%d",&genid);
		printf("\nStarting election algorithm ");
		while(1)
		{
			flag=0;
			for(i=genid+1;i<=no_proc;i++)
			{
				printf("\nSending request from %d to %d",genid,i);
				if(list[i])
				{
					flag=1;
					gen_coOrdinator=i;
				}
			}
			if(flag)
			{
				genid=gen_coOrdinator;
				printf("\nNew coordinator selected :  %d",genid);
			}
			else
				break;
		}
		CoOrdinator_id=genid;
	}
	break;
	
	case 2:
		printf("\nSelect a process to activate : ");
		scanf("%d",&activate);
		if(list[activate]!=0)
				break;
		list[activate]=1;
		if(list[activate]==no_proc)
		{
				printf("\nNew coordinator selected : %d",activate);
				CoOrdinator_id=activate;
		}
		else
		{
				printf("Starting election algorithm");
				flag=0;
				for(i=activate;i<=no_proc;i++)
				{
						printf("\nSending request from %d to %d",activate,i);
						
						//page 34

				if(list[i])
				{
			
					flag=1;
					gen_coOrdinator=i;
				}
		
			}
			if(flag)
			{
				printf("\nNew co-ordinator selected : %d",gen_coOrdinator);
				CoOrdinator_id=gen_coOrdinator;
			}
			else
			{
				printf("\nActivated Proess %d is the new co-ordinator",activate);
				CoOrdinator_id=activate;
			}
		}


		break;
		case 3:

		display();
		break;
	
		case 4:
		break;
		}
	}
}
void ring()
{
	int choice=1,crash,activate,genid,i,flag,temp,message[max],mid=0,maxid;

	while(choice<4)
	{
		printf("\nCurrent co-ordinator : %d",CoOrdinator_id);
		printf("\n1: Crash a process");
		printf("\n2: Activate a process");	
		printf("\n3: Display");
		printf("\n4: Exit");	
		printf("\nEnter Your Option :");	
		scanf("%d",&choice);
		switch(choice)
		{
			case 1 : 
			printf("\nEnter the process id to crash : ");
			scanf("%d",&crash);
list[crash]=0;
if(crash==CoOrdinator_id)
{
	printf("\nEnter generator id : ");
	scanf("%d",&genid);
	printf("\nStarting election algorithm");
	mid = 1;
	message[mid]=genid;
	for(i=(genid+1)%no_proc ; i!=genid ; i=(i+1)%no_proc)
	{
		if(list[i])
		{
			printf("\nSending election message to %d",i);
			message[++mid]=i;
		}
	}
	
	temp = 0;
	for(i=1 ; i<=mid ; i++)
	{
		if(message[i]>temp)
		{
			temp = message[i];
			maxid = temp;
		}
	}
	
	printf("\nNew Co-ordinbator selected : %d",maxid);
	
	for(i=(genid+1)%no_proc ; i!=genid ; i=(i+1)%no_proc)
	{
		if(list[i])
		{
			printf("\nCo-ordinator message delivered to : %d",i);
		}
	}
	
	CoOrdinator_id = maxid;
}

break;

case 2:
printf("\nSelect a process to activate : ");
scanf("%d",&activate);

if(list[activate]!=0)
	break;
list[activate] = 1;

if(list[activate]==no_proc)
{
	printf("\nNew co-ordinator selected : %d",activate);	
	CoOrdinator_id = activate;
}
else
{
	printf("\nStarting election");
	for(i=(activate+1)%no_proc; i!=activate; i=(i+1)%no_proc)
	{
		if(list[i])
		{
			printf("\nMessage forwarded to : %d",i);
			if(i==CoOrdinator_id)
					break;
		}
	}
	printf("\nSending response from co-ordinator %d to %d",i,activate);
}
break;

case 3:	display();
	break;

case 4: break;
	
}
}
}


int main()
{
	int i,choice = 1;
	printf("\nEnter no. of processes : ");
	scanf("%d",&no_proc);
	for(i=1;i<=no_proc;i++)
	{
		printf("\nProcess id = %d is alive?(0/1) : ");
		scanf("%d",&list[i]);	
	}
	CoOrdinator_id = no_proc;
	display();
	while(choice<3)
	{
		printf("\n---Election Algorithym---");
		printf("\n1. Bully Algo");
		printf("\n2. Ring Algo");
				printf("\n3.Exit");
		printf("Enter your choice : ");
		scanf("%d",&choice);
		
		switch(choice)
		{
			case 1:
					bully();
					break;
			case 2:
					ring();
					break;
			case 3:
					exit(0);		
					break;	
		
		}
	}
	return 0;
}
			
	

