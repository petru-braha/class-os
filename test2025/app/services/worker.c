#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include "../util.h"

void business(const int rd, const int wd)
{
  const char vocals[] = "aeiouAEIOU";
  char word[4] = "";
  for (size_t bytes = read(rd, word, 3); bytes; bytes = read(rd, word, 3))
  {
    if (strchr(vocals, word[0]) ||
        NULL == strchr(vocals, word[1]) ||
        strchr(vocals, word[2]))
      continue;

    if (-1 == write(wd, word, 3))
      report_error(__LINE__, __FILE__, "write() failed");
  }
}

int main()
{
  int fifo = open(FIFO_PATH, O_WRONLY);
  if (-1 == fifo)
    report_error(__LINE__, __FILE__, "open() failed");

  //! transmission
  business(STDIN_FILENO, fifo);

  // close
  if (-1 == close(fifo))
    report_error(__LINE__, __FILE__, "close() failed");
}
