#include "Api.h"


int API_INIT() 
{
    GetTheFunc();
    if (loadlib == NULL) return -1;
    // k32api
    getcurrentp = (GETCURRENTPROCESS)getproc((*pk32address), getcustr);
    virta = (VIRTUALALLOC)getproc((*pk32address), virtastr);
    virtaex = (VIRTUALALLOCEX)getproc((*pk32address), virtaestr);
    virfre = (VIRTUALFREE)getproc((*pk32address), virtfstr);
    findr = (FINDRESOURCEA)getproc((*pk32address), findrstr);
    loadr = (LOADRESOURCE)getproc((*pk32address), loadrstr);
    lockr = (LOCKRESOURCE)getproc((*pk32address), lockrstr);

    // cabinet api for decompression
    HANDLE lib = loadlib("Cabinet.dll");
    createdec  =  (CREATEDECOMPRESSION)getproc(lib, createdecstr);
    decompress = (DECOMPRESS)getproc(lib, decostr);

    
    return 0;

}