#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char** argv) 
{
    if (argc != 2) {
        fprintf(stderr, "No arguments\n");
        return 2;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 3;
    }

    int count = 0;
    char ch = 0;
    while ((ch = fgetc(file)) != EOF) 
        if (islower(ch)) 
            count++;

    fclose(file);
    printf("%s:%d:%d\n", argv[1], (int)strlen(argv[1])-count, count);
    return 0;
}