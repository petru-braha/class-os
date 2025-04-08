#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <errno.h>

#define handler(msg)\
    { perror(msg); exit(EXIT_FAILURE); }
#define fifo_name "FIFO_W1W2"
#define map_name "MAP_SW2"
#define max_len 256

int main()
{
    // supervisor => worker1
    int p[2];
    if(-1 == pipe(p)) handler("pipe");

    // parent process
    pid_t pid_child = fork();
    if(pid_child == 0)
    {
        close(p[1]);
        char text[2];
        dup2(p[0], STDIN_FILENO);
        execl("./worker1", "./worker1", NULL);
        perror("execl");
    }
    else
    {

    
    // parent process
    if(-1 == close(p[0])) handler("close2");

    // send data
    char text[300] = "da";
    write(p[1], text, 2);
    
    if(-1 == close(p[1])) handler("close3");

    wait(NULL);
    }
    return 0;
}