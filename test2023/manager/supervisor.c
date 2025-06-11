//! 0 is treated as the end of the file

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "../util.h"

void send(const int fd, const char *const path_input)
{
  FILE *file = fopen(path_input, "r");
  if (NULL == file)
    report_error(__LINE__, __FILE__, "error: open() failed");

  // read file
  for (char line[STRG_SIZE] = ""; fgets(line, STRG_SIZE, file);)
  {
    // delete \n
    line[strlen(line) - 1] = '\0';
    int number = atoi(line);
    if (-1 == write(fd, &number, sizeof(number)))
      report_error(__LINE__, __FILE__, "error: write() failed");

    if (0 == number)
      break;
  }

  if (-1 == fclose(file))
    report_error(__LINE__, __FILE__, "error: write() failed");
}

void receive(const int fifo)
{
  int results[2] = {0, 0};
  if (-1 == read(fifo, results, sizeof(results)))
    report_error(__LINE__, __FILE__, "read() failed");

  // todo operations

  // except 50520 as a sum
  printf("RESULTS\nsum: %d, odds: %d.\n", results[0], results[1]);
}

int main(int argc, char *argv[])
{
  if (2 != argc)
    report_error(__LINE__, __FILE__, "wrong number of arguments");

  int pipe_anonymous[2] = {0, 0};
  if (-1 == pipe(pipe_anonymous))
    report_error(__LINE__, __FILE__, "pipe() failed");

  if (-1 == mkfifo(FIFO_PATH, 0777))
    report_error(__LINE__, __FILE__, "mkfifo() failed");

  pid_t pid = fork();
  if (-1 == pid)
    report_error(__LINE__, __FILE__, "fork() failed");

  // child
  if (0 == pid)
  {
    if (-1 == close(pipe_anonymous[1]))
      report_error(__LINE__, __FILE__, "child close() failed");
    if (-1 == dup2(pipe_anonymous[0], STDIN_FILENO))
      report_error(__LINE__, __FILE__, "parent close() failed");

    execl(WRK1_PATH, WRK1_PATH, NULL);
    report_error(__LINE__, __FILE__, "child execl() failed");
  }

  // parent
  else
  {
    if (-1 == close(pipe_anonymous[0]))
      report_error(__LINE__, __FILE__, "parent close(0) failed");

    send(pipe_anonymous[1], argv[1]);
    if (-1 == close(pipe_anonymous[1]))
      report_error(__LINE__, __FILE__, "parent close(1) failed");

    int fifo = open(FIFO_PATH, O_RDONLY);
    if (-1 == fifo)
      report_error(__LINE__, __FILE__, "parent open() failed");

    receive(fifo);

    if (-1 == wait(NULL))
      report_error(__LINE__, __FILE__, "wait() failed");
    if (-1 == close(fifo))
      report_error(__LINE__, __FILE__, "parent close() failed");
  }
}
