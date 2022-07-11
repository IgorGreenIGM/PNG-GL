#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "../../include/PNG/PNG_Structures.h"
#include "../../include/PNG/PNG_Functions.h"

void flipPixels(unsigned char *pixelsArray, int s_width, int s_height, int colorChannels); // the fliping glreadpixels function

/**
 * @brief opengl screenshot function
 *
 * @param _pngDirectory the png output directory
 * @param  x the absciss point for start screenshot. 0 is the bottom left point
 * @param  y the ord point for start screenshot. 0 is the bottom left point
 * @param width the screenshot width
 * @param height the screenshot height
 * @param colorChannel the output color channel, only availble is 3 (RGB)
 * @param bitDepth     bit depth, the only avaible is 8
 * @param colorMode     the png color mode, only avaible is 2 (RGB true color)
 */
void glScreenshotToPng(const char *_pngDirectory, int x, int y, int width, int height, int colorChannel, int bitDepth, int colorMode)
{
    FILE *fp = fopen(_pngDirectory, "wb+");     // Opening the file in binary mode
    PNG *myPNG = (PNG *)malloc(sizeof(*myPNG)); // mng structure allocation
    PNG_HEADER_initialize(myPNG);               // we initialize the png header

    int s_width = width - x, s_height = height - y;             //setting the image size

    int pixelArraySize = s_width * s_height * colorChannel;
    unsigned char *pixelsArray = (unsigned char *)malloc(pixelArraySize); // memory allocation of the pixel Array buffer

    glPixelStorei(GL_PACK_ALIGNMENT, 1); // reading the pixels to screen and storing it in the pixels buffer
    glReadBuffer(GL_FRONT);
    glReadPixels(x, y, (unsigned short)s_width, (unsigned short)s_height, GL_RGB, GL_UNSIGNED_BYTE, pixelsArray);

    // because the opengl pixel storing is only compatible with bottom-left format, we need to store flip it to top-left format
    flipPixels(pixelsArray, s_width, s_height, colorChannel);

    // then we write the png header and the chunks into the file stream
    fwrite(myPNG->signature, 8, 1, fp);
    saveIHDR(fp, &(myPNG->IHDR), s_width, s_height, bitDepth, colorMode);
    savepHYs(fp, &(myPNG->pHYs), s_width, s_height);
    saveIDAT(fp, &(myPNG->IDAT), pixelsArray, s_width, s_height, colorChannel);
    saveIEND(fp, &(myPNG->IEND));
    free(pixelsArray);
    free(myPNG->signature);
    free(myPNG); // Freeing the allocated memory
    fclose(fp);  //closing the output png stream
}

/// Flipping opengl read pixels
void flipPixels(unsigned char *pixelsArray, int s_width, int s_heigth, int colorChannel)
{
    int totalLength = s_width * s_heigth * colorChannel;
    int oneLineLength = s_width * colorChannel;
    unsigned char *tmp = (unsigned char *)malloc(totalLength * sizeof(unsigned char));
    memcpy(tmp, pixelsArray, totalLength);
    for (int i = 0; i < s_heigth; i++)
        memcpy(pixelsArray + oneLineLength * i, tmp + totalLength - oneLineLength * (i + 1), oneLineLength);
    free(tmp);
}