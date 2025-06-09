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
#define fifo_name "FIFO_SW1"
#define max_len 256
#define map_name "MAP_W1W2"

bool verification(float numbers[3])
{
    for(int i = 0;i < 3; i++)
        if(numbers[i] <= 0)
            return false;

    if(numbers[0] + numbers[1] >= numbers[2])
        return false;
    if(numbers[1] + numbers[2] >= numbers[0])
        return false;
    if(numbers[0] + numbers[2] >= numbers[1])
        return false;
    return true;
}

void receive_supervisor(int pipe[2], void* map)
{
    size_t bytes = -1;
    float numbers[3];

    while( (bytes = read(pipe[0], numbers, sizeof(numbers))) )
    {
        if(bytes != sizeof(numbers)) handler("read");
        bool triangle = verification(numbers);

        // write to map for worker 2
        map = numbers;
        map += sizeof(numbers);
    }
}

void receive_worker2(int pipe[2], void* map)
{
    float numbers[3], decision;
    size_t bytes = -1;

    float A = 0, P = 0;

    // read from map and pass to pipe
    while(true)
    {   
        memcpy(numbers, (float*)map, sizeof(numbers));
        map += sizeof(numbers);
        if(numbers[0] == EOF)
            break;

        memcpy(&decision, (float*)map, sizeof(float));
        map += sizeof(decision);

        // calculate ap
        if(decision)
        {
            P = 2*decision;
            A = 
            // change P A

        }

        // write to pipe
        bytes = write(pipe[1], numbers, sizeof(numbers));
        if(-1 == bytes) handler("write");
        
        bytes = write(pipe[1], &P, sizeof(float));
        if(-1 == bytes) handler("write");
        
        bytes = write(pipe[1], &A, sizeof(float));
        if(-1 == bytes) handler("write");
    }
} 

int main(int argc, char* argv[])
{
    if(argc != 2) handler("argv");

    // pipe
    int pipe_sw1[2];
    if(-1 == pipe(pipe_sw1)) handler("pipe");

    // inside supervisor's process
    pid_t pid_supervisor = fork();
    if(-1 == pid_supervisor) handler("fork");
    if(pid_supervisor == 0)
    {   
        // supervisor writes in pipe
        if(-1 == close(pipe_sw1[0])) handler("close"); 
        if(-1 == dup2(pipe_sw1[1], STDOUT_FILENO)) handler("dup2");
        excl("./..coordonator.supervisor", argv[1], NULL);
        handler("excl");
    }

    // fifo
    int fifo_descriptor = mkfifo(fifo_name, 0666);
    if(-1 == fifo_descriptor) handler("mkfifo");
    int fd = open(fifo_name, O_RDWR);
    if(-1 == fd) handler("open");

    // map w1w2
    int map = shm_open(map_name, O_CREAT | O_RDWR, 0666);
    if(-1 == map) handler("shm_open");
    int length = 1024; 

    void* map_address = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, map, 0);
    if(map_address == MAP_FAILED) handler("mmap");

    // operation
    receive_supervisor(pipe_sw1, map_address);
    receive_worker2(pipe_sw1, map_address);
    
    // terminate the program
    if(-1 == close(fd)) handler("close");
    if(-1 == munmap(map_address, length)) handler("munmap");
    return 0;
}