#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include "../util/h0.h"

int get_area(const int a, const int b, const int c, const int p)
{
  return (int)sqrt((double)(p - a) * (p - b) * (p - c) * p);
}

void check_triangle(int numbers[INPUT_SIZE][5], const int index_row)
{
  // assign r
  for (int i = 0; i < index_row; i++)
  {
    if (numbers[i][0] <= 0 || numbers[i][1] <= 0 ||
        numbers[i][2] <= 0)
      continue;

    if (numbers[i][0] + numbers[i][1] < numbers[i][2] ||
        numbers[i][1] + numbers[i][2] < numbers[i][0] ||
        numbers[i][2] + numbers[i][0] < numbers[i][1])
      continue;

    numbers[i][3] = 1;
  }

  // map send
  int size_map = 4 + index_row * 5 * sizeof(int);
  const char *shm_name = "./shared_memory";
  int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0600);
  if (-1 == shm_fd)
    report_error(__LINE__, "error: shm_open()", ROLE_WRK1);

  if (-1 == ftruncate(shm_fd, size_map))
    report_error(__LINE__, "error: ftruncate()", ROLE_WRK1);

  void *shm_ptr = mmap(
      NULL, size_map,
      PROT_WRITE | PROT_READ,
      MAP_SHARED, shm_fd, 0);
  if (MAP_FAILED == shm_ptr)
    report_error(__LINE__, "error: MAP_FAILED()", ROLE_WRK1);

  if (-1 == close(shm_fd))
    report_error(__LINE__, "error: close() - check()", ROLE_WRK1);

  *(int *)shm_ptr = index_row;
  memcpy(shm_ptr + sizeof(int), numbers, index_row * 5 * sizeof(int));
  // todo get data;

  // check for p
  for (int i = 0; i < index_row; i++)
  {
    if (0 == numbers[i][3])
      continue;
    numbers[i][3] *= 2;
    numbers[i][4] = get_area(
        numbers[i][0], numbers[i][1], numbers[i][2],
        numbers[i][3] / 2);
  }
}

int main(int argc, char *argv[])
{
  // input validation
  if (2 != argc)
    report_error(__LINE__, "wrong number of arguments", ROLE_WRK1);

  int pipe_anonymous[2] = {0, 0};
  if (-1 == pipe(pipe_anonymous))
    report_error(__LINE__, "pipe() failed", ROLE_WRK1);
  if (-1 == mkfifo(FIFO_STRG, 0777))
    report_error(__LINE__, "mkfifo() failed", ROLE_WRK1);

  pid_t pid = fork();
  if (-1 == pid)
    report_error(__LINE__, "fork() failed", ROLE_WRK1);

  // child
  if (0 == pid)
  {
    if (-1 == close(pipe_anonymous[0]))
      report_error(__LINE__, "error: close() failed in child", ROLE_WRK1);
    if (-1 == dup2(pipe_anonymous[1], STDOUT_FILENO))
      report_error(__LINE__, "error: close() failed in child", ROLE_WRK1);
    if (-1 == close(pipe_anonymous[1]))
      report_error(__LINE__, "error: close() failed in child", ROLE_WRK1);

    char *path_supervisor = "./coordonator/supervisor.out";
    char *argv_supervisor[] = {path_supervisor, argv[1], NULL};
    execv(path_supervisor, argv_supervisor);
    report_error(__LINE__, "execv() failed", ROLE_WRK1);
  }

  // parent
  else
  {
    // get from supervisor from file
    if (-1 == close(pipe_anonymous[1]))
      report_error(__LINE__, "error: close() failed in parent", ROLE_WRK1);

    int numbers[INPUT_SIZE][3] = {0}, index_row = 0;
    if (-1 == read(pipe_anonymous[0], &index_row, sizeof(int)))
      report_error(__LINE__, "read() failed in send()", ROLE_WRK1);
    if (-1 == read(pipe_anonymous[0], numbers, index_row * 3 * sizeof(int)))
      report_error(__LINE__, "read() failed in send()", ROLE_WRK1);

    // send to worker2
    int results[INPUT_SIZE][5] = {0};
    for (int i = 0; i < index_row; i++)
    {
      results[i][0] = numbers[i][0];
      results[i][1] = numbers[i][1];
      results[i][2] = numbers[i][2];
    }

    check_triangle(results, index_row);

    // send to supervisor
    int fifo = open(FIFO_STRG, O_WRONLY);
    if (-1 == fifo)
      report_error(__LINE__, "error: fifo open() failed in parent", ROLE_WRK1);

    if (-1 == write(fifo, results, index_row * 5 * sizeof(int)))
      report_error(__LINE__, "read() failed in send()", ROLE_WRK1);
  }
}
