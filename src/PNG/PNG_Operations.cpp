/**
 * @file PNG_Operations.cpp
 * @author Mogou Igor Green (igormogou86@gmail.com)
 * @brief PNG Operations functions, most of these are used to fix endianess of png files and deflate IDAT chunk datas
 * @date 2022-07-09
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "../../include/zlib/zlib.h"

// convert an array of base 2 number stored in an array to an unsigned long in base 10
unsigned long Base2_to_Base10(char *b2_array, int len)
{
    unsigned long result = 0;
    for (int i = 0; i <= len; i++)
    {
        char thisNumber[1] = {b2_array[i]};
        result += atoi(thisNumber) * (pow(2, len - i));
    }
    return result;
}

// function for converting the truncating of a binary array and return an unsigned long decimal(base 10)
// start and end are included in the binary array
unsigned long slide_atoi(char *b2_array, int start, int end)
{
    char *b10_array = (char *)malloc((end - start + 1) * sizeof(char));
    for (int i = 0, k = start; i <= end - start; i++, k++)
        b10_array[i] = b2_array[k];
    return Base2_to_Base10(b10_array, end - start - 1);
}

// function which for an integer, return an array of his coding in 4 bytes
//  example : x=340 -> array[4] = {0, 0, 1, 84}
unsigned char *int_to_unsignedCharArray(unsigned long number)
{
    unsigned char *unsignedCharArray = (unsigned char *)malloc(sizeof(4 * sizeof(unsigned char)));

    if (number == 255) // we deal with the limiting case
    {
        for (int i = 0, k = 0; i < 4; i++, k += 8)
            unsignedCharArray[i] = 1;
    }
    else
    {
        char buff[32];         // 1 int = 4*byte = 32 bits
        ltoa(number, buff, 2); // we convert in binary the number
        char _32bytes_Table[32];

        for (int i = 0; i < 32; i++) // then we memset the array to 0
            _32bytes_Table[i] = 0;

        for (int k = 0, i = 32 - strlen(buff); i < 32; i++, k++) // we copy the binaries values in the array
            _32bytes_Table[i] = buff[k];

        for (int i = 0, k = 0; i < 4; i++, k += 8) // we store the result in an unsigned int array
            unsignedCharArray[i] = slide_atoi(_32bytes_Table, k, k + 8);
    }
    return unsignedCharArray; // we return the array
}

// concatenation function of 2 arrays in one
unsigned char *concat2Tables(unsigned char *t1, unsigned char *t2, int size1, int size2)
{
    unsigned char *table = (unsigned char *)malloc(size1 + size2);
    int i = 0, j = 0, k = 0;
    for (i = 0; i < size1; i++)
    {
        table[i] = t1[i];
    }
    for (j = i; j < size1 + size2; j++)
    {
        table[j] = t2[k];
        k++;
    }
    return table; // we return the concatened array
}

// array inversion function
unsigned char *invertArray(unsigned char *myArray, int end)
{
    unsigned char *invertedtableau = (unsigned char *)malloc(end * sizeof(unsigned char));
    for (int i = 0; i <= end; i++)
        invertedtableau[i] = myArray[end - i];
    return invertedtableau; // we return the inverted array
}

// function for writing an unsigned long in a file in the inverted order (fixing endianess)
//  see endian representation on Wikipedia for more informations
void fwriteUnsignedLong(FILE *fp, unsigned long number)
{
    unsigned char myArray[32];   // we define an array of 4*8=32bits
    memcpy(myArray, &number, 4); // we coype the number in the array

    unsigned char *invertedArray = invertArray(myArray, 3); // we invert the bytes
    realloc(invertedArray, 32);                             // we realloc memory in the inverted array

    for (int i = 4; i < 32; i++) // we set all values in the array to 0
        invertedArray[i] = 0;

    fwrite(invertedArray, 4, 1, fp); // we write files in the specified file
}

// generating scanline function
unsigned char *generateScanlines(unsigned char *pixels, int s_width, int s_height, int colorChannel)
{
    int eachScanlineLength = 1 + s_width * colorChannel, i = 1, j = 0;                            // one scanline length
    unsigned char *scanlines = (unsigned char *)malloc(s_height * eachScanlineLength); // memory allocation for the scanline output

    memset(scanlines, 0, s_height * eachScanlineLength * sizeof(char)); // we set all the output values to 0

    // then we copy pixels elements in the output, skipping the fisrt output values, that should ever be 0
    for (i = 1, j = 0; i < s_height && j < s_height; i++, j++)
        memcpy(scanlines + 1 + (i - 1) * eachScanlineLength, pixels + j * (eachScanlineLength - 1), eachScanlineLength - 1);

    memcpy(scanlines + 1 + (i - 1) * eachScanlineLength, pixels + j * (eachScanlineLength - 1), eachScanlineLength - 1);

    return scanlines;
}

// deflating IDAT CHUNK data algorithm
unsigned char *deflateDatas(unsigned char *pixels, int s_width, int s_height, int colorChannel, int *deflatedLen)
{
    unsigned long inLen = s_height * (1 + s_width * colorChannel), tmpLen = 0;          // input len of scanlines datas
    unsigned char *scanlines = generateScanlines(pixels, s_width, s_height, colorChannel); // generating scanlines from the pixels

    unsigned char *deflatedDatas = NULL; // setting up the deflated datas output
    int result = 0;

    // initialising zlib
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = inLen;
    defstream.next_in = (Bytef *)scanlines;
    defstream.avail_out = 0;
    defstream.next_out = (Bytef *)deflatedDatas;

    if ((result = deflateInit(&defstream, Z_DEFAULT_COMPRESSION)) == Z_OK)
    {
        // calculate the actual length and update zlib structure
        unsigned long estimateLen = deflateBound(&defstream, inLen);
        deflatedDatas = (unsigned char *)malloc(estimateLen);
        if (deflatedDatas != NULL)
        {
            // updation zlib configuration
            defstream.avail_out = (uInt)estimateLen;
            defstream.next_out = (Bytef *)deflatedDatas;

            // do the compression
            deflate(&defstream, Z_FINISH);
            tmpLen = (unsigned char *)defstream.next_out - deflatedDatas;
        }
    }
    deflateEnd(&defstream);       // end of deflating algorithm
    *deflatedLen = tmpLen; // copying the defalted data length to the IDAT->length
    free(scanlines);

    return deflatedDatas;
}
