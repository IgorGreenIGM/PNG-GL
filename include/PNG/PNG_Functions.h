#ifndef PNGFUNCTIONS_H_INCLUDED
#define PNGFUNCTIONS_H_INCLUDED

void saveIEND(FILE *fp, CHUNK_IEND *IEND);

void saveIDAT(FILE *fp, CHUNK_IDAT *IDAT, unsigned char *pixels, int s_width, int s_height, int colorChannel);

void saveIHDR(FILE *fp, CHUNK_IHDR *IHDR, int s_width, int s_height, int bitDepth, int colorChannel);

void savepHYs(FILE *fp, CHUNK_pHYs *pHYs, int s_width, int s_height);

void PNG_HEADER_initialize(PNG *myPNG);

#endif // PNGFUNCTIONS_H_INCLUDED