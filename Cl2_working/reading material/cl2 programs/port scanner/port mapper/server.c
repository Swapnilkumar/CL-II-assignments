#include <rpc/rpc.h>
#include "porth.h"
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

out * getopenports_1(in * p1,CLIENT *client) {
   out * output_struct;
   int sockfd, port;
    struct hostent *he;
    int j=1;
    output_struct = (out*) malloc(sizeof(out));

    if ((he=gethostbyname(p1->host1)) == NULL)    // get the host info
    {
        perror("gethostbyname");
  	exit(1);
    }

    printf("\n\nPort Number\tStatus");
    for(port=p1->start; port<=p1->end; port++)
    {

        struct sockaddr_in their_addr; // connector's address information
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket");
            exit(1);
        }

        their_addr.sin_family = AF_INET;    // host byte order
        their_addr.sin_port = htons(port);  // short, network byte order
        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        memset(&(their_addr.sin_zero), '\0', 8);  // zero the rest of the struct

    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
        {
            close(sockfd);
        }
        else
        {
            output_struct->port_no[j]=port;
            printf("OPEN");
            j++;
            close(sockfd);
        }
    }
    output_struct->tot = j-1;
    return (output_struct);
}

out *  getopenports_1_svc(in * p1,struct svc_req *svc) {
  CLIENT *client;
  return(getopenports_1(p1,client));
}



