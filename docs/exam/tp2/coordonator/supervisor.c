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

void send_worker1(const char* file_name)
{
    // create fd
    int fd_read_file = open(file_name, O_RDONLY);
    if(-1 == fd_read_file) handler("open");

    // read 
    // convert to binar
    size_t bytes = -1;
    float number1 = -1, number2 = -1, number3 = -1;

    while( (bytes = read(fd_read_file, &number1, sizeof(float))) ) // read the first number
    {
        if(-1 == bytes) handler("read");

        bytes = read(fd_read_file, &number2, sizeof(float));
        if(-1 == bytes) handler("read");

        bytes = read(fd_read_file, &number3, sizeof(float));
        if(-1 == bytes) handler("read");

        // convert them to binary format
        // write
        bytes = write(fd_read_file, &number2, sizeof(float));
        if(-1 == bytes) handler("write");
        
        bytes = write(fd_read_file, &number2, sizeof(float));
        if(-1 == bytes) handler("write");

        bytes = write(fd_read_file, &number3, sizeof(float));
        if(-1 == bytes) handler("write");
    }
}

void receive_worker1(int fd)
{
    size_t bytes = -1;
    float numbers[5];

    int yes = 0, no = 0;
    while( (bytes = read(fd, numbers, 5*sizeof(float))) )
    {
        if(bytes != sizeof(float)*5) handler("read");

        if(numbers[3] == 0)
        {
            no++;
            printf("tripleta %d,%d,%d nu poate reprezenta lungimile laturilor unui triunghi", numbers[0], numbers[1], numbers[2]);
        }
        else
        {
            printf("tripleta %d,%d,%d reprezintă lungimile laturilor unui triunghi ce are perimetrul %d și aria %d", numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5]);
            yes++;
        }
    }

    printf("%d triplete pot fi reprezentate, iar %d nu.\n", yes, no);
}

int main(int argc, char* argv[])
{
    if(argc != 2) handler("argv");
    const char* file_name = argv[1];

    // fifo
    int fifo_descriptor = mkfifo(fifo_name, 0666);
    if(-1 == fifo_descriptor) handler("mkfifo");
    int fd_read = open(fifo_name, O_RDWR);
    if(-1 == fd_read) handler("open");

    // actual execution
    send_worker1(file_name);
    receive_worker1(fd_read);
    
    // terminate program
    if(-1 == close(fd_read)) handler("close");
    unlink(fifo_name);
    return 0;
}