#ifndef NOISE_CORRECTION_H
#define NOISE_CORRECTION_H

#include <SDL/SDL.h>

// void noise(SDL_Surface *image, SDL_Surface *image_to_return, int n);
void MedianFilter(SDL_Surface *image);
void Gauss(SDL_Surface *image);

#endif
