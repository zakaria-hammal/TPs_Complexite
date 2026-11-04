#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long int read_number(FILE *f)
{
    // Read the number from the chosen file and convert to long long int
    char c;
    int i = 0;
    char number[32];
    while ((c = fgetc(f)) != '\n' && c != EOF)
        if (i < (int)sizeof(number) - 1) number[i++] = (char)c;

    number[i] = '\0';

    return atoll(number);
}

int main(int argc, char *argv[])
{
    int cpt = 100;
    long long int table[78330];
    long long chosen[78330] = {0};

    FILE *f; 

    f = fopen("data/input/primes10.txt", "r+");
    
    if (f == NULL)
    {
        perror("Error Opening the prime numbers file !!!\n");
    }

    FILE *foutput = fopen("data/output/Test-1", "w+");

    for (int i = 0; i < 168; i++)
    {
        read_number(f);
    }

    srand(time(NULL));

    long long int number;

    for (int i = 0; i < 77095; i++)
    {
        table[i] = read_number(f);
    }

    fclose(f);
    
    while (cpt > 0)
    {
        number = rand() % 77095;

        if (!chosen[number])
        {
            fprintf(foutput, "%lld\n", table[number]);
            chosen[number] = 1;
            cpt--;
        }
        
    }

    fclose(foutput);

    return EXIT_SUCCESS;
}
