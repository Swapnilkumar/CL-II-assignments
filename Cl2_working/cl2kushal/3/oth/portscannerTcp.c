/* A TCP port scanner created by billy www.softhardware.co.uk*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>


/* Main programs starts*/
int main(int argc, char **argv)
{
   int   sd;         //socket descriptor
   int    port;         //port number
   int   start;         //start port
   int    end;         //end port
   int    rval;         //socket descriptor for connect   
   char    responce[1024];      //to receive data
   char   *message="shell";       //data to send
   struct hostent *hostaddr;   //To be used for IPaddress
   struct sockaddr_in servaddr;   //socket structure
   
   if (argc < 4 )
   {
      printf("------Created By www.Softhardware.co.uk-----------\n");
      printf("--------------------------------------------------\n");
      printf("Usage: ./tscan <IPaddress> <Start Port> <End Port>\n");
      printf("--------------------------------------------------\n");
      return (EINVAL);
   }
   start = atoi(argv[2]);
   end   = atoi(argv[3]);
   for (port=start; port<=end; port++)
   {

         //portno is ascii to int second argument     

   sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //created the tcp socket
   if (sd == -1)
   {
     perror("Socket()\n");
     return (errno);
   }   

   memset( &servaddr, 0, sizeof(servaddr));

   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(port); //set the portno
   
   hostaddr = gethostbyname( argv[1] ); //get the ip 1st argument
   
   memcpy(&servaddr.sin_addr, hostaddr->h_addr, hostaddr->h_length);
     
   //below connects to the specified ip in hostaddr
   
 

   rval = connect(sd, (struct sockaddr *) &servaddr, sizeof(servaddr));
   if (rval == -1)
   {
   printf("Port %d is closed\n", port);
   close(sd);
   }
   else
   printf("Port %d is open\n",port);
   
   close(sd);         //socket descriptor
   }
   
}

