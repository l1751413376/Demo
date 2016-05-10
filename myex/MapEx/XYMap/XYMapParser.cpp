//----------------------------------------------------------------------------------------------------
// File  : XYMapParser.cpp
// Title : CXYMapParser 网易西游系列地图提取类(只支持梦幻和大话2)
// Date  : 2013-4-1
// Desc  : 
//
//
// Copyright(c) WeiWuYuan
//----------------------------------------------------------------------------------------------------
#include "XYMapParser.h"
#include <math.h>
#include "WYImgHelper.h"


//-------------------------------------------------------------------------
// jpg
#ifdef  __cplusplus
extern "C"
{
#endif
#include "jpeglib/jpeg.h"
#ifdef  __cplusplus
}
#endif

#ifdef _DEBUG
#pragma comment (lib,"jpeglib/debug/jpeglib.lib")
#else
#pragma comment (lib,"jpeglib/release/jpeglib.lib")
#endif
//-------------------------------------------------------------------------




CXYMapParser::CXYMapParser()
{
	m_pFile = NULL;
	memset(&m_FileHeader, 0, sizeof(SXYMap_Header));
	m_MapBlockRowNum = 0;
	m_MapBlockColNum = 0;

	m_pMapBmp = NULL;
	m_MapBmpWidth = 0;
	m_MapBmpHeight = 0;
	m_szSrcMapFileName[0] = 0;

	init_jpeg();
}

CXYMapParser::~CXYMapParser()
{
	Close();
}

bool CXYMapParser::Open(const char* szMapFile)
{
	Close();
	m_pFile = fopen(szMapFile, "rb");
	if (m_pFile == NULL)
		ErrorDialog("%s 文件未找到", szMapFile);

	//----------------------------------
	// 读文件头
	//----------------------------------
	fread(&m_FileHeader, sizeof(SXYMap_Header), 1, m_pFile);

	// 文件标志
	// 0x4D415058 (MAPX) 大话2旧地图
	// 0x4D312E30 (M1.0) 大话2新地图 梦幻地图
	// 0x4D322E35 (M2.5) 大话3内测版
	// 0x4D332E30 (M3.0) 大话3正式版
	// 0x524F4C30 (ROL0) 大话3地图背景文件
	switch (m_FileHeader.flag)
	{
	case 0x4D415058:
	case 0x4D312E30:
	case 0x4D322E35:
	case 0x4D332E30:
	case 0x524F4C30:
		break;
	default:
		ErrorDialog("%s 不是有效的map文件", szMapFile);
		break;
	}

	strcpy(m_szSrcMapFileName, szMapFile);

	// 注：因为有些地图的尺寸并一定可以被小地图尺寸整除，所以需要趋向最大取整
	m_MapBlockRowNum = ceil(m_FileHeader.map_height / (float)MAP_BLOCK_HEIGHT);
	m_MapBlockColNum = ceil(m_FileHeader.map_width / (float)MAP_BLOCK_WIDTH);

	m_MapBmpWidth = m_MapBlockColNum * MAP_BLOCK_WIDTH;
	m_MapBmpHeight = m_MapBlockRowNum * MAP_BLOCK_HEIGHT;
	m_pMapBmp = new WORD[m_MapBmpWidth * m_MapBmpWidth];

	//----------------------------------
	// 读地图块的偏移信息
	//----------------------------------
	int block_num = m_MapBlockRowNum * m_MapBlockColNum;
	UINT* pBlockOffset = new UINT[block_num];
	fread(pBlockOffset, sizeof(UINT)*block_num, 1, m_pFile);

	//----------------------------------
	// 读SMaskHead数据
	//----------------------------------
	SMaskHead maskHead = { 0 };
	fread(&maskHead, sizeof(SMaskHead), 1, m_pFile);

	//----------------------------------
	// 读mask数据的偏移信息
	//----------------------------------
	UINT* pMaskDataOffset = new UINT[maskHead.mask_num];
	fread(pMaskDataOffset, sizeof(UINT)*maskHead.mask_num, 1, m_pFile);

	//----------------------------------
	// 读取所有数据块
	//----------------------------------
	for (int i=0; i<block_num; i++)
		ReadBlock(i, pBlockOffset[i]);

	//----------------------------------
	// 读取所有mask数据
	//----------------------------------
	for (int i = 0; i<maskHead.mask_num; i++)
		ReadMask(pMaskDataOffset[i], i);

	// 将地图保存为TGA图片
	{
		char szTgaFile[256];
		sprintf(szTgaFile, "%s.tga", szMapFile);
		WuYuan::TGA_SaveFile(szTgaFile, m_pMapBmp, m_MapBmpWidth, m_MapBmpHeight, 16);
	}

	if (pMaskDataOffset != NULL)
	{
		delete [] pMaskDataOffset;
		pMaskDataOffset = NULL;
	}
	if (pBlockOffset != NULL)
	{
		delete [] pBlockOffset;
		pBlockOffset = NULL;
	}

	return true;
}

