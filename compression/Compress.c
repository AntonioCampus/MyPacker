#include "Compress.h"


int compressPayload(PPACKEDINFO payload) 
{
	COMPRESSOR_HANDLE Compressor = NULL;
	PBYTE InputBuffer = NULL;
	BOOL Success;
	SIZE_T CompressedDataSize, CompressedBufferSize;
	PBYTE cBuffer = NULL;

	// compression object
	Success = CreateCompressor(
		COMPRESS_ALGORITHM_MSZIP,		//  Compression Algorithm
		NULL,                           //  Optional allocation routine
		&Compressor);
	if (!Success) return -1;

	//compression query size
	//  Query compressed buffer size.
	Success = Compress(
		Compressor,                  //  Compressor Handle
		payload->file,                   //  Input buffer, Uncompressed data
		payload->data.filesize,               //  Uncompressed data size
		NULL,                        //  Compressed Buffer
		0,                           //  Compressed Buffer size
		&CompressedBufferSize);      //  Compressed Data size
	
	if (!Success)
	{
		DWORD ErrorCode = GetLastError();
		if (ErrorCode != ERROR_INSUFFICIENT_BUFFER) return -1;
		else
		{
			cBuffer = (PBYTE)VirtualAlloc(NULL, CompressedBufferSize,MEM_COMMIT| MEM_RESERVE, PAGE_READWRITE);
			if (cBuffer == NULL) return -1;
		}
	}

	Success = Compress(
		Compressor,             //   Compressor Handle
		payload->file,            //    Input buffer, Uncompressed data
		payload->data.filesize,          //  Uncompressed data size
		cBuffer,               //   Compressed Buffer
		CompressedBufferSize,   //   Compressed Buffer size
		&CompressedDataSize);   //   Compressed Data size
	
	if (!Success) 
	{
		VirtualFree(cBuffer, 0, MEM_RELEASE);
		return -1;
	}
	// free the old buffer, and write on it the new ones with compressed payload
	VirtualFree(payload->file, 0, MEM_RELEASE);
	payload->file = cBuffer;
	payload->data.cfilesize = CompressedDataSize;

	return 0;
}



int DecompressPayload(PPACKEDINFO payload)
{
	COMPRESSOR_HANDLE Decompressor = NULL;
	BOOL Success;
	SIZE_T DecompressedBufferSize, DecompressedDataSize = 0;
	PBYTE dBuffer = NULL;

	Success = CreateDecompressor(
		COMPRESS_ALGORITHM_MSZIP,		//  Compression Algorithm
		NULL,                           //  Optional allocation routine
		&Decompressor);
	if (!Success) return -1;

	dBuffer = (PBYTE)VirtualAlloc(NULL, payload->data.filesize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (dBuffer == NULL) return -1;
	Success = Decompress(
		Decompressor,               //  Decompressor handle
		payload->file,					//  Compressed data
		payload->data.cfilesize,            //  Compressed data size
		dBuffer,                    //  Decompressed buffer
		payload->data.filesize,          //  Decompressed buffer size
		&DecompressedDataSize);     //  Decompressed data size

	if (!Success) 
	{
		VirtualFree(dBuffer, 0, MEM_RELEASE);
		return -1;
	}
	if (DecompressedDataSize != payload->data.filesize) return -1;
	// free the old buffer, and write on it the new ones with compressed payload
	VirtualFree(payload->file, 0, MEM_RELEASE);
	payload->file = dBuffer;

	return 0;
}


