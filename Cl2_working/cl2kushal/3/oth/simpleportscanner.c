#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

struct hostent *host;
struct sockaddr_in addr;
int PortSocket;
int from_port = atoi(argv[2]);
int to_port = atoi(argv[3]);


if((host = gethostbyname((const char*)argv[1])) == NULL) {
        puts("Bad address\n");
        _exit(-1);
}
int i = 0;
for(i = from_port; i <= to_port; i++) {
if((PortSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        puts("Can not create socket\n");
        _exit(-1);
}

addr.sin_family = AF_INET;
memcpy(&(addr.sin_addr), host->h_addr, host->h_length);
addr.sin_port = htons(i);

        if(connect(PortSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1)continue;
        printf("Port %d open\n", i);
close(PortSocket);
        }
return 0;
}

