#ifndef IMAGE_H
#define IMAGE_H

#include <SDL/SDL.h>

struct Pixel
{
    int r, g, b;
};

typedef struct Pixel Pixel;

struct Image
{
    unsigned int height;
    unsigned int width;
    Pixel** pixel;
    char* path; 
    SDL_PixelFormat* format;
};

typedef struct Image Image;

void newImage(Image *image);

void freeImage(Image *image);

void SDL_FreeSurface(SDL_Surface *surface);

SDL_Surface* loadImage(char *path);

SDL_Surface* display_image(SDL_Surface *img);

void wait_for_keypressed();

#endif
