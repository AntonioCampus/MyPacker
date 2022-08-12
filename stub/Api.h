#ifndef API_H
#define API_H
#include <Windows.h>
#include "apiFinder.h"
#include <compressapi.h>


static char virtastr[] = { 'V','i','r','t','u','a','l','A','l','l','o','c',0 };
static char virtaestr[] = { 'V','i','r','t','u','a','l','A','l','l','o','c','E','x',0 };
static char virtfstr[] = { 'V','i','r','t','u','a','l','F','r','e','e',0 };
static char findrstr[] = { 'F','i','n','d','R','e','s','o','u','r','c','e','A',0 };
static char loadrstr[] = { 'L','o','a','d','R','e','s','o','u','r','c','e',0 };
static char lockrstr[] = { 'L','o','c','k','R','e','s','o','u','r','c','e',0 };
static char getcustr[] = { 'G','e','t','C','u','r','r','e','n','t','P','r','o','c','e','s','s',0 };

static char createdecstr[] = { 'C','r','e','a','t','e','D','e','c','o','m','p','r','e','s','s','o','r',0 };
static char decostr[] = { 'D','e','c','o','m','p','r','e','s','s',0 };

// apis type
typedef HMODULE(WINAPI * VIRTUALALLOC)(LPVOID,SIZE_T,DWORD,DWORD);

typedef HMODULE(WINAPI * VIRTUALALLOCEX)(HANDLE,LPVOID, SIZE_T, DWORD, DWORD);

typedef HMODULE(WINAPI * GETCURRENTPROCESS)();

typedef BOOL(WINAPI* VIRTUALFREE)(LPVOID,SIZE_T,DWORD);

typedef HMODULE(WINAPI * FINDRESOURCEA)(HANDLE,LPCSTR,LPCSTR);

typedef HMODULE(WINAPI * LOADRESOURCE)(HMODULE,HRSRC);

typedef HMODULE(WINAPI * LOCKRESOURCE)(HGLOBAL);

typedef int(WINAPI* MESSAGEBOXA)(HWND, LPCSTR, LPCSTR, UINT);

typedef BOOL(WINAPI* CREATEDECOMPRESSION)(DWORD,
                                          PCOMPRESS_ALLOCATION_ROUTINES,
                                          PDECOMPRESSOR_HANDLE);

typedef BOOL(WINAPI* DECOMPRESS)(DECOMPRESSOR_HANDLE,
                                 LPCVOID,
                                  SIZE_T,
                                  PVOID,
                                  SIZE_T,
                                  PSIZE_T);
// api address
VIRTUALALLOC        virta;
VIRTUALALLOCEX      virtaex;
VIRTUALFREE         virfre;
FINDRESOURCEA       findr;
LOADRESOURCE        loadr;
LOCKRESOURCE        lockr;
MESSAGEBOXA         msgbo;
CREATEDECOMPRESSION createdec;
DECOMPRESS          decompress;
GETCURRENTPROCESS   getcurrentp;


// initialize all api that will be use by the program
int API_INIT();




#endif // !API_H
