#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int choose_file()
{
    // This function chooses a random file between the  files provided in the website : http://www.prime-numbers.org/
    return rand() % 10;
}

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
    int cpt = 100;
    FILE *f[10]; 

    f[0] = fopen("data/input/primes10.txt", "r+");
    f[1] = fopen("data/input/lino1", "r+");
    f[2] = fopen("data/input/lino2", "r+");
    f[3] = fopen("data/input/lino3", "r+");
    f[4] = fopen("data/input/lino4", "r+");
    f[5] = fopen("data/input/lino5", "r+");
    f[6] = fopen("data/input/lino6", "r+");
    f[7] = fopen("data/input/lino7", "r+");
    f[8] = fopen("data/input/lino8", "r+");
    f[9] = fopen("data/input/lino9", "r+");


    FILE *foutput = fopen("data/output/Random100", "w+");

    srand(time(NULL));

    long long int number;

    while (cpt > 0)
    {
        number = read_number(f[choose_file()]);
        if (throw_coin() == 1)
        {
            fprintf(foutput, "%lld\n", number);
            cpt--;
        }
    }

    for (int j = 0; j < 10; j++)
    {
        fclose(f[j]);
    }

    fclose(foutput);

    return EXIT_SUCCESS;
}
