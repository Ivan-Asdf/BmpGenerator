#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

#pragma pack(push, 1)
struct Header // 12bytes
{
    char sign[2]; //"BM"
    unsigned int filesize;
    int reserved; // 0
    unsigned int pixelArrayStart;
};

struct DibHeader
{
    unsigned int headerSize; // 40bytes
    unsigned int widthPixels;
    unsigned int heightPixels;
    unsigned short colorPlanes; //1
    unsigned short bitsPerPixel;//24
    unsigned int compressionMethod;//0
    unsigned int rawBitmapSize;
    int hRes;//2835
    int vRes;//2835
    unsigned int nColor;//0
    unsigned int vipNColor;//0
};
#pragma pack(pop)


void initHeaders(int width, int height, struct Header* h, struct DibHeader* dH) {
    int sizeH = sizeof(struct Header);
    int sizeDH = sizeof(struct DibHeader);

    memcpy(h->sign, "BM", 2);
    // filesize assigned below
    h->reserved = 0;
    h->pixelArrayStart = sizeH + sizeDH;

    dH->headerSize = sizeDH; 
    dH->widthPixels = width;
    dH->heightPixels = height;
    dH->colorPlanes = 1;
    dH->bitsPerPixel = 24;
    dH->compressionMethod = 0;
    // rawBitmapSize assigned below
    dH->hRes = 2835;
    dH->vRes = 2835;
    dH->nColor = 0;
    dH->vipNColor = 0;

    int rowWidth = width * 3 + (4 - (width * 3) % 4);
    dH->rawBitmapSize = rowWidth * height;
    h->filesize = sizeH + sizeDH + dH->rawBitmapSize;
}

unsigned char* generateRawBitmap(int width, int height, struct Pixel pixels[height][width])
{
    int rowSize = width * 3 + ((4 - (height * 3) % 4) % 4);
    char* bitmap = malloc(height * rowSize);

    for (int h = 0; h < width; ++h) {
        for (int w = 0; w < height; ++w) {
            int bitmapIndex = h * rowSize + w*3;
            memcpy(&bitmap[bitmapIndex], &pixels[h][w], 3);
        }
    }

    return bitmap;
}

void createBmpFile(int imageW, int imageH, struct Pixel pixels[imageH][imageW])
{
    FILE* file;
    file = fopen("image2.bmp", "w+");

    struct Header h;
    struct DibHeader dH;
    initHeaders(imageW, imageH, &h, &dH);
    fwrite(&h, sizeof(struct Header), 1, file);
    fwrite(&dH, sizeof(struct DibHeader), 1, file);

    unsigned char* bitmap = generateRawBitmap(imageW, imageH, pixels);
    int bytesWritten = fwrite(bitmap, dH.rawBitmapSize, 1, file);

    fclose(file);
}




