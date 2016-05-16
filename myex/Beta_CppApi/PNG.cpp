#include <Windows.h>
#include <iostream>
#include <io.h>
#include "zlib.h"
#include "Png.h"
#pragma warning( disable : 4996 )//���Էǰ�ȫ����
#pragma comment (lib,"lib/zlib.lib")

#define png_IHDR 0x49484452
#define png_IDAT 0x49444154
#define png_IEND 0x49454e44
#define png_IEND_CRC 0xAE426082
const static BYTE png_Title[] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A }; //.PNG....

DWORD ChgDWORD(DWORD value)
{
	DWORD ret = 0;
	auto pret = (BYTE*)&ret;
	auto pvalue = (BYTE*)&value;
	*pret++ = pvalue[3];
	*pret++ = pvalue[2];
	*pret++ = pvalue[1];
	*pret++ = pvalue[0];
	return ret;

}
WORD ChgWORD(WORD value)
{
	__asm
	{
		mov ax, value
		xchg al, ah
			mov value, ax
	}
	return value;
}
//�������ݲ���ͬʱ�ƶ�ָ��
inline void Xmemmove(BYTE* &buff, void * value, int size)
{
	memmove(buff, value, size);
	buff += size;
}
#pragma pack (push)
#pragma pack (1)
class Chunk
{
public:
	DWORD ChunkLen;//ChunkLen ���� ChunkLen��Tag��CRC�ĳ���
	DWORD Tag;//4byte
	DWORD CRC;
};
class IHDR : public Chunk
{
public:
	DWORD Width;//��
	DWORD Height;//��
	BYTE  BitDepth;//λ�� ���ɫͼ��8��16
	BYTE  ColorType;//��ɫ���� 6������ͨ�����ݵ����ɫͼ��8��16
	BYTE  CompressionMethod;//ѹ������(LZ77�����㷨) �̶�0
	BYTE  FilterMethod;//�˲������� �̶�0
	BYTE  InterlaceMethod;//����ɨ�跽����0���Ǹ���ɨ�� 1�� Adam7(��Adam M.Costello������7�����ɨ�跽��)
public:
	void WriteMemoryAndCRC(BYTE* &buff)
	{
		DWORD tempDWORD = 0;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(ChunkLen)), sizeof(ChunkLen));
		auto buff_begin = buff;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Tag)), sizeof(Tag));
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Width)), sizeof(Width));
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Height)), sizeof(Height));
		Xmemmove(buff, &BitDepth, sizeof(BitDepth));
		Xmemmove(buff, &ColorType, sizeof(ColorType));
		Xmemmove(buff, &CompressionMethod, sizeof(CompressionMethod));
		Xmemmove(buff, &FilterMethod, sizeof(FilterMethod));
		Xmemmove(buff, &InterlaceMethod, sizeof(InterlaceMethod));
		//CRC
		CRC = crc32(0, buff_begin, buff - buff_begin);
		Xmemmove(buff, &(tempDWORD = ChgDWORD(CRC)), sizeof(CRC));
	}
};
class IDAT :public Chunk
{
public:
	BYTE* CompressData;
	DWORD CompressDataLen;
public:
	void WriteMemoryAndCRC(BYTE* &buff)
	{
		DWORD tempDWORD = 0;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(ChunkLen)), sizeof(ChunkLen));
		auto buff_begin = buff;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Tag)), sizeof(Tag));
		Xmemmove(buff, CompressData, CompressDataLen);
		//CRC
		CRC = crc32(0, buff_begin, buff - buff_begin);
		Xmemmove(buff, &(tempDWORD = ChgDWORD(CRC)), sizeof(CRC));
	};
};
class IEND :public Chunk
{
public:
	void WriteMemoryAndCRC(BYTE* &buff)
	{
		DWORD tempDWORD = 0;

		Xmemmove(buff, &(tempDWORD = ChgDWORD(ChunkLen)), sizeof(ChunkLen));
		Xmemmove(buff, &(tempDWORD = ChgDWORD(Tag)), sizeof(Tag));
		CRC = png_IEND_CRC;
		Xmemmove(buff, &(tempDWORD = ChgDWORD(CRC)), sizeof(CRC));
	}
};
#pragma pack (pop)
/*����Png�������Ѿ���ʽ�����ݣ�
ImgData=>(������A)(ÿ�п�ʼ�и�0)(��һ��0RGBA RGBA RGBA....�ڶ���0RGBA RGBA RGBA....)(A=0͸��A=0xFF��͸��)
PNGBuff=>�����ڲ�����
*/
void CreatePNGByFormatData(DWORD Witdh, DWORD Height, BYTE *ImgBuff, int ImgBuffLen, void *& PNGBuff, int & PNGBuffLen)
{
	//head info
	IHDR head;
	head.ChunkLen = sizeof(IHDR) - sizeof(Chunk);
	head.Tag = png_IHDR;
	head.Width = Witdh;
	head.Height = Height;
	head.BitDepth = 8;
	head.ColorType = 6;
	head.CompressionMethod = 0;
	head.FilterMethod = 0;
	head.InterlaceMethod = 0;

	//���ݶ�
	IDAT data;
	data.Tag = png_IDAT;
	auto compressLen = compressBound(ImgBuffLen);
	data.CompressData = new BYTE[compressLen];
	data.CompressDataLen = compressLen;
	//�˺��� �ڶ������� �贫��dest�ĳ��� ͬʱ����ѹ����ʵ�ʵ����ݳ���
	compress2(data.CompressData, &data.CompressDataLen, ImgBuff, ImgBuffLen, 0);
	data.ChunkLen = sizeof(data.CRC) + sizeof(data.Tag) + data.CompressDataLen;

	//IENDinfo
	IEND end;
	end.Tag = png_IEND;
	//IEND����Ϊ0
	end.ChunkLen = 0;

	//���㻺������С
	int buff_size = sizeof(png_Title) + sizeof(IHDR) + sizeof(IEND);
	//����IDAT(���ݶ�)
	buff_size += data.ChunkLen + sizeof(data.ChunkLen);
	PNGBuffLen = buff_size;
	//����������
	BYTE *buff = new BYTE[buff_size];
	PNGBuff = buff;
	//title
	Xmemmove(buff, (void *)png_Title, sizeof(png_Title));
	//head
	head.WriteMemoryAndCRC(buff);
	//data1
	data.WriteMemoryAndCRC(buff);
	//�ͷ�data1.CompressData
	delete[] data.CompressData;
	//delete[] ImgBuff;
	//end
	end.WriteMemoryAndCRC(buff);
}
/*��ʽ������*/
void PNGFormatData_R8G8B8A8(BYTE *& dest, int & desLen, BYTE * source, int sourceWitdh, int sourceHeight)
{
	//pngÿ�ж�һ���ֽ�0
	desLen = sourceWitdh * sourceHeight * 4 + sourceHeight;
	BYTE* buff = new BYTE[desLen];
	dest = buff;
	//Դλͼÿ���ֽ�����
	int witdhSteps = sourceWitdh * 4;
	for (int i = 0; i < sourceHeight; i++)
	{
		*buff++ = 0;
		memcpy(buff, source, witdhSteps);
		buff += witdhSteps;
		source += witdhSteps;
	}
}

