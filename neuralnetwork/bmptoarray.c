#include "bmptoarray.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../utils/pixel_operations.h"

float *bmptoarray(SDL_Surface *image)
{
	int width = image->w;
	int height = image->h;

	float *res = malloc(sizeof(float)*height*width);
	memset(res, 0, sizeof(float)*height*width);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Uint32 pixel = get_pixel(image, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image->format, &r, &g, &b);

			if (r > 0)
			{
				res[x + y*width] = 1;
			}
		}
	}

	return res;
}
