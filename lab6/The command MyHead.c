#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define DEFAULT_PARAMETER 10
#define ERROR_CODE -1

void head(const int fd, const int n)
{
  char buffer;
  int cnt = 0;

  while (cnt < n && read(fd, &buffer, 1) > 0)
  {
    if (buffer == '\n')
      cnt++;
    if (-1 == write(STDOUT_FILENO, &buffer, 1))
    {
      perror("write");
      exit(ERROR_CODE);
    }
  }

  printf("\n");
}

void head_bytes(int fd, int n)
{
  char buffer;
  int cnt = 0;

  while (cnt < n && read(fd, &buffer, 1) > 0)
  {
    cnt++;
    if (-1 == write(STDOUT_FILENO, &buffer, 1))
    {
      perror("write");
      exit(ERROR_CODE);
    }
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
  if (4 != argc && 2 != argc)
  {
    perror("USAGE: <option> <number> <filename>\n");
    return ERROR_CODE;
  }

  if (2 == argc)
  {
    int fd = open(argv[1], O_RDONLY, 0644);
    if (-1 == fd)
    {
      perror("open");
      return ERROR_CODE;
    }

    head(fd, 10);
    close(fd);
    return 0;
  }

  // if (4 == argc)
  char *option = argv[1];
  int n = atoi(argv[2]);
  int fd = open(argv[3], O_RDONLY, 0644);
  if (-1 == fd)
  {
    perror("open");
    return ERROR_CODE;
  }

  if (strncmp(option, "-n", 2) == 0)
    head(fd, n);
  else if (strncmp(option, "-c", 2) == 0)
    head_bytes(fd, n);
  else
  {
    printf("UNKNOWN PARAMETER: %s", option);
    return ERROR_CODE;
  }

  close(fd);
}