void PNGFormatData_ARGB1555(BYTE *& dest, int & desLen, BYTE * source, int sourceWitdh, int sourceHeight)
{
	//pngÿ�ж�һ���ֽ�0
	desLen = sourceWitdh * sourceHeight * 4 + sourceHeight;
	BYTE* buff = new BYTE[desLen];
	dest = buff;
	//Դλͼÿ���ֽ�����
	for (int i = 0; i < sourceHeight; i++)
	{
		*buff++ = 0;
		for (int w = 0; w < sourceWitdh; w++)
		{
			auto ARGB1555 = *(WORD*)(source);
			source += 2;
			byte R8G8B8A8[4];//0-R 1-G 2-B 3-A
			R8G8B8A8[2] = (ARGB1555 & 0x1F) << 3;
			R8G8B8A8[1] = (ARGB1555 & 0x3E0) >> 2;
			R8G8B8A8[0] = (ARGB1555 & 0x7C00) >> 7;
			R8G8B8A8[3] = (ARGB1555 & 0x8000) != 0 ? 0 : 0xff;
			memcpy(buff, &R8G8B8A8, 4);
			buff += 4;
		}

	}
}



void main1()
{

	//Դͼ��

	int imgWidth = 512;
	int imgHeight = 512;
	BYTE *ImgData = new BYTE[imgWidth*imgHeight * 4];
	for (size_t i = 0, len = imgHeight*imgWidth; i < len; i++)
	{
		auto item = ImgData + int(i << 2);
		item[0] = 0xff;//R
		item[1] = 0x00;//G
		item[2] = 0x00;//B
		item[3] = 0xff;//A
	}
	{
		auto item = ImgData + imgHeight*imgWidth * 4 - 4;
		item[0] = 0x00;//R
		item[1] = 0xff;//G
		item[2] = 0x00;//B
		item[3] = 0xff;//A
	}
	//pngͼ��
	void * pngbuff = 0;
	int buff_len = 0;
	//pngIDATImg����
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;
	//��������
	PNGFormatData_R8G8B8A8(pngIDATbuff, pngIDATbuffLen, ImgData, imgWidth, imgHeight);
	CreatePNGByFormatData(imgWidth, imgHeight, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	//��Դ�ͷ�
	delete pngIDATbuff;
	//�ļ�����
	auto filename = "D:/Demo/myex/Beta/img/testcrc.png";
	auto file = fopen(filename, "wb+");
	fwrite(pngbuff, buff_len, 1, file);
	fclose(file);
	auto ret = 0;
	/*

	DWORD ImgData[] = { 0xff00ff00,0xff00ffff };

	BYTE source[20];
	ULONG sourceLen=20;

	memset(source,0,sizeof(source));

	compress2(source, &sourceLen, (Byte *)ImgData,8,0);
	int ret = 0;*/

}