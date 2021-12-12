#ifndef PIXEL_OPERATIONS_H_
#define PIXEL_OPERATIONS_H_

#include <stdlib.h>
#include <SDL/SDL.h>

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);

int image_pixel_average(SDL_Surface *image);
int Truncate(int value);
uint8_t Min(uint8_t r, uint8_t g, uint8_t b);

void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel);
int setPixelVerif(SDL_Surface *Screen,int x, int y);

#endif
