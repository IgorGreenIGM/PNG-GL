 * author Mogou Igor Green 
 * mail igormogou86@gmail.com
 * date 2022-07-10

**Resume**
    This is a little plugin written in C and fully compatible with C++ (tested with g++ and gcc), 
    Which can make a screenshot from an openGL color buffer an store the result in an PNG file.

 All the process consist to read the pixels in the openglBuffer and store it to a pixelBuffer, then write the pixel buffer in the specified file.

**Details**
    Basically, a png file, contains 02 parts :
        - the png signature (that should occurs on every png file) 
        - the png datas

    The png datas are stored in chunks.
    A very basic png contain 03 criticals chunks (IHDR, IDAT and IEND), but oders can appears
    Each chunk is characterized by his type(his name in hexadecimal), his length(chunk data length), his datas and a crc value.
    crc(cyclic checksum redundance code) value is generated by an algorithm(see crc.cpp) from the concatened values of the type chunk and the datas of the chunk.   

**Note**
    This plugin encode 04 chunks (IHDR, pHYs(physical pixel dimension), IDAT, IEND).
    For each chunk, there is defined a chunk structure(seed PNG_STRUCTURES.h), a chunk initialization function and a chunk saving function, then it may be easier to add     another chunk.

**Important**  
    The png output parameters are not fully editable, so the only png output modes parameters are : 
        - color mode 2 (RGB true color)
        - color channel 3 (each sample contain an Red, Green and Blue value)
        - bit depth 8 (each value is stored on 1 byte = 8 bits)

    The C 'fwrite' function having an incompatible endianess mode for integer values(the png file needs BIG endian), we created our own "fwrite" function for integers       values, see PNG_Operations.cpp
