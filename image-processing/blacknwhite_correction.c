#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "blacknwhite_correction.h"

#include "../utils/pixel_operations.h"
#include "../utils/image.h"

void Blacknwhite(SDL_Surface *image, int *inf)
{
    // Loop to modify the image pixel.
    int width = image->w;
    int height = image->h;

    SDL_LockSurface(image);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Uint32 pixel = get_pixel(image, x ,y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            Uint8 ave = Min(r, g, b) < *inf ? 255 : 0;

            Uint32 pixelG = SDL_MapRGB(image->format, ave, ave, ave);

            put_pixel(image, x, y, pixelG);
        }
    }

    SDL_UnlockSurface(image);
    SDL_SaveBMP(image, "output/blacknwhite2.bmp");
    SDL_FreeSurface(image);
}

int max1(int m1, int m2)
{
  return (m1 > m2) ? m1 : m2;
}

int min1(int m1, int m2)
{
  return (m1 < m2) ? m1 : m2;
}

void free_array(Uint32 **array, int width)
{
  int i;
  for(i = 0; i < width; i++)
    {
      free(array[i]);
    }
  free(array);
}

void Adaptative(SDL_Surface *image)
{
  SDL_Surface *to_return = NULL;
  to_return = image;

  int width = image->w;
  int height = image->h;

  Uint32 sum = 0;

  Uint32 **int_img;

  int_img = malloc(sizeof(Uint32*)*width);

  for(int i = 0; i < width; i++)
      int_img[i] = malloc(sizeof(Uint32)*height);

  for(int i = 0; i < width; i++)
    {
      sum = 0;
      for(int j = 0; j < height; j++)
	{
	  Uint32 pixel = get_pixel(image, i, j);
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image->format, &r, &g, &b);
	  sum = sum + r;
	  

	  if(i == 0)
	    {
	      int_img[i][j] = sum;
	    }
	  else
	    { 
	      int_img[i][j] = sum + int_img[i - 1][j];
	    }
	}
    }

  int s = width / 8;
  int s2 = s / 2;
  int t = 15;
  Uint32 count = 0;

  for(int i = 0; i < width; i++)
    {
      for(int j = 0; j < height; j++)
	{
	  int y0 = max1(j - s2, 0);
	  int y1 = min1(j + s2, height - 1);
	  int x0 = max1(i - s2, 0);
	  int x1 = min1(i + s2, width - 1);
	  
	  count = (y1 - y0) * (x1 - x0);
	  
	  sum = int_img[x1][y1] - int_img[x1][y0] - int_img[x0][y1] + int_img[x0][y0];

	  //printf("here\n");
	  Uint32 pixelT = get_pixel(image, i, j);
	  Uint8 r, g, b;

	  SDL_GetRGB(pixelT, image -> format, &r, &g, &b);

	  if((r * count) <= (sum * (100 - t) / 100))
	    {
	      Uint32 pixel = SDL_MapRGB(image->format, 255, 255, 255);
	      put_pixel(to_return, i, j, pixel);
	    }

	  else
	    {
	      Uint32 pixel = SDL_MapRGB(image->format, 0, 0, 0);
	      put_pixel(to_return, i, j, pixel);
	    }
	}
	}

  SDL_SaveBMP(to_return, "output/blacknwhite.bmp");
  SDL_FreeSurface(to_return);
  //SDL_FreeSurface(image);

  free_array(int_img, width);
}

unsigned long *get_histogram(SDL_Surface* image_surface, unsigned long *array)
{
  
    int width = image_surface->w;
    int height = image_surface->h;
    for(int i = 0; i < width; i++)
        for(int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 r,g,b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            g=b;
            Uint8 n = r;
            if(array[n] < 4294967295)
                array[n] += 1;
        }
    return array;
}

Uint8 get_threshold(unsigned long *histogram, int total)
{
    unsigned long sum = 0, wf = 0, sumb = 0;
    unsigned long mb, mf;
    float between = 0, max = 0;
    Uint8 threshold1 = 0, threshold2 =0;
    for(int i = 0; i < 256; i++)
        sum += i * histogram[i];
    unsigned long wb = 0;
    for(int i = 0; i < 256; i++)
    {
        wb += histogram[i];
        if(wb == 0)
            continue;
        wf = total - wb;
        if( wf == 0)
            break;
        sumb += i * histogram[i];
        mb = sumb / wb;
        mf = (sum - sumb) / wf;
        between = wb * wf * (mb - mf) * (mb - mf);
        if(between >= max)
        {
            threshold1 = i;
            if(between > max)
                threshold2 = i;
            max = between;
        }
    }
    Uint8 threshold = ((threshold1 + threshold2) / 2);
    //printf("threshold = %u\n", threshold);
    return threshold;
}
void Otsu(SDL_Surface *image_surface, char *path)
{
    int width = image_surface->w;
    int height = image_surface->h;

    unsigned long *hist = malloc(256 * sizeof(unsigned long));
    for(int i = 0; i < 256; i++)
        hist[i] = 0;
    hist = get_histogram(image_surface, hist);
    Uint8 threshold = get_threshold(hist, width*height);
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 r,g,b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            
            if(r > threshold)
                r=g=b=255;
            else
                r=g=b=0;

            pixel = SDL_MapRGB(image_surface->format, r, g, b);
            put_pixel(image_surface, i, j, pixel);
        }
    }
    free(hist);
    SDL_SaveBMP (image_surface ,path);
    SDL_FreeSurface(image_surface);
    
}
