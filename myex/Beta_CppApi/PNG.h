#include <Windows.h>
namespace PNG {}

using namespace PNG;


#define png_IHDR 0x49484452
#define png_IDAT 0x49444154
#define png_IEND 0x49454e44
#define png_IEND_CRC 0xAE426082
const static UINT64 png_Title = 0x89504E470D0A1A0A; //.PNG....

struct IHDR
{
	DWORD Tag= png_IHDR;
	DWORD Width;//宽
	DWORD Height;//高
	BYTE  BitDepth;//位深 真彩色图像：8或16
	BYTE  ColorType;//颜色类型 6：带α通道数据的真彩色图像，8或16
	BYTE  CompressionMethod;//压缩方法(LZ77派生算法) 固定0
	BYTE  FilterMethod;//滤波器方法 固定0
	BYTE  InterlaceMethod;//隔行扫描方法：0：非隔行扫描 1： Adam7(由Adam M.Costello开发的7遍隔行扫描方法)
	DWORD CRC;
};


#pragma pack (1)
struct IDAT
{
	DWORD Tag = png_IHDR;
	BYTE CompressionAlgorithm[3] = { 0x78,0x01,0x00 };//不压缩
	WORD Len;//图像数据(字节数+1)
	WORD nLen;//len的补数0xFFFF-len
	BYTE b1;//占位符
	DWORD ImgData[1];//图像数据
	BYTE ImgDataEnd[5] = {0x01,0x00,0x00,0xFF,0xFF};//尾部 
	DWORD Adler32;//不用
	DWORD CRC;//冗余检测
};
#pragma pack (pop)

struct IEND 
{
	DWORD Tag = png_IEND;
	DWORD CRC = png_IEND_CRC;
};