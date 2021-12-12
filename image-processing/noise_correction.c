#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "../utils/pixel_operations.h"
#include "../utils/image.h"

void InsertionSort(Uint32 window[])
{
    int i, j;
    Uint32 temp;
    for (i = 0; i < 25; i++)
    {
        temp = window[i];

        for (j = i - 1; j >= 0 && temp < window[j] ; j--)
            window[j + 1] = window[j];

        window[j + 1] = temp;
    }
}

void MedianFilter(SDL_Surface *image)
{
    int width = image->w;
    int height = image->h;

    Uint32 window[25];

    for (int x = 1; x < width - 1; x++)
    {
        for (int y = 1; y < height - 1; y++)
        {
            window[0] = get_pixel(image, x - 1, y - 1);
            window[1] = get_pixel(image, x, y - 1);
            window[2] = get_pixel(image, x + 1, y - 1);
            window[3] = get_pixel(image, x - 1, y);
            window[4] = get_pixel(image, x, y);
            window[5] = get_pixel(image, x + 1, y);
            window[6] = get_pixel(image, x - 1, y + 1);
            window[7] = get_pixel(image, x, y + 1);
            window[8] = get_pixel(image, x + 1, y + 1);

            InsertionSort(window);

            put_pixel(image, x, y, window[4]);
        }
    }

    SDL_SaveBMP(image, "output/noise.bmp");
    SDL_FreeSurface(image);
}

// Apply the Gaussian matrix to a define position in the image.
void ApplyFilter(SDL_Surface* image, SDL_Surface* destination, int x, int y)
{
    if (x >= 2 && y >= 2 && x < image->w - 2 && y < image->h - 2) // condition to apply filter
    {
        Uint32 pixel_1 = get_pixel(image, x - 2, y - 2); // first row
        Uint32 pixel_2 = get_pixel(image, x - 1, y - 2);
        Uint32 pixel_3 = get_pixel(image, x, y - 2);
        Uint32 pixel_4 = get_pixel(image, x + 1, y - 2);
        Uint32 pixel_5 = get_pixel(image, x + 2, y - 2);
        Uint32 pixel_6 = get_pixel(image, x - 2, y - 1); // second row
        Uint32 pixel_7 = get_pixel(image, x - 1, y - 1);
        Uint32 pixel_8 = get_pixel(image, x, y - 1);
        Uint32 pixel_9 = get_pixel(image, x + 1, y - 1);
        Uint32 pixel_10 = get_pixel(image, x + 2, y - 1);
        Uint32 pixel_11 = get_pixel(image, x - 2, y); // third row
        Uint32 pixel_12 = get_pixel(image, x - 1, y);
        Uint32 pixel_13 = get_pixel(image, x, y);
        Uint32 pixel_14 = get_pixel(image, x + 1, y);
        Uint32 pixel_15 = get_pixel(image, x + 2, y);
        Uint32 pixel_16 = get_pixel(image, x - 2, y + 1); // fourth row
        Uint32 pixel_17 = get_pixel(image, x - 1, y + 1);
        Uint32 pixel_18 = get_pixel(image, x, y + 1);
        Uint32 pixel_19 = get_pixel(image, x + 1, y + 1);
        Uint32 pixel_20 = get_pixel(image, x + 2, y + 1);
        Uint32 pixel_21 = get_pixel(image, x - 2, y + 2); // fifth row
        Uint32 pixel_22 = get_pixel(image, x - 1, y + 2);
        Uint32 pixel_23 = get_pixel(image, x, y + 2);
        Uint32 pixel_24 = get_pixel(image, x + 1, y + 2);
        Uint32 pixel_25 = get_pixel(image, x + 2, y + 2);


        Uint8 r;
        long totr = 0;
        
        SDL_GetRGB(pixel_1, image->format, &r, &r, &r);

        totr += (long) r * 2;

        SDL_GetRGB(pixel_2, image->format, &r, &r, &r);

        totr += (long) r * 4;

        SDL_GetRGB(pixel_3, image->format, &r, &r, &r);

        totr += (long) r * 5;
        
        SDL_GetRGB(pixel_4, image->format, &r, &r, &r);

        totr += (long) r * 4;

        SDL_GetRGB(pixel_5, image->format, &r, &r, &r);

        totr += (long) r * 2;

        SDL_GetRGB(pixel_6, image->format, &r, &r, &r);

        totr += (long) r * 4;

        SDL_GetRGB(pixel_7, image->format, &r, &r, &r);

        totr += (long) r * 9;

        SDL_GetRGB(pixel_8, image->format, &r, &r, &r);

        totr += (long) r * 12;
        
        SDL_GetRGB(pixel_9, image->format, &r, &r, &r);

        totr += (long) r * 9;
        
        SDL_GetRGB(pixel_10, image->format, &r, &r, &r);

        totr += (long) r * 4;
        
        SDL_GetRGB(pixel_11, image->format, &r, &r, &r);

        totr += (long) r * 5;
        
        SDL_GetRGB(pixel_12, image->format, &r, &r, &r);

        totr += (long) r * 12;
        
        SDL_GetRGB(pixel_13, image->format, &r, &r, &r);

        totr += (long) r * 15;
        
        SDL_GetRGB(pixel_14, image->format, &r, &r, &r);

        totr += (long) r * 12;
        
        SDL_GetRGB(pixel_15, image->format, &r, &r, &r);

        totr += (long) r * 5;
        
        SDL_GetRGB(pixel_16, image->format, &r, &r, &r);

        totr += (long) r * 4;
        
        SDL_GetRGB(pixel_17, image->format, &r, &r, &r);

        totr += (long) r * 9;
        
        SDL_GetRGB(pixel_18, image->format, &r, &r, &r);

        totr += (long) r * 12;
        
        SDL_GetRGB(pixel_19, image->format, &r, &r, &r);

        totr += (long) r * 9;
        
        SDL_GetRGB(pixel_20, image->format, &r, &r, &r);

        totr += (long) r * 4;
        
        SDL_GetRGB(pixel_21, image->format, &r, &r, &r);

        totr += (long) r * 2;
        
        SDL_GetRGB(pixel_22, image->format, &r, &r, &r);

        totr += (long) r * 4;
        
        SDL_GetRGB(pixel_23, image->format, &r, &r, &r);

        totr += (long) r * 5;
        
        SDL_GetRGB(pixel_24, image->format, &r, &r, &r);

        totr += (long) r * 4;
        
        SDL_GetRGB(pixel_25, image->format, &r, &r, &r);

        totr += (long) r * 2;

        totr = totr / 159;

        r = (Uint8) totr;

        Uint32 pixel = SDL_MapRGB(destination->format, r, r, r);

        put_pixel(destination, x, y, pixel);
    }
}

// Applys the Gaussian filter to the image.
void Gauss(SDL_Surface* image)
{
    SDL_Surface *destination;

    // printf("Noise removing : applying Gaussian blur...\n");

    destination = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            ApplyFilter(image, destination, i, j);
        }
    }

    if (SDL_SaveBMP(destination, "output/gauss.bmp") != 0) // save the grid in .bmp format
    {
        // Error saving Bitmap
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(destination);
    SDL_FreeSurface(image);
}