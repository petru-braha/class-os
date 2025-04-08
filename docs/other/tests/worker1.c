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
    char text[2];
    read(STDIN_FILENO, text, 2);
    printf("%s", text);

    return 0;
}
