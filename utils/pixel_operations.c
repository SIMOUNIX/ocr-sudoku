// Simple get/put pixel for SDL
// Inspired by code from SDL documentation
// (http://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html)

#include <err.h>
#include <math.h>
#include "pixel_operations.h"

static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel) {
	    int i;
	    double x = x1 - x0;
	    double y = y1 - y0;
	    double length = sqrt( x*x + y*y );
	    double addx = x / length;
	    double addy = y / length;
	    x = x0;
	    y = y0;
	    for ( i = 0; i < length; i += 1) {
		if(setPixelVerif(Screen, x, y) == 0)
		{
			put_pixel(Screen, x, y, pixel );
		}
	        x += addx;
	        y += addy;
	         
	}
}

int image_pixel_average(SDL_Surface *image)
{
    int sum_red = 0;
    int sum_green = 0;
    int sum_blue = 0;

    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            uint32_t pixel =  get_pixel(image, x , y);
            uint8_t r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            sum_blue += b;
            sum_green += g;
            sum_red += r;
        }
    }

    int numberPixels = image->h * image->w;
    SDL_UnlockSurface(image);
    return (sum_red / numberPixels + sum_green / numberPixels + sum_blue / numberPixels) / 3;
}

int Truncate(int value)
{
    if (value < 0)
        return 0;

    if (value > 255)
        return 255;
    
    return value;
}

uint8_t Min(uint8_t r, uint8_t g, uint8_t b)
{
    if (r < g)
        if (r < b)
            return r;
    if (g < b)
        return g;
    return b;    
}

int setPixelVerif(SDL_Surface *Screen,int x, int y)
{
  if (x >= 0 && x < Screen->w &&
      y >= 0 && y < Screen->h)
  {
	  return 0;
  }
  return 1;
}

