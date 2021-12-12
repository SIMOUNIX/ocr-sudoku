#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>
#include <string.h>
#include "nn.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../utils/pixel_operations.h"

#define inputs 784
#define hiddenNodes 15
#define outputs 10

void init_sdl()
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

void toMat(char *img, double arr[])
{
  SDL_Surface* image_surface = load_image(img);
  
  int width = image_surface->w;
  int height = image_surface->h;
  for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width; x++)
	{
	  Uint32 pixel = get_pixel(image_surface, x, y);
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	  if (r == 255)
	    arr[y*width+x] = 1;
	  else
	    arr[y*width+x] = 0;
	}
    }
  SDL_FreeSurface(image_surface);
  
}

int isValid(char *img)
{
  SDL_Surface* image_surface = load_image(img);
  int p = 0;
  int width = image_surface->w;
  int height = image_surface->h;
  for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width; x++)
	{
	  Uint32 pixel = get_pixel(image_surface, x, y);
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	  if (r > 50)
              p++;
	}
    }
  SDL_FreeSurface(image_surface);

  if (p > 12)
      return 1;
  else
      return 0;
}


double relu(double x)
{
  if (x > 0)
    return x;
  else
    return 0.01f*x;
}

int NN(char* img)
{
  double hl[hiddenNodes];
  double ol[outputs] = {0};

  double hlBias[hiddenNodes];
  double olBias[outputs];

  double hw[inputs][hiddenNodes];
  double ow[hiddenNodes][outputs];

  int l = 1;
  
  if (l == 1) // load weights and biases from file
    {
      FILE* fptr;
      fptr = fopen("nn/load_weights.txt", "r");
      
      int j = 0;
      int k = 0;
      double nb;
      while (j < hiddenNodes)
	{

	  int a = fscanf(fptr, "%lf", &nb);
          if (a == -5)
              printf("jsp test\n");
	  if (k == inputs)
	    {
	      k = 0;
	      j++;
	    }
	  hw[k][j] = nb;
	  k++;
	}
      
      j = 0;
      while (j < hiddenNodes)
	{
	  hlBias[j] = nb;
	  int a = fscanf(fptr, "%lf", &nb);
          if (a == -5)
              printf("jsp test\n");
	  j++;
	}
      
      j = 0;
      k = 0;
      while (j < outputs)
	{
	  if (k == hiddenNodes)
	    {
	      k = 0;
	      j++;
	    }
	  ow[k][j] = nb;
	  if (j != outputs-1 || k != hiddenNodes-1)
	    {
	      int a = fscanf(fptr, "%lf", &nb);
              if (a == -5)
                  printf("jsp test\n");
	    }
	  k++;
	}
      
      j = 0;
      while (j < outputs)
	{
	  olBias[j] = nb;
	  int a = fscanf(fptr, "%lf", &nb);
          if (a == -5)
              printf("jsp test\n");
	  j++;
	}
      
      fclose(fptr);
    }
   
  double imgArr[784] = {0};
  toMat(img, imgArr);
  
  for (int i = 0; i < hiddenNodes; i++)
    {
      double actv = hlBias[i];
      for (int k = 0; k < inputs ; k++)
	{
	  actv += imgArr[k] * hw[k][i];
	}
      hl[i] = relu(actv);
    }
  
  double C = ol[0];
  for (int j = 1; j < outputs; j++)
    {
      if (ol[j] > C)
	C = ol[j];
    }
  
  double sumExp = 0.0f;
  for (int j = 0; j < outputs; j++)
    {
      double actv = olBias[j];
      for (int k = 0; k < hiddenNodes; k++)
	actv += hl[k] * ow[k][j];
      double z = actv - C;
      sumExp += exp(z);
      ol[j] = exp(z);
    }
  
  for (int j = 0; j < outputs; j++)
    ol[j] = (ol[j] / (sumExp));
  
  int imax = 0;
  float vmax = 0;
  for (int i = 0; i < 10; i++)
    {
      if (ol[i] > vmax)
	{
	  vmax = ol[i];
	  imax = i;
	}
    }
  
  return imax;	  
}
