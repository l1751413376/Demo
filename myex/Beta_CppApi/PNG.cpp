#include <Windows.h>
#include <iostream>
#include <io.h>
#include "zlib.h"
#include "Png.h"
#pragma warning( disable : 4996 )//忽略非安全函数
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
//复制内容并且同时移动指针
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
/*创建Png函数（已经格式化数据）
ImgData=>(红绿蓝A)(每行开始有个0)(第一行0RGBA RGBA RGBA....第二行0RGBA RGBA RGBA....)(A=0透明A=0xFF不透明)
PNGBuff=>函数内部创建
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

	//数据段
	IDAT data;
	data.Tag = png_IDAT;
	auto compressLen = compressBound(ImgBuffLen);
	data.CompressData = new BYTE[compressLen];
	data.CompressDataLen = compressLen;
	//此含树 第二个参数 需传入dest的长度 同时返回压缩后实际的数据长度
	compress2(data.CompressData, &data.CompressDataLen, ImgBuff, ImgBuffLen, 0);
	data.ChunkLen = sizeof(data.CRC) + sizeof(data.Tag) + data.CompressDataLen;

	//IENDinfo
	IEND end;
	end.Tag = png_IEND;
	//IEND长度为0
	end.ChunkLen = 0;

	//计算缓冲区大小
	int buff_size = sizeof(png_Title) + sizeof(IHDR) + sizeof(IEND);
	//加上IDAT(数据段)
	buff_size += data.ChunkLen + sizeof(data.ChunkLen);
	PNGBuffLen = buff_size;
	//创建缓冲区
	BYTE *buff = new BYTE[buff_size];
	PNGBuff = buff;
	//title
	Xmemmove(buff, (void *)png_Title, sizeof(png_Title));
	//head
	head.WriteMemoryAndCRC(buff);
	//data1
	data.WriteMemoryAndCRC(buff);
	//释放data1.CompressData
	delete[] data.CompressData;
	//delete[] ImgBuff;
	//end
	end.WriteMemoryAndCRC(buff);
}
/*格式化函数*/
void PNGFormatData_R8G8B8A8(BYTE *& dest, int & desLen, BYTE * source, int sourceWitdh, int sourceHeight)
{
	//png每行多一个字节0
	desLen = sourceWitdh * sourceHeight * 4 + sourceHeight;
	BYTE* buff = new BYTE[desLen];
	dest = buff;
	//源位图每行字节数量
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
	//png每行多一个字节0
	desLen = sourceWitdh * sourceHeight * 4 + sourceHeight;
	BYTE* buff = new BYTE[desLen];
	dest = buff;
	//源位图每行字节数量
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

	//源图像

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
	//png图像
	void * pngbuff = 0;
	int buff_len = 0;
	//pngIDATImg数据
	BYTE * pngIDATbuff = 0;
	int pngIDATbuffLen = 0;
	//函数调用
	PNGFormatData_R8G8B8A8(pngIDATbuff, pngIDATbuffLen, ImgData, imgWidth, imgHeight);
	CreatePNGByFormatData(imgWidth, imgHeight, pngIDATbuff, pngIDATbuffLen, pngbuff, buff_len);

	//资源释放
	delete pngIDATbuff;
	//文件操作
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