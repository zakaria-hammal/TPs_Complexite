#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct fichier_tp fichier_tp;
struct fichier_tp
{
    FILE *f;
    int nb_nombres;
};


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

int est_premier(int n)
{
    int trouv = 0;
    int i = 2;
    double r = sqrt(n);

    if (n != 2)
    {
        if (n % 2 == 0)
        {
            trouv = 1;
        }
        else
        {
            while (i < r && trouv == 0)
            {
                if (n % i == 0)
                {
                    trouv = 1;
                }
                else
                {
                    i  = i + 2;
                }
            }
        }
    }
    
    return !trouv;
}

int main(int argc, char *argv[])
{
    fichier_tp files[5] = {
        {
            fopen("data/output/Random100", "r+"),
            100
        },
        {
            fopen("data/output/Random1000", "r+"),
            1000
        },
        {
            fopen("data/output/Test-1", "r+"),
            100
        },
        {
            fopen("data/output/Test-2", "r+"),
            30
        },
        {
            fopen("data/output/Test-3", "r+"),
            40
        }
    };

    long long int *nombres_fixes;

    for (int j = 0; j < 4; j++)
    {
        nombres_fixes = calloc(sizeof(long long int), files[j].nb_nombres);
        for (int i = 0; i < files[j].nb_nombres; i++)
        {
            nombres_fixes[i] = read_number(files[j].f);
            clock_t debut = clock();
            int premier = est_premier(nombres_fixes[i]);
            clock_t fin = clock();

            double temps_ms = (double)(fin - debut) * 1000.0 / CLOCKS_PER_SEC;
            printf("%-15lld | %-10s | %-10.5f\n",
                nombres_fixes[i],
                premier ? "Oui" : "Non",
                temps_ms);
        }

        fclose(files[j].f);
        free(nombres_fixes);
    }

    return EXIT_SUCCESS;
}