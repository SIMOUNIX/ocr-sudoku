#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include "../utils/pixel_operations.h"
#include "../utils/image.h"
#include "final.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

char solver[81];
char to_solve[81];

void init_solver(char* path)
{
    FILE *file = fopen(path, "r");

    if (file != NULL)
    {
        char c;
        size_t i = 0;

        while ((c = fgetc(file)) != EOF)
        {

            if (c != '\n' && c != ' ')
            {
                solver[i] = c - '0';
            }
            else
            {
                i -= 1;
            }

            i += 1;
        }
    }

    fclose(file);
}

void init_to_solve(char* path)
{
    FILE *file = fopen(path, "r");

    if (file != NULL)
    {
        char c;
        size_t i = 0;

        while ((c = fgetc(file)) != EOF)
        {

            if (c != '\n' && c != ' ')
            {
                to_solve[i] = c - '0';
            }
            else
            {
                i -= 1;
            }

            i += 1;
        }
    }

    fclose(file);
}

void copy (SDL_Surface *grid, SDL_Surface *number, unsigned x, unsigned y)
{
  size_t w = number -> w;
  size_t h = number -> h;

  for (size_t i = 0; i < w; i++)
  {
    for (size_t j = 0; j < h; j++)
    {
      Uint32 pixel = get_pixel(number, i, j);
      // printf("i:%ld j:%ld w:%ld h:%ld\n", i, j, w, h);
      put_pixel(grid, x + i, y + j, pixel);
    }
  }
}

void display()
{
  init_solver("output/grid_00.result");
  init_to_solve("output/grid_00");
  SDL_Surface *grid = IMG_Load("output/final/1800.jpg");
  unsigned x = 10;
  unsigned y = 10;

  // size_t width = grid->w;
  // size_t heigth = grid->h;

  for (size_t i = 0; i < 81; i++)
  {
      // printf("height grid: %ld width grid: %ld\n", heigth, width);
    if (solver[i] == 1)
    {
        if (solver[i] != to_solve[i])
        {
            SDL_Surface *number = IMG_Load("output/final/1_blue.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
        else
        {
            SDL_Surface *number = IMG_Load("output/final/1.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
    }
    else if (solver[i] == 2)
    {
        if (solver[i] != to_solve[i])
        {
            SDL_Surface *number = IMG_Load("output/final/2_blue.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
        else
        {
            SDL_Surface *number = IMG_Load("output/final/2.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
    }

    else if (solver[i] == 3)
    {
        if (solver[i] != to_solve[i])
        {
            SDL_Surface *number = IMG_Load("output/final/3_blue.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
        else
        {
            SDL_Surface *number = IMG_Load("output/final/3.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
    }

    else if (solver[i] == 4)
    {
        if (solver[i] != to_solve[i])
        {
            SDL_Surface *number = IMG_Load("output/final/4_blue.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
        else
        {
            SDL_Surface *number = IMG_Load("output/final/4.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
    }

    else if (solver[i] == 5)
    {
        if (solver[i] != to_solve[i])
        {
            SDL_Surface *number = IMG_Load("output/final/5_blue.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
        else
        {
            SDL_Surface *number = IMG_Load("output/final/5.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
    }

    else if (solver[i] == 6)
    {
        if (solver[i] != to_solve[i])
        {
            SDL_Surface *number = IMG_Load("output/final/6_blue.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
        else
        {
            SDL_Surface *number = IMG_Load("output/final/6.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
    }

    else if (solver[i] == 7)
    {
        if (solver[i] != to_solve[i])
        {
            SDL_Surface *number = IMG_Load("output/final/7_blue.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
        else
        {
            SDL_Surface *number = IMG_Load("output/final/7.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
    }

    else if (solver[i] == 8)
    {
        if (solver[i] != to_solve[i])
        {
            SDL_Surface *number = IMG_Load("output/final/8_blue.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
        else
        {
            SDL_Surface *number = IMG_Load("output/final/8.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
    }

    else if (solver[i] == 9)
    {
        if (solver[i] != to_solve[i])
        {
            SDL_Surface *number = IMG_Load("output/final/9_blue.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
        else
        {
            SDL_Surface *number = IMG_Load("output/final/9.png");

            copy(grid, number, x, y);

            SDL_FreeSurface(number);
        }
    }

    if (solver[i] != 0)
    {
        x += 200;
        if (x >= 1800)
        {
            x = 10;
            y += 200;
        }
    }
  }

  SDL_SaveBMP(grid, "output/final/final.bmp");
  SDL_FreeSurface(grid);
}
