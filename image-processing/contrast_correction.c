#include "../utils/pixel_operations.h"

void Contrast(SDL_Surface *image)
{
    int c_value = image_pixel_average(image);
    int av = image_pixel_average(image);

    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            Uint8 r, g, b;
            Uint32 pixel = get_pixel(image, x, y);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            double factor = (259 * (c_value + 255)) / (255.0* (259.0 - c_value));

            int new_red = Truncate(factor * (r - av) + av);
            int new_green = Truncate(factor * (g - av) + av);
            int new_blue = Truncate(factor * (b - av) + av);
            int mini = Min(new_red, new_green, new_blue);

            pixel = SDL_MapRGB(image->format, mini, mini, mini);
            put_pixel(image, x, y, pixel);
        }
    }
    SDL_UnlockSurface(image);
    SDL_SaveBMP(image, "output/contrast.bmp");
    SDL_FreeSurface(image);
}