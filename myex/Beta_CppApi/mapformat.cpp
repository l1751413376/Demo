#include "mapformat.h"
#include <math.h>

#include "jpeg.h"
#pragma comment (lib,"../Debug/jpeglib.lib")
//-------------------------------------------------------------------------



using namespace map_filespace;
bool TGA_SaveFile(const char* szDstFile, const void* pBmp, int width, int height, int pixelBits=32);

void mapfile::load_file(const char* path)
{
	open((char*)path);
	load_data(pfhead);
	close();
}

void mapfile::load_data(BYTE * data)
{
	pfmoving=pfhead=data;
	read(&map_header,sizeof(SXYMap_Header));

	// 文件标志
	// 0x4D415058 (MAPX) 大话2旧地图
	// 0x4D312E30 (M1.0) 大话2新地图 梦幻地图
	// 0x4D322E35 (M2.5) 大话3内测版
	// 0x4D332E30 (M3.0) 大话3正式版
	// 0x524F4C30 (ROL0) 大话3地图背景文件
	switch (map_header.flag)
	{
	case 0x4D415058:
	case 0x4D312E30:
	case 0x4D322E35:
	case 0x4D332E30:
	case 0x524F4C30:
		break;
	default:
		//ErrorDialog("%s 不是有效的map文件", szMapFile);
		return;
	}

	// 注：因为有些地图的尺寸并一定可以被小地图尺寸整除，所以需要趋向最大取整
	m_MapBlockRowNum =(int)ceil(map_header.map_height / (float)MAP_BLOCK_HEIGHT);
	m_MapBlockColNum =(int)ceil(map_header.map_width / (float)MAP_BLOCK_WIDTH);

	m_MapBmpWidth = m_MapBlockColNum * MAP_BLOCK_WIDTH;
	m_MapBmpHeight = m_MapBlockRowNum * MAP_BLOCK_HEIGHT;
	m_pMapBmp = new WORD[m_MapBmpWidth * m_MapBmpWidth];

	//----------------------------------
	// 读地图块的偏移信息
	//----------------------------------
	int block_num = m_MapBlockRowNum * m_MapBlockColNum;
	DWORD* pBlockOffset = new DWORD[block_num];
	read(pBlockOffset, sizeof(DWORD)*block_num);

	//----------------------------------
	// 读SMaskHead数据
	//----------------------------------
	SMaskHead maskHead = { 0 };
	read(&maskHead, sizeof(SMaskHead));

	//----------------------------------
	// 读mask数据的偏移信息
	//----------------------------------
	DWORD* pMaskDataOffset = new DWORD[maskHead.mask_num];
	read(pMaskDataOffset, sizeof(DWORD)*maskHead.mask_num);

	//----------------------------------
	// 读取所有数据块
	//----------------------------------
	for (int i=0; i<block_num; i++)
		ReadBlock(i, pBlockOffset[i]);

	//----------------------------------
	// 读取所有mask数据
	//----------------------------------
	for (DWORD i = 0; i<maskHead.mask_num; i++)
		ReadMask(pMaskDataOffset[i], i);
}
void mapfile::ReadMask(int offset, int mask_index)
{
	SMaskInfo maskInfo = { 0 };

	// 读mask信息
	seek(offset);
	read(&maskInfo, sizeof(SMaskInfo));

	if (maskInfo.width==0 || maskInfo.height==0 || maskInfo.mask_size==0)
		return;

	// 读mask数据
	BYTE* pMaskData = new BYTE[maskInfo.mask_size];
	read(pMaskData, maskInfo.mask_size);

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

			char szDstFileName[512];
			sprintf_s(szDstFileName, "%s_mask\\%03d.tga", m_szSrcMapFileName, mask_index);
			TGA_SaveFile(szDstFileName, pOutMaskBmp, maskInfo.width, maskInfo.height);

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
			sprintf_s(szDstFileName, "%s_mask\\mask_info.ini", m_szSrcMapFileName);
			FILE* pFile;
			fopen_s(&pFile,szDstFileName, "ab+");

			char szString[256];
			sprintf_s(szString, "mask_%03d_x_y_w_h = %4d,%4d, %3d,%3d\r\n", mask_index, maskInfo.startX, maskInfo.startY, maskInfo.width, maskInfo.height);
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
int  mapfile::DecompressMask(void* in , void* out)
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

void mapfile::ReadBlock(int block_index, int block_offset){
	SBlockInfo blockInfo = { 0 };
	BYTE* pUnitNData = NULL;

	seek(block_offset);

	read(&blockInfo.UnitN, sizeof(DWORD));

	if (blockInfo.UnitN > 0)
	{
		pUnitNData = new BYTE[blockInfo.UnitN * 4];
		read(pUnitNData, blockInfo.UnitN * 4);
	}

	read(&blockInfo.flag, 8);

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
		delete[] pUnitNData;
		pUnitNData = NULL;
	}
}

