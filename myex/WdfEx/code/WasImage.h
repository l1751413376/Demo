#pragma once

#include "WasFrame.h"
#include <Windows.h>
#include <vector>
using namespace std;


class CWASImage
{
public:
	CWASImage(void);
	virtual ~CWASImage(void);

	void Release();
	BOOL LoadWAS(CGraphics* pGraphics, const char* strFileName);
	BOOL LoadWAS(CGraphics* pGraphics, const void* pData, UINT nDatLen);

	const char* getName() 
	{
		return name;
	}

	SIZE getSize() 
	{
		SIZE size = {width, height};
		return size;
	}

	/**
	* 获取某帧的延时帧数
	*/
	int getFrameDelay(int index) 
	{
		return delayLine[index];
	}

private:
	const BYTE* m_pWasFileDataMoving;	// 移动指针
	const BYTE* m_pWasFileData;			// 固定指针

	void Parse(CWASFrame* pFrame);

	int readInt()
	{
		int ch = 0;
		memcpy(&ch, m_pWasFileDataMoving, 4);
		m_pWasFileDataMoving += 4;
		return ch;
	}

	short readUnsignedShort()
	{
		short ch = 0;
		memcpy(&ch, m_pWasFileDataMoving, 2);
		m_pWasFileDataMoving += 2;
		return ch;
	}

	BYTE readByte()
	{
		BYTE ch = *m_pWasFileDataMoving;
		m_pWasFileDataMoving += 1;
		return ch;
	}

	void seek(int pos)
	{
		m_pWasFileDataMoving = m_pWasFileData + pos;
	}

public:
	int width;
	int height;
	int centerX;
	int centerY;
	BYTE* delayLine;
	int frameCount;
	CWASFrame** frames;
	short imageHeaderSize;
	char name[256];
	WORD palette[256];
//	char path[256];
	int spriteCount;
};
