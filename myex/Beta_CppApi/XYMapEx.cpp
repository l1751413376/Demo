#include "XYMapEx.h"

#ifdef  __cplusplus
extern "C" {
#endif
#include "JPEG.H"
#ifdef  __cplusplus
}
#endif

#ifdef _DEBUG
#pragma comment (lib,"../Debug/jpeglib.lib")
#else
#pragma comment (lib,"../Debug/jpeglib.lib")
#endif



//-------------------------------------------------------------------------
#define MAP_BLOCK_HEIGHT 240.0
#define MAP_BLOCK_WIDTH  320.0
//mask解码
int DecompressMask(void* in, void* out)
{
	register BYTE *op;
	register  BYTE *ip;
	register unsigned t;
	register  BYTE *m_pos;

	op = (BYTE *)out;
	ip = (BYTE *)in;

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

		*(unsigned *)op = *(unsigned *)ip;
		op += 4; ip += 4;
		if (--t > 0)
		{
			if (t >= 4)
			{
				do {
					*(unsigned *)op = *(unsigned *)ip;
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
				if (t == 0) {
					while (*ip == 0) {
						t += 255;
						ip++;
					}
					t += 31 + *ip++;
				}

				m_pos = op - 1;
				m_pos -= (*(unsigned short *)ip) >> 2;
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
				m_pos -= (*(unsigned short *)ip) >> 2;
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
				*(unsigned *)op = *(unsigned *)m_pos;
				op += 4; m_pos += 4; t -= 2;
				do {
					*(unsigned *)op = *(unsigned *)m_pos;
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
void ByteSwap(WORD& value)
{
	WORD tempvalue = value >> 8;
	value = (value << 8) | tempvalue;
}
// 地图的JPEG数据处理器
BYTE* JpgHandler(BYTE* Buffer, int inSize, UINT* outSize)
{
	// JPEG数据处理原理
	// 1、复制D8到D9的数据到缓冲区中
	// 2、删除第3、4个字节 FFA0
	// 3、修改FFDA的长度00 09 为 00 0C
	// 4、在FFDA数据的最后添加00 3F 00
	// 5、替换FFDA到FF D9之间的FF数据为FF 00

	UINT TempNum = 0;						// 临时变量，表示已读取的长度
	BYTE* outBuffer;
	BYTE* TempBuffer = new BYTE[inSize * 2];		// 临时变量，表示处理后的数据
	memset(TempBuffer, 0, inSize * 2);
	outBuffer = TempBuffer;					// 已处理数据的开始地址
	WORD TempTimes = 0;					// 临时变量，表示循环的次数
	UINT Temp = 0;

	// 当已读取数据的长度小于总长度时继续
	while (TempNum < inSize && *Buffer++ == 0xFF)
	{
		*TempBuffer++ = 0xFF;
		TempNum++;
		switch (*Buffer)
		{
		case 0xD8:
			*TempBuffer++ = 0xD8;
			*Buffer++;
			TempNum++;
			break;
		case 0xA0:
			*Buffer++;
			TempBuffer--;
			TempNum++;
			break;
		case 0xC0:
			*TempBuffer++ = 0xC0;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序


			for (int i = 0; i < TempTimes; i++)
			{
				*TempBuffer++ = *Buffer++;
				TempNum++;
			}

			break;
		case 0xC4:
			*TempBuffer++ = 0xC4;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for (int i = 0; i < TempTimes; i++)
			{
				*TempBuffer++ = *Buffer++;
				TempNum++;
			}
			break;
		case 0xDB:
			*TempBuffer++ = 0xDB;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for (int i = 0; i < TempTimes; i++)
			{
				*TempBuffer++ = *Buffer++;
				TempNum++;
			}
			break;
		case 0xDA:
			*TempBuffer++ = 0xDA;
			*TempBuffer++ = 0x00;
			*TempBuffer++ = 0x0C;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序
			*Buffer++;
			TempNum++;
			*Buffer++;

			for (int i = 2; i < TempTimes; i++)
			{
				*TempBuffer++ = *Buffer++;
				TempNum++;
			}
			*TempBuffer++ = 0x00;
			*TempBuffer++ = 0x3F;
			*TempBuffer++ = 0x00;
			Temp += 1; // 这里应该是+3的，因为前面的0xFFA0没有-2，所以这里只+1。

					   // 循环处理0xFFDA到0xFFD9之间所有的0xFF替换为0xFF00
			for (; TempNum < inSize - 2;)
			{
				if (*Buffer == 0xFF)
				{
					*TempBuffer++ = 0xFF;
					*TempBuffer++ = 0x00;
					*Buffer++;
					TempNum++;
					Temp++;
				}
				else
				{
					*TempBuffer++ = *Buffer++;
					TempNum++;
				}
			}
			// 直接在这里写上了0xFFD9结束Jpeg图片.
			Temp--; // 这里多了一个字节，所以减去。
			*TempBuffer--;
			*TempBuffer-- = 0xD9;
			break;
		case 0xD9:
			// 算法问题，这里不会被执行，但结果一样。
			*TempBuffer++ = 0xD9;
			TempNum++;
			break;
		default:
			break;
		}
	}
	Temp += inSize;
	*outSize = Temp;
	return outBuffer;
}





void WriteMapPixel(BMP* bmp, int map_row, int map_col, MapInfo* mapInfo)
{
	int dst_x = map_col * MAP_BLOCK_WIDTH;
	int dst_y = map_row * MAP_BLOCK_HEIGHT;

	WORD* pSrc = bmp->line[0];
	WORD* pDst = (WORD*)mapInfo->MapImg + (dst_y)*mapInfo->m_MapBmpWidth + dst_x;
	int dst_pitch = (int)mapInfo->m_MapBmpWidth;

	for (int h = 0; h < bmp->h; h++)
	{
		memcpy(pDst, pSrc, bmp->pitch);
		pSrc += bmp->w;
		pDst += dst_pitch;
	}
}
void LoadMap(const char * FileName, MapInfo &mapInfo)
{
	strcpy(mapInfo.FileName, FileName);
	__file file;
	file.open(FileName);
	mapInfo.Flag = file.read_dword();
	mapInfo.Width = file.read_dword();
	mapInfo.Height = file.read_dword();

	//读取地图块
	int m_MapBlockRowNum = ceil(mapInfo.Height / MAP_BLOCK_HEIGHT);
	int m_MapBlockColNum = ceil(mapInfo.Width / MAP_BLOCK_WIDTH);
	mapInfo.m_MapBmpWidth = m_MapBlockColNum * MAP_BLOCK_WIDTH;
	mapInfo.m_MapBmpHeight = m_MapBlockRowNum * MAP_BLOCK_HEIGHT;
	int block_num = m_MapBlockRowNum * m_MapBlockColNum;
	UINT* pBlockOffset = new UINT[block_num];
	fread(pBlockOffset, sizeof(UINT)*block_num, 1, file.pfile);

	//读取Mask数量和偏移量
	mapInfo.MaskFlag = file.read_dword();
	mapInfo.MaskNum = file.read_dword();
	UINT* pMaskDataOffset = new UINT[mapInfo.MaskNum];
	fread(pMaskDataOffset, sizeof(UINT)*mapInfo.MaskNum, 1, file.pfile);

	//操作块
	mapInfo.MapImg = (byte*)new WORD[mapInfo.m_MapBmpWidth * mapInfo.m_MapBmpHeight];
	for (int i = 0; i < block_num; i++)
	{
		file.seek(pBlockOffset[i], SEEK_SET);
		UINT usize = file.read_dword();
		file.seek(usize*sizeof(UINT));
		auto flag = file.read_dword();
		auto size = file.read_dword();


		BYTE* pJpgData = 0;
		UINT jpg_size = 0;
		if (flag == 0x4A504547) {
			BYTE* pMapJpgData = new BYTE[size];
			fread(pMapJpgData, size, 1, file.pfile);
			pJpgData = JpgHandler(pMapJpgData, size, &jpg_size);
			delete[] pMapJpgData;
		}
		else if (flag == 0x4A504732)
		{
			BYTE* pMapJpgData = new BYTE[size];
			fread(pMapJpgData, size, 1, file.pfile);
			pJpgData = new BYTE[size];
			memcpy(pJpgData, pMapJpgData, size);
			jpg_size = size;
			delete[] pMapJpgData;
		}
		else
		{
			//无法识别格式
			continue;
		}
		int map_row = i / m_MapBlockColNum;
		int map_col = i % m_MapBlockColNum;

		BMP* pBmpData = Unpak_jpg(pJpgData, jpg_size);

		// 将解压后的16位bmp写到m_pMapBmp中
		WriteMapPixel(pBmpData, map_row, map_col, &mapInfo);

		destroy_bitmap(pBmpData);

		delete[] pJpgData;

	}
	//goto shipmask;
	mapInfo.MaskInfos = new MaskInfo[mapInfo.MaskNum];
	for (int i = 0; i < mapInfo.MaskNum; i++)
	{
		file.seek(pMaskDataOffset[i], SEEK_SET);
		auto item = &mapInfo.MaskInfos[i];
		item->startX = file.read_dword();
		item->startY = file.read_dword();
		item->width = file.read_dword();
		item->height = file.read_dword();
		item->mask_size = file.read_dword();

		byte* pMaskDataOrgin = new byte[item->mask_size];
		file.read(pMaskDataOrgin, item->mask_size);
		//解压mask
		int align_width = (item->width / 4 + ((item->width % 4 != 0) ? 1 : 0)) * 4;	// 以4对齐的宽度
		byte* pMaskDataDec = new byte[align_width * item->height / 4];

		int dec_mask_size = DecompressMask(pMaskDataOrgin, pMaskDataDec);


		int pixel_num = item->width * item->height;

		WORD* pOutMaskBmp = new WORD[pixel_num];
		memset(pOutMaskBmp, 0, sizeof(WORD) * pixel_num);
		// 提取mask像素
		for (int h = 0; h < item->height; h++)
		{
			for (int w = 0; w < item->width; w++)
			{
				int mask_index = (h * align_width + w) * 2;		// 单位:位
				BYTE mask_value = pMaskDataDec[mask_index / 8];	// 定位到字节
				mask_value = mask_value >> (mask_index % 8);	// 定位到位

				if ((mask_value & 3) == 3)
				{
					int mapX = item->startX + w;	// 地图图像中的X位置
					int mapY = item->startY + h;	// 地图图像中的Y位置

					WORD dstPixel = *(WORD *)(mapInfo.MapImg + ((mapY * mapInfo.m_MapBmpWidth + mapX) << 1));
					pOutMaskBmp[(h)*item->width + w] = dstPixel;
				}
			}
		}
		//格式化为Png阵列图
		BYTE * pngIDATbuff = 0;
		int pngIDATbuffLen = 0;
		//函数调用

		PNGFormatData_ARGB1555(pngIDATbuff, pngIDATbuffLen, (byte*)pOutMaskBmp, item->width, item->height);
		//png图像
		void * pngbuff = 0;
		int buff_len = 0;
		CreatePNGByFormatData(item->width, item->height, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);
		item->img = (byte*)pngbuff;
		item->imgsize = buff_len;

		

	}
	//格式化为Png阵列图
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;
	//函数调用
	PNGFormatData_ARGB1555(pngIDATbuff, pngIDATbuffLen, mapInfo.MapImg, mapInfo.m_MapBmpWidth, mapInfo.m_MapBmpHeight);

	//png图像
	void * pngbuff = 0;
	int buff_len = 0;
	CreatePNGByFormatData(mapInfo.m_MapBmpWidth, mapInfo.m_MapBmpHeight, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	//资源释放
	mapInfo.MapImg = (byte*)pngbuff;
	mapInfo.MapImgSize = buff_len;
}

void main33()
{
	init_jpeg();
	auto filename = "D:\\BaiduYunDownload\\1002.map";
	MapInfo* mapInfo=new MapInfo();
	LoadMap(filename, *mapInfo);
	


}