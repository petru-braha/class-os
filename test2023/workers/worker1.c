#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "semaphore.h"
#include <fcntl.h>
#include "../util.h"

// receive from supervisor, send to map, returns how many odds
int send_w2(int *pointer, sem_t *sem)
{
  int count_odd = 0;
  for (int number = 0, bytes = -2; bytes;
       bytes = read(STDIN_FILENO, &number, sizeof(number)))
  {
    if (-2 == bytes)
      continue;
    if (-1 == bytes)
      report_error(__LINE__, __FILE__, "read() failed");

    if (number % 2)
    {
      number *= 2;
      count_odd++;
    }

    *pointer = number;
    pointer++;

    if (0 == number)
      break;
  }

  if (-1 == sem_post(sem))
    report_error(__LINE__, __FILE__, "sem_post() failed");

  return count_odd;
}

// receive from w2, send to supervisor
void send_sp(const int fifo, int *pointer, const int count_odd, sem_t *sem)
{
  if (-1 == sem_wait(sem))
    report_error(__LINE__, __FILE__, "sem_wait() failed");

  int sum = 0;
  for (int *end = pointer + SHMO_SIZE; pointer != end; pointer++)
  {
    sum += *pointer;
    if (0 == *pointer)
      break;
  }

  int results[2] = {sum, count_odd};
  if (-1 == write(fifo, results, sizeof(results)))
    report_error(__LINE__, __FILE__, "write() failed");
}

int main()
{
  // fifo
  int fifo = open(FIFO_PATH, O_WRONLY);
  if (-1 == fifo)
    report_error(__LINE__, __FILE__, "open() failed");

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

  //! transmission
  int count_odd = send_w2((int *)shm_ptr, sem_send);
  send_sp(fifo, (int *)shm_ptr, count_odd, sem_recv);

  // close fifo, sem, and map
  if (-1 == close(fifo))
    report_error(__LINE__, __FILE__, "close()");
  if (-1 == sem_close(sem_send))
    report_error(__LINE__, __FILE__, "sem_close()");
  if (-1 == sem_close(sem_recv))
    report_error(__LINE__, __FILE__, "sem_close()");
  if (-1 == munmap(shm_ptr, SHMO_SIZE))
    report_error(__LINE__, __FILE__, "munmap()");
}