void CXYMapParser::Close()
{
	if (m_pMapBmp != NULL)
	{
		delete [] m_pMapBmp;
		m_pMapBmp = NULL;
	}
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	memset(&m_FileHeader, 0, sizeof(SXYMap_Header));
}

void CXYMapParser::ReadBlock(int block_index, int block_offset)
{
	SBlockInfo blockInfo = { 0 };
	BYTE* pUnitNData = NULL;

	fseek(m_pFile, block_offset, SEEK_SET);
	fread(&blockInfo.UnitN, sizeof(UINT), 1, m_pFile);

	if (blockInfo.UnitN > 0)
	{
		pUnitNData = new BYTE[blockInfo.UnitN*4];
		fread(pUnitNData, blockInfo.UnitN*4, 1, m_pFile);
	}

	fread(&blockInfo.flag, 8, 1, m_pFile);

	switch (blockInfo.flag)
	{
		// GEPJ "47 45 50 4A"
	case 0x4A504547:
		ReadJPEG(block_index, blockInfo.size);
		break;
		// LLEC "4C 4C 45 43"
	case 0x43454C4C:
	//	ReadCELL(Mapfile, unit[index].UnitFlag, unit[index].Size, index);
		break;
		// GIRB "47 49 52 42"
	case 0x42524947:
	//	ReadBRIG(Mapfile, unit[index].UnitFlag, unit[index].Size, index);
		break;
	default:
		// 错误标志
		break;
	}

	if (pUnitNData != NULL)
	{
		delete [] pUnitNData;
		pUnitNData = NULL;
	}
}

void CXYMapParser::ReadJPEG(int block_index, int size)
{
	BYTE* pMapJpgData = new BYTE[size];
	fread(pMapJpgData, size, 1, m_pFile);

	UINT jpg_size = 0;
	BYTE* pJpgData = JpgHandler(pMapJpgData, size, &jpg_size);

// 	{
// 		FILE* pFile = fopen("t.jpg", "wb");
// 		fwrite(jpgdata, TempSize, 1, pFile);
// 		fclose(pFile);
// 	}

	// 解压jpg
	BMP* pBmpData = Unpak_jpg(pJpgData, jpg_size);	// 解压后的像素格式为ARGB1555

	// 将解压后的16位bmp写到m_pMapBmp中
	WriteMapPixel(block_index, pBmpData->line[0], pBmpData->w, pBmpData->h, pBmpData->pitch);

	destroy_bitmap(pBmpData);
	delete [] pJpgData;
	delete [] pMapJpgData;
}

