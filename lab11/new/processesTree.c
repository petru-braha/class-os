// set 1 ex 1 - A particular tree of processes

// ordine pid ppid child_pid child_code

#include <stdio.h>
#include <stdlib.h>
#include "sys/types.h"
#include "unistd.h"

void check_error(int status)
{
  if (-1 == status)
  {
    perror("good luck");
    exit(EXIT_FAILURE);
  }
}

void mutate(
    const int count_children,
    const int count_grandchildren,
    const int level)
{
  // edge cases
  if (count_children < 0 && count_children < 0)
  {
    fprintf(stderr, "error: %s.\n", "wrong arguments");
    exit(EXIT_FAILURE);
  }

  if (count_children == 0 && count_children == 0)
    return;
  if (level == 3)
    return;

  // actual work
  const pid_t pid = fork();
  if (pid == 0)
  {
    mutate(count_children, count_grandchildren - 1, level + 1);
    int exit_code = 0;
    check_error(wait(&exit_code));
  }
  else
  {
    mutate(count_children - 1, count_grandchildren, level + 1);
    int exit_code = 0;
    check_error(wait(&exit_code));
  }
}

int main()
{
  const int ch = 4, gc = 3, level = 1;
  mutate(ch, gc, level);
  printf("the program finished.\n");
}
