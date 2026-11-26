#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int n;
    long long s;
    char filename[100];

    srand(time(NULL));

    FILE *f;

    for (int i = 0; i < 10; i++)
    {
        n = 100 + (int)((((unsigned long long)rand() << 15) ^ rand()) % 9901);
        
        sprintf(filename, "data_%dx%d.txt", n, n);
        f = fopen(filename, "w+");

        s = (long long)n * n;

        for (long long i = 0; i < s; i++)
        {
            fprintf(f, "%d\n", (int)((((unsigned long long)rand() << 15) ^ rand()) & 0x7FFFFFFF));
        }
        
        fclose(f);
    }
    
    return EXIT_SUCCESS;
}