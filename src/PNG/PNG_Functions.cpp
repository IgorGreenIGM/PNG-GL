#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/PNG/PNG_Structures.h"
#include "../../include/PNG/PNG_Functions.h"
#include "../../include/PNG/PNG_Operations.h"

unsigned long crc(unsigned char *dataChunk, int len);                                       //crc calculation signature function


///IHDR CHUNK initialization function
void IHDR_CHUNK_initialize(CHUNK_IHDR *IHDR, int s_width, int s_height, int bitDepth, int colorMode)
{
    IHDR->length = 13;                                              //setting the IHDR data length

    IHDR->type = (unsigned char*)malloc(4*sizeof(unsigned char));   //setting the IHDR type (IHDR in hexadecimal)
    IHDR->type[0] = 73;
    IHDR->type[1] = 72;
    IHDR->type[2] = 68;
    IHDR->type[3] = 82;

    IHDR->width = s_width;                                          //setting the PNG width and height according to the screen
    IHDR->height = s_height;

    IHDR->data = (unsigned char*)malloc(5*sizeof(unsigned char));
    IHDR->data[0] = bitDepth;                                       //bit Depth
    IHDR->data[1] = colorMode;                                      //color mode (2) true color : the only that this plugin can manage
    IHDR->data[2] = 0;                                              //compression method (always 0, Deflate Algorithm)
    IHDR->data[3] = 0;                                              //filter method (0)
    IHDR->data[4] = 0;                                              //interlacing method (0)
}

///pHYs CHUNK initialization function
void pHYs_CHUNK_initialize(CHUNK_pHYs *pHYs, int s_width, int s_height)
{
    pHYs->length = 9;                                                   //setting the pHYs data length   

    pHYs->type = (unsigned char*)malloc(4*sizeof(unsigned char));       //setting the pHYs type (pHYs in Hexadecimal)
    pHYs->type[0] = 0x70;
    pHYs->type[1] = 0x48;
    pHYs->type[2] = 0x59;
    pHYs->type[3] = 0x73;           

    pHYs->_ppuX = s_width;                                              //setting the physical pixels dimensions
    pHYs->_ppuY = s_height;

    pHYs->unitSpecifier = (unsigned char*)malloc(1*sizeof(unsigned char));  //setting the unit specifier
    pHYs->unitSpecifier[0] = 0;
}

///IDAT CHUNK initialization function
void IDAT_CHUNK_initialize(CHUNK_IDAT *IDAT, unsigned char *pixels, int s_width, int s_height, int colorChannel)
{
    IDAT->type = (unsigned char*)malloc(4*sizeof(unsigned char));       //setting the IDAT type (IDAT in Hexadecimal)
    IDAT->type[0] = 73;
    IDAT->type[1] = 68;
    IDAT->type[2] = 65;
    IDAT->type[3] = 84;

    IDAT->data = deflateDatas(pixels, s_width, s_height, colorChannel, &(IDAT->length));      //getting the deflated data output
}

///IEND CHUNK initialization function
void IEND_CHUNK_initialize(CHUNK_IEND *IEND)
{
    IEND->length = 0;                                                           //setting the IEND data length

    IEND->type = (unsigned char*)malloc(4*sizeof(unsigned char));               //setting the IEND type (IEND in Hexadecimal)
    IEND->type[0] = 73;
    IEND->type[1] = 69;
    IEND->type[2] = 78;
    IEND->type[3] = 68;
}


