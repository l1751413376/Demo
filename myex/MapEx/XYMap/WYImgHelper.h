//----------------------------------------------------------------------------------------------------
// File  : WYImgHelper.h
// Title : 图像文件读写助手
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
	// 保存对齐状态
#pragma pack(push)
#pragma pack(1)

	// TGA文件头
	struct STGA_HEADER 
	{
		// TGA像素顺序：B G R A
		BYTE	IdLength;				// 图像信息字段(默认:0)
		BYTE	ColorMapType;			// 颜色表的类型（0或者1，0表示没有颜色表,1表示颜色表存在.格式 2,3,10 是无颜色表的，故一般为0）
		BYTE	ImageType;				// 图像类型码(2-未压缩的RGB图像，3-未压缩的黑白图像，10-runlength编码的RGB图像)
		WORD	ColorMapFirstIndex;		// 颜色表的索引(默认:0)
		WORD	ColorMapLength;			// 颜色表的长度(默认:0)
		BYTE	ColorMapEntrySize;		// 颜色表表项的为数(默认:0，支持16/24/32)
		WORD	XOrigin;				// 图像X坐标的起始位置(默认:0)
		WORD	YOrigin;				// 图像Y坐标的起始位置(默认:0)
		WORD	ImageWidth;				// 图像的宽度
		WORD	ImageHeight;			// 图像的高度
		BYTE	PixelBits;				// 像素位数
		BYTE	ImageDescruptor;		// 图像描述字符字节(默认:0，不支持16bit 565格式)
	};

	//恢复对齐状态
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