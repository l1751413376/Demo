//----------------------------------------------------------------------------------------------------
// File  : WYImgHelper.cpp
// Title : ͼ���ļ���д����
// Date  : 2011-11-12
// Desc  : 
//
// Copyright(c) WeiWuYuan
//----------------------------------------------------------------------------------------------------
#include "WYImgHelper.h"
#include <assert.h>



// ��ȡtga�ļ�ͷ��Ϣ
FILE* WuYuan:: TGA_OpenFile(const char* szFile, STGA_HEADER* pOutHeader)
{
	assert(pOutHeader != NULL);
	memset(pOutHeader, 0, sizeof(STGA_HEADER));

	FILE* pFile = fopen(szFile, "rb");
	if (pFile == NULL)	return NULL;

	fread(pOutHeader, sizeof(STGA_HEADER), 1, pFile);

	return pFile;
}

// ��TGA��������
// bUpToDown���Ƿ�ʹ���ذ����϶��µ�˳��洢
bool WuYuan::TGA_ReadPixel(FILE* pFile, const STGA_HEADER* pTgaHeader, UINT* pOutBmp32, bool bUpToDown/*=false*/)
{
	assert(pFile != NULL);
	assert(pOutBmp32 != NULL);

	if (pTgaHeader->ImageType!=2 && pTgaHeader->ImageType!=10 && pTgaHeader->ImageType!=3) 
		return false;
	if (pTgaHeader->ColorMapType != 0)
		return false;

	fseek(pFile, sizeof(STGA_HEADER), SEEK_SET);

	if (pTgaHeader->IdLength > 0)
		fseek(pFile, pTgaHeader->IdLength, SEEK_CUR);	// skip TARGA image comment

	BYTE* pOutBmp = (BYTE*)pOutBmp32;

	switch (pTgaHeader->ImageType)
	{
	case 2:	// ��ѹ��ͼ��
	case 3:	// 8λ�ĺڰ�ͼ��
		{
			int pixel_bytes = pTgaHeader->PixelBits/8;
			int bmpSize = pTgaHeader->ImageWidth*pTgaHeader->ImageHeight*pixel_bytes;
			int src_pitch = pTgaHeader->ImageWidth * pixel_bytes;

			// ����pTgaHeader->ImageDescruptor & 0x20��Ϊ�棬�������϶��´洢��ͼ��
			if ((int)bUpToDown != (pTgaHeader->ImageDescruptor & 0x20))
				src_pitch = -src_pitch;

			switch (pTgaHeader->PixelBits)
			{
			case 8:
				{
					BYTE* pBmp8 = (BYTE*)malloc(bmpSize);
					fread(pBmp8, bmpSize, 1, pFile);

					BYTE* pSrc = pBmp8;
					BYTE* pDst = pOutBmp;
					if (src_pitch < 0)
						pSrc += (pTgaHeader->ImageHeight-1)*(-src_pitch);

					for (int h=0, w=0; h<pTgaHeader->ImageHeight; h++)
					{
						BYTE* pLine = pSrc;
						for (w=0; w<pTgaHeader->ImageWidth; w++)
						{
							*pDst++ = *pLine;	// b
							*pDst++ = *pLine;	// g
							*pDst++ = *pLine++;	// r
							*pDst++ = 255;		// alpha
						}
						pSrc += src_pitch;
					}

					free(pBmp8);
				}
				break;
				// a1r5g5b5
			case 16:
				{
					src_pitch /= pixel_bytes;

					BYTE* pBmp16 = (BYTE*)malloc(bmpSize);
					fread(pBmp16, bmpSize, 1, pFile);

					WORD* pSrc = (WORD*)pBmp16;
					BYTE* pDst = pOutBmp;
					if (src_pitch < 0)
						pSrc += (pTgaHeader->ImageHeight-1)*(-src_pitch);

					BYTE r, g, b;

					// ����16λTGA��ImageDescruptor��0~3λ��ֵ��0��ʾalpha��Ч 1��ʾalpha��Ч
					if (pTgaHeader->ImageDescruptor & 0x1)
					{
						for (int h=0, w=0; h<pTgaHeader->ImageHeight; h++)
						{
							WORD* pLine = pSrc;
							for (w=0; w<pTgaHeader->ImageWidth; w++)
							{
								b = (*pLine) & 0x1F;
								g = ((*pLine)>>5) & 0x1F;
								r = ((*pLine)>>10) & 0x1F;
								*pDst++ = (b<<3) | (b>>2);		// b
								*pDst++ = (g<<3) | (g>>2);		// g
								*pDst++ = (r<<3) | (r>>2);		// r
								*pDst++ = ((*pLine)>>15)*255;	// alpha
								pLine++;
							}
							pSrc += src_pitch;
						}
					}
					else
					{
						for (int h=0, w=0; h<pTgaHeader->ImageHeight; h++)
						{
							WORD* pLine = pSrc;
							for (w=0; w<pTgaHeader->ImageWidth; w++)
							{
								b = (*pLine) & 0x1F;
								g = ((*pLine)>>5) & 0x1F;
								r = ((*pLine)>>10) & 0x1F;
								*pDst++ = (b<<3) | (b>>2);	// b
								*pDst++ = (g<<3) | (g>>2);	// g
								*pDst++ = (r<<3) | (r>>2);	// r
								*pDst++ = 255;				// alpha
								pLine++;
							}
							pSrc += src_pitch;
						}
					}

					free(pBmp16);
				}
				break;
			case 24:
				{
					BYTE* pBmp24 = (BYTE*)malloc(bmpSize);
					fread(pBmp24, bmpSize, 1, pFile);

					BYTE* pSrc = pBmp24;
					BYTE* pDst = pOutBmp;
					if (src_pitch < 0)
						pSrc += (pTgaHeader->ImageHeight-1)*(-src_pitch);

					for (int h=0, w=0; h<pTgaHeader->ImageHeight; h++)
					{
						BYTE* pLine = pSrc;
						for (w=0; w<pTgaHeader->ImageWidth; w++)
						{
							*pDst++ = *pLine++;	// b
							*pDst++ = *pLine++;	// g
							*pDst++ = *pLine++;	// r
							*pDst++ = 255;		// alpha
						}
						pSrc += src_pitch;
					}

					free(pBmp24);
				}
				break;
			case 32:
				{
					if (src_pitch > 0)
					{
						fread(pOutBmp, bmpSize, 1, pFile);
					}
					else
					{
						int src_pitch_8 = -src_pitch;
						src_pitch /= pixel_bytes;

						UINT* pBmp32 = (UINT*)malloc(bmpSize);
						fread(pBmp32, bmpSize, 1, pFile);

						UINT* pSrc = pBmp32 + (pTgaHeader->ImageHeight-1)*(-src_pitch);
						UINT* pDst = (UINT*)pOutBmp;

						for (int h=0; h<pTgaHeader->ImageHeight; h++)
						{
							memcpy(pDst, pSrc, src_pitch_8);
							pSrc += src_pitch;
							pDst += pTgaHeader->ImageWidth;
						}

						free(pBmp32);
					}
				}
				break;
			}
		}
		break;
		// RLEѹ������
	case 10:
		{
			if (pTgaHeader->PixelBits < 24)
			{
				MessageBoxA(0, "��֧�ֵ���24λ��TGA-RLEѹ����ʽ", 0, MB_OK);
			}
			else
			{
				TGA_DeCompress(pFile, pTgaHeader, pOutBmp);

				// ����pTgaHeader->ImageDescruptor & 0x20��Ϊ�棬�������϶��´洢��ͼ��
				if (bUpToDown != ((pTgaHeader->ImageDescruptor & 0x20)!=0))
					ReverseBmp(pOutBmp, pTgaHeader->ImageWidth, pTgaHeader->ImageHeight, pTgaHeader->PixelBits);

				if (pTgaHeader->PixelBits == 24)
					Bmp24ToBmp32(pOutBmp, pTgaHeader->ImageWidth, pTgaHeader->ImageHeight);
			}
		}
		break;
	}

	return true;
}

