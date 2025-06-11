#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include "../util.h"

// send list of string
void send(const char *const path, void *dest, sem_t *sem)
{
  FILE *file = fopen(path, "r");
  if (NULL == file)
    report_error(__LINE__, __FILE__, "error: fopen() failed");

  // read file
  for (char line[STRG_SIZE] = ""; fgets(line, STRG_SIZE, file);)
  {
    int n = strlen(line);
    char buffer[STRG_SIZE] = "";
    int index = 0;

    for (int i = 0; i < n; i++)
    {
      if ((line[i] > 'a' && line[i] < 'z') ||
          (line[i] > 'A' && line[i] < 'Z'))
      {
        buffer[index++] = line[i];
        buffer[index++] = '\0';
        continue;
      }

      //! issue
      // separator
      if (strlen(buffer) < 3)
      {
        index = 0;
        continue;
      }

      printf("%s\n", buffer);
      *(int *)dest = index;
      dest += sizeof(index);
      strcpy(dest, buffer);
      dest += index;
      index = 0;
    }
  }

  *(int *)dest = 0;
  if (-1 == fclose(file))
    report_error(__LINE__, __FILE__, "error: fclose() failed");

  if (-1 == sem_post(sem))
    report_error(__LINE__, __FILE__, "sem_post() failed");
}

// receives string, string, int
void recv(void *src, sem_t *sem)
{
  if (-1 == sem_wait(sem))
    report_error(__LINE__, __FILE__, "sem_wait() failed");

  char first[4] = "";
  char last[4] = "";
  int count = 0;
  strncpy(first, src, 3);
  strncpy(last, src + 3, 3);
  count = *(int *)(src + 6);

  printf("RESULTS:\nfirst: %s\nlast: %s\ncount: %d;", first, last, count);
}

int main(int argc, char *argv[])
{
  // input validation
  if (2 != argc)
    report_error(__LINE__, __FILE__, "wrong number of arguments");
  if (-1 == access(argv[1], F_OK | R_OK))
    report_error(__LINE__, __FILE__, "no reading access right");

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
  send(argv[1], shm_ptr, sem_send);
  recv(shm_ptr, sem_recv);

  // close
  if (-1 == sem_close(sem_send))
    report_error(__LINE__, __FILE__, "sem_close()");
  if (-1 == sem_close(sem_recv))
    report_error(__LINE__, __FILE__, "sem_close()");
  if (-1 == munmap(shm_ptr, SHMO_SIZE))
    report_error(__LINE__, __FILE__, "munmap()");
}