///IHDR CHUNK saving function
void saveIHDR(FILE *fp, CHUNK_IHDR *IHDR, int s_width, int s_height, int bitDepth, int colorMode)
{
    IHDR_CHUNK_initialize(IHDR, s_width, s_height, bitDepth, colorMode);       //we init the chunk

    //then we write the type, the length, the screen dimensions and the chunk datas
    fwrite(int_to_unsignedCharArray(IHDR->length), 4, 1, fp);                         
    fwrite(IHDR->type, 4, 1, fp);                                          
    fwrite(int_to_unsignedCharArray(IHDR->width), 4, 1, fp);                    
    fwrite(int_to_unsignedCharArray(IHDR->height), 4, 1, fp);                       
    fwrite(IHDR->data, 5, 1, fp);                                                   

    //the crc needs the concatened array of the chunk type and the chunk datas
    unsigned char *dataCRC = concat2Tables(concat2Tables(IHDR->type, int_to_unsignedCharArray(IHDR->width), 4, 4),
                                           concat2Tables(int_to_unsignedCharArray(IHDR->height), IHDR->data,4,5), 8, 9);

    fwriteUnsignedLong(fp, crc(dataCRC, 17));                                      //we calculate the crc and write it into the file
}

///pHYs CHUNK saving function
void savepHYs(FILE *fp, CHUNK_pHYs *pHYs, int s_width, int s_height)
{
    pHYs_CHUNK_initialize(pHYs, s_width, s_height);         //we init the chunk

    //then we write the type, the length, the screen dimensions and the chunk datas
    fwrite(int_to_unsignedCharArray(pHYs->length), 4, 1, fp);
    fwrite(pHYs->type, 4, 1, fp);
    fwrite(int_to_unsignedCharArray(pHYs->_ppuX), 4, 1, fp);
    fwrite(int_to_unsignedCharArray(pHYs->_ppuY), 4, 1, fp);
    fwrite(pHYs->unitSpecifier, 1, 1, fp);

    //the crc needs the concatened array of the chunk type and the chunk datas
    unsigned char *dataCRC = concat2Tables(concat2Tables(pHYs->type, int_to_unsignedCharArray(pHYs->_ppuX),4, 4),
                                           concat2Tables(int_to_unsignedCharArray(pHYs->_ppuY), pHYs->unitSpecifier, 4, 1), 8, 5);

    fwriteUnsignedLong(fp, crc(dataCRC, 13));   //we calculate the crc and write it into the file
}

///IDAT CHUNK saving function
void saveIDAT(FILE *fp, CHUNK_IDAT *IDAT, unsigned char *pixels, int s_width, int s_height, int colorChannel)
{
    IDAT_CHUNK_initialize(IDAT, pixels, s_width, s_height, colorChannel);    //we init the chunk

    //then we write the type, the length, the screen dimensions and the chunk datas
    fwrite(int_to_unsignedCharArray(IDAT->length), 4, 1, fp);                       
    fwrite(IDAT->type, 4, 1, fp);                                                   
    fwrite(IDAT->data, IDAT->length, 1, fp);                                     

    //the crc needs the concatened array of the chunk type and the chunk datas
    unsigned char *dataCRC = concat2Tables(IDAT->type, IDAT->data, 4, IDAT->length); 
    fwriteUnsignedLong(fp, crc(dataCRC, IDAT->length));                             //we calculate the crc and write it into the file
}

//IEND CHUNK saving Function
void saveIEND(FILE *fp, CHUNK_IEND *IEND)
{
    IEND_CHUNK_initialize(IEND);                                          //we init the chunk

    //then we write the type, the length, the screen dimensions and the chunk datas
    fwrite(int_to_unsignedCharArray(IEND->length), 4, 1, fp);                  
    fwrite(IEND->type, 4, 1, fp);                                           
    fwriteUnsignedLong(fp, crc(IEND->type, 4));                             //we calculate the crc and write it into the file                            
}


///PNG header initialization function
void PNG_HEADER_initialize(PNG *myPNG)
{
    myPNG->signature = (unsigned char*)malloc(8*sizeof(unsigned char));     //we memory allocate the file signature 
    myPNG->signature[0] = 0x89;                                             //we assign the PNG header
    myPNG->signature[1] = 0x50;
    myPNG->signature[2] = 0x4E;
    myPNG->signature[3] = 0x47;
    myPNG->signature[4] = 0x0D;
    myPNG->signature[5] = 0x0A;
    myPNG->signature[6] = 0x1A;
    myPNG->signature[7] = 0x0A;
}