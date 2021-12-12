#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include "grayscale_correction.h"
#include "../utils/pixel_operations.h"
#include "../utils/image.h"
#include "image_rotation.h"




void ManualRotate(char *path, double angle)
{
    SDL_Surface *image = loadImage(path);
  SDL_Surface *rotation = NULL;

  rotation = rotozoomSurface(image, angle, 1.0, 2);

  SDL_SaveBMP(rotation, "output/rotate.bmp");
  SDL_FreeSurface(image);
  SDL_FreeSurface(rotation);
}

int AutoRotate(char *path)
{
    SDL_Surface *image = loadImage(path);
    int width = image->w;
    int height = image->h;
    int diagonale = floor(sqrt((double) (width * width + height * height)));

    // there will be 180 teta since we go from 0 include to 180 excluded
    // there will be diagonale rho
    int A[180][diagonale];
    for (int i = 0; i < diagonale; i++)
    {
        for (int j = 0; j < 180; j++)
        {
            A[j][i] = 0;
        }
    }
    

    for (double x = 10; x < width-10; x++)
    {
        for (double y = 10; y < height-10; y++)
        {
            Uint32 pixel = get_pixel(image, floor(x), floor(y));
            Uint8 r,g,b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            if (r > 250)
            {
                for (int teta = 0; teta < 180; teta++)
                {
                	double tetaRad = teta * (3.141592653589 / 180);
                        int rho = abs((int) floor(x * cos(tetaRad) + y * sin(tetaRad)));
                        A[teta][rho] += 1;
                }
            }
        }
    }
    int maxHorizontal = 0;
    // int maxVertical = 45;
    int max1 = 0;
    // int max2 = 0;
    for(int i = 0; i < 180; i++)
    {
	    int tmp = 0;
	    if(i < 45 || i > 135)
	    {
		for(int j = 0; j < diagonale;j++)
		{
			if(A[i][j] >= 500)
			{
				tmp += 1;
				
			}
		}
		if(max1 < tmp)
		{
			max1 = tmp;
			maxHorizontal = i;
		}
		//printf("%i\n",tmp);
	    }
    }
    //A[43][255] = 124;
    // printf("A : %i\n",A[43][255]);
    // printf("A : %i\n",A[44][255]);
    SDL_FreeSurface(image);
    // printf("%i",maxHorizontal);
    ManualRotate(path, maxHorizontal);
    return maxHorizontal;
}
