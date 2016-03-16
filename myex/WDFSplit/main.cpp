#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "file.h"
#include <tchar.h>
using namespace std;

struct HEADER
{
	DWORD FileTypeName; // �ļ���������
	DWORD Number; // �����е��ļ�����
	DWORD Offset; // �����е��ļ��б�ƫ��λ��
};
class FileItem
{
public :DWORD Uid;           //�ļ��ڲ�ID
		DWORD FileType;      //�ļ�����
		DWORD FileSize;       //�ļ���С
		DWORD FileOffset;    //ƫ����
		DWORD FileSpace;     //�ļ�ʣ��ռ�
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

//�ļ����Ͷ���
//1 was/tcp/wap/tca    ͷ�����ֽ�        SP    53 50
//2 mp3                ͷ�����ֽ�              FF / FF
//3 jpg                ���߸��ֽڿ�ʼ    JFIF  10 4A 46 49
//4 TGA                �������ֽ�        FILE  46 49 4C 45
//5 WAV                ͷ�ĸ��ֽ� | �ھŸ��ֽڿ�ʼ    RIFF | WAVE  52 49 46 46  | 57 41 56 45
//6 BMP                ͷ�����ֽ�        BM   42 4D
//7 lua�ű�	     �����ֽڿ�ʼ  00 00 00 10 �� 00 00 00 0F   ���������ֽ� 11 00 00 
//0 δ֪
struct FileExs
{ 
	WORD  Hdw;  //��1���ֽڿ�ʼ�Ķ����ֽ������ֽ�0-1
	DWORD Sst;  //���߸��ֽڿ�ʼ���ĸ��ֽ�6-9
	DWORD Nst;  //�ھŸ��ֽڿ�ʼ���ĸ��ֽ�8-11
	WORD  Sss;	//������ֽڿ�ʼ�Ķ����ֽ�4-5
	DWORD Dss;  //���������ֽڿ�ʼ���ĸ��ֽ�
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
	//����ƫ������ȡ����
	BYTE * get_data(DWORD offset);
};

void main()
{
	printf("WDFSplit Program ly1751413376 \n");
	printf("run?(Y/N) \n");
	char str[8];
	cin.getline(str, 2);
	if(str[0]=='N'||str[0]=='n')//ȡ��	
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
	if (esdate.Hdw == 0x5053) return 1;                             //was/tcp�ļ�  "PS"
	else if (esdate.Hdw == 0x4D42) return 6;						   //bmp�ļ�  BM   42 4D
	else if (esdate.Sst == 0x49464a10) return 3;                      //jpg�ļ�  "FIFJ"
	else if (esdate.Dss == 0x454c4946) return 4;                      //tga�ļ�  "ELIF"
	else if (esdate.Hdw == 0x4952 && esdate.Nst == 0x45564157) return 5;  //wav�ļ�RIFF | WAVE  52 49 46 46  | 57 41 56 45
	else if ((esdate.Dsgs[0] == 0x11) && (esdate.Dsgs[1] == 0) && (esdate.Dsgs[2] == 0) && ((esdate.Sss == 0x1000) || (esdate.Sss == 0x0f00))) return 7;
	else if (esdate.Hdw == 0x00ff) return 2;			   //mp3�ļ�
	else return 0;
}
void WdfFile::load_data()
{
	fileInfo.FileTypeName = read_dword();
	fileInfo.Number = read_dword();
	fileInfo.Offset = read_dword();

	fileList = new FileItem[fileInfo.Number];

	seek(fileInfo.Offset);//�ƶ����ļ�������

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
		//��ȡ����
		item->FileType=GetFileType(fexs);
	}

}
//����ƫ������ȡ����
BYTE * WdfFile::get_data(DWORD offset)
{
	
	FileItem* temp=NULL;
	//�ҵ��ļ���Ϣ
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
