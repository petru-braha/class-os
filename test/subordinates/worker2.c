#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include "../util/h0.h"

int main()
{
  const char *shm_name = "./shared_memory";
  int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0600);
  if (-1 == shm_fd)
    report_error(__LINE__, "error: shm_open()", ROLE_WRK2);

  void *shm_ptr = mmap(
      NULL, INPUT_SIZE * 5 * sizeof(int),
      PROT_WRITE | PROT_READ,
      MAP_SHARED, shm_fd, 0);
  if (MAP_FAILED == shm_ptr)
    report_error(__LINE__, "error: MAP_FAILED()", ROLE_WRK2);

  if (-1 == close(shm_fd))
    report_error(__LINE__, "error: close() - check()", ROLE_WRK2);

  int index_row = *(int *)shm_ptr;
  int *numbers = shm_ptr + sizeof(int);
  // read
  // calc p
  // write
}