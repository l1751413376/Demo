#include "CRC.h"
#include <Windows.h>
#include <iostream>
#include <io.h>
#include "zlib.h"

#pragma warning( disable : 4996 )//忽略非安全函数

namespace PNG {}
#pragma comment (lib,"zlib.lib")
using namespace PNG;
#define out //参数输出

#define png_IHDR 0x49484452
#define png_IDAT 0x49444154
#define png_IEND 0x49454e44
#define png_IEND_CRC 0xAE426082
const static BYTE png_Title[] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A }; //.PNG....
const static BYTE png_CompressionAlgorithm[3] = { 0x78,0x01,0x00 };
const static BYTE png_ImgDataEnd[10] = { 0,0,0,0xff,0xff,0x01,0x00,0x00,0xFF,0xFF };

DWORD ChgDWORD(DWORD value);
inline WORD ChgWORD(WORD value);
//复制内容并且同时移动指针
inline void Xmemmove(BYTE* &buff, void * value, int size)
{
	memmove(buff, value, size);
	buff += size;
}
#define BASE 65521 
unsigned long adler32(unsigned char *buf, int len)
{

	unsigned long adler = 1;
	unsigned long s1 = adler & 0xffff;
	unsigned long s2 = (adler >> 16) & 0xffff;

	int i;
	for (i = 0; i < len; i++)
	{
		s1 = (s1 + buf[i]) % BASE;
		s2 = (s2 + s1) % BASE;
	}
	return (s2 << 16) + s1;
}


#pragma pack (push)
#pragma pack (1)
class Chunk
{
public:
	DWORD ChunkLen;//ChunkLen 不算 ChunkLen，Tag，CRC的长度
	DWORD Tag;//4byte
	DWORD CRC;
};



class IHDR : public Chunk
{
public:
	DWORD Width;//宽
	DWORD Height;//高
	BYTE  BitDepth;//位深 真彩色图像：8或16
	BYTE  ColorType;//颜色类型 6：带α通道数据的真彩色图像，8或16
	BYTE  CompressionMethod;//压缩方法(LZ77派生算法) 固定0
	BYTE  FilterMethod;//滤波器方法 固定0
	BYTE  InterlaceMethod;//隔行扫描方法：0：非隔行扫描 1： Adam7(由Adam M.Costello开发的7遍隔行扫描方法)
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
		CRC = crc32(buff_begin, buff- buff_begin);
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
		CRC = crc32(buff_begin, buff - buff_begin);
		Xmemmove(buff, &(tempDWORD = ChgDWORD(CRC)), sizeof(CRC));
	};
};
class IEND:public Chunk
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
		mov ax , value
		xchg al, ah
		mov value, ax
	}
	return value;
}
#pragma pack (pop)
//ImgData=>ABGR(A蓝绿红)
void CreatePNG(DWORD Witdh, DWORD Height, DWORD *ImgData, void *& out PNGBuff, int & PNGBuff_Len)
{
	//计算缓冲区大小
	int buff_size = sizeof(png_Title)+sizeof(IHDR)+sizeof(IDAT)+sizeof(IEND);
	//加上图片数据大小“-4”IDATA带了一个图像指针
	buff_size += Witdh * Height * 4 - 4;
	PNGBuff_Len = buff_size;
	//创建缓冲区
	BYTE *buff = new BYTE[buff_size];
	PNGBuff = buff;

	//title
	Xmemmove(buff, (void *)png_Title, sizeof(png_Title));
	//head info
	IHDR head;
	head.ChunkLen = sizeof(IHDR)-sizeof(Chunk);
	head.Tag = png_IHDR;
	head.Width = Witdh;
	head.Height = Height;
	head.BitDepth = 8;
	head.ColorType = 6;
	head.CompressionMethod = 0;
	head.FilterMethod = 0;
	head.InterlaceMethod = 0;
	head.WriteMemoryAndCRC(buff);
	//数据段1
	IDAT data1;
	data1.Tag = png_IDAT;
	memcpy(data1.CompressionAlgorithm, png_CompressionAlgorithm, sizeof(png_CompressionAlgorithm));
	data1.Len = Witdh*Height * 4 + 1;
	data1.nLen = 0xFFFF - data1.Len;
	data1.ImgData = ImgData;
	memcpy(data1.ImgDataEnd, png_ImgDataEnd, sizeof(png_ImgDataEnd));
	data1.ChunkLen = sizeof(IDAT) - sizeof(Chunk)+ data1.Len - 5;
	data1.Adler32 = 0;
	data1.b1 = 0;
	data1.WriteMemoryAndCRC(buff);

	IEND end;
	end.Tag = png_IEND;
	//IEND长度为0
	end.ChunkLen = 0;
	end.WriteMemoryAndCRC(buff);
}
#define OF(args)  args
typedef void   (*free_func) OF((int opaque,int items,int size));
void main()
{
	DWORD ImgData[] = { 0xff0000ff,0x00ffffff,0xff0000ff,0x00ffffff,0xff0000ff,0x00ffffff,0xff0000ff,0x00ffffff,0xff0000ff };
	void * buff = 0;
	int buff_len = 0;
	CreatePNG(sizeof(ImgData)/4, 1, ImgData, buff, buff_len);
	auto filename = "D:/Demo/myex/Beta/img/testcrc.png";
	auto file = fopen(filename, "wb+");
	fwrite(buff, buff_len, 1, file);
	fclose(file);
	auto ret = 0;
	
}

