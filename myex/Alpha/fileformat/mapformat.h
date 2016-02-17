#pragma once

#include <Windows.h>
#include <atlimage.h>
#include <assert.h>
#include <tchar.h>
#include "../app.h"

namespace map_filespace
{
	struct SXYMap_Header
	{
		DWORD	flag;		// 1296126040,即"XPAM"
		DWORD	map_width;
		DWORD	map_height;
	};

	enum
	{
		MAP_BLOCK_WIDTH = 320,			// 地图被分割成了N个320×240的块
		MAP_BLOCK_HEIGHT = 240,
	};

	struct SMaskHead
	{
		DWORD	unknown;
		DWORD	mask_num;
	};

	struct SBlockInfo
	{
		DWORD	UnitN;	// 未知数据大小
		DWORD	flag;
		DWORD	size;
	};
	struct SMaskInfo
	{
		int		startX;
		int		startY;
		int		width;
		int		height;
		int		mask_size;	// mask数据大小
	};

	// 保存对齐状态
#pragma pack(push)
#pragma pack(1)

	// TGA文件头
	struct STGA_HEADER
	{
		// TGA像素顺序：B G R A
		BYTE	IdLength;				// 图像信息字段(默认:0)
		BYTE	ColorMapType;			// 颜色表的类型（0或者1，0表示没有颜色表,1表示颜色表存在.格式 2,3,10 是无颜色表的，故一般为0）
		BYTE	ImageType;				// 图像类型码(2-未压缩的RGB图像，3-未压缩的黑白图像，10-runlength编码的RGB图像)
		WORD	ColorMapFirstIndex;		// 颜色表的索引(默认:0)
		WORD	ColorMapLength;			// 颜色表的长度(默认:0)
		BYTE	ColorMapEntrySize;		// 颜色表表项的为数(默认:0，支持16/24/32)
		WORD	XOrigin;				// 图像X坐标的起始位置(默认:0)
		WORD	YOrigin;				// 图像Y坐标的起始位置(默认:0)
		WORD	ImageWidth;				// 图像的宽度
		WORD	ImageHeight;			// 图像的高度
		BYTE	PixelBits;				// 像素位数
		BYTE	ImageDescruptor;		// 图像描述字符字节(默认:0，不支持16bit 565格式)
	};

	//恢复对齐状态
#pragma pack(pop)
}

class mapfile :__bfile
{
public:
	mapfile(){};
	~mapfile(){};
	void load_file(const char* path);
	void load_data(BYTE* data);
	void ReadBlock(int block_index, int block_offset);
	void ReadJPEG(int block_index, int size);
	BYTE* JpgHandler(BYTE* Buffer, UINT inSize, UINT* outSize);
	void ByteSwap(WORD& value)
	{
		WORD tempvalue = value >> 8;
		value = (value << 8) | tempvalue;
	}
	void WriteMapPixel(int block_index, WORD* pBmp16, int width, int height, int pitch);
	void ReadMask(int offset, int mask_index);
	int DecompressMask(void* in, void* out);
public:
	map_filespace::SXYMap_Header map_header;
private:
	int				m_MapBlockRowNum;		// 地图块的行数
	int				m_MapBlockColNum;		// 地图块的列数

	WORD*			m_pMapBmp;				// ARGB1555格式像素
	int				m_MapBmpWidth;
	int				m_MapBmpHeight;
	char			m_szSrcMapFileName[256];

};

class tagfile:__bfile
{
public:
	tagfile(){};
	~tagfile(){};
	void load_file(const char* path);
	void load_data(BYTE* data,bool bUpToDown=false);
	void TGA_DeCompress();
	void ReverseBmp(void* pBmp, int width, int height, int bits/*=32*/);
	// 注意：pDstBmp24必须是32位的像素空间
	void Bmp24ToBmp32(void* pDstBmp24, int width, int height);

	map_filespace::STGA_HEADER header;
	UINT* bmp32;
};

