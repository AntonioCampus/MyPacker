#ifndef XORENC_H
#define XORECN_H
#include <Windows.h>
#include "../common/structure.h"

int xor_encdecrypt(BYTE *inputBuffer, BYTE *outputbuffer, int size,DWORD32 key);
int xor_payloadEncDec(PPACKEDINFO payloadInfo);

#endif // !XORENC_H