void CXYMapParser::ReadMask(int offset, int mask_index)
{
	SMaskInfo maskInfo = { 0 };

	// 读mask信息
	fseek(m_pFile, offset, SEEK_SET);
	fread(&maskInfo, sizeof(SMaskInfo), 1, m_pFile);

	if (maskInfo.width==0 || maskInfo.height==0 || maskInfo.mask_size==0)
		return;

	// 读mask数据
	BYTE* pMaskData = new BYTE[maskInfo.mask_size];
	fread(pMaskData, maskInfo.mask_size, 1, m_pFile);

	// 解密mask数据
	int align_width = (maskInfo.width / 4 + (maskInfo.width % 4 != 0)) * 4;	// 以4对齐的宽度
	BYTE* pMaskDataDec = new BYTE[align_width * maskInfo.height / 4];		// 1个字节4个像素，故要除以4
	int dec_mask_size = DecompressMask(pMaskData, pMaskDataDec);

	// 输出mask数据到文件
	{
		// 将mask数据转换成TGA图像
		{
			int pixel_num = maskInfo.width * maskInfo.height;
			UINT* pOutMaskBmp = new UINT[pixel_num];
			memset(pOutMaskBmp, 0, sizeof(UINT) * pixel_num);

			// 提取mask像素
			for (int h=0; h<maskInfo.height; h++)
			{
				for (int w=0; w<maskInfo.width; w++)
				{
					int mask_index = (h * align_width + w) * 2;		// 单位:位
 					BYTE mask_value = pMaskDataDec[mask_index/8];	// 定位到字节
 					mask_value = mask_value >> (mask_index % 8);	// 定位到位

					if ((mask_value & 3) == 3)
					{
						int mapX = maskInfo.startX + w;	// 地图图像中的X位置
						int mapY = maskInfo.startY + h;	// 地图图像中的Y位置

						WORD dstPixel = m_pMapBmp[(m_MapBmpHeight-1-mapY)*m_MapBmpWidth + mapX];
						pOutMaskBmp[(maskInfo.height-1-h)*maskInfo.width + w] = 0xFF000000 | ((dstPixel & 0x7C00) << 9) | ((dstPixel & 0x3E0) << 6) | ((dstPixel & 0x1F) << 3);
					}
				}
			}

			char szDstFileName[256];
			sprintf(szDstFileName, "%s_mask\\%03d.tga", m_szSrcMapFileName, mask_index);
			WuYuan::TGA_SaveFile(szDstFileName, pOutMaskBmp, maskInfo.width, maskInfo.height);

			delete [] pOutMaskBmp;

			// 原作者代码
// 			int align_width = (maskInfo.width / 4 + (maskInfo.width % 4 != 0)) * 4;	// 以4对齐的宽度
// 
// 			int* buffer = new int[maskInfo.width*maskInfo.height];
// 			for (int i=0; i<maskInfo.height; i++)
// 			{
// 				for (int j=0; j<maskInfo.width; j++)
// 				{
// 					int mask_index = (i * align_width + j) * 2;
// 					BYTE mask_value = pMaskDataDec[mask_index/8];
// 					mask_value = (mask_value >> (mask_index % 8)) & 3;
//
// 					int color[] = { 0x80FF0000, 0x8000FF00, 0x800000FF, 0x80FFFF00 };
// 					buffer[i*maskInfo.width + j] = color[mask_value];
// 				}
// 			}
// 
// 			char szDstFileName[256];
// 			sprintf(szDstFileName, "%s_mask\\%03d.tga", m_szSrcMapFileName, mask_index);
// 			WuYuan::TGA_SaveFile(szDstFileName, buffer, maskInfo.width, maskInfo.height);
// 			delete [] buffer;
		}

		// 输出描述信息到ini文件
		{
			char szDstFileName[256];
			sprintf(szDstFileName, "%s_mask\\mask_info.ini", m_szSrcMapFileName);
			FILE* pFile = fopen(szDstFileName, "ab+");

			char szString[256];
			sprintf(szString, "mask_%03d_x_y_w_h = %4d,%4d, %3d,%3d\r\n", mask_index, maskInfo.startX, maskInfo.startY, maskInfo.width, maskInfo.height);
			fwrite(szString, strlen(szString), 1, pFile);
			fclose(pFile);
		}
	}

	if (pMaskDataDec != NULL)
	{
		delete [] pMaskDataDec;
		pMaskDataDec = NULL;
	}
	if (pMaskData != NULL)
	{
		delete [] pMaskData;
		pMaskData = NULL;
	}
}

