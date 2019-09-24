#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"


struct Gradient
{
    int strength;
    float bgr[3];
    int startX, startY;
    int endX, endY;
};

int isNumInRange(int num, int x1, int x2);

int isPointInside(int xp, int yp, int x1, int y1, int x2, int y2)
{
    return isNumInRange(xp, x1, x2) && isNumInRange(yp, y1, y2);
}

int isNumInRange(int num, int x1, int x2) 
{
 	return ((x2 < num && num < x1) ||
		 	(x1 < num && num < x2));
}

void applyGradient(int imageW, int imageH, struct Pixel pixels [imageH][imageW], struct Gradient* g)
{
    double factor = 127.0/imageW;
    for(int h = 0; h < imageH; ++h)
    {
        for (int w = 0; w < imageW; ++w)
        {
            if (isPointInside(w, h, g->startX, g->startY, g->endX, g->endY))
            {
                int deltaX = abs(w - g->startX);
                int deltaY = abs(h - g->startY);
                pixels[h][w].bgr[0] += (deltaX + deltaY) * factor * g->strength * g->bgr[0];
                pixels[h][w].bgr[1] += (deltaX + deltaY) * factor * g->strength * g->bgr[1];
                pixels[h][w].bgr[2] += (deltaX + deltaY) * factor * g->strength * g->bgr[2];
            }
        }
    }
}

int main()
{
    int imageW = 1000;
    int imageH = 1000;

    struct Pixel pixels[imageH][imageW];

    struct Gradient g = {1, {1, 0.5, 0}, 800, 800, 200, 200};
    struct Gradient g1 = {1, {0, 0, 1}, 0, 0, 1000, 1000};

    applyGradient(imageW, imageH, pixels, &g);
    applyGradient(imageW, imageH, pixels, &g1);

    createBmpFile(imageW, imageH, pixels);
}
