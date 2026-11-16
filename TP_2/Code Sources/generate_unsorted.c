#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int n;
    char filename[100];

    srand(time(NULL));

    FILE *f;

    for (int i = 0; i < 10; i++)
    {
        n = rand() % 1900 + 100;
        sprintf(filename, "data_%dx%d.txt", n, n);
        f = fopen(filename, "w+");

        int s = n * n;

        for (int i = 0; i < s; i++)
        {
            fprintf(f, "%d\n", rand());
        }
        
        fclose(f);
    }
    
    return EXIT_SUCCESS;
}