// 地图的JPEG数据处理器
BYTE* CXYMapParser::JpgHandler(BYTE* Buffer, int inSize, UINT* outSize)
{
	// JPEG数据处理原理
	// 1、复制D8到D9的数据到缓冲区中
	// 2、删除第3、4个字节 FFA0
	// 3、修改FFDA的长度00 09 为 00 0C
	// 4、在FFDA数据的最后添加00 3F 00
	// 5、替换FFDA到FF D9之间的FF数据为FF 00

	UINT TempNum = 0;						// 临时变量，表示已读取的长度
	BYTE* outBuffer;
	BYTE* TempBuffer = new BYTE[inSize*2];		// 临时变量，表示处理后的数据
	memset(TempBuffer,0,inSize*2);
	outBuffer = TempBuffer;					// 已处理数据的开始地址
	WORD TempTimes = 0;					// 临时变量，表示循环的次数
	UINT Temp = 0;

	// 当已读取数据的长度小于总长度时继续
	while(TempNum < inSize && *Buffer++==0xFF)
	{
		*TempBuffer++=0xFF;
		TempNum++;
		switch(*Buffer)
		{
		case 0xD8:
			*TempBuffer++=0xD8;
			*Buffer++;
			TempNum++;
			break;
		case 0xA0:
			*Buffer++;
			TempBuffer--;
			TempNum++;
			break;
		case 0xC0:
			*TempBuffer++=0xC0;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序


			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}

			break;
		case 0xC4:
			*TempBuffer++=0xC4;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			break;
		case 0xDB:
			*TempBuffer++=0xDB;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			break;
		case 0xDA:
			*TempBuffer++=0xDA;
			*TempBuffer++=0x00;
			*TempBuffer++=0x0C;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序
			*Buffer++;
			TempNum++;
			*Buffer++;

			for(int i=2;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			*TempBuffer++=0x00;
			*TempBuffer++=0x3F;
			*TempBuffer++=0x00;
			Temp += 1; // 这里应该是+3的，因为前面的0xFFA0没有-2，所以这里只+1。

			// 循环处理0xFFDA到0xFFD9之间所有的0xFF替换为0xFF00
			for (;TempNum < inSize-2;)
			{
				if (*Buffer==0xFF)
				{
					*TempBuffer++=0xFF;
					*TempBuffer++=0x00;
					*Buffer++;
					TempNum++;	
					Temp ++;
				}
				else
				{
					*TempBuffer++=*Buffer++;
					TempNum++;
				}
			}
			// 直接在这里写上了0xFFD9结束Jpeg图片.
			Temp--; // 这里多了一个字节，所以减去。
			*TempBuffer--;
			*TempBuffer--=0xD9;
			break;
		case 0xD9:
			// 算法问题，这里不会被执行，但结果一样。
			*TempBuffer++=0xD9;
			TempNum++;
			break;
		default:
			break;
		}
	}
	Temp+=inSize;
	*outSize=Temp;
	return outBuffer;
}

