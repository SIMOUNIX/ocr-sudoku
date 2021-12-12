#ifndef SOLVER_H
#define SOLVER_H

int **from_photo_to_array(int** array);
char *file_to_str(char const *filename);
int **to_array(int **array, char *str);
int is_num(char c);
int is_acceptable(int **grid, int row, int col, int num);
int solve_array(int **grid, int row, int col);
void write_output(int **array);
void write_input(int **array);
#endif
