#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  if (-1 == mkfifo("dada.fifo", 0600))
    perror("error - fifo");
  int fd = open("dada.fifo", O_RDONLY);
  if (-1 == fd)
    perror("error - open");

  printf("dadadadadaa\n");
  if (-1 == close(fd))
    perror("error - clse");
}
