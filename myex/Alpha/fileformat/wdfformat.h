#pragma once
// 本文件为utf-8 编码格式
#include <Windows.h>
#include <atlimage.h>
#include <assert.h>
#include <tchar.h>
#include "../app.h"

namespace wdf{

	struct HEADER
	{
		DWORD FileTypeName; // 文件类型名称
		DWORD Number; // 包裹中的文件数量
		DWORD Offset; // 包裹中的文件列表偏移位置
	};
	class FileItem
	{
	public :DWORD Uid;           //文件内部ID
			DWORD FileType;      //文件类型
			DWORD FileSize;       //文件大小
			DWORD FileOffset;    //偏移量
			DWORD FileSpace;     //文件剩余空间
			BYTE* FileData;
			FileItem(){
			}

			~FileItem(){
				if(FileData!=0) 
				{
					delete[]FileData;
				}
			}
	};

	//文件类型定义
	//1 was/tcp/wap/tca    头两个字节        SP    53 50
	//2 mp3                头两个字节              FF / FF
	//3 jpg                第七个字节开始    JFIF  10 4A 46 49
	//4 TGA                倒数六字节        FILE  46 49 4C 45
	//5 WAV                头四个字节 | 第九个字节开始    RIFF | WAVE  52 49 46 46  | 57 41 56 45
	//6 BMP                头两个字节        BM   42 4D
	//7 lua脚本	     第三字节开始  00 00 00 10 或 00 00 00 0F   倒数三个字节 11 00 00 
	//0 未知
	struct FileExs
	{ 
		WORD  Hdw;  //第1个字节开始的二个字节两个字节0-1
		DWORD Sst;  //第七个字节开始的四个字节6-9
		DWORD Nst;  //第九个字节开始的四个字节8-11
		WORD  Sss;	//第五个字节开始的二个字节4-5
		DWORD Dss;  //倒数六个字节开始的四个字节
		union
		{
			DWORD Dsg;
			BYTE Dsgs[4];
		};
		
	};
}


class WdfFile:__file
{
public :
	WdfFile(){};
	~WdfFile()
	{
		if(fileList!=0)
		{
			delete[] fileList;
		}
		close();
	};
	wdf::HEADER fileInfo;
	wdf::FileItem* fileList;
	void load_file(const char* strFileName);
	void load_data();
	//根据偏移量获取数据
	BYTE * get_data(DWORD offset);
};