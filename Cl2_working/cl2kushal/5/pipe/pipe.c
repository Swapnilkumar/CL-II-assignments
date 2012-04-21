This is a simple example of sending data over a pipe in Unix/Linux , from parent to child.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int     numbytes;
    int     fd[2];
    pid_t   pid;
    char    line[1024];

    if (pipe(fd) < 0)
    {
      printf("Pipe Error");
      exit(0);
    }
    if ((pid = fork()) < 0)
    {
      printf("Fork Error");
      exit(0);
    }
    else if (pid > 0)           /* parent */
    {
        close(fd[0]);
        write(fd[1], "Howdy\n", 6);
    }
    else                    /* child */
    {
        close(fd[1]);
        numbytes = read(fd[0], line,1024);
        write(STDOUT_FILENO, line, numbytes);
    }
    exit(0);
}


