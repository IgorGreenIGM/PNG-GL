/**
 * @brief checksum 32 control algorithm, in this case used to generate the crc of png chunks
 * @param CHUNK datas
 * @param length of the datas CHUNK
 * @return the crc ofthe corresponding CHUNK (unsigned long)
 */


//crc table containing and bool if the table is already computed
unsigned long crc_table[255];
int crc_table_computed = 0;

//crc table computation function
void crc_table_compute(void)
{
    unsigned long c;
    int i, j;
    for(i=0; i<256; i++){
        c=i;
        for(j=0; j<8; j++){
            if(c & 1){
                c=0xedb88320L ^ (c >> 1);
            }else{
                c = c >> 1;
            }
        }
        crc_table[i]=c;
    }
    crc_table_computed = 1;
}

//crc update function by the chunk datas
unsigned long crc_update(unsigned long crc, unsigned char *dataCHUNK, int len)
{
    unsigned long c = crc;
    int i;
    if(!crc_table_computed){
        crc_table_compute();
    }
    for(i=0; i<len; i++){
        c=crc_table[(c ^ dataCHUNK[i]) & 0xff] ^ (c >> 8);
    }
    return c;
}

//crc calculation function, it's this that should be called
unsigned long crc(unsigned char *dataChunk, int len)
{
    return crc_update(0xffffffffL, dataChunk, len) ^  0xffffffffL;
}
