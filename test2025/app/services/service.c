#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "semaphore.h"
#include <fcntl.h>
#include "../util.h"

// receive from analyzer, send to pipe, returns how many odds
void send_w2(void *shm_ptr, int fd, sem_t *sem)
{
  if (-1 == sem_wait(sem))
    report_error(__LINE__, __FILE__, "sem_post() failed");
  printf("hey\n");

  for (int bytes = *(int *)shm_ptr; bytes; bytes = *(int *)shm_ptr)
  {
    printf("%d\n", bytes);
    shm_ptr += sizeof(int);
    void *ptr = shm_ptr;
    shm_ptr += bytes;

    for (; ptr + 3 != shm_ptr; ptr++)
      if (-1 == write(fd, ptr, 3))
        report_error(__LINE__, __FILE__, "write() failed");
  }
}

// receive from w2, send to supervisor
void send_an(const int fd, void *shm_ptr, sem_t *sem)
{
  size_t bytes = 0, count = 1;

  char first[4] = "";
  bytes = read(fd, first, 3);
  if (-1 == bytes)
    report_error(__LINE__, __FILE__, "read() failed");

  char last[4] = "";
  for (; bytes; bytes = read(fd, last, 3), count++)
    if (-1 == bytes)
      report_error(__LINE__, __FILE__, "read() failed");

  // write
  strncpy(shm_ptr, first, 3);
  strncpy(shm_ptr + 3, last, 3);
  *(int *)(shm_ptr + 6) = count;

  if (-1 == sem_post(sem))
    report_error(__LINE__, __FILE__, "sem_post() failed");
}

int main()
{
  // pipe
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
    send_w2(shm_ptr, pipe_anonymous[1], sem_send);

    int fifo = open(FIFO_PATH, O_RDONLY);
    if (-1 == fifo)
      report_error(__LINE__, __FILE__, "parent open() failed");
    send_an(fifo, shm_ptr, sem_recv);

    // close
    if (-1 == wait(NULL))
      report_error(__LINE__, __FILE__, "wait() failed");

    if (-1 == close(pipe_anonymous[1]))
      report_error(__LINE__, __FILE__, "parent close(1) failed");
    if (-1 == close(fifo))
      report_error(__LINE__, __FILE__, "close()");

    if (-1 == sem_close(sem_send))
      report_error(__LINE__, __FILE__, "sem_close()");
    if (-1 == sem_close(sem_recv))
      report_error(__LINE__, __FILE__, "sem_close()");
    if (-1 == munmap(shm_ptr, SHMO_SIZE))
      report_error(__LINE__, __FILE__, "munmap()");
  }
}
