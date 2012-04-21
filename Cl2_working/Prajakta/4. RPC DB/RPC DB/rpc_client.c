//CLIENT PROGRAM


#include "DBHandle.h"
#include <stdlib.h>

void dbHandler( char* host, int choice)
{
   CLIENT *clnt;
   double  *result_1, *dp, f;
   char *endptr;
   int i;
   int *res;
   record  user_record_arg,*rec;
    
   
   clnt = clnt_create(host, DBPROG,DBPROG1, "udp");		/*clnt_create() -- Generic client creation. The program tells clnt_create() where the server is 																						located and the type of transport to use.*/
   if (clnt == NULL) {
      clnt_pcreateerror(host);
      exit(1);
   }
   
   switch(choice)
   {
	case 1: 
		///////ACCEPT///////
		printf("Enter the Roll no : "); 
		scanf("%d",&user_record_arg.rollno);
		printf("Enter the Name ");
		scanf("%s",user_record_arg.name);
		printf("Enter the Marks"); 
		scanf("%f",&user_record_arg.marks);
   		
   		////////CALL//////////
   		res = insert_1(&user_record_arg, clnt);
   		if (res == 0) 
		{
			clnt_perror(clnt, "call failed:");
   		}
		else
		{
			printf("RECORD INSERTED SUCCESSFULLY....");
		}
		clnt_destroy( clnt );
		
	break;
	case 2: 
		///////DELETE///////
		printf("Enter the Roll no : "); 
		scanf("%d",&user_record_arg.rollno);
   		
   		////////CALL//////////
   		res = delete_1(&user_record_arg.rollno, clnt);
   		if (res == 0) 
		{
			clnt_perror(clnt, "call failed:");
   		}
		else
		{
			printf("RECORD DELETED SUCCESSFULLY....");

		}
		clnt_destroy( clnt );
	break;
	case 3: 
		///////ACCEPT///////
		printf("Enter the Roll no : "); 
		scanf("%d",&user_record_arg.rollno);
		printf("Enter the Name ");
		scanf("%s",user_record_arg.name);
		printf("Enter the Marks"); 
		scanf("%f",&user_record_arg.marks);
   		
   		////////CALL//////////
   		res = update_1(&user_record_arg, clnt);
   		if (res == 0) 
		{
			clnt_perror(clnt, "call failed:");
   		}
		clnt_destroy( clnt );
		if(res != 0)
			printf("RECORD UPDATED SUCCESSFULLY....");
		else 
			printf("RECORD Error....");
	break;
	case 4: 
		///////ACCEPT///////
		printf("Enter the Roll no : "); 
		scanf("%d",&user_record_arg.rollno);
   		////////CALL//////////
   		rec = retrive_1(&user_record_arg.rollno, clnt);
  
		clnt_destroy( clnt );
		if(rec->rollno == 0)
		{
		
		printf("RECORD NOT FOUND....");
			
			

		}		
		else 
		{
			printf(" Roll no : %d\n",rec->rollno); 
			printf(" Name :%s\n",rec->name);
			printf(" Marks : %f\n\n\n",rec->marks); 
		}
			
	break;


   }
}


int menu(){
	int ch;	
	printf("RPC DATABASE CLIENT\n");
	printf("1) Insert Record\n");
	printf("2) Delete Record\n");
	printf("3) Update Record\n");
	printf("4) Show Record\n");
	printf("5) Exit\n");	
	printf("Enter Your choice :");
	
	scanf("%d",&ch);
	return ch;
		
}

main( int argc, char* argv[] )
{
   char *host;
	int choice;
   if(argc < 1) {
     printf(
      "usage: %s server_host value ...\n",
      argv[0]);
      exit(1);
   }
	
   printf("\nEnter choice : ");
   host = argv[1];

   while(1){
	choice = menu();
	if(choice == 5 )
		exit(0);
	dbHandler( host, choice);
   }
}


/************************OUTPUT***************************************

SERVER SIDE


[oslab@ws203-026 RPC DB]$ rpcgen DBHandle.x 

[oslab@ws203-026 RPC DB]$ rpcgen DBHandle.x [oslab@ws203-026 RPC DB]$ gcc rpc_client.c DBHandle_clnt.c DBHandle_xdr.c -o client
[oslab@ws203-026 RPC DB]$ gcc rpc_server.c DBHandle_svc.c DBHandle_xdr.c -o server
[oslab@ws203-026 RPC DB]$ su
Password: 
[root@ws203-026 RPC DB]# ./server


CLIENT SIDE

[oslab@ws203-026 RPC DB]$ ./client 127.0.0.1

Enter choice : 

RPC DATABASE CLIENT
1) Insert Record
2) Delete Record
3) Update Record
4) Show Record
5) Exit
Enter Your choice :1
Enter the Roll no : 4165
Enter the Name Prajakta
Enter the Marks 64
RECORD INSERTED SUCCESSFULLY....

RPC DATABASE CLIENT
1) Insert Record
2) Delete Record
3) Update Record
4) Show Record
5) Exit
Enter Your choice :1
Enter the Roll no : 4168
Enter the Name Neeraj
Enter the Marks 63
RECORD INSERTED SUCCESSFULLY....

RPC DATABASE CLIENT
1) Insert Record
2) Delete Record
3) Update Record
4) Show Record
5) Exit
Enter Your choice :4
Enter the Roll no : 4168 
 Roll no : 4168
 Name :Neeraj
 Marks : 63.000000


RPC DATABASE CLIENT
1) Insert Record
2) Delete Record
3) Update Record
4) Show Record
5) Exit
Enter Your choice :2
Enter the Roll no : 4165
RECORD DELETED SUCCESSFULLY....

RPC DATABASE CLIENT
1) Insert Record
2) Delete Record
3) Update Record
4) Show Record
5) Exit
Enter Your choice :3
Enter the Roll no : 4168 
Enter the Name Neeraj       
Enter the Marks 65
RECORD UPDATED SUCCESSFULLY....

RPC DATABASE CLIENT
1) Insert Record
2) Delete Record
3) Update Record
4) Show Record
5) Exit
Enter Your choice :4
Enter the Roll no : 4168
 Roll no : 4168
 Name :Neeraj
 Marks : 65.000000


RPC DATABASE CLIENT
1) Insert Record
2) Delete Record
3) Update Record
4) Show Record
5) Exit
Enter Your choice :

*************************************************************************/
