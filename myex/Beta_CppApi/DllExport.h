#pragma once
#include <Windows.h>
#define C_dllexport extern "C" __declspec(dllexport) 

C_dllexport int __stdcall add(int x, int y);

C_dllexport int __stdcall GetBitMap(void **Pointer);

C_dllexport int __stdcall GetBitMap2(void **Pointer, int filename);

C_dllexport void __stdcall GetWasFileInfo(const char* filename,int** ptr,int* directionCount,int* frameCount);

C_dllexport void __stdcall GetWasFrame(int width,int height,int* wasPtr,int directionIndex,int frameIndex,int** dataptr,int* dataptrLen);



