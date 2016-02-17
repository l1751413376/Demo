#include "WasFrame.h" 
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

CWASFrame::CWASFrame()
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
}

CWASFrame::~CWASFrame()
{
	Release();
	delete m_pImage;
	m_pImage = NULL;
}

void CWASFrame::Release()
{
	if (lineOffsets != NULL)
	{
		delete[] lineOffsets;
		lineOffsets = NULL;
	}

	if (pixels!=NULL)
	{
		for (int y=0; y<height; y++)
		{
			delete[] pixels[y];
		}
		delete[] pixels;
		pixels = NULL;
	}
}



/**
* ��ͼ�����ݻ���Image��
// ͼ��ԭʼ����<br>
// 0-15λRGB��ɫ(565)<br>
// 16-20Ϊalphaֵ<br>
// pixels[y][x]
*/
void CWASFrame::MakeImage(CGraphics* pGraphics)
{
	// ����32λ����ͨ����Image
	delete m_pImage;
	m_pImage = pGraphics->CreateImage(width, height, 32, TRUE);
	assert(m_pImage != NULL);
	DWORD* pImageData = (DWORD*)m_pImage->GetBitsPtr();

	int tx = 0;
	int ty = 0;
	WORD wColor = 0;
	BYTE Alpha = 0;
	BYTE b = 0;
	BYTE g = 0;
	BYTE r = 0;
	BYTE a = 0;

	BYTE iArray[4];
	for (int y1 = 0; y1 < height; y1++)
	{
		for (int x1 = 0; x1 < width; x1++) 
		{
			wColor = (WORD)(pixels[y1][x1] & 0x00FFFF);
			Alpha  = (BYTE)((pixels[y1][x1] & 0xFF0000)>>16);

			b = ((wColor & 0x001F) << 3);
			g = ((wColor & 0x07E0) >> 3);
			r = ((wColor & 0xF800) >> 8);
			// a = ֵ��0-31,  ҪתΪ0-255
			a = Alpha *  8;

			// 			char szBuff[50];
			// 			sprintf(szBuff, "%d -> %d\n", a, a*8);
			// 			OutputDebugString(szBuff);

			iArray[0] = b; // blue
			iArray[1] = g; // green
			iArray[2] = r; // red
			iArray[3] = a; // alpha
			
			DWORD* pPixel = pImageData + ((y1 + ty) * width + (x1 + tx));
			memcpy(pPixel, iArray, 4);
		}
	}

	// ����ͼƬ���ͷ�ԭʼ��������
	Release();
}


CImage* CWASFrame::GetImage()
{
	return m_pImage;
}


