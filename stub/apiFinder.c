#include "apiFinder.h"  
#include <stdio.h>


DWORD32 GetTheFunc()
{

#ifdef _WIN64
    ULONG_PTR pebAddress = __readgsqword(0x60);
#else
    ULONG_PTR pebAddress = __readfsdword(0x30);
#endif

    _PPEB peb = (_PPEB)pebAddress;
    PPEB_LDR_DATA ldr = peb->pLdr;
    LIST_ENTRY *listEntry = ldr->InMemoryOrderModuleList.Flink;
    LDR_DATA_TABLE_ENTRY *ldrData;
    ULONG_PTR k32dllBase;

    ldrData = (LDR_DATA_TABLE_ENTRY*)listEntry->Flink->Flink;
    

    k32dllBase = (ULONG_PTR)ldrData->DllBase;

    //apis->k32address = (HMODULE)k32dllBase;

    k32address = (HMODULE)k32dllBase;
    pk32address = &k32address;

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)k32dllBase;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(k32dllBase + dosHeader->e_lfanew);

    DWORD32 siz = sizeof(ntHeaders->FileHeader);
    IMAGE_OPTIONAL_HEADER optHeaders = ntHeaders->OptionalHeader;
    IMAGE_DATA_DIRECTORY dirEntry = optHeaders.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    IMAGE_EXPORT_DIRECTORY* expDir = (IMAGE_EXPORT_DIRECTORY*)(dirEntry.VirtualAddress + k32dllBase);

    ULONG_PTR addOfname = expDir->AddressOfNames + k32dllBase;
    ULONG_PTR addOfnameOrd = expDir->AddressOfNameOrdinals + k32dllBase;
    ULONG_PTR addOfFunc = expDir->AddressOfFunctions + k32dllBase;
    ULONG_PTR address = 0;
    DWORD32 hash = 0;
    LOADLIBRARYA loadLibaddr = NULL;
    GETPROCADDRESS getproaddr = NULL;
    DWORD find = 0;

    for (int x = 0; x < expDir->NumberOfFunctions && find < 2; x++) {
        // even in 64bit the rva are 32bit long
        address = *(DWORD32*)(addOfname + x * (sizeof(BYTE) * 4));
        address += k32dllBase;
        hash = calcHash((BYTE*)address);
        if (hash == LOADLIBRARYAH || hash == GETPROCCADDH) {
            address = addOfnameOrd + (x * 2);
            USHORT ordval = *(USHORT*)address;
            address = *(DWORD32*)(addOfFunc + (ordval * 4));
            ULONG_PTR tmp = (ordval * 4);
            address = address + k32dllBase;
            switch (hash)
            {
            case LOADLIBRARYAH:loadlib = (LOADLIBRARYA)address;
                find++;
                break;
            case GETPROCCADDH: getproc = (GETPROCADDRESS)address;
                find++;
                break;
            default:
                break;
            }
        }
    }
    return 0;
}


DWORD32 ror(DWORD32 i, DWORD32 j)
{
    DWORD32 tmp1 = (i >> j);
    DWORD32 tmp2 = (i << (32 - j));
    return (tmp1 | tmp2) & 0xffffffff;
}

DWORD32 calcHash(char *str)
{
    DWORD32 size = strLen(str);
    DWORD32 hash = 0;
    for (int x = 0; x < size; x++)
    {
        hash = ror(hash, 14);
        hash += str[x];
    }
    return hash;
}


DWORD32 strLen(BYTE *str)
{
    DWORD32 i = 0;
    while (str[i] != 0) {
        i++;
    }
    return i;
}