int CXYMapParser::DecompressMask(void* in , void* out)
{
	register BYTE *op;
	register  BYTE *ip;
	register unsigned t;
	register  BYTE *m_pos;

	op = (BYTE *) out;
	ip = (BYTE *) in;

	if (*ip > 17) {
		t = *ip++ - 17;
		if (t < 4)
			goto match_next;
		do *op++ = *ip++; while (--t > 0);
		goto first_literal_run;
	}

	while (1) {
		t = *ip++;
		if (t >= 16) goto match;
		if (t == 0) {
			while (*ip == 0) {
				t += 255;
				ip++;
			}
			t += 15 + *ip++;
		}

		* (unsigned *) op = * ( unsigned *) ip;
		op += 4; ip += 4;
		if (--t > 0)
		{
			if (t >= 4)
			{
				do {
					* (unsigned * ) op = * ( unsigned * ) ip;
					op += 4; ip += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *ip++; while (--t > 0);
			}
			else do *op++ = *ip++; while (--t > 0);
		}

first_literal_run:

		t = *ip++;
		if (t >= 16)
			goto match;

		m_pos = op - 0x0801;
		m_pos -= t >> 2;
		m_pos -= *ip++ << 2;

		*op++ = *m_pos++; *op++ = *m_pos++; *op++ = *m_pos;

		goto match_done;

		while (1)
		{
match:
			if (t >= 64)
			{

				m_pos = op - 1;
				m_pos -= (t >> 2) & 7;
				m_pos -= *ip++ << 3;
				t = (t >> 5) - 1;

				goto copy_match;

			}
			else if (t >= 32)
			{
				t &= 31;
				if (t == 0)	{
					while (*ip == 0) {
						t += 255;
						ip++;
					}
					t += 31 + *ip++;
				}

				m_pos = op - 1;
				m_pos -= (* ( unsigned short * ) ip) >> 2;
				ip += 2;
			}
			else if (t >= 16) {
				m_pos = op;
				m_pos -= (t & 8) << 11;
				t &= 7;
				if (t == 0) {
					while (*ip == 0) {
						t += 255;
						ip++;
					}
					t += 7 + *ip++;
				}
				m_pos -= (* ( unsigned short *) ip) >> 2;
				ip += 2;
				if (m_pos == op)
					goto eof_found;
				m_pos -= 0x4000;
			}
			else {
				m_pos = op - 1;
				m_pos -= t >> 2;
				m_pos -= *ip++ << 2;
				*op++ = *m_pos++; *op++ = *m_pos;
				goto match_done;
			}

			if (t >= 6 && (op - m_pos) >= 4) {
				* (unsigned *) op = * ( unsigned *) m_pos;
				op += 4; m_pos += 4; t -= 2;
				do {
					* (unsigned *) op = * ( unsigned *) m_pos;
					op += 4; m_pos += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *m_pos++; while (--t > 0);
			}
			else {
copy_match:
				*op++ = *m_pos++; *op++ = *m_pos++;
				do *op++ = *m_pos++; while (--t > 0);
			}

match_done:

			t = ip[-2] & 3;
			if (t == 0)	break;

match_next:
			do *op++ = *ip++; while (--t > 0);
			t = *ip++;
		}
	}

eof_found:
	//   if (ip != ip_end) return -1;
	return (op - (BYTE*)out);
}

// 2字节高低位调换
void CXYMapParser::ByteSwap(WORD& value)
{
	WORD tempvalue = value >> 8;
	value = (value << 8) | tempvalue;
}

void CXYMapParser::WriteMapPixel(int block_index, WORD* pBmp16, int width, int height, int pitch)
{
	int map_row = block_index / m_MapBlockColNum;
	int map_col = block_index % m_MapBlockColNum;
	int dst_x = map_col * MAP_BLOCK_WIDTH;
	int dst_y = map_row * MAP_BLOCK_HEIGHT;

	WORD* pSrc = pBmp16;
	WORD* pDst = m_pMapBmp + (m_MapBmpHeight-1-dst_y)*m_MapBmpWidth + dst_x;
	int dst_pitch = -m_MapBmpWidth;

	for (int h=0; h<height; h++)
	{
		memcpy(pDst, pSrc, pitch);
		pSrc += width;
		pDst += dst_pitch;
	}
}

void CXYMapParser::ErrorDialog(const char* szFormat, ...)
{
	if (szFormat == NULL)	return;

	char szInfo[1024] = { 0 };

	// 获取信息
	{
		va_list argPtr;
		va_start(argPtr, szFormat);
		vsprintf(szInfo, szFormat, argPtr);	// 将格式化符转换为最终的字符串
		va_end(argPtr);
	}

	MessageBoxA(0, szInfo, "提示", MB_OK);
	exit(0);
}