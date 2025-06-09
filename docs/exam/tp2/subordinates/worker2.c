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

void communication(void* map)
{
    
    

}

int main()
{
    // create map
    int map = shm_open(map_name, O_CREAT | O_RDWR, 0666);
    if(-1 == map) handler("shm_open");
    int length = 1024;

    void* map_address = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, map, 0);
    if(map_address == MAP_FAILED) handler("mmap");

    // actual execution
    communication(map_address);

    // terminate the program
    if(-1 == munmap(map_address, length)) handler("munmap");
    return 0;
}