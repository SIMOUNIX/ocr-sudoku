#ifndef BLACKNWHITE_CORRECTION_H
#define BLACKNWHITE_CORRECTION_H

#include <SDL/SDL.h>

void Blacknwhite(SDL_Surface *image, int *inf);
void Adaptative(SDL_Surface *image);
void Otsu(SDL_Surface* image_surface, char *path);

#endif