// ����tga��ʽ�ļ�
bool WuYuan::TGA_SaveFile(const char* szDstFile, const void* pBmp, int width, int height, int pixelBits/*=32*/)
{
	assert(pixelBits==8 || pixelBits==16 || pixelBits==24 || pixelBits==32);

	// ������ļ����������ȫ��·��
	{
		char szPath[MAX_PATH] = { 0 };
		for (int i=0, begin=0, len=int(strlen(szDstFile)); i<len; i++)
		{
			if (szDstFile[i]=='\\' || szDstFile[i]=='/')
			{
				memcpy(szPath, szDstFile, i);
				szPath[i+1] = '\0';
				CreateDirectoryA(szPath, NULL);	// ����Ŀ¼
			}
		}
	}

	STGA_HEADER tgaHeader;
	memset(&tgaHeader, 0, sizeof(STGA_HEADER));

	tgaHeader.IdLength = 0;						// ͼ����Ϣ�ֶ�(Ĭ��:0)
	tgaHeader.ColorMapType = 0;					// ��ɫ�������(Ĭ��0)
	tgaHeader.ImageType = (pixelBits==8? 3 : 2);// ͼ��������(֧��2��10)
	tgaHeader.ColorMapFirstIndex = 0;			// ��ɫ�������(Ĭ��:0)
	tgaHeader.ColorMapLength = 0;				// ��ɫ��ĳ���(Ĭ��:0)
	tgaHeader.ColorMapEntrySize = 0;			// ��ɫ������Ϊ��(Ĭ��:0��֧��16/24/32)
	tgaHeader.XOrigin = 0;						// ͼ��X�������ʼλ��(Ĭ��:0)
	tgaHeader.YOrigin = 0;						// ͼ��Y�������ʼλ��(Ĭ��:0)
	tgaHeader.ImageWidth = (WORD)width;			// ͼ��Ŀ��
	tgaHeader.ImageHeight = (WORD)height;		// ͼ��ĸ߶�
	tgaHeader.PixelBits = pixelBits;			// ͼ��ÿ���ش洢ռ��λ��
	tgaHeader.ImageDescruptor = 8;				// ͼ�������ַ��ֽ�(Ĭ��:0)

	FILE* pFile = fopen(szDstFile, "wb");
	assert(pFile != NULL);

	fwrite(&tgaHeader, sizeof(tgaHeader), 1, pFile);		// дtgaͼ���ļ�ͷ
	fwrite(pBmp, width*height*(pixelBits/8), 1, pFile);	// дͼ������
	fclose(pFile);

	return true;
}

