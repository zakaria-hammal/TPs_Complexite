#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int n;
    char filename[100];

    srand(time(NULL));

    FILE *f;

    int **mat;

    for (int i = 0; i < 10; i++)
    {
        n = rand() % 1900 + 100;
        mat = malloc(sizeof(int*) * n);
        for (int i = 0; i < n; i++)
        {
            mat[i] = malloc(sizeof(int) * n);
        }

        int dec_lig;
        int dec_col;

        mat[0][0] = rand() % 100;

        for (int i = 1; i < n; i++)
        {
            dec_lig = rand() % 10;
            mat[0][i] = mat[0][i-1] + dec_lig;
        }

        for (int j = 1; j < n; j++)
        {
            dec_col = rand() % 10;
            mat[j][0] = mat[j-1][0] + dec_col;
        }

        for (int i = 1; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                dec_lig = rand() % 10;
                dec_col = rand() % 10;
                mat[i][j] = mat[i-1][j] + mat[i][j-1] + dec_lig + dec_col; 
            }
            
        }
        
        sprintf(filename, "data_sorted_%dx%d.txt", n, n);
        f = fopen(filename, "w+");

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                fprintf(f, "%d\n", mat[i][j]);
            }
            
        }
        
        fclose(f);
    }
    
    return EXIT_SUCCESS;
}