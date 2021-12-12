#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "grayscale_correction.h"
#include "../utils/pixel_operations.h"
#include "../utils/image.h"

void Grayscale(SDL_Surface *image)
{
    // Loop to modify the image pixel.
    int width = image->w;
    int height = image->h;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Uint32 pixel = get_pixel(image, x ,y);

            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            Uint8 ave;
            ave = 0.3 * r + 0.59 * g + 0.11 * b;

            Uint32 pixelG = SDL_MapRGB(image->format, ave, ave, ave);

            put_pixel(image, x, y, pixelG);
        }
    }

    SDL_UnlockSurface(image);
    SDL_SaveBMP(image, "output/grayscale.bmp");
    SDL_FreeSurface(image);
}