// ��ѹ����TGA��������
void WuYuan::TGA_DeCompress(FILE* pFile, const STGA_HEADER* pTgaHeader, BYTE* pOutBmp32)
{
	assert(pFile != NULL);
	assert(pOutBmp32 != NULL);
	assert(pTgaHeader->ImageType == 10);
	assert(pTgaHeader->PixelBits==24 || pTgaHeader->PixelBits==32);

	BYTE*			pOutBmp			= pOutBmp32;
	unsigned int	pixelcount		= pTgaHeader->ImageHeight * pTgaHeader->ImageWidth;
	unsigned int	currentpixel	= 0;
	unsigned int	currentbyte		= 0;
	int				m_BytesPerPixel	= (pTgaHeader->PixelBits / 8);
	unsigned char	colorbuffer[4]	= { 0 };

	do
	{
		unsigned char chunkheader = 0;

		if (fread(&chunkheader, sizeof(unsigned char), 1, pFile) == 0)
			return;

		if (chunkheader < 128)
		{
			chunkheader++;
			for(short counter = 0; counter < chunkheader; counter++)
			{
				if (fread(colorbuffer, 1, m_BytesPerPixel, pFile) != m_BytesPerPixel)
					return;

				// write to memory
				pOutBmp[currentbyte	] = colorbuffer[0];
				pOutBmp[currentbyte + 1] = colorbuffer[1];
				pOutBmp[currentbyte + 2] = colorbuffer[2];

				if (m_BytesPerPixel == 4)
				{
					pOutBmp[currentbyte + 3] = colorbuffer[3];
				}

				currentbyte += m_BytesPerPixel;
				currentpixel++;

				if (currentpixel > pixelcount)
					return;
			}
		}
		else
		{
			chunkheader -= 127;
			if (fread(colorbuffer, 1, m_BytesPerPixel, pFile) != m_BytesPerPixel)
				return;

			for (short counter=0; counter<chunkheader; counter++)
			{
				pOutBmp[currentbyte	] = colorbuffer[0];
				pOutBmp[currentbyte + 1] = colorbuffer[1];
				pOutBmp[currentbyte + 2] = colorbuffer[2];

				if (m_BytesPerPixel == 4)
				{
					pOutBmp[currentbyte + 3] = colorbuffer[3];
				}

				currentbyte += m_BytesPerPixel;
				currentpixel++;

				if (currentpixel > pixelcount)
					return;
			}
		}
	} while(currentpixel < pixelcount);
}

