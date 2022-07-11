#ifndef BINAIRIES_OPERATIONS_H_INCLUDED
#define BINAIRIES_OPERATIONS_H_INCLUDED

void fwriteUnsignedLong(FILE *fp, unsigned long nbre);

unsigned char *concat2Tables(unsigned char *t1, unsigned char *t2, int size1, int size2);

unsigned char *int_to_unsignedCharArray(unsigned long nombre);

unsigned char *invertArray(unsigned char *myArray, int end);

unsigned char *deflateDatas(unsigned char *pixels, int s_width, int s_height, int colorChannel, int *deflatedLen);

#endif // BINAIRIES_OPERATIONS_H_INCLUDED
