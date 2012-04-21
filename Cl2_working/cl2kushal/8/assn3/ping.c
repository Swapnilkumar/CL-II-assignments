#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(){
int ping_ret, status;

status = system("ping -w 2 192.168.10.114");

if (-1 != status)
ping_ret = WEXITSTATUS(status);
printf("%d ",ping_ret);
}
