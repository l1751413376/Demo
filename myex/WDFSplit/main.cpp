#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "file.h"
#include <tchar.h>
using namespace std;

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
	HEADER fileInfo;
	FileItem* fileList;
	void load_file(const char* strFileName);
	void load_data();
	//根据偏移量获取数据
	BYTE * get_data(DWORD offset);
};

void main()
{
	printf("WDFSplit Program ly1751413376 \n");
	printf("run?(Y/N) \n");
	char str[8];
	cin.getline(str, 2);
	if(str[0]=='N'||str[0]=='n')//取消	
	{
		return;
	}
	while (true)
	{
		
		OPENFILENAMEA ofn ={0};
		// a another memory buffer to contain the file name
		CHAR szFile[200];
		// open a file name
		ZeroMemory( &ofn , sizeof( ofn));
		ofn.lStructSize = sizeof ( ofn );
		ofn.hwndOwner = NULL  ;
		ofn.lpstrFile = szFile ;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof( szFile );
		//ofn.lpstrFilter = TEXT("*.TXT;*.DOC;*.was");
		ofn.nFilterIndex =1;
		ofn.lpstrFileTitle = NULL ;
		ofn.nMaxFileTitle = 0 ;
		ofn.lpstrInitialDir=NULL ;
		ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
			//|OFN_ALLOWMULTISELECT |OFN_EXPLORER;
		GetOpenFileNameA( &ofn );
		if(ofn.lpstrFile[0]==0)
		{
			printf("not chonse any files \n");
			break;
		}
		else
		{
			WdfFile wdf;
			wdf.load_file((char*)ofn.lpstrFile);
			wdf.load_data();
		}

	}

	// Now simpley display the file name
	//MessageBoxW ( NULL , ofn.lpstrFile , TEXT("File Name") , MB_OK);
}





void WdfFile::load_file(const char* strFileName)
{
	open(strFileName);
}

int GetFileType(FileExs esdate)
{
	if (esdate.Hdw == 0x5053) return 1;                             //was/tcp文件  "PS"
	else if (esdate.Hdw == 0x4D42) return 6;						   //bmp文件  BM   42 4D
	else if (esdate.Sst == 0x49464a10) return 3;                      //jpg文件  "FIFJ"
	else if (esdate.Dss == 0x454c4946) return 4;                      //tga文件  "ELIF"
	else if (esdate.Hdw == 0x4952 && esdate.Nst == 0x45564157) return 5;  //wav文件RIFF | WAVE  52 49 46 46  | 57 41 56 45
	else if ((esdate.Dsgs[0] == 0x11) && (esdate.Dsgs[1] == 0) && (esdate.Dsgs[2] == 0) && ((esdate.Sss == 0x1000) || (esdate.Sss == 0x0f00))) return 7;
	else if (esdate.Hdw == 0x00ff) return 2;			   //mp3文件
	else return 0;
}
void WdfFile::load_data()
{
	fileInfo.FileTypeName = read_dword();
	fileInfo.Number = read_dword();
	fileInfo.Offset = read_dword();

	fileList = new FileItem[fileInfo.Number];

	seek(fileInfo.Offset);//移动到文件项区域

	for (DWORD i = 0; i < fileInfo.Number; i++)
	{
		auto item = &fileList[i];
		item->Uid = read_dword();
		item->FileOffset = read_dword();
		item->FileSize = read_dword();
		item->FileSpace = read_dword();
	}
	BYTE b[12];
	FileExs fexs;
	for (DWORD i = 0; i < fileInfo.Number; i++)
	{
		auto item = &fileList[i];
		seek(item->FileOffset);
		read(b, 12);
		fexs.Hdw = b[0] + (b[1] << 8);
		fexs.Sss = b[4] + (b[5] << 8);
		fexs.Sst = b[6] + (b[7] << 8) + (b[8] << 16) + (b[9] << 24);
		fexs.Nst = b[8] + (b[9] << 8) + (b[10] << 16) + (b[11] << 24);
		seek(item->FileOffset+item->FileSize-6,0);
		read(&fexs.Dss,4);
		seek(item->FileOffset+item->FileSize-3,0);
		read(&fexs.Dsg,3);
		//获取类型
		item->FileType=GetFileType(fexs);
	}

}
//根据偏移量获取数据
BYTE * WdfFile::get_data(DWORD offset)
{
	
	FileItem* temp=NULL;
	//找到文件信息
	for (DWORD i = 0; i < fileInfo.Number; i++)
	{
		auto item = &fileList[i];
		if(item->FileOffset == offset)
		{
			temp=item;
			break;
		}
	}
	if(temp==NULL) {return NULL;}
	auto Dest=new byte[temp->FileSize];
	seek(temp->FileOffset,0);
	read(Dest,temp->FileSize);
	return Dest;
}
