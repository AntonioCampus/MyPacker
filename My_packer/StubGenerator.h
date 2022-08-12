#ifndef STUBGENERATOR
#define STUBGENERATOR


#include "../common/structure.h"
#include "..\compression\Compress.h"
#include "resource1.h"
#include "../xor_encryption/xorEnc.h"
#include <stdio.h>
#include <Windows.h>



int GenerateStubFile(PPACKEDINFO payloadInfo, LPSTR *outFilename);
PPACKEDINFO ParseInFile(LPSTR *file);






#endif // !STUBGENERATOR
