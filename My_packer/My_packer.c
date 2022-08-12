#include <Windows.h>
#include "StubGenerator.h"
#include "../compression/Compress.h"
#include "../stub/loadpe.h"
#include "../xor_encryption/xorEnc.h"


#ifdef  _DEBUG
    #include <stdio.h>
    #define outFilePath   "..\\Debug\\packed.exe"
    #define inputFilePath "C:\\Windows\\SysWOW64\\calc.exe"
#endif //  _DEBUG



int main(int argc,char**argv)
{
    
    PPACKEDINFO pinfo;
#ifdef _DEBUG
    pinfo = ParseInFile(inputFilePath);
    GenerateStubFile(pinfo, outFilePath);

#else
    switch (argc)
    {
    case 3:
        if (strlen(argv[1]) > 0 && strlen(argv[2]) > 0)
        {
            pinfo = ParseInFile(argv[1]);
            GenerateStubFile(pinfo, argv[2]);
        }
        break;
    default:
        printf("Usage: My_packer input_name.exe output_name.exe\n");
        break;
    }
#endif // _DEBUG
    
    return 0;
}
