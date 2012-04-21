/* Client Prgram for the Sort the number through RPC
   Roll No:4169 (BE-I)
*/
   

#include "sort.h"
#include <stdlib.h>


void sortprog_1( char* host, int argc, char *argv[])
{
   CLIENT *clnt;
   double  *result_1, *dp,*dp1, f;
      char *endptr;
      int i, j;
	 
   input_data  sort_1_arg;
   input_data *output;
       
       
     sort_1_arg.input_data.input_data_val =(double*)malloc(MAXAVGSIZE*sizeof(double));
     dp = sort_1_arg.input_data.input_data_val;
     sort_1_arg.input_data.input_data_len = argc - 2;
      for (i=1;i<=(argc - 2);i++) 
{
        f = strtod(argv[i+1],&endptr);
        printf("value   = %e\n",f);
        *dp = f;
        dp++;
      }
   clnt = clnt_create(host, SORTPROG, SORTVERS, "udp");
   if (clnt == NULL) {
      clnt_pcreateerror(host);
      exit(1);
   }
   output= sort_1(&sort_1_arg, clnt);
printf("\nSorted Data :   \n");
dp1=output->input_data.input_data_val;

for(j=1;j<=output->input_data.input_data_len;j++)
{  printf("%e \n ",*dp1);
dp1++;
}
clnt_destroy(clnt);
}

main( int argc, char* argv[] )
{
   char *host;

   if(argc < 3)
 {
     printf("usage: %s server_host value ...\n",argv[0]);
      exit(1);
   }
        if(argc > MAXAVGSIZE + 2)
       {printf("Two many input values\n");
          exit(2);
        }
   host = argv[1];
   sortprog_1( host, argc, argv);
}
