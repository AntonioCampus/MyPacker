#include "xorEnc.h"


int xor_encdecrypt(BYTE *inputBuffer, BYTE *outputbuffer, int size, DWORD32 key)
{
    for (int i = 0; i < size; i++)
    {
        outputbuffer[i] = inputBuffer[i] ^ key;
    }

    return 0;
}

int xor_payloadEncDec(PPACKEDINFO payload)
{
    PBYTE Buffer = (PBYTE)VirtualAlloc(NULL, payload->data.filesize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (Buffer == NULL) return -1;

    DWORD32 size = (payload->data.cfilesize> 0) ? payload->data.cfilesize : payload->data.filesize;

    xor_encdecrypt(payload->file, Buffer, size,size,payload->data.xorKey);
    VirtualFree(payload->file, 0, MEM_RELEASE);
    payload->file = Buffer;
    return 0;

}
