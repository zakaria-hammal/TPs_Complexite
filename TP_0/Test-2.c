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
    int tab3[1000];
    int tab4[1100];
    int tab5[8400];

    int chosen3[1000];
    int chosen4[1100];
    int chosen5[8400];

    FILE *f = fopen("data/input/primes10.txt", "r+");; 

    FILE *foutput = fopen("data/output/Test-2", "w+");

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

    i = 0;
    do
    {
        tab4[i] = number;
        i++;
    } while ((number = read_number(f)) < 10000);
    int n2 = i;

    i = 0;
    do
    {
        tab5[i] = number;
        i++;
    } while ((number = read_number(f)) < 100000);
    int n3 = i;

    int cpt = 10;
    while (cpt > 0)
    {
        number = rand() % n1;

        if (!chosen3[number])
        {
            fprintf(foutput, "%d\n", tab3[number]);
            chosen3[number] = 1;
            cpt--;
        }
        
    }   
    
    cpt = 10;
    while (cpt > 0)
    {
        number = rand() % n2;

        if (!chosen4[number])
        {
            fprintf(foutput, "%d\n", tab4[number]);
            chosen4[number] = 1;
            cpt--;
        }
        
    }   

    cpt = 10;
    while (cpt > 0)
    {
        number = rand() % n3;

        if (!chosen5[number])
        {
            fprintf(foutput, "%d\n", tab5[number]);
            chosen5[number] = 1;
            cpt--;
        }
        
    }   

    fclose(f);
    fclose(foutput);
    
    return EXIT_SUCCESS;
}
