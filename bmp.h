
struct Pixel
{
    char bgr[3];
};

void createBmpFile(int imageW, int imageH, struct Pixel pixels[imageH][imageW]);
