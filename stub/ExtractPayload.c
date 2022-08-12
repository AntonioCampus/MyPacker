#include "ExtractPayload.h"

/*
    virta VirtualAlloc
    findr FindResourceA
    loadr LoadResource
    lockr LockResource
*/


PPACKEDINFO ExtractPayload() {
    // prepare the struct
    PPACKEDINFO payloadinfo = (PPACKEDEVENTINFO)virta(NULL, sizeof(PACKEDINFO), MEM_COMMIT, PAGE_READWRITE);
    if (payloadinfo == NULL) return NULL;

    // get the first resource, the original file
    HRSRC res = findr(NULL, MAKEINTRESOURCE(IDR_DATA1), "DATA");
    if (res == NULL) return res;
    HGLOBAL loadR = loadr(NULL, res);
    if (loadR == NULL) return loadR;
    payloadinfo->file = (PBYTE)lockr(loadR);

    
    // get the second resource, the metadata
    res = findr(NULL, MAKEINTRESOURCE(IDR_DATA2), "DATA");
    if (res == NULL) return res;
    loadR = loadr(NULL, res);
    if (loadR == NULL) return loadR;
    payloadinfo->data = *(PPACKDATA)lockr(loadR);

    return payloadinfo;
    

    
}