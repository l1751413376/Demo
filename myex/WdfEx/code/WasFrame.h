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
	// ֡����ƫ��
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
