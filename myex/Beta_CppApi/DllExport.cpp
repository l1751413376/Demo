#include "DllExport.h"

int g_count = 0;

int __stdcall add(int x, int y)
{
	return g_count++;
}

DWORD* __stdcall GetBitMap() 
{
	int width = 200;
	int height = 200;
	DWORD* bitmap = new DWORD[width*height];
	auto i_bitmap = bitmap;
	for (size_t i = 0,len= width*height; i < len; i++)
	{
		i_bitmap[i] = 0xff0000;
	}
	return bitmap;
}
