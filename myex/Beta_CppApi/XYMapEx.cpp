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
//mask����
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

// 2�ֽڸߵ�λ����
void ByteSwap(WORD& value)
{
	WORD tempvalue = value >> 8;
	value = (value << 8) | tempvalue;
}
// ��ͼ��JPEG���ݴ�����
BYTE* JpgHandler(BYTE* Buffer, int inSize, UINT* outSize)
{
	// JPEG���ݴ���ԭ��
	// 1������D8��D9�����ݵ���������
	// 2��ɾ����3��4���ֽ� FFA0
	// 3���޸�FFDA�ĳ���00 09 Ϊ 00 0C
	// 4����FFDA���ݵ�������00 3F 00
	// 5���滻FFDA��FF D9֮���FF����ΪFF 00

	UINT TempNum = 0;						// ��ʱ��������ʾ�Ѷ�ȡ�ĳ���
	BYTE* outBuffer;
	BYTE* TempBuffer = new BYTE[inSize * 2];		// ��ʱ��������ʾ����������
	memset(TempBuffer, 0, inSize * 2);
	outBuffer = TempBuffer;					// �Ѵ������ݵĿ�ʼ��ַ
	WORD TempTimes = 0;					// ��ʱ��������ʾѭ���Ĵ���
	UINT Temp = 0;

	// ���Ѷ�ȡ���ݵĳ���С���ܳ���ʱ����
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

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // ��ȡ����
			ByteSwap(TempTimes); // ������ת��ΪIntel˳��


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

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // ��ȡ����
			ByteSwap(TempTimes); // ������ת��ΪIntel˳��

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

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // ��ȡ����
			ByteSwap(TempTimes); // ������ת��ΪIntel˳��

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

			memcpy(&TempTimes, Buffer, sizeof(WORD)); // ��ȡ����
			ByteSwap(TempTimes); // ������ת��ΪIntel˳��
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
			Temp += 1; // ����Ӧ����+3�ģ���Ϊǰ���0xFFA0û��-2����������ֻ+1��

					   // ѭ������0xFFDA��0xFFD9֮�����е�0xFF�滻Ϊ0xFF00
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
			// ֱ��������д����0xFFD9����JpegͼƬ.
			Temp--; // �������һ���ֽڣ����Լ�ȥ��
			*TempBuffer--;
			*TempBuffer-- = 0xD9;
			break;
		case 0xD9:
			// �㷨���⣬���ﲻ�ᱻִ�У������һ����
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


struct MaskInfo
{
	int startX;
	int startY;
	int width;
	int height;
	int mask_size;
	byte* img;
	int imgsize;
};

struct MapInfo
{
	char FileName[256];
	DWORD Flag;
	DWORD Width;
	DWORD Height;
	DWORD MaskFlag;
	int m_MapBmpWidth;
	int m_MapBmpHeight;
	/// <summary>
	/// mask����
	/// </summary>
	DWORD MaskNum;
	MaskInfo* MaskInfos[1];
	/// <summary>
	/// ͼ��
	/// </summary>
	byte* MapImg;

	DWORD MapImgSize;
};



void WriteMapPixel(BMP* bmp, int map_row, int map_col, MapInfo* mapInfo)
{
	int dst_x = map_col * MAP_BLOCK_WIDTH;
	int dst_y = map_row * MAP_BLOCK_HEIGHT;

	WORD* pSrc = bmp->line[0];
	WORD* pDst = (WORD*)mapInfo->MapImg + (dst_y + 1)*mapInfo->m_MapBmpWidth + dst_x;
	int dst_pitch = (int)mapInfo->m_MapBmpWidth;

	for (int h = 0; h < bmp->h; h++)
	{
		memcpy(pDst, pSrc, bmp->pitch);
		pSrc += bmp->w;
		pDst += dst_pitch;
	}
}
void LoadMap(const char * FileName, byte* MapInfoBuff, DWORD* MapInfoBuffSize)
{
	MapInfo mapInfo;
	strcpy(mapInfo.FileName, FileName);
	__file file;
	file.open(FileName);
	mapInfo.Flag = file.read_dword();
	mapInfo.Height = file.read_dword();
	mapInfo.Width = file.read_dword();
	//��ȡ��ͼ��
	int m_MapBlockRowNum = ceil(mapInfo.Height / MAP_BLOCK_HEIGHT);
	int m_MapBlockColNum = ceil(mapInfo.Width / MAP_BLOCK_WIDTH);
	mapInfo.m_MapBmpWidth = m_MapBlockColNum * MAP_BLOCK_WIDTH;
	mapInfo.m_MapBmpHeight = m_MapBlockRowNum * MAP_BLOCK_HEIGHT;
	int block_num = m_MapBlockRowNum * m_MapBlockColNum;
	UINT* pBlockOffset = new UINT[block_num];
	fread(pBlockOffset, sizeof(UINT)*block_num, 1, file.pfile);

	//��ȡMask������ƫ����
	mapInfo.MaskFlag = file.read_dword();
	mapInfo.MaskNum = file.read_dword();
	UINT* pMaskDataOffset = new UINT[mapInfo.MaskNum];
	fread(pMaskDataOffset, sizeof(UINT)*mapInfo.MaskNum, 1, file.pfile);

	//������
	mapInfo.MapImg = (byte*)new WORD[mapInfo.m_MapBmpWidth * mapInfo.m_MapBmpHeight];
	for (int i = 0; i < block_num; i++)
	{
		file.seek(pBlockOffset[i], SEEK_SET);
		UINT usize = file.read_dword();
		file.seek(usize*sizeof(UINT));
		auto flag = file.read_dword();
		auto size = file.read_dword();

		BYTE* pMapJpgData = new BYTE[size];
		fread(pMapJpgData, size, 1, file.pfile);
		UINT jpg_size = 0;
		BYTE* pJpgData = JpgHandler(pMapJpgData, size, &jpg_size);

		char outfileitem[] = "D:\\BaiduYunDownload\\1222.map.1.jpg";
		outfileitem[29] = i + 48;
		auto outfile = fopen(outfileitem, "wb+");
		fwrite(pJpgData, jpg_size, 1, outfile);
		fclose(outfile);

		int map_row = i / m_MapBlockColNum;
		int map_col = i % m_MapBlockColNum;

		BMP* pBmpData = Unpak_jpg(pJpgData, jpg_size);

		// ����ѹ���16λbmpд��m_pMapBmp��
		WriteMapPixel(pBmpData, map_row, map_col, &mapInfo);

		destroy_bitmap(pBmpData);
		delete[] pJpgData;
		delete[] pMapJpgData;
	}

	mapInfo.MaskInfos[0] = new MaskInfo[mapInfo.MaskNum];
	for (int i = 0; i < mapInfo.MaskNum; i++)
	{
		file.seek(pMaskDataOffset[i], SEEK_SET);
		auto item = mapInfo.MaskInfos[i]=new MaskInfo();
		item->startX = file.read_dword();
		item->startY = file.read_dword();
		item->width = file.read_dword();
		item->height = file.read_dword();
		item->mask_size = file.read_dword();

		byte* pMaskDataOrgin = new byte[item->mask_size];
		file.read(pMaskDataOrgin, item->mask_size);
		//��ѹmask
		int align_width = (item->width / 4 + ((item->width % 4 != 0) ? 1 : 0)) * 4;	// ��4����Ŀ��
		byte* pMaskDataDec = new byte[align_width * item->height / 4];

		int dec_mask_size = DecompressMask(pMaskDataOrgin, pMaskDataDec);


		int pixel_num = item->width * item->height;
		WORD* pOutMaskBmp = new WORD[pixel_num];
		memset(pOutMaskBmp, 0, sizeof(WORD) * pixel_num);

		// ��ȡmask����
		for (int h = 0; h < item->height; h++)
		{
			for (int w = 0; w < item->width; w++)
			{
				int mask_index = (h * align_width + w) * 2;		// ��λ:λ
				BYTE mask_value = pMaskDataDec[mask_index / 8];	// ��λ���ֽ�
				mask_value = mask_value >> (mask_index % 8);	// ��λ��λ

				if ((mask_value & 3) == 3)
				{
					int mapX = item->startX + w;	// ��ͼͼ���е�Xλ��
					int mapY = item->startY + h;	// ��ͼͼ���е�Yλ��

					WORD dstPixel = *(WORD *)(mapInfo.MapImg + ((mapY * mapInfo.m_MapBmpWidth + mapX) << 1));
					pOutMaskBmp[item->height*item->width] = dstPixel;
				}
			}
		}
		//��ʽ��ΪPng����ͼ
		BYTE * pngIDATbuff = 0;
		int pngIDATbuffLen = 0;
		//��������
		PNGFormatData_ARGB1555(pngIDATbuff, pngIDATbuffLen, (byte*)pOutMaskBmp, item->width, item->height);
		//pngͼ��
		void * pngbuff = 0;
		int buff_len = 0;
		CreatePNGByFormatData(item->width, item->height, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);
		item->img = (byte*)pngbuff;
		item->imgsize = buff_len;

		char outfileitem[] = "D:\\BaiduYunDownload\\1222.mask.1.jpg";
		outfileitem[30] = i + 48;
		auto outfile = fopen(outfileitem, "wb+");
		fwrite(item->img, item->imgsize, 1, outfile);
		fclose(outfile);

	}
	//��ʽ��ΪPng����ͼ
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;
	//��������
	PNGFormatData_ARGB1555(pngIDATbuff, pngIDATbuffLen, mapInfo.MapImg, mapInfo.m_MapBmpWidth, mapInfo.m_MapBmpHeight);

	//pngͼ��
	void * pngbuff = 0;
	int buff_len = 0;
	CreatePNGByFormatData(mapInfo.m_MapBmpWidth, mapInfo.m_MapBmpHeight, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	//��Դ�ͷ�
	mapInfo.MapImg = (byte*)pngbuff;
	mapInfo.MapImgSize = buff_len;
	auto outfilepath = "D:\\BaiduYunDownload\\1222.map.png";
	auto outfile = fopen(outfilepath, "wb+");
	fwrite(mapInfo.MapImg, mapInfo.MapImgSize, 1, outfile);
	fclose(outfile);
}

void main()
{
	init_jpeg();
	auto filename = "D:\\BaiduYunDownload\\1222.map";

	LoadMap(filename, 0, 0);
}