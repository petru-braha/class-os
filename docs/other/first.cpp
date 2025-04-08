#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    printf("ma fut in crucea mati");
    execl("delete stream.sh", NULL);

    return 0;
}
