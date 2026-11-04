#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int throw_coin()
{
    // To choose the number randomly in the file itself
    return rand() % 2;
}

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
    long long int tab3[1000] = {0};
    long long int tab6[69000] = {0};

    long long int chosen3[1000] = {0};
    long long int chosen6[69000] = {0};

    FILE *f = fopen("data/input/primes10.txt", "r+");

    FILE *foutput = fopen("data/output/Test-3", "w+");

    srand(time(NULL));


    long long int number;

    while ((number = read_number(f)) < 100);
    
    int i = 0;
    do
    {
        tab3[i] = number;
        i++;
    } while ((number = read_number(f)) < 1000);
    int n1 = i;

    while ((number = read_number(f)) < 100000);

    i = 0;
    do
    {
        tab6[i] = number;
        i++;
    } while ((number = read_number(f)) < 1000000);
    int n2 = i;

    while ((number = read_number(f)) < 100000000);

    int cpt = 10;
    while (cpt > 0)
    {
        number = rand() % n1;

        if (!chosen3[number])
        {
            fprintf(foutput, "%lld\n", tab3[number]);
            chosen3[number] = 1;
            cpt--;
        }
        
    }   
    
    cpt = 10;
    while (cpt > 0)
    {
        number = rand() % n2;

        if (!chosen6[number])
        {
            fprintf(foutput, "%lld\n", tab6[number]);
            chosen6[number] = 1;
            cpt--;
        }
        
    }   
  
    cpt = 10;
    while (cpt > 0)
    {
        number = read_number(f);
        if (throw_coin() == 1)
        {
            fprintf(foutput, "%lld\n", number);
            cpt--;
        }
    }

    fclose(f);

    f = fopen("data/input/primes_12_digit.txt", "r+");
    
    cpt = 10;
    while (cpt > 0)
    {
        number = read_number(f);
        if (throw_coin() == 1)
        {
            fprintf(foutput, "%lld\n", number);
            cpt--;
        }
    }

    fclose(f);

    fclose(foutput);
    
    return EXIT_SUCCESS;
}
