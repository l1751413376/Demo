#pragma once

#include <afx.h>

// 本文件为utf-8 编码格式
typedef unsigned int uint32;

struct HEADER
{
	uint32 Flag; // 包裹的标签
	uint32 Number; // 包裹中的文件数量
	uint32 Offset; // 包裹中的文件列表偏移位置
};

struct FILELIST
{
	uint32 Hash; // 文件的名字散列
	uint32 Offset; // 文件的偏移
	uint32 Size; // 文件的大小
	uint32 Spaces; // 文件的空间
};
struct HEADER_XML
{
	HEADER Header; // WDF文件头指针

	uint32 Type; // 包裹的类型
	uint32 Size; // 包裹大小
	char Hash[16]; // 包裹的散列
	char Name[20]; // 包裹的名字
	char Desc[512]; // 包裹的描述
};

struct FILELIST_XML
{
	FILELIST FileList; // 文件列表的指针

	unsigned char Key; // WDFH 文件解码Key
	char Name[20]; // 中文名字
	char Path[260]; // 路径
	char Desc[512]; // 描述
};