// ��ȡbmp�ļ���Ϣ
FILE* WuYuan::BMP_OpenFile(const char* szFile, BITMAPINFO* pOutBmpInfo)
{
	assert(pOutBmpInfo != NULL);
	memset(pOutBmpInfo, 0, sizeof(BITMAPINFO));

	FILE* pFile = fopen(szFile, "rb");
	if (pFile == NULL)	return NULL;

	// ����λͼ�ļ�ͷ��
	BITMAPFILEHEADER bmpHeader;
	memset(&bmpHeader, 0, sizeof(bmpHeader));
	fread(&bmpHeader, sizeof(bmpHeader), 1, pFile);

	if (bmpHeader.bfType != 0x4D42)	return NULL;	// ����bmp�ļ�

	// ����λͼ��Ϣͷ��BITMAPINFO��bmiHeader��Ա����
	fread(&pOutBmpInfo->bmiHeader, sizeof(pOutBmpInfo->bmiHeader), 1, pFile);

	return pFile;
}

// ��bmp�ļ�ͼ��
// ��pDstBmp ָ��Ŀ��λͼ������
// ����GetBmpFileInfo()���ʹ��
bool WuYuan::BMP_ReadPixel(FILE* pFile, const BITMAPINFO* pBmpInfo, UINT* pOutBmp32)
{
	assert(pFile != NULL);
	assert(pBmpInfo != NULL);
	assert(pOutBmp32 != NULL);

	int pixelCount = pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biHeight;
	int bmpSize = pixelCount * (pBmpInfo->bmiHeader.biBitCount/8);
	BYTE* pOutBmp = (BYTE*)pOutBmp32;

	fseek(pFile, sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER), SEEK_SET);

	switch (pBmpInfo->bmiHeader.biBitCount)
	{
	case 8:
		{
			BYTE* pBmp8 = (BYTE*)malloc(bmpSize);
			fread(pBmp8, bmpSize, 1, pFile);

			BYTE* pSrc = pBmp8;
			BYTE* pDst = pOutBmp;

			for (int i=0; i<pixelCount; i++)
			{
				*pDst++ = *pSrc;	// b
				*pDst++ = *pSrc;	// g
				*pDst++ = *pSrc++;	// r
				*pDst++ = 255;		// alpha
			}

			free(pBmp8);
		}
		break;
	case 24:
		{
			BYTE* pBmp24 = (BYTE*)malloc(bmpSize);
			fread(pBmp24, bmpSize, 1, pFile);

			BYTE* pSrc = pBmp24;
			BYTE* pDst = pOutBmp;

			for (int i=0; i<pixelCount; i++)
			{
				*pDst++ = *pSrc++;	// b
				*pDst++ = *pSrc++;	// g
				*pDst++ = *pSrc++;	// r
				*pDst++ = 255;		// alpha
			}

			free(pBmp24);
		}
		break;
	case 32:
		fread(pOutBmp, bmpSize, 1, pFile);
		break;
	default:
		assert(false && "�ݲ�֧��");
		break;
	}

	return true;
}

