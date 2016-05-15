#pragma once
#include "file.h"
struct WdfItem
{
	DWORD Hash; // �ļ�������ɢ��
	DWORD Offset; // �ļ���ƫ��
	DWORD Size; // �ļ��Ĵ�С
	DWORD Spaces;
};

struct WdfFile
{
	char FilePath[256];

	__file* FileHandle;
	WdfItem* WdfItems;
	DWORD WdfItemssNumber;//�������
};

WdfFile* LoadWdf(char *filePath);

WdfItem* GetWasSizeByOffset(WdfFile* wdfFile, DWORD hash);
