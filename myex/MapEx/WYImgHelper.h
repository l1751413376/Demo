//----------------------------------------------------------------------------------------------------
// File  : WYImgHelper.h
// Title : ͼ���ļ���д����
// Date  : 2011-11-12
// Desc  : 
//
// Copyright(c) WeiWuYuan
//----------------------------------------------------------------------------------------------------
#ifndef _WYIMGHELPER_H_
#define _WYIMGHELPER_H_

#include <windows.h>
#include <stdio.h>



namespace WuYuan
{
	// �������״̬
#pragma pack(push)
#pragma pack(1)

	// TGA�ļ�ͷ
	struct STGA_HEADER 
	{
		// TGA����˳��B G R A
		BYTE	IdLength;				// ͼ����Ϣ�ֶ�(Ĭ��:0)
		BYTE	ColorMapType;			// ��ɫ������ͣ�0����1��0��ʾû����ɫ��,1��ʾ��ɫ�����.��ʽ 2,3,10 ������ɫ��ģ���һ��Ϊ0��
		BYTE	ImageType;				// ͼ��������(2-δѹ����RGBͼ��3-δѹ���ĺڰ�ͼ��10-runlength�����RGBͼ��)
		WORD	ColorMapFirstIndex;		// ��ɫ�������(Ĭ��:0)
		WORD	ColorMapLength;			// ��ɫ��ĳ���(Ĭ��:0)
		BYTE	ColorMapEntrySize;		// ��ɫ������Ϊ��(Ĭ��:0��֧��16/24/32)
		WORD	XOrigin;				// ͼ��X�������ʼλ��(Ĭ��:0)
		WORD	YOrigin;				// ͼ��Y�������ʼλ��(Ĭ��:0)
		WORD	ImageWidth;				// ͼ��Ŀ��
		WORD	ImageHeight;			// ͼ��ĸ߶�
		BYTE	PixelBits;				// ����λ��
		BYTE	ImageDescruptor;		// ͼ�������ַ��ֽ�(Ĭ��:0����֧��16bit 565��ʽ)
	};

	//�ָ�����״̬
#pragma pack(pop)


	//-----------------------------------------------------------------
	// Function
	//-----------------------------------------------------------------
	// tga
	FILE*	TGA_OpenFile(const char* szFile, STGA_HEADER* pOutHeader);
	bool	TGA_ReadPixel(FILE* pFile, const STGA_HEADER* pTgaHeader, UINT* pOutBmp32, bool bUpToDown=false);
	bool	TGA_SaveFile(const char* szDstFile, const void* pBmp, int width, int height, int pixelBits=32);
	void	TGA_DeCompress(FILE* pFile, const STGA_HEADER* pTgaHeader, BYTE* pOutBmp32);

	// bmp
	FILE*	BMP_OpenFile(const char* szFile, BITMAPINFO* pOutBmpInfo);
	bool	BMP_ReadPixel(FILE* pFile, const BITMAPINFO* pBmpInfo, UINT* pOutBmp32);
	bool	BMP_SaveFile(const char* szFile, const void* pBmp, int width, int height, int bitCount=32);

	// other
	void	ReverseBmp(void* pBmp, int width, int height, int bits=32);
	void	ConvertBmpToGray(UINT* pSrcBmp32, int width, int height);
	void	Bmp24ToBmp32(void* pDstBmp24, int width, int height);
}


#endif