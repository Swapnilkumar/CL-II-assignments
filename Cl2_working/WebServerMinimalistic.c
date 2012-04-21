#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 8096

void mylog(char *s1, char *s2, int num)
{
	int fd ;
	char logbuffer[BUFSIZE*2];

	(void)sprintf(logbuffer," INFO: %s:%s:%d",s1, s2,num);
	fd = open("WebServer.log", O_CREAT| O_WRONLY | O_APPEND,0644);
	write(fd,logbuffer,strlen(logbuffer)); 
	write(fd,"\n",1);      close(fd);
}

void web(int fd, int hit)
{
	int j, file_fd, buflen, len;
	long i, ret;	char * fstr;
	static char buffer[BUFSIZE+1]; /* static so zero filled */

	ret =read(fd,buffer,BUFSIZE); 	/* read Web request in one go */
	mylog("request",buffer,hit);

	for(i=4;i<BUFSIZE;i++) { /*ignore string after "GET URL "*/
		if(buffer[i] == ' ') { 
			buffer[i] = 0;/*null terminate*/
			break;
		}
	}

	if(!strncmp(&buffer[0],"GET /\0",6)||!strncmp(&buffer[0],"get /\0",6) ) 
		(void)strcpy(buffer,"GET /index.html");

	buflen=strlen(buffer);

	file_fd = open(&buffer[5],O_RDONLY); /* open the file for reading */

	mylog("SEND",&buffer[5],hit);

	sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n", fstr);
	write(fd,buffer,strlen(buffer));

	/* send file in 8KB block - last block may be smaller */
	while (	(ret = read(file_fd, buffer, BUFSIZE)) > 0 ) {
		write(fd,buffer,ret);
	}
#ifdef LINUX
	sleep(1);	/* to allow socket to drain */
#endif
	exit(1);
}

int main(int argc, char **argv)
{
	int i, port, pid, listenfd, socketfd, hit;
	size_t length;
	static struct sockaddr_in serv_addr, cli_addr;

	if( argc != 3 ) {
		printf("usage: %s <port> <top-directory>\n\n",argv[0]);
		exit(0);
	}
	chdir(argv[2]);

	if(fork() != 0)	return 0; /*become deamon*/

	mylog("web server starting",argv[1],getpid());

	listenfd = socket(AF_INET, SOCK_STREAM,0);

	port = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr));
	listen(listenfd,64);

	for(hit=1; ;hit++) {
		length = sizeof(cli_addr);
		socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length);

		if((pid = fork()) == 0) {/* child */
			(void)close(listenfd);
			web(socketfd,hit); /* never returns */
		}
		else (void)close(socketfd); /* parent */
	}
}
