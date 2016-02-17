#pragma once

#include <Windows.h>
#include "../app.h"
#include <atlimage.h>
#include <assert.h>
#include <tchar.h>

#define img_4565 1
DWORD colour4565to8888(DWORD c);

const int TYPE_ALPHA = 0x00;// 前2位

const int TYPE_ALPHA_PIXEL = 0x20;// 前3位 0010 0000

const int TYPE_ALPHA_REPEAT = 0x00;// 前3位

const int TYPE_FLAG = 0xC0;// 2进制前2位 1100 0000

const int TYPE_PIXELS = 0x40;// 以下前2位 0100 0000

const int TYPE_REPEAT = 0x80;// 1000 0000

const int TYPE_SKIP = 0xC0; // 1100 0000

const int WAS_IMAGE_HEADER_SIZE = 12;

#define WAS_FILE_TAG			'PS'	// 'SP'文件前缀


class WasFrame
{
public:
	WasFrame()
	{
		height = 0;
		width = 0;
		offX = 0;
		offY = 0;
		addrOffset = 0;
		delay = 1;
		lineOffsets = NULL;
		pixels = NULL;
		m_pImage = NULL;
	};
	~WasFrame()
	{
		Release();
		delete m_pImage;
		m_pImage = NULL;
	};
	void Release()
	{
		if (lineOffsets != NULL)
		{
			delete[] lineOffsets;
			lineOffsets = NULL;
		}

		if (pixels != NULL)
		{
			for (int y = 0; y<height; y++)
			{
				delete[] pixels[y];
			}
			delete[] pixels;
			pixels = NULL;
		}
	};
private:
	
public:// 帧数据偏移
	int addrOffset;

	// 行数据偏移
	int* lineOffsets;

	int delay;// 延时帧数
	int height;
	int width;
	int offX;// 图像偏移
	int offY;

	// Image格式图像
	CImage* m_pImage;

	// 图像原始数据<br>
	// 0-15位RGB颜色(565)<br>
	// 16-20为alpha值<br>
	// pixels[y][x]
	DWORD **pixels;

};

class WasImg:__bfile
{
public:
	int width;
	int height;
	int centerX;
	int centerY;
	BYTE* delayLine;
	int frameCount;
	WasFrame** frames;
	short imageHeaderSize;
	char name[256];
	WORD palette[256];
	//	char path[256];
	int spriteCount;

public:
	WasImg()
	{
		width = 0;
		height = 0;
		centerX = 0;
		centerY = 0;
		delayLine = NULL;
		frameCount = 0;
		frames = NULL;
		imageHeaderSize = 0;
		ZeroMemory(name, sizeof(name));
		ZeroMemory(palette, sizeof(palette));
		//ZeroMemory(path, sizeof(path));
		spriteCount = 0;
	}
	~WasImg()
	{
		Release();
	}
	SIZE getSize()
	{
		SIZE size = { width, height };
		return size;
	}
	void load_file(const char* strFileName);
	void load_data(const BYTE* pData);
	/**
	* 获取某帧的延时帧数
	*/
	int getFrameDelay(int index)
	{
		return delayLine[index];
	}
private:
	void Parse(WasFrame* pFrame);
	void Release()
	{
		if (delayLine != NULL)
		{
			delete[] delayLine;
			delayLine = NULL;
		}

		if (frames != NULL)
		{
			for (int i = 0; i<spriteCount; i++)
			{
				delete[] frames[i];
			}
			delete[] frames;
			frames = NULL;
		}
	}
};