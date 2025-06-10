#ifndef __H0__
#define __H0__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char FIFO_STRG[10] = "./fifo";
#define STRG_SIZE 256
#define INPUT_SIZE 1024

const char *const NAME_SHM = "shared_memory";
const int MMAP_KEY = -5;
const int ROLE_SUPV = 0;
const int ROLE_WRK1 = 1;
const int ROLE_WRK2 = 2;

void report_error(const int line, const char *const message, const int role)
{
  char role_strg[15] = "";
  if (role == ROLE_SUPV)
    strcpy(role_strg, "supervisor");
  else if (role == ROLE_WRK1)
    strcpy(role_strg, "worker1");
  else
    strcpy(role_strg, "worker2");

  printf("role: %s - line: %d - error: %s.\n", role_strg, line, message);
  exit(EXIT_FAILURE);
}

#endif
