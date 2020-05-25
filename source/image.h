#ifndef IMAGE_H
#define IMAGE_H


#pragma pack(push, 1)
struct BitmapHeader
{
    unsigned short fileType;
    unsigned fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned bitmapOffset;
    unsigned size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned compression;
    unsigned sizeOfBitmap;
    int horzResolution;
    int vertResolution;
    unsigned colorsUsed;
    unsigned colorsImportant;
};
#pragma pack(pop)

void writeImage(unsigned width, unsigned height, const unsigned* pixels, const char* filename);


#endif
