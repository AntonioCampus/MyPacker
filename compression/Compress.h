#ifndef COMPRESS_H
#define COMPRESS_H


#include "../common/structure.h"
#include <compressapi.h>

#pragma comment (lib,"Cabinet.lib")

int compressPayload(PPACKEDINFO data);
int DecompressPayload(PPACKEDINFO data);




#endif // !COMPRESS_H
