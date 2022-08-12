
#include <Windows.h>
#include "loadpe.h"
#include "ExtractPayload.h"
#include "decompression.h"
#include "Api.h"
#include "../xor_encryption/xorEnc.h"
#include <stdio.h>


// debug   entry point : main
// release entry point : start

#ifdef _DEBUG
    int main()
#else
    int start()
#endif
{
    
    API_INIT();
    PPACKEDINFO pe = ExtractPayload();

#ifdef _DEBUG
    printf("%d\n", pe->data.filesize);
    printf("%d\n", pe->data.cfilesize);
    printf("%x\n", pe->data.xorKey);
#endif

    xor_payloadEncDec(pe);
    DecompressPayload(pe);

    LoadPayload(pe->file);
    
    
    return 0;
}