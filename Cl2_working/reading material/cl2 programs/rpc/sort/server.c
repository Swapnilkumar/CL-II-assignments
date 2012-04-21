/*Server Prgram for the Sort the number through RPC
   Roll No:4169 (BE-I)
*/

#include <rpc/rpc.h>
#include "sort.h"
#include <stdio.h>


input_data * sort_1(input_data *input,CLIENT *client)
 {
double *dp1 = input->input_data.input_data_val;
double *dp2 = input->input_data.input_data_val;
double temp;

  u_int i,j;
  for(i=1,dp1=input->input_data.input_data_val;i<=input->input_data.input_data_len;i++,dp1++) 
{for(j=1,dp2=input->input_data.input_data_val;j<=input->input_data.input_data_len;j++,dp2++) 
    {   if((*dp2)>(*dp1))
       { temp=*dp2;
	 *dp2=*dp1;
	 *dp1=temp;
       }
    }
}
  return(input);
}

input_data *sort_1_svc(input_data *input,struct svc_req *svc)
 {
  CLIENT *client;
  return(sort_1(input,client));
}
