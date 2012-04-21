
#include "porth.h"
#include <stdlib.h>
#include <string.h>

void getports_1( char* host, int argc, char *argv[])
{
   CLIENT *clnt;
   in  *input_struct;
   out *output_struct;


   int i,f;

    input_struct = (in*) malloc(sizeof(in));


    strcpy(input_struct->host1,argv[2]);
    input_struct->start = atoi(argv[3]);
    input_struct->end  = atoi(argv[4]);
        printf("%s",input_struct->host1);
   clnt = clnt_create(host, PORTPROG, PORTS, "udp");

   if (clnt == NULL)
   {
      clnt_pcreateerror(host);
      exit(1);
   }

   output_struct=getopenports_1(input_struct,clnt);


   clnt_destroy( clnt );

   printf("\n\nPorts which are opened on %s \n", input_struct->host1);

   for(i=1;i<=output_struct->tot;i++)
   {
        printf("%d\n",output_struct->port_no[i]);
   }

}

main( int argc, char* argv[] )
{
   char *host;

   if(argc < 5) {
     printf("usage: %s server_host remote_machine_ip start_port_value  end_port_value...\n",argv[0]);
      exit(1);
   }
   host = argv[1];
   getports_1( host, argc, argv);
}


