#ifndef LOADPE_H
#define LOADPE_H

#include <Windows.h>
#include "Api.h"

typedef INT(*entryPoint)();


typedef struct RELOCATION_ENTRY {
    USHORT Offset : 12;
    USHORT Type : 4;
} RELOCATION_ENTRY, *PRELOCATION_ENTRY;


int LoadPayload(PBYTE peMemMap);

#endif