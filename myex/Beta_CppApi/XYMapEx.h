#pragma once
#include <Windows.h>
#include "file.h"
#include <math.h>
#include "PNG.h"

struct MaskInfo
{
	int startX;
	int startY;
	int width;
	int height;
	int mask_size;
	byte* img;
	int imgsize;
};

struct MapInfo
{
	char FileName[256];
	DWORD Flag;
	DWORD Width;
	DWORD Height;
	DWORD MaskFlag;
	int m_MapBmpWidth;
	int m_MapBmpHeight;
	/// <summary>
	/// maskÊýÁ¿
	/// </summary>
	DWORD MaskNum;
	MaskInfo* MaskInfos;
	int MaskInfoSize = sizeof(MaskInfo);
	/// <summary>
	/// Í¼Ïñ
	/// </summary>
	DWORD MapImgSize;
	byte* MapImg;
};
void LoadMap(const char * FileName, MapInfo &mapInfo);
