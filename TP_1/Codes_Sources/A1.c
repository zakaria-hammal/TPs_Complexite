#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct fichier_tp fichier_tp;
struct fichier_tp
{
    FILE *f;
    char filename[100];
    int nb_nombres;
};


long long int read_number(FILE *f)
{
    // Read the number from the chosen file and convert to long long int
    char c;
    int i = 0;
    char number[13];

    while ((c = fgetc(f)) != '\n' && c != EOF)
    {
        number[i] = c;
        i++;
    }

    number[i] = '\0';

    return atoll(number);
}

int est_premier(long long int n)
{
    int trouv = 0;
    long long int i = 2;

    while (i < n && trouv == 0)
    {
        if (n % i == 0)
        {
            trouv = 1;
        }
        else
        {
            i  = i + 1;
        }
    }

    return !trouv;
}

int main(int argc, char *argv[])
{
    fichier_tp files[5] = {
        {
            fopen("../data/output/Random100", "r+"),
            "Random100",
            100
        },
        {
            fopen("../data/output/Random1000", "r+"),
            "Random1000",
            1000
        },
        {
            fopen("../data/output/Test-1", "r+"),
            "Test-1",
            100
        },
        {
            fopen("../data/output/Test-2", "r+"),
            "Test-2",
            30
        },
        {
            fopen("../data/output/Test-3", "r+"),
            "Test-3",
            40
        }
    };

    FILE *f_result[5];
    char file_name[256];

    long long int *nombres_fixes;

    for (int j = 0; j < 5; j++)
    {
        sprintf(file_name, "A1_%s.csv", files[j].filename);
        f_result[j] = fopen(file_name, "w+");
        nombres_fixes = calloc(files[j].nb_nombres, sizeof(long long int));
        for (int i = 0; i < files[j].nb_nombres; i++)
        {
            nombres_fixes[i] = read_number(files[j].f);
            clock_t debut = clock();
            int premier = est_premier(nombres_fixes[i]);
            clock_t fin = clock();

            double temps_ms = (double)(fin - debut) * 1000.0 / CLOCKS_PER_SEC;
            fprintf(f_result[j], "%lld,%s,%.9f\n",
                    nombres_fixes[i],
                    premier ? "Oui" : "Non",
                    temps_ms);
        }

        fclose(files[j].f);
        fclose(f_result[j]);
        free(nombres_fixes);
    }

    return EXIT_SUCCESS;
}
