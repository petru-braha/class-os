#include <unistd.h> 
#include <errno.h>      
#include <string.h>     
#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    if(argc == 1)
        exit(1);

    if(open(argv[1], O_RDWR))
        exit(2);

    unsigned char first_byte = 0;
    if(read(argv[1], first_byte, sizeof(unsigned char)))
        exit(3);
    
    unsigned char last_byte = 0;
    if(lseek(argv[1], -1, SEEK_END)) 
        exit(4);

    if(read(argv[1], last_byte, sizeof(unsigned char)))
        exit(5);

    printf(stdout, "%s=%d\n", argv[1], first_byte + last_byte);
    return 0;
}