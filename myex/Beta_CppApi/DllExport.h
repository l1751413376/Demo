#pragma once
#include <Windows.h>
#define C_dllexport extern "C" __declspec(dllexport) 

C_dllexport int __stdcall add(int x, int y);

C_dllexport DWORD* __stdcall GetBitMap();
