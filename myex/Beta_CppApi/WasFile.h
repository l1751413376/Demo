#pragma once
#include <Windows.h>
#include "file.h"
#include <atlimage.h>
#include <assert.h>
#include <tchar.h>

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
	
public:// ֡����ƫ��
	int addrOffset;

	// ������ƫ��
	int* lineOffsets;

	int delay;// ��ʱ֡��
	int height;
	int width;
	int offX;// ͼ��ƫ��
	int offY;

	// Image��ʽͼ��
	CImage* m_pImage;

	// ͼ��ԭʼ����<br>
	// 0-15λRGB��ɫ(565)<br>
	// 16-20Ϊalphaֵ<br>
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
	* ��ȡĳ֡����ʱ֡��
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