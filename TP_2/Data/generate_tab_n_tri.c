#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    FILE *f;

    srand(time(NULL));
    char filename[150];

    int sizes[14] = {100000, 200000, 400000, 600000, 800000, 1000000, 1200000, 1400000, 1600000, 1800000, 2000000, 4000000, 6000000, 8000000};

    for (int j = 0; j < 14; j++)
    {   
        sprintf(filename, "tab_non_trie_%d.txt", sizes[j]);
        f = fopen(filename, "w+");

        if (f == NULL)
        {
            perror("Erreur creation du fichier tab_non_trie.txt\n");
            return EXIT_FAILURE;
        }
        
        for (int i = 0; i < sizes[j]; i++)
        {
            int val = (((rand() & 0x7FFF) << 15) | (rand() & 0x7FFF));
            fprintf(f, "%d\n", val);
        }

        fclose(f);
        printf("Fichier tab_non_trie.txt genere avec succes (%d elements).\n", sizes[j]);
    }

    return EXIT_SUCCESS;
}
