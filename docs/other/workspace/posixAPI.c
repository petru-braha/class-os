#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[]) 
{
    int fd = creat("alx2.cpp", O_WRONLY);
    int can_accessed = access("alx2.cpp", O_RDONLY);
    off_t filesize = lseek(fd, 0, SEEK_END);
    //if (lseek(fd, -4, SEEK_END) == -1)
    
    printf("%d ma cacccccccccccccccc\n", can_accessed);

    close(fd);
    return 0;
}