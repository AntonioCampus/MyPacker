#include "decompression.h"


int DecompressPayload(PPACKEDINFO payload)
{
    COMPRESSOR_HANDLE Decompressor = NULL;
    BOOL Success;
    SIZE_T DecompressedBufferSize, DecompressedDataSize = 0;
    PBYTE dBuffer = NULL;

    Success = createdec(
        COMPRESS_ALGORITHM_MSZIP,       //  Compression Algorithm
        NULL,                           //  Optional allocation routine
        &Decompressor);
    if (!Success) return -1;

    dBuffer = (PBYTE)virta(NULL, payload->data.filesize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    Success = decompress(
        Decompressor,               //  Decompressor handle
        payload->file,                  //  Compressed data
        payload->data.cfilesize,            //  Compressed data size
        dBuffer,                    //  Decompressed buffer
        payload->data.filesize,          //  Decompressed buffer size
        &DecompressedDataSize);     //  Decompressed data size

    if (!Success)
    {
        virfre(dBuffer, 0, MEM_RELEASE);
        return -1;
    }
    if (DecompressedDataSize != payload->data.filesize) return -1;
    // free the old buffer, and write on it the new ones with compressed payload
    virfre(payload->file, 0, MEM_RELEASE);
    payload->file = dBuffer;

    return 0;
}