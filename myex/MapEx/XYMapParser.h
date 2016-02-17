//----------------------------------------------------------------------------------------------------
// File  : XYMapParser.h
// Title : CXYMapParser 网易西游系列地图提取类(只支持梦幻和大话2)
// Date  : 2013-4-1
// Desc  : 
//	[关于本代码]
//	本代码来自三位作者，一个是CSDN的freegodly,提供了map文件格式说明和读取方式,
//  另一个作者是王大理,提供了jpg数据的解压,第三位作者是千里附骥,提供了mask数据的解压算法,
//
// Copyright(c) WeiWuYuan
//----------------------------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <windows.h>



/*----------------------------------------------------------------------------------------------
【大话西游2和梦幻西游的地图文件格式】

从文件起始处开始依次为：
	[文件头]
	·4字节flag,值为1296126040,即"XPAM"
	·4字节地图宽度
	·4字节地图高度

	[地图块偏移信息]
	·4*block_num字节，每4个字节保存1个地图块的偏移地址，其中block_num = 地图块的行数*地图块的列数，该行数和列数由地图尺寸计算得来

	[mask head数据]
	·4字节未知数据
	·4字节mask_num，即遮挡图的数量

	[mask数据的偏移信息]
	·4*mask_num字节，每4个字节保存1个mask数据的偏移地址


单元数据：

[地图块数据]
·根据地图块偏移信息，定位到地图块数据的位置，以下以此位置为起始
·4字节未知数据
·4*n字节数据，n为上一个4字节的值

·4字节jpg标记，值为：0x4A504547，即'GEPJ'
·4字节jpg数据大小
·n字节jpg数据，n为上一个4字节的值

·4字节CELL标记(地图规则)，值为：0x43454C4C，即'LLEC'
·4字节CELL数据大小
·n字节CELL数据，n为上一个4字节的值

·4字节BRIG标记(光亮规则)，值为：0x42524947，即'GIRB'
·4字节BRIG数据大小
·n字节BRIG数据，n为上一个4字节的值

·4字节结束单元

[mask数据]
·根据mask偏移信息，定位到mask数据的位置，以下以此位置为起始
·4字节mask位于地图的x坐标
·4字节mask位于地图的y坐标
·4字节mask图像的宽度
·4字节mask图像的高度
·4字节mask数据的大小
·n字节mask数据，n为上一个4字节的值
/*----------------------------------------------------------------------------------------------*/



class CXYMapParser
{
public:
	CXYMapParser();
	~CXYMapParser();

public:
	struct SXYMap_Header
	{
		int				flag;		// 1296126040,即"XPAM"
		unsigned int	map_width;
		unsigned int	map_height;
	};

	struct SMaskHead
	{
		unsigned int	unknown;
		int				mask_num;
	};

	struct SMaskInfo
	{
		int		startX;
		int		startY;
		int		width;
		int		height;
		int		mask_size;	// mask数据大小
	};

	struct SBlockInfo
	{
		unsigned int	UnitN;	// 未知数据大小
		unsigned int	flag;
		unsigned int	size;
	};

public:
	bool	Open(const char* szMapFile);
	void	Close();

private:
	void	ReadBlock(int block_index, int block_offset);
	void	ReadJPEG(int block_index, int size);
	void	ReadMask(int offset, int mask_index);
	BYTE*	JpgHandler(BYTE* Buffer, int inSize, UINT* outSize);
	int		DecompressMask(void* in , void* out);
	void	ByteSwap(WORD& value);
	void	WriteMapPixel(int block_index, WORD* pBmp16, int width, int height, int pitch);

private:
	void	ErrorDialog(const char* szFormat, ...);

public:
	enum
	{
		MAP_BLOCK_WIDTH		= 320,			// 地图被分割成了N个320×240的块
		MAP_BLOCK_HEIGHT	= 240,
	};

private:
	FILE*			m_pFile;
	SXYMap_Header	m_FileHeader;
	int				m_MapBlockRowNum;		// 地图块的行数
	int				m_MapBlockColNum;		// 地图块的列数

	WORD*			m_pMapBmp;				// ARGB1555格式像素
	int				m_MapBmpWidth;
	int				m_MapBmpHeight;
	char			m_szSrcMapFileName[256];
};