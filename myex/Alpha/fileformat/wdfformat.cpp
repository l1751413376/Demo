#include "wdfformat.h"


void WdfFile::load_file(const char* strFileName)
{
	open(strFileName);
}

int GetFileType(wdf::FileExs esdate)
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

	fileList = new wdf::FileItem[fileInfo.Number];

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
	wdf::FileExs fexs;
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
	
	wdf::FileItem* temp=NULL;
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