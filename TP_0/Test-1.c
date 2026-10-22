#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long int read_number(FILE *f)
{
    // Read the number from the chosen file and convert to long long int
    char c;
    int i = 0;
    char number[12];

    while ((c = fgetc(f)) != '\n' && c != EOF)
    {
        number[i] = c;
        i++;
    }

    number[i] = '\0';

    return atoll(number);
}

int main(int argc, char *argv[])
{
    int cpt = 100;
    int table[77263];
    int chosen[77263] = {0};

    FILE *f; 

    f = fopen("data/input/primes10.txt", "r+");

    FILE *foutput = fopen("data/output/Test-1", "w+");

    for (int i = 0; i < 168; i++)
    {
        read_number(f);
    }

    srand(time(NULL));

    if (f == NULL)
    {
        perror("Error Opening the prime numbers file !!!\n");
    }

    long long int number;

    for (int i = 0; i < 77263; i++)
    {
        table[i] = read_number(f);
    }

    fclose(f);
    
    while (cpt > 0)
    {
        number = rand() % 77263;

        if (!chosen[number])
        {
            fprintf(foutput, "%d\n", table[number]);
            chosen[number] = 1;
            cpt--;
        }
        
    }

    fclose(f);
    fclose(foutput);

    return EXIT_SUCCESS;
}
