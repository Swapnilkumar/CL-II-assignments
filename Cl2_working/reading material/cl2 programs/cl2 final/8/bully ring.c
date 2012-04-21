

#include<stdio.h>
#include<stdlib.h>
#define max 20
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int ping_node(char *host){
	int ping_ret, status;
	char sbuf[50];
	sprintf(sbuf,"ping -c 4 %s",host);
	status = system(sbuf);

	if (-1 != status)
	ping_ret = WEXITSTATUS(status);
	return ping_ret;
}

struct Node_Struct
{
	char list[max];
	int alive;
}node[max];

int no_proc, CoOrdinator_id;

void display()
{
	int i;
	printf("\nID\t\tIP\t\tAlive");
	for(i=1; i<=no_proc; i++)
	{
		printf("\n\n%2d", i);
		printf("\t%s", node[i].list);
		printf("\t\t%2d\n", node[i].alive);
	}

}

void bully()
{
	int choice=1, crash, activate, genid, i, flag, gen_coOrdinator,is_alive,j;


	printf("\nCurrent co-ordinator: %d", CoOrdinator_id);

	for(i=no_proc;i>=1;i--)
	{
			is_alive=!(ping_node(node[i].list));
			if(is_alive!=node[i].alive && is_alive==0)
			{
				node[i].alive=0;
				/**/
				if(i==CoOrdinator_id)
				{
					display();
					printf("\nEnter generator id :");
					scanf("%d",&genid);
					printf("\nStarting election algorithm ");
					while(1)
					{
							flag=0;
						for(i=genid+1;i<=no_proc;i++)
						{
								printf("\nSending request from %d to %d",genid,i);
								if(node[i].alive)
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

						CoOrdinator_id=genid;
					}
				}
			}
			if(is_alive!=node[i].alive && is_alive==1)
			{
					node[i].alive=1;
					if(i>CoOrdinator_id)
					{
							printf("\nNew coordinator selected : %d",i);
							CoOrdinator_id=i;
					}
					else
					{
							printf("Starting election algorithm");
							flag=0;
							for(j=i;j<=no_proc;j++)
							{
									printf("\nSending request from %d to %d",i,j);

									if(node[j].alive)
									{
										flag=1;
										gen_coOrdinator=j;
									}

							}
							if(flag)
							{
									printf("\nNew co-ordinator selected : %d",gen_coOrdinator);
									CoOrdinator_id=gen_coOrdinator;
							}
							else
							{
									printf("\nActivated Proess %d is the new co-ordinator",i);
									CoOrdinator_id=i;
							}
					}
			}

	}
}

void ring()
{
	int choice=1,crash,activate,genid,i,flag,temp,message[max],mid=0,maxid,j,is_alive;


	printf("\nCurrent co-ordinator: %d", CoOrdinator_id);

	for(i=no_proc;i>=1;i--)
	{
			is_alive=!(ping_node(node[i].list));
			if(is_alive!=node[i].alive && is_alive==0)
			{
				if(i==CoOrdinator_id)
				{
					printf("\nEnter generator id : ");
					scanf("%d",&genid);
					printf("\nStarting election algorithm");
					mid = 1;
					message[mid]=genid;
					for(j=(genid+1)%no_proc ; j!=genid ; j=(j+1)%no_proc)
					{
						if(node[j].alive)
						{
								printf("\nSending election message to %d",j);
								message[++mid]=j;
						}
					}

					temp = 0;
					for(j=1 ; j<=mid ; j++)
					{
						if(message[j]>temp)
						{
							temp = message[j];
							maxid = temp;
						}
					}

					printf("\nNew Co-ordinbator selected : %d",maxid);

					for(j=(genid+1)%no_proc ; j!=genid ; j=(j+1)%no_proc)
					{
						if(node[j].alive)
						{
							printf("\nCo-ordinator message delivered to : %d",j);
						}
					}

					CoOrdinator_id = maxid;
				}
				printf("\nCurrent  coordinator : %d",CoOrdinator_id);
			}
			if(is_alive!=node[i].alive && is_alive==1)
			{
					node[i].alive=1;

					if(i > CoOrdinator_id)
					{
							printf("\nNew co-ordinator selected : %d",i);
							CoOrdinator_id = i;
					}
					else
					{
							printf("\nStarting election");
							for(j=(activate+1)%no_proc; j!=activate; j=(j+1)%no_proc)
							{
								if(node[j].alive)
								{
										printf("\nMessage forwarded to : %d",j);
										if(j==CoOrdinator_id)
											break;
								}
							}
							printf("\nSending response from co-ordinator %d to %d",j,i);
					}
					printf("\nCurrent  coordinator : %d",CoOrdinator_id);
			}
	}
}


int main()
{
	int i,choice = 1;

	printf("\nEnter no. of Nodes : ");
	scanf("%d",&no_proc);
	for(i=1;i<=no_proc;i++)
	{
		printf("\nEnter Node %d IP  : ",i);
		scanf("%s",node[i].list);
		node[i].alive = !(ping_node(node[i].list));
		printf("\nGot the host %s is",node[i].list);
		node[i].alive==0? printf(" Dead\n") : printf(" Alive\n");


	}
	for(i=no_proc;i>1;i--)
	{
		if(node[i].alive==1)
		{
			CoOrdinator_id = i;
			break;
		}
	}
	if(i==0)
		printf("All dead.........");
	display();
	while(choice<4)
	{
		printf("\n---Election Algorithym---");
		printf("\n1. Bully Algo");
		printf("\n2. Ring Algo");
		printf("\n3. Display");
				printf("\n4.Exit");
		printf("\nEnter your choice : ");
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
					display();
					break;
			case 4:
					exit(0);
					break;

		}
	}
	return 0;
}



