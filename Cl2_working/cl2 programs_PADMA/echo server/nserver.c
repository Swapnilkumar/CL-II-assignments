//Expt 1 : Socket Programming

/* A Multithreaded server in the internet domain using TCP
   The port number is passed as an argument */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void myaccept(int newsockfd){
     int n;
     char buffer[256];

     if (newsockfd < 0) 
          error("ERROR on accept");
while(strcmp(buffer,"exit\n")){
     bzero(buffer,256);
//-----------Read the Message ---------------------//
     n = recv(newsockfd,buffer,255,0);
     if (n < 0) error("ERROR reading from socket");
     printf("the message from client: %s\n",buffer);
//----------- Send the Reply---------------------//
     n = send(newsockfd,buffer,255,0);
     if (n < 0) error("ERROR writing to socket");
}
close(newsockfd);
}

int main(int argc, char *argv[])
{//----------- Declaration---------------------//
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	pthread_t acc_thread;
	int cnt=0;
     if (argc < 2) {
         printf("ERROR, no port provided\n");
         exit(1);
     }
//----------- Socket Creation---------------------//
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
//-----------Binding of Socket ---------------------//
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
//----------- Listening to the socket---------------------//
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
//----------- Accept the Connection---------------------//

do{
	
	printf("Waiting for Client\n");
	newsockfd = accept(sockfd, 		
        	         (struct sockaddr *) &cli_addr, 
        	         &clilen);
	cnt++;
	
	if(!pthread_create(&acc_thread,0,&myaccept,newsockfd)){
		printf("\nNEW thread Created : %d\n",cnt);
	}	


}while (1);
     
     close(sockfd);
     return 0; 
}


