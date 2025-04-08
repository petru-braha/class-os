#include <stdio.h>

int main()
{
    const char buffer[] = "12 12";
    
    int number = -1, termen = -1;
    sscanf(buffer, "%d %d", number, termen);
    printf("%d %d", number, termen);
}