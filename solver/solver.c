#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../utils/pixel_operations.h"
#include "../nn/nn.h"
#include "solver.h"

int **from_photo_to_array(int** array)
{
	
    int x = 0;
    int y = 0;
    for(int i = 0; i < 81; i++)
    {
	    char buffer[20];
	    snprintf(buffer,20,"output/split/%i.png",i);
	    int v;
	    if(isValid(buffer))
	    {
		v = NN(buffer);
		while(v == 0)
		{
			v = NN(buffer);
		}
		array[y][x] = v;
	    }
	    else
	    {
		v = -1;
		array[y][x] = -1;
	    }
	    // printf("%i,[%i][%i] = %i ",i,y,x,v);
	    // printf(buffer);
	    // printf("\n");
	    y += 1;
	    if(y == 9)
	    {
		x += 1;
		y = 0;
	    }
    }
    return array;
}


char *file_to_str(char const *filename)
{
    char *file_str;
    int fd;
    struct stat sb;

    if (stat(filename, &sb) == -1 || sb.st_size == 0)
        return (NULL);
    file_str = malloc(sizeof(char) * sb.st_size + 1);
    if (file_str == NULL)
        return (NULL);
    fd = open(filename, O_RDONLY);
    if (fd == -1 || read(fd, file_str, sb.st_size) == -1) {
        free(file_str);
        return (NULL);
    }
    file_str[sb.st_size] = '\0';
    close(fd);
    return (file_str);
}

int is_num(char c)
{
    if ((c >= '0' && c <= '9') || c == '.')
        return 1;
    return 0;
}

int **to_array(int **array, char *str)
{
    int x = 0;
    int y = 0;
    for (unsigned int i = 0; str[i]; ++i) {
        if (is_num(str[i])) {
            if (str[i] == '.')
                array[x][y] = -1;
            else
                array[x][y] = str[i] - '0';
            if (y < 8) {
                ++y;
            } else {
                ++x;
                y = 0;
            }
        }
    }
    return array;
}

int is_acceptable(int **grid, int row, int col, int num)
{

    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return 0;
    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num)
            return 0;

    int startRow = row - row % 3,
                 startCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j +
                          startCol] == num)
                return 0;

    return 1;
}

int solve_array(int **grid, int row, int col)
{
    if (row == 9 - 1 && col == 9)
        return 1;
    if (col == 9) {
        row++;
        col = 0;
    }
    if (grid[row][col] > 0)
        return solve_array(grid, row, col + 1);
    for (int num = 1; num <= 9; num++) {
        if (is_acceptable(grid, row, col, num)==1) {
            grid[row][col] = num;
            if (solve_array(grid, row, col + 1) == 1)
                return 1;
        }
        grid[row][col] = 0;
    }
    return 0;
}

void write_output(int **array)
{
    FILE *output = fopen("output/grid_00.result", "w");
    for (unsigned char x = 0; array[x]; ++x) {
        for (unsigned char y = 0; array[x][y]; ++y) {
            fprintf(output,"%d", array[x][y]);
            if (y == 2 || y == 5)
                fprintf(output, " ");
        }
        fprintf(output, "\n");
        if (x == 2 || x == 5)
            fprintf(output, "\n");
    }
    fclose(output);
}

void write_input(int **array)
{
    FILE *input = fopen("output/grid_00", "w");
    for (unsigned char x = 0; array[x]; ++x) {
        for (unsigned char y = 0; array[x][y]; ++y) {
	    if(array[x][y] == -1)
	    {
		fprintf(input,".");
	    }
	    else{
            	fprintf(input,"%d", array[x][y]);
	    }
            if (y == 2 || y == 5)
                fprintf(input, " ");
        }
        fprintf(input, "\n");
        if (x == 2 || x == 5)
            fprintf(input, "\n");
    }
    fprintf(input,"\n");
    fclose(input);
}



