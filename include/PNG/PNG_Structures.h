#ifndef PNGSTRUCTS_H_INCLUDED
#define PNGSTRUCTS_H_INCLUDED

// png header chunk structure
typedef struct CHUNK_IHDR
{
    int length;          // 04 bytes
    unsigned char *type; // 04 bytes
    int width;
    int height;
    unsigned char *data; // variable size
    unsigned long crc;   // 04 bytes
} CHUNK_IHDR;

//png physical pixel dimension structure
typedef struct CHUNK_pHYs
{
    int length;          // 04 bytes
    unsigned char *type; // 04 bytes
    unsigned int _ppuX;
    unsigned int _ppuY; // ppu : pixels per unit 04 bytes
    unsigned char *unitSpecifier;
    unsigned long crc; // 04 bytes
} CHUNK_pHYs;

//png data chunk structure
typedef struct CHUNK_IDAT
{
    int length;          // 04 bytes
    unsigned char *type; // 4 bytes
    unsigned char *data; // variable size
    unsigned long crc;   // 4bytes
} CHUNK_IDAT;

//png end chunck structure
typedef struct CHUNK_IEND
{
    int length;          // 4 bytes
    unsigned char *type; // 4 bytes
    unsigned char *data; // variable size
    unsigned long crc;   // 4bytes
} CHUNK_IEND;

//whole png structure
typedef struct PNG
{
    unsigned char *signature;
    CHUNK_IHDR IHDR; // sur 25 bytes
    CHUNK_pHYs pHYs;
    CHUNK_IDAT IDAT; // longueur variable
    CHUNK_IEND IEND; // sur 12 bytes
} PNG;

#endif // PNGSTRUCTS_H_INCLUDED