void mapfile::ReadJPEG(int block_index, int size)
{
	BYTE* pMapJpgData = new BYTE[size];
	read(pMapJpgData, size);

	UINT jpg_size = 0;
	BYTE* pJpgData = JpgHandler(pMapJpgData, size, &jpg_size);

	// 	{
	// 		FILE* pFile = fopen("t.jpg", "wb");
	// 		fwrite(jpgdata, TempSize, 1, pFile);
	// 		fclose(pFile);
	// 	}

	// 解压jpg
	BMP* pBmpData = 0;//Unpak_jpg(pJpgData, jpg_size);	// 解压后的像素格式为ARGB1555

	// 将解压后的16位bmp写到m_pMapBmp中
	WriteMapPixel(block_index, pBmpData->line[0], pBmpData->w, pBmpData->h, pBmpData->pitch);

	destroy_bitmap(pBmpData);
	delete [] pJpgData;
	delete [] pMapJpgData;
}

// 地图的JPEG数据处理器
BYTE* mapfile::JpgHandler(BYTE* Buffer, UINT inSize, UINT* outSize)
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
void  mapfile::WriteMapPixel(int block_index, WORD* pBmp16, int width, int height, int pitch)
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
// 保存tga格式文件
bool TGA_SaveFile(const char* szDstFile, const void* pBmp, int width, int height, int pixelBits)
{
	assert(pixelBits==8 || pixelBits==16 || pixelBits==24 || pixelBits==32);

	// 给输出文件创建所需的全部路径
	{
		char szPath[MAX_PATH] = { 0 };
		for (int i=0, begin=0, len=int(strlen(szDstFile)); i<len; i++)
		{
			if (szDstFile[i]=='\\' || szDstFile[i]=='/')
			{
				memcpy(szPath, szDstFile, i);
				szPath[i+1] = '\0';
				CreateDirectoryA(szPath, NULL);	// 创建目录
			}
		}
	}

	STGA_HEADER tgaHeader;
	memset(&tgaHeader, 0, sizeof(STGA_HEADER));

	tgaHeader.IdLength = 0;						// 图像信息字段(默认:0)
	tgaHeader.ColorMapType = 0;					// 颜色表的类型(默认0)
	tgaHeader.ImageType = (pixelBits==8? 3 : 2);// 图像类型码(支持2或10)
	tgaHeader.ColorMapFirstIndex = 0;			// 颜色表的索引(默认:0)
	tgaHeader.ColorMapLength = 0;				// 颜色表的长度(默认:0)
	tgaHeader.ColorMapEntrySize = 0;			// 颜色表表项的为数(默认:0，支持16/24/32)
	tgaHeader.XOrigin = 0;						// 图像X坐标的起始位置(默认:0)
	tgaHeader.YOrigin = 0;						// 图像Y坐标的起始位置(默认:0)
	tgaHeader.ImageWidth = (WORD)width;			// 图像的宽度
	tgaHeader.ImageHeight = (WORD)height;		// 图像的高度
	tgaHeader.PixelBits = pixelBits;			// 图像每像素存储占用位数
	tgaHeader.ImageDescruptor = 8;				// 图像描述字符字节(默认:0)
	FILE* pFile;
	fopen_s(&pFile,szDstFile, "wb");
	assert(pFile != NULL);

	fwrite(&tgaHeader, sizeof(tgaHeader), 1, pFile);		// 写tga图像文件头
	fwrite(pBmp, width*height*(pixelBits/8), 1, pFile);	// 写图像数据
	fclose(pFile);

	return true;
}

void tagfile::load_file(const char* path)
{
	open((char* )path);
	//处理数据
	load_data(pfhead);
	close();
}