bool WuYuan::BMP_SaveFile(const char* szFile, const void* pBmp, int width, int height, int bitCount)
{
	FILE* pFile = fopen(szFile, "wb");
	assert(pFile != NULL);

	int bmp_size = width*height*(bitCount/8);

	// ��дλͼ�ļ�ͷ��
	BITMAPFILEHEADER bmpHeader;
	bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmp_size;	// BMPͼ���ļ��Ĵ�С
	bmpHeader.bfType = 0x4D42;	// λͼ��𣬸��ݲ�ͬ�Ĳ���ϵͳ����ͬ����Windows�У����ֶε�ֵ��Ϊ��BM��
	bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);			// BMPͼ�����ݵ�ƫ��λ��
	bmpHeader.bfReserved1 = 0;	// ��Ϊ0
	bmpHeader.bfReserved2 = 0;	// ��Ϊ0
	fwrite(&bmpHeader, sizeof(bmpHeader), 1, pFile);

	// ��дλͼ��Ϣͷ��BITMAPINFO��bmiHeader��Ա����
	BITMAPINFOHEADER bmiHeader;
	bmiHeader.biSize = sizeof(bmiHeader);				// ���ṹ��ռ���ֽ�������sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = width;							// λͼ��ȣ���λ�����أ�
	bmiHeader.biHeight = height;						// λͼ�߶ȣ���λ�����أ�
	bmiHeader.biPlanes = 1;								// Ŀ���豸�ļ��𣬱���Ϊ1
	bmiHeader.biBitCount = bitCount;					// ���ص�λ����ÿ�����������λ������Χ��1��4��8��24��32��
	bmiHeader.biCompression = 0;						// ѹ�����ͣ�0����ѹ�� 1��BI_RLE8ѹ������ 2��BI_RLE4ѹ�����ͣ�
	bmiHeader.biSizeImage = bmp_size;					// λͼ��С����λ���ֽڣ�
	bmiHeader.biXPelsPerMeter = 0;						// ˮƽ�ֱ���(����/��)
	bmiHeader.biYPelsPerMeter = 0;						// ��ֱ�ֱ���(����/��)
	bmiHeader.biClrUsed = 0;							// λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
	bmiHeader.biClrImportant = 0;						// ��ͼ����ʾ����ҪӰ�����ɫ��������Ŀ
	fwrite(&bmiHeader, sizeof(bmiHeader), 1, pFile);

	// ��д�������ݡ�
	fwrite(pBmp, bmp_size, 1, pFile);
	fclose(pFile);

	return true;
}

// ת��bmp���صĴ洢˳����pSrcBmpΪ���϶��´洢����ת�������¶��ϣ���֮ͬ��
void WuYuan::ReverseBmp(void* pBmp, int width, int height, int bits/*=32*/)
{
	int pixel_bytes = bits/8;
	int pitch = width * pixel_bytes;
	int bmpSize = height * pitch;

	BYTE* pBuffer = (BYTE*)malloc(bmpSize);
	BYTE* pSrc = (BYTE*)pBmp + (height-1)*pitch;
	BYTE* pDst = pBuffer;

	for (int h=0; h<height; h++)
	{
		memcpy(pDst, pSrc, pitch);
		pSrc -= pitch;
		pDst += pitch;
	}

	memcpy(pBmp, pBuffer, bmpSize);
	free(pBuffer);
}

// ��ͼ��ת���ɻҶ�ͼ
void WuYuan::ConvertBmpToGray(UINT* pSrcBmp32, int width, int height)
{
	BYTE* pBmp8 = (BYTE*)pSrcBmp32;
	BYTE color;

	for (int h=0, w=0, widthBytes=width*4; h<height; h++)
	{
		for (w=0; w<widthBytes; w+=4)
		{
			color = BYTE(pBmp8[h*widthBytes+w]*0.114 + pBmp8[h*widthBytes+w+1]*0.587 + pBmp8[h*widthBytes+w+2]*0.299);
			pBmp8[h*widthBytes+w] = color;
			pBmp8[h*widthBytes+w+1] = color;
			pBmp8[h*widthBytes+w+2] = color;
		}
	}
}

// ��24λ����ת����32λ
// ע�⣺pDstBmp24������32λ�����ؿռ�
void WuYuan::Bmp24ToBmp32(void* pDstBmp24, int width, int height)
{
	int pixel_num = width*height;
	BYTE* p24 = (BYTE*)pDstBmp24 + (width*height*3) - 3;
	UINT* p32 = (UINT*)pDstBmp24;

	for (int i=pixel_num-1; i>=0; i--)
	{
		p32[i] = ((*(UINT*)p24) & 0xFFFFFF) | 0xFF000000;
		p24 -= 3;
	}

	assert(p24 == (BYTE*)pDstBmp24-3);
}