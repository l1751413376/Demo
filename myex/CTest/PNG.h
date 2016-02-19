#include "CRC.h"
#include <Windows.h>
namespace PNG {}

using namespace PNG;


#define png_IHDR 0x49484452
#define png_IDAT 0x49444154
#define png_IEND 0x49454e44
#define png_IEND_CRC 0xAE426082
const static UINT64 png_Title = 0x89504E470D0A1A0A; //.PNG....
const static BYTE   png_CompressionAlgorithm[3] = { 0x78,0x01,0x00 };
const static BYTE   png_ImgDataEnd[5]= { 0x01,0x00,0x00,0xFF,0xFF };

#pragma pack (1)
struct IHDR
{
	DWORD Tag;
	DWORD Width;//宽
	DWORD Height;//高
	BYTE  BitDepth;//位深 真彩色图像：8或16
	BYTE  ColorType;//颜色类型 6：带α通道数据的真彩色图像，8或16
	BYTE  CompressionMethod;//压缩方法(LZ77派生算法) 固定0
	BYTE  FilterMethod;//滤波器方法 固定0
	BYTE  InterlaceMethod;//隔行扫描方法：0：非隔行扫描 1： Adam7(由Adam M.Costello开发的7遍隔行扫描方法)
	DWORD CRC;
};
struct IDAT
{
	DWORD Tag;
	BYTE CompressionAlgorithm[3];//不压缩
	WORD Len;//图像数据(字节数+1)
	WORD nLen;//len的补数0xFFFF-len
	BYTE b1;//占位符
	DWORD *ImgData;//图像数据
	BYTE ImgDataEnd[5];//尾部 
	DWORD Adler32;//不用
	DWORD CRC;//冗余检测
};
struct IEND 
{
	DWORD Tag;
	DWORD CRC;
};
#define write_byte(buff,src,size)  memcpy(buff,src,size) buff + = size
#pragma pack (pop)
void CreatePNG(DWORD Witdh, DWORD Height,DWORD *ImgData)
{
	
	//计算缓冲区大小
	int buff_size = sizeof(png_Title)+4+sizeof(IHDR)+4+sizeof(IDAT)+4+sizeof(IEND);
	//加上图片数据大小
	buff_size += Witdh * Height * 4;
	//创建缓冲区
	BYTE *buff = new BYTE[buff_size];
	
	//head info
	IHDR head = { 0 };
	head.Tag = png_IHDR;
	head.Width = Witdh;
	head.Height = Height;
	head.BitDepth = 8;
	head.ColorType = 6;
	head.CRC = crc32((uchar*)&head,sizeof(IHDR)-sizeof(DWORD));
	write_byte(buff, &head, sizeof(IHDR));
	//数据段1
	IDAT data1 = { 0 };
	data1.Tag = png_IDAT;
	memcpy(data1.CompressionAlgorithm, png_CompressionAlgorithm, sizeof(png_CompressionAlgorithm));
	data1.Len = Witdh*Height * 4 + 1;
	data1.nLen = 0xFFFF - data1.Len;
	data1.ImgData = ImgData;
	memcpy(data1.ImgDataEnd, png_ImgDataEnd, sizeof(png_ImgDataEnd));

	IEND end = { png_IEND ,png_IEND_CRC };
}