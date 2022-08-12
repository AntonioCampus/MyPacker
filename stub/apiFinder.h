#ifndef API_FINDER_H
#define API_FINDER_H

#include <Windows.h>
#include "../common/structure.h"
#include <string.h>

#define LOADLIBRARYAH   0xa97f1df9  
#define GETPROCCADDH    0xd8c0b5a5


typedef HMODULE(WINAPI * LOADLIBRARYA)(LPCSTR);
typedef FARPROC(WINAPI * GETPROCADDRESS)(HMODULE, LPCSTR);


LOADLIBRARYA loadlib;
GETPROCADDRESS getproc;
static HMODULE k32address;

HMODULE *pk32address;



DWORD32 GetTheFunc();
DWORD32 ror(DWORD32 i, DWORD32 j);
DWORD32 calcHash(char *str);
DWORD32 strLen(BYTE *str);


#endif // !API_FINDER_H
