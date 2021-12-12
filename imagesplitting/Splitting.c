#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string.h>
#include <math.h>
#include "../utils/image.h"
#include "../utils/pixel_operations.h"
#include "Splitting.h"
#include "SDL/SDL_rotozoom.h"
#include <sys/types.h>
#include <sys/stat.h>

void clean(SDL_Surface* image)
{
    //__clean(image);
    int limitw = image->w/7;
    int limith = image->h/7;
    Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);

    // up clean

    for (int i = 0; i < (limith * 7)/5; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            put_pixel(image, j, i, white);
        }
    }
    
    // down clean

    for (int i = image->h - limith; i < image->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            put_pixel(image, j, i, white);
        }
    }

    // left clean

    for (int i = 0; i < image->h; i++)
    {
        for (int j = 0; j < (limitw * 7)/5; j++)
        {
            put_pixel(image, j, i, white);
        }
    }
    
    // right clean

    for (int i = 0; i < image->h; i++)
    {
        for (int j = image->w - limitw; j < image->w; j++)
        {
            put_pixel(image, j, i, white);   
        }
    }
}



SDL_Surface* move(SDL_Surface* image, int decalagex, int decalagey)
{
    Uint32 pixel;
    Uint8 r1,g1,b1;
    SDL_Surface* centered = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r1,&g1,&b1);

            if (r1 == 255)
            {
                if (i - decalagex > 0 && j - decalagey && i - decalagex < 28 && j - decalagey < 28)
                {
                    put_pixel(centered, i - decalagex, j - decalagey, pixel);
                }
            }
        }
    }

    return centered;
}



SDL_Surface* center(SDL_Surface* image)
{
    int totx = 0;
    int toty = 0;
    int numberpixel = 0;
    SDL_Surface* BlackSurface = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
    
    Uint32 pixel;
    Uint8 r,g,b;

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r,&g,&b);

            if (r == 255)
            {
                totx += i;
                toty += j;
                numberpixel++;   
            }
        }
    }

    if (numberpixel != 0)
    {
        float indicatorx = ((float)totx/numberpixel) - 14;
        float indicatory = ((float)toty/numberpixel) - 14;

        return move(image, (int) floor(indicatorx), (int) floor(indicatory));
    }
    else
    {
        return BlackSurface;
    }
}

void invert (SDL_Surface* image)
{
    Uint32 black = SDL_MapRGB(image->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);

    Uint32 pixel;
    Uint8 r,g,b;

    for (int i = 0; i < image->w; i++)
    {
        for (int j = 0; j < image->h; j++)
        {
            pixel = get_pixel(image,i,j);
            SDL_GetRGB(pixel, image->format, &r,&g,&b);

            if (r > 100)
            {
                put_pixel(image,i,j,black);
            }
            else
            {
                put_pixel(image,i,j,white);
            }      
        }
    }
    
}




void splitting(char* path)
{
	SDL_Surface* image = loadImage(path);
	int x = image->w;
	int y = image->h;
	//char* dos = remove_end_str(path,5);
	_mkdir("output/split");
	int e = 0;
	for(int i = 0; i < 9; i += 1)
	{
		for(int j = 0; j < 9; j += 1)
		{
			
			SDL_Surface* imagedest = SDL_CreateRGBSurface(0,x/9,y/9,32,0,0,0,0);
			SDL_Rect leftR = {i * x/ 9,j * y/9, x/9, y/9};
			double zoomx = ((double) 28) / (x/9);
           		double zoomy = ((double) 28) / (y/9);
			SDL_BlitSurface(image,&leftR,imagedest,NULL);
			clean(imagedest);
			SDL_Surface* UwU = rotozoomSurfaceXY(imagedest, 0, zoomx, zoomy,0);
			invert(UwU);
			SDL_Surface* finish = center(UwU);
			char file_name[200];
			snprintf(file_name, 200, "output/split/%i.png",e);
			e += 1;
			SDL_SaveBMP(finish,file_name);
			SDL_FreeSurface(finish);
			SDL_FreeSurface(UwU);
			SDL_FreeSurface(imagedest);
		}
	}
	SDL_FreeSurface(image);
}

char *remove_end_str(char *input, int to_remove)
{
    int len = strlen(input);
    int new_len = len - to_remove;
    char *output = malloc(new_len * sizeof(char) + 1);

    for (int i = 0; i < new_len; i++)
        output[i] = input[i];

    return input;
}

void _mkdir(const char *dir)
{
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/')
        {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);
}
