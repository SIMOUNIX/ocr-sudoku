#include "image.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <err.h>



SDL_Surface* loadImage(char *path)
{
    SDL_Surface *img;

    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());
    
    return img;
}

/*void newImage(Image *image)
{
    SDL_Surface *surface = loadImage(image->path);

    image->width = surface->w;
    image->height = surface->h;

    image->pixel = malloc((image->width + 1) * sizeof(Pixel*));

    for (size_t i = 0; i < image->width; i++)
    {
        image->pixel[i] = malloc((image->height + 1) * sizeof(Pixel));
    }
}*/

/*void freeImage(Image *image)
{
    for (size_t i = 0; i < image->width; i++)
    {
        free(image->pixel[i]);
    }

    free(image->pixel);
}*/

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

