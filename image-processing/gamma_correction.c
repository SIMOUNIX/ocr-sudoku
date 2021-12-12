#include "../utils/pixel_operations.h"

#include <math.h>

void Gamma(SDL_Surface *image)
{
    float gamma_correction = 2.0f / (float) (255 - image_pixel_average(image));

    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            uint32_t pixel = get_pixel(image, x, y);
            uint8_t r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            int new_red = Truncate(pow((float) (r / 255.0), gamma_correction) * 255.0f);
            int new_green = Truncate(pow((float) (g / 255.0), gamma_correction) * 255.0f);
            int new_blue = Truncate(pow((float) (b / 255.0), gamma_correction) * 255.0f);

            pixel = SDL_MapRGB(image->format, new_red, new_green, new_blue);
            put_pixel(image, x, y, pixel);
        }
    }
    
    SDL_UnlockSurface(image);
    SDL_SaveBMP(image, "output/gamma.bmp");
    SDL_FreeSurface(image);
}