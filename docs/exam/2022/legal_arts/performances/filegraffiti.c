#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc != 3) 
        exit(2);

    const char *filepath = argv[1];
    const char *word = argv[2];
    int N = strlen(word);
    
    int fd = open(filepath, O_RDWR);
    if (fd == -1)
        exit(3); 
    
    off_t filesize = lseek(fd, 0, SEEK_END);
    if (filesize == -1) 
    {
        close(fd);
        exit(4);
    }

    if (filesize < N) 
    {
        printf("CANVAS TOO SMALL\n");
        close(fd);
        exit(0);
    }
    
    if (lseek(fd, -N, SEEK_END) == -1) 
    {
        close(fd);
        exit(5);
    }
    
    if (write(fd, word, N) != N) 
    {
        close(fd);
        exit(6);
    }
    
    printf("FILE VANDALIZED: %s\n", filepath);
    close(fd);
    return 0;
}