void tagfile::load_data(BYTE * data,bool bUpToDown)
{
	read(&header,sizeof(STGA_HEADER));

	if (header.ImageType!=2 && header.ImageType!=10 && header.ImageType!=3) 
		return;
	if (header.ColorMapType != 0)
		return;
	bmp32 = new UINT[header.ImageWidth*header.ImageHeight];
	if (header.IdLength > 0)
		seek(header.IdLength,SEEK_CUR);	// skip TARGA image comment

	BYTE* bmp = (BYTE*)bmp32;


	switch (header.ImageType)
	{
	case 2:	// 无压缩图像
	case 3:	// 8位的黑白图像
		{
			int pixel_bytes = header.PixelBits/8;
			int bmpSize = header.ImageWidth*header.ImageHeight*pixel_bytes;
			int src_pitch = header.ImageWidth * pixel_bytes;
			bUpToDown = true;
			// 若“pTgaHeader->ImageDescruptor & 0x20”为真，则是自上而下存储的图像
			if ((int)bUpToDown != (header.ImageDescruptor & 0x20))
				src_pitch = -src_pitch;

			switch (header.PixelBits)
			{
			case 8:
				{
					BYTE* pBmp8 = (BYTE*)malloc(bmpSize);
					read(pBmp8, bmpSize);

					BYTE* pSrc = pBmp8;
					BYTE* pDst = bmp;
					if (src_pitch < 0)
						pSrc += (header.ImageHeight-1)*(-src_pitch);

					for (int h=0, w=0; h<header.ImageHeight; h++)
					{
						BYTE* pLine = pSrc;
						for (w=0; w<header.ImageWidth; w++)
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
					read(pBmp16, bmpSize);

					WORD* pSrc = (WORD*)pBmp16;
					BYTE* pDst = bmp;
					if (src_pitch < 0)
						pSrc += (header.ImageHeight-1)*(-src_pitch);
					BYTE r, g, b;
					// 对于16位TGA，ImageDescruptor的0~3位的值：0表示alpha无效 1表示alpha有效
					if (header.ImageDescruptor & 0x1)
					{
						for (int h=0; h<header.ImageHeight; h++)
						{
							WORD* pLine = pSrc;
							for (int w=0; w<header.ImageWidth; w++)
							{
								b = (*pLine) & 0x1F;
								g = ((*pLine)>>5) & 0x1F;
								r = ((*pLine)>>10) & 0x1F;
								*pDst++ = (b<<3) | (b>>2);		// b
								*pDst++ = (g<<3) | (g>>2);		// g
								*pDst++ = (r<<3) | (r>>2);		// r
								*pDst++ = ((*pLine)>>15)==0?0:255;	// alpha
								pLine++;
							}
							pSrc += src_pitch;
						}
					}
					else
					{
						for (int h=0; h<header.ImageHeight; h++)
						{
							WORD* pLine = pSrc;
							for (int w=0; w<header.ImageWidth; w++)
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
					read(pBmp24, bmpSize);

					BYTE* pSrc = pBmp24;
					BYTE* pDst = bmp;
					if (src_pitch < 0)
						pSrc += (header.ImageHeight-1)*(-src_pitch);

					for (int h=0, w=0; h<header.ImageHeight; h++)
					{
						BYTE* pLine = pSrc;
						for (w=0; w<header.ImageWidth; w++)
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
						read(bmp, bmpSize);
					}
					else
					{
						int src_pitch_8 = -src_pitch;
						src_pitch /= pixel_bytes;

						UINT* pBmp32 = (UINT*)malloc(bmpSize);
						read(pBmp32, bmpSize);

						UINT* pSrc = pBmp32 + (header.ImageHeight-1)*(-src_pitch);
						UINT* pDst = (UINT*)bmp;

						for (int h=0; h<header.ImageHeight; h++)
						{
							memcpy(pDst, pSrc, src_pitch_8);
							pSrc += src_pitch;
							pDst += header.ImageWidth;
						}

						free(pBmp32);
					}
				}
				break;
			}
		}
		break;
		// RLE压缩类型
	case 10:
		{
			if (header.PixelBits < 24)
			{
				MessageBoxA(0, "不支持低于24位的TGA-RLE压缩格式", 0, MB_OK);
			}
			else
			{
				TGA_DeCompress();

				// 若“pTgaHeader->ImageDescruptor & 0x20”为真，则是自上而下存储的图像
				if (bUpToDown != ((header.ImageDescruptor & 0x20)!=0))
					ReverseBmp(bmp, header.ImageWidth, header.ImageHeight, header.PixelBits);

				if (header.PixelBits == 24)
					Bmp24ToBmp32(bmp, header.ImageWidth, header.ImageHeight);
			}
		}
		break;
	}
}

// 读压缩的TGA像素数据
void tagfile::TGA_DeCompress()
{
	BYTE*			pOutBmp			=(BYTE*)bmp32;
	unsigned int	pixelcount		= header.ImageHeight * header.ImageWidth;
	unsigned int	currentpixel	= 0;
	unsigned int	currentbyte		= 0;
	int				m_BytesPerPixel	= (header.PixelBits / 8);
	unsigned char	colorbuffer[4]	= { 0 };

	do
	{
		unsigned char chunkheader = 0;

		if (read(&chunkheader, sizeof(unsigned char)) == 0)
			return;

		if (chunkheader < 128)
		{
			chunkheader++;
			for(short counter = 0; counter < chunkheader; counter++)
			{
				if (read(colorbuffer, 1, m_BytesPerPixel) != m_BytesPerPixel)
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
			if (read(colorbuffer, 1, m_BytesPerPixel) != m_BytesPerPixel)
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

// 转换bmp像素的存储顺序，若pSrcBmp为自上而下存储，则转换成自下而上，反之同理
void tagfile::ReverseBmp(void* pBmp, int width, int height, int bits/*=32*/)
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

// 注意：pDstBmp24必须是32位的像素空间
void tagfile::Bmp24ToBmp32(void* pDstBmp24, int width, int height)
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


