#pragma once
#include <Windows.h>
#define C_dllexport extern "C" __declspec(dllexport) 

C_dllexport int __stdcall add(int x, int y);

C_dllexport void __stdcall GetBitMap(char * filename,void **Pointer,int * len);

C_dllexport void __stdcall GetWasFileInfo(const char* filename,int** ptr,int* directionCount,int* frameCount,int* width,int *height,int *centerX,int *centerY);

C_dllexport void __stdcall GetWasFrame(int width,int height,int* wasPtr,int directionIndex,int frameIndex,int** dataptr,int* dataptrLen);

C_dllexport void __stdcall Delete(void *ptr)
{
	delete[] ptr;
}

C_dllexport int __stdcall DecompressMask(void* in , void* out);


