#pragma once
#include "file.h"
struct WdfItem
{
	DWORD Hash; // 文件的名字散列
	DWORD Offset; // 文件的偏移
	DWORD Size; // 文件的大小
	DWORD Spaces;
};

struct WdfFile
{
	char FilePath[256];

	__file* FileHandle;
	WdfItem* WdfItems;
	DWORD WdfItemssNumber;//项的数量
};

WdfFile* LoadWdf(char *filePath);

WdfItem* GetWasSizeByOffset(WdfFile* wdfFile, DWORD hash);
