#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include "../util.h"
#include "semaphore.h"

// send: w1 => w2
void business(void *shm_ptr, sem_t *sem_send, sem_t *sem_recv)
{
  if (-1 == sem_wait(sem_send))
    report_error(__LINE__, __FILE__, "sem_wait()");

  int *pointer = (int *)shm_ptr;
  for (int *end = pointer + SHMO_SIZE; pointer != end; pointer++)
  {
    *pointer = *pointer * *pointer;
    if (0 == *pointer)
      break;
  }

  if (-1 == sem_post(sem_recv))
    report_error(__LINE__, __FILE__, "sem_post()");
}

int main()
{
  // map
  int shm_fd = shm_open(SHMO_PATH, O_CREAT | O_RDWR, 0600);
  if (-1 == shm_fd)
    report_error(__LINE__, __FILE__, "shm_open()");

  if (-1 == ftruncate(shm_fd, SHMO_SIZE))
    report_error(__LINE__, __FILE__, "ftruncate()");

  void *shm_ptr = mmap(
      NULL, SHMO_SIZE,
      PROT_WRITE | PROT_READ,
      MAP_SHARED, shm_fd, 0);
  if (MAP_FAILED == shm_ptr)
    report_error(__LINE__, __FILE__, "mmap()");

  if (-1 == close(shm_fd))
    report_error(__LINE__, __FILE__, "close()");

  // semaphore
  sem_t *sem_send = sem_open(SEM0_PATH, O_CREAT | O_RDWR, 0600, 0);
  if (SEM_FAILED == sem_send)
    report_error(__LINE__, __FILE__, "sem_open()");
  sem_t *sem_recv = sem_open(SEM1_PATH, O_CREAT | O_RDWR, 0600, 0);
  if (SEM_FAILED == sem_recv)
    report_error(__LINE__, __FILE__, "sem_open()");

  business(shm_ptr, sem_send, sem_recv);

  // close
  if (-1 == sem_close(sem_send))
    report_error(__LINE__, __FILE__, "sem_close()");
  if (-1 == sem_close(sem_recv))
    report_error(__LINE__, __FILE__, "sem_close()");
  if (-1 == munmap(shm_ptr, SHMO_SIZE))
    report_error(__LINE__, __FILE__, "munmap()");
}
