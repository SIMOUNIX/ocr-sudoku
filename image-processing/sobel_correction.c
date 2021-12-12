#include <err.h>
#include <stdio.h>
#include <math.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../utils/pixel_operations.h"
#include "sobel_correction.h"


// returns the value of the Gx pixel (to put onto gx surface at [x; y])
int Get_Gx(SDL_Surface* image, int x, int y)
{
    Uint32 pixel_1 = get_pixel(image, x - 1, y - 1);
    Uint32 pixel_2 = get_pixel(image, x - 1, y);
    Uint32 pixel_3 = get_pixel(image, x - 1, y + 1);
    Uint32 pixel_4 = get_pixel(image, x + 1, y - 1);
    Uint32 pixel_5 = get_pixel(image, x + 1, y);
    Uint32 pixel_6 = get_pixel(image, x + 1, y + 1);

    /*
    [1][0][4]                           [-1][0][1]
    [2][0][5]   the pixel references    [-2][0][2]
    [3][0][6]                           [-1][0][1]
    */

    Uint8 a, b, c, d, e, f;

    int tot = 0;

    SDL_GetRGB(pixel_1, image->format, &a, &a, &a);
    SDL_GetRGB(pixel_2, image->format, &b, &b, &b);
    SDL_GetRGB(pixel_3, image->format, &c, &c, &c);
    SDL_GetRGB(pixel_4, image->format, &d, &d, &d);
    SDL_GetRGB(pixel_5, image->format, &e, &e, &e);
    SDL_GetRGB(pixel_6, image->format, &f, &f, &f);

    tot = d + (e*2) + f - a -c - (b*2);

    return tot;
}

// returns the value of the Gy pixel (to put onto gy surface at [x; y])
int Get_Gy(SDL_Surface* image, int x, int y)
{
    Uint32 pixel_1 = get_pixel(image, x - 1, y - 1);
    Uint32 pixel_2 = get_pixel(image, x, y - 1);
    Uint32 pixel_3 = get_pixel(image, x + 1, y - 1);
    Uint32 pixel_4 = get_pixel(image, x - 1, y + 1);
    Uint32 pixel_5 = get_pixel(image, x, y + 1);
    Uint32 pixel_6 = get_pixel(image, x + 1, y + 1);

    /*
    [1][2][3]                           [-1][-2][-1]
    [0][0][0]   the pixel references    [0] [0] [0]
    [4][5][6]                           [1] [2] [1]
    */

    Uint8 a, b, c, d, e, f;

    int tot = 0;

    SDL_GetRGB(pixel_1, image->format, &a, &a, &a);
    SDL_GetRGB(pixel_2, image->format, &b, &b, &b);
    SDL_GetRGB(pixel_3, image->format, &c, &c, &c);
    SDL_GetRGB(pixel_4, image->format, &d, &d, &d);
    SDL_GetRGB(pixel_5, image->format, &e, &e, &e);
    SDL_GetRGB(pixel_6, image->format, &f, &f, &f);

    tot = d + (e*2) + f - a -c - (b*2);

    return tot;
}

// compute and return the gradiant value of the pixel (to put onto return matrix in Sobel())
double get_edge_direction(double x, double y)
{
    double thisAngle = atan2(y, x) * 180.0/3.14159;    // Calculate actual direction of edge

	/* Convert actual edge direction to approximate value */
	if ( ( (thisAngle < 22.5) && (thisAngle > -22.5) ) || (thisAngle > 157.5) || (thisAngle < -157.5) )
		thisAngle = 0;
	if ( ( (thisAngle > 22.5) && (thisAngle < 67.5) ) || ( (thisAngle < -112.5) && (thisAngle > -157.5) ) )
		thisAngle = 45;
	if ( ( (thisAngle > 67.5) && (thisAngle < 112.5) ) || ( (thisAngle < -67.5) && (thisAngle > -112.5) ) )
		thisAngle = 90;
	if ( ( (thisAngle > 112.5) && (thisAngle < 157.5) ) || ( (thisAngle < -22.5) && (thisAngle > -67.5) ) )
		thisAngle = 135;
    return thisAngle;
}

// Applies Sobel filter onto the image and returns gradiant matrix
void Sobel(SDL_Surface *image, double **M)
{
    // SDL_Surface* image = display_bmp(path);
    
    SDL_Surface *destination;

    destination = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0); 

    // printf("Sobel : applying Sobel filter...\n");

    int x;
    int y;

    for (int i = 1; i < image->w - 1; i++) // compute Sobel image and matrix
    {
        for (int j = 1; j < image->h - 1; j++)
        {
            x = Get_Gx(image, i, j);
            y = Get_Gy(image, i, j);
            M[i][j] = get_edge_direction(x,y); // fill the matrix with the direction

            int value =(int) sqrt(pow(x,2.0) + pow(y,2.0));
            Uint32 pixel = SDL_MapRGB(image->format, value, value, value);

            put_pixel(destination, i, j, pixel); // apply the Sobel filter
        }
    }

    if (SDL_SaveBMP(destination, "output/sobel.bmp") != 0) // save the grid in .bmp format
    {
        // Error saving Bitmap
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }

    // printf("Sobel : Image saved!\n");

    SDL_FreeSurface(image);
    SDL_FreeSurface(destination);
    //return M;
}
