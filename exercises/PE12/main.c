#include <stdio.h>

int main(int argc, char* argv[]) {
    // open files
    FILE *infp = fopen(argv[1], "r");
    FILE *outfp = fopen(argv[2], "w");

    // create buffer
    char buff[100];

    // read first line of infile as dimensions
    int rows, cols;
    fgets(buff, sizeof(buff), infp);
    sscanf(buff, "%d %d", &rows, &cols);

    // write transposed dimensions to output
    fprintf(outfp, "%d %d\n", cols, rows);

    // read matrix from infile
    int matrix[rows][cols];
    for (int i = 0; i < rows; i++) {
        fgets(buff, 2 * cols + 1, infp);
        for (int j = 0; j < cols; j++) {
            sscanf(buff + (2 * j), "%d", &matrix[i][j]);
        }
    }
    
    // transpose matrix
    int matrix_T[cols][rows];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix_T[j][i] = matrix[i][j];
        }
    }

    // write transposed matrix to outfile
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            fprintf(outfp, "%d", matrix_T[i][j]);
            if (j != rows - 1) {
                fprintf(outfp, " ");
            }
        }
        fprintf(outfp, "\n");
    }

    // close files
    fclose(infp);
    fclose(outfp);
}