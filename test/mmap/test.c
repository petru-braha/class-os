#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
  int fd = open("dada.txt", O_RDWR);
  if (-1 == fd)
    perror("error - open");

  char *map = (char *)mmap(
      NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (MAP_FAILED == map)
    perror("error - mmap");
  if (-1 == close(fd))
    perror("error - fcls");

  map[0] = '2';
  map[1] = '2';
  map[2] = '2';
  map[3] = '2';
  if (-1 == munmap(map, 4096))
    perror("error - unmp");
}
