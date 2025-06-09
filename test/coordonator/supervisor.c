#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "../util/h0.h"

int send(const char *const path, const int fd)
{
  FILE *file = fopen(path, "r");
  if (NULL == file)
    report_error(__LINE__, "error: open() failed", ROLE_SUPV);

  // read file
  int numbers[INPUT_SIZE][3] = {0}, index_row = 0;
  for (char line[STRG_SIZE] = ""; fgets(line, STRG_SIZE, file);)
  {
    char *token = strtok(line, " \n");
    numbers[index_row][0] = atoi(token);
    token = strtok(NULL, " \n");
    numbers[index_row][1] = atoi(token);
    token = strtok(NULL, " \n");
    numbers[index_row][2] = atoi(token);
    index_row++;
  }

  // write to pipe
  if (-1 == write(fd, &index_row, sizeof(int)))
    report_error(__LINE__, "write() failed in send()", ROLE_SUPV);
  if (-1 == write(fd, numbers, index_row * 3 * sizeof(int)))
    report_error(__LINE__, "write() failed in send()", ROLE_SUPV);
  if (-1 == fclose(file))
    report_error(__LINE__, "fclose() failed in send()", ROLE_SUPV);

  return index_row;
}

void receive(const int count, const int fd)
{
  int numbers[INPUT_SIZE][5] = {0};
  if (-1 == read(fd, numbers, count * 5 * sizeof(int)))
    report_error(__LINE__, "read() failed in receive()", ROLE_SUPV);

  int count_invalid = 0;
  for (int i = 0; i < count; i++)
  {
    if (0 == numbers[i][4])
    {
      count_invalid++;
      fprintf(stderr, "tripleta a,b,c nu poate reprezenta"
                      " lungimile laturilor unui triunghi.\n");
    }
    else
      fprintf(stderr, "tipleta a,b,c"
                      " reprezintă lungimile laturilor unui triunghi"
                      " ce are perimetrul %d și aria %d.\n",
              numbers[i][4], numbers[i][5]);
  }

  fprintf(stderr, "variante totale: %d; variante invalide: %d.\n",
          count, count_invalid);
}

int main(int argc, char *argv[])
{
  if (2 != argc)
    report_error(__LINE__, "wrong number of arguments", ROLE_SUPV);

  int count = send(argv[1], STDOUT_FILENO);

  int fifo = open(FIFO_STRG, O_RDONLY);
  if (-1 == fifo)
    report_error(__LINE__, "error: fifo open()", ROLE_WRK1);

  receive(count, fifo);
  if (-1 == close(fifo))
    report_error(__LINE__, "error: close() fifo", ROLE_SUPV);
}
