#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int n;
    char filename[100];
    int val;

    srand(time(NULL));

    FILE *f;

    int **mat;

    for (int i = 0; i < 10; i++)
    {
        int n = 100 + (int)((((unsigned long long)rand() << 15) ^ rand()) % 9901);

        mat = malloc(sizeof(int*) * n);

        for (int i = 0; i < n; i++)
        {
            mat[i] = malloc(sizeof(int) * n);
        }

        mat[0][0] = rand() % 10;

        for (int j = 1; j < n; j++)
        {
            mat[0][j] = mat[0][j-1] + rand() % 10;
        }

        for (int i = 1; i < n; i++)
        {
            mat[i][0] = mat[i-1][0] + rand() % 10;
        }

        for (int i = 1; i < n; i++)
        {
            for (int j = 1; j < n; j++)
            {
                if (mat[i-1][j] > mat[i][j-1])
                {
                    val = mat[i-1][j];
                }
                else
                {
                    val = mat[i][j-1];
                }
                
                mat[i][j] = val + rand() % 10; 
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

        for (int i = 0; i < n; i++)
        {
            free(mat[i]);
        }
        
        free(mat);
        
        fclose(f);
    }
    
    return EXIT_SUCCESS;
}
