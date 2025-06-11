#define _DEFAULT_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include "util.h"

int get_area(const int a, const int b, const int c, const int p)
{
  return (int)sqrt((double)(p - a) * (p - b) * (p - c) * p);
}

void worker2(int *map_anonymouS)
{
  // map
  int shm_fd = shm_open(SHMO_PATH, O_CREAT | O_RDWR, 0600);
  if (-1 == shm_fd)
    report_error(__LINE__, __FILE__, "shm_open() failed");

  if (-1 == ftruncate(shm_fd, SHMO_SIZE))
    report_error(__LINE__, __FILE__, "ftruncate() failed");

  void *shm_ptr = mmap(
      NULL, SHMO_SIZE,
      PROT_WRITE | PROT_READ,
      MAP_SHARED, shm_fd, 0);
  if (MAP_FAILED == shm_ptr)
    report_error(__LINE__, __FILE__, "mmap() failed");

  if (-1 == close(shm_fd))
    report_error(__LINE__, __FILE__, "close() failed");

  // semaphore
  sem_t *sem_send = sem_open(SEM0_PATH, O_CREAT | O_RDWR, 0600, 0);
  if (SEM_FAILED == sem_send)
    report_error(__LINE__, __FILE__, "sem_open() failed");

  if (-1 == sem_wait(sem_send))
    report_error(__LINE__, __FILE__, "sem_wait() failed");

  //! transmission
  for (int *arr = shm_ptr;; arr += 4) // 4 * sizeof(int)
  {
    if (arr[3])
      arr[3] = (arr[0] + arr[1] + arr[2]) / 2;
    memcpy(map_anonymouS, arr, sizeof(arr));
    map_anonymouS += 4;

    // see line 36 at worker1
    if (0 == arr[0] && 0 == arr[1] && 0 == arr[2] && 0 == arr[3])
      break;
  }

  // close
  if (-1 == munmap(shm_ptr, SHMO_SIZE))
    report_error(__LINE__, __FILE__, "munmap() failed");
}

void worker3(int *map_anonymouS)
{
  int fifo = open(FIFO_PATH, O_RDONLY);
  if (-1 == fifo)
    report_error(__LINE__, __FILE__, "parent open() failed");

  sem_t *sem_recv = sem_open(SEM1_PATH, O_CREAT | O_RDWR, 0600, 0);
  if (SEM_FAILED == sem_recv)
    report_error(__LINE__, __FILE__, "sem_open() failed");

  //! transmission
  int *arr = map_anonymouS;
  for (;; arr += 4) // 4 * sizeof(int)
  {
    int number[5] = {0, 0, 0, 0, 0};
    memcpy(number, arr, 4 * sizeof(int));
    number[4] = get_area(arr[0], arr[1], arr[2], arr[3]);
    if (-1 == write(fifo, number, sizeof(number)))
      report_error(__LINE__, __FILE__, "write() failed");

    // see line 36 at worker1
    if (0 == arr[0] && 0 == arr[1] &&
        0 == arr[2] && 0 == arr[3])
      break;
  }

  // close
  if (-1 == close(fifo))
    report_error(__LINE__, __FILE__, "close() failed");
  if (-1 == sem_post(sem_recv))
    report_error(__LINE__, __FILE__, "sem_post() failed");
  if (-1 == sem_close(sem_recv))
    report_error(__LINE__, __FILE__, "sem_close() failed");
}

// sem, memory - read; fifo - write
int main(int argc)
{

  // map
  void *map_anonymouS = mmap(
      NULL, SHMO_SIZE,
      PROT_WRITE | PROT_READ,
      MAP_ANONYMOUS, -1, 0);
  if (MAP_FAILED == map_anonymouS)
    report_error(__LINE__, __FILE__, "mmap() failed");

  pid_t pid = fork();
  if (-1 == pid)
    report_error(__LINE__, __FILE__, "fork() failed");

  // child
  if (0 == pid)
  {
    worker2((int *)map_anonymouS);
  }

  // parent
  else
  {
    worker3((int *)map_anonymouS);
    wait(NULL);
  }
}
