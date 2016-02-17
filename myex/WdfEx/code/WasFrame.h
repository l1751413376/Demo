#pragma once

#include <assert.h>
//#include <FSEngine.h>
//#include <FSEngineFactory.h>
using namespace FSEngine;



class CWASFrame
{
public:
	CWASFrame(void);
	virtual ~CWASFrame(void);

public:
	void MakeImage(CGraphics* pGraphics);
	CImage* GetImage();
 	void Release();

public: 
	// 帧数据偏移
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
