#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

/*read
  put lock
  read again
*/
int main()
{
  struct flock lock;
  lock.l_type = F_RDLCK;
  lock.l_whence = SEEK_CUR;
  lock.l_start = 0;
  lock.l_len = sizeof(int);
  return 0;
}
