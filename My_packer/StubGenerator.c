#include "StubGenerator.h"



#define IDR_DATA1_REMOTE                       105
#define IDR_DATA2_REMOTE                       106
#define IDR_DATA3_REMOTE                       107


int GenerateStubFile(PPACKEDINFO payloadInfo, LPSTR *outFilename) {
    // accessing to resource
    HRSRC res1 = FindResourceA(NULL, MAKEINTRESOURCE(IDR_DATA1), "DATA");
    if (res1 == NULL) return -1;
    HGLOBAL stubFile = LoadResource(NULL, res1);
    if (stubFile == NULL) return -1;
    DWORD dwSize = SizeofResource(NULL, res1);
    PBYTE pBuffer = (PBYTE)LockResource(stubFile);

    // create file and add the stub as main program
    HANDLE Outfile = CreateFileA(outFilename, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (Outfile == INVALID_HANDLE_VALUE) return -1;
    WriteFile(Outfile, pBuffer, dwSize, NULL, NULL);
    CloseHandle(Outfile);

    
    // compress the payload
    compressPayload(payloadInfo);

    //encrypt the payload
    payloadInfo->data.xorKey = 0x41;
    xor_payloadEncDec(payloadInfo);
    


    printf("[1] Size Of Original Executable: %dB\n", payloadInfo->data.filesize);
    printf("[2] Size Of Compressed Executable(COMPRESS_ALGORITHM_MSZIP): %dB\n", payloadInfo->data.cfilesize);
    printf("[3] Size Of stub: %dB\n", dwSize);

    
    // add compressed payload to the stub resource
    HANDLE hUpdate = BeginUpdateResourceA(outFilename, FALSE);

    // write the metadata
    if (UpdateResourceA(hUpdate, "DATA", MAKEINTRESOURCE(IDR_DATA2_REMOTE), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPVOID)&payloadInfo->data, sizeof(PACKDATA)) == FALSE) {
        return -1;
    }
    // write the compressed payload
    if (UpdateResourceA(hUpdate, "DATA", MAKEINTRESOURCE(IDR_DATA1_REMOTE), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), payloadInfo->file, payloadInfo->data.cfilesize) == FALSE) {
        return -1;
    }
    EndUpdateResource(hUpdate, FALSE);
    

    HANDLE hFile = CreateFileA(outFilename, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
    DWORD32 FileSize = GetFileSize(hFile, NULL);
    CloseHandle(hFile);
    printf("[*] Final size of packed executable: %dB\n", FileSize);

    
    return 0;

}

PPACKEDINFO ParseInFile(LPSTR *file) {
    PPACKEDINFO payloadinfo = (PPACKEDEVENTINFO)VirtualAlloc(NULL, sizeof(PACKEDINFO), MEM_COMMIT, PAGE_READWRITE);
    HANDLE hFile = CreateFileA(file, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)return -1;
    DWORD32 FileSize = GetFileSize(hFile,NULL);
    PBYTE pbuffer = (PBYTE)VirtualAlloc(NULL, FileSize, MEM_COMMIT, PAGE_READWRITE);
    DWORD lpNumberOfBytesRead;
    if (ReadFile(hFile, pbuffer, FileSize, &lpNumberOfBytesRead, NULL) == FALSE) {
        VirtualFree(payloadinfo, 0, MEM_RELEASE);
        VirtualFree(pbuffer,0,MEM_RELEASE);
        return NULL;
    };
    payloadinfo->file = pbuffer;
    payloadinfo->data.filesize = FileSize;
    return payloadinfo;
}
