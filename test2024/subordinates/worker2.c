#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include "../util/h0.h"

int main(int argc)
{
  int shm_fd = shm_open(NAME_SHM, O_CREAT | O_RDWR, 0600);
  if (-1 == shm_fd)
    report_error(__LINE__, "shm_open()", ROLE_WRK2);

  int size_map = sizeof(int) + INPUT_SIZE * 5 * sizeof(int);

  // if called before worker1
  if (argc == 2)
    if (-1 == ftruncate(shm_fd, size_map))
      report_error(__LINE__, "ftruncate()", ROLE_WRK1);

  void *shm_ptr = mmap(
      NULL, size_map,
      PROT_WRITE | PROT_READ,
      MAP_SHARED, shm_fd, 0);
  if (MAP_FAILED == shm_ptr)
    report_error(__LINE__, "mmap()", ROLE_WRK2);

  if (-1 == close(shm_fd))
    report_error(__LINE__, "close() - check()", ROLE_WRK2);

  int *index_row = (int *)shm_ptr;
  while (*index_row <= 0)
    ; // wait for worker1 to send data

  shm_ptr += sizeof(int);

  for (void *end = shm_ptr + *index_row * 5 * sizeof(int); shm_ptr < end;
       shm_ptr += 5 * sizeof(int))
  {
    int *arr = (int *)shm_ptr;
    if (arr[3])
      arr[3] = (arr[0] + arr[1] + arr[2]) / 2;
  }

  // end
  *index_row = MMAP_KEY;
  shm_ptr = index_row;

  if (-1 == munmap(shm_ptr, size_map))
    report_error(__LINE__, "munmap()", ROLE_WRK2);
}
