// WdfEx.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "wdf.h"

uint32 m_Number;
uint32 m_Offset;
uint32 m_FileType;
FILELIST *m_FileList;
FILELIST_XML *m_FileListXml;

void GetHeader(CFile* file);
void GetFileList(CFile* file);


int _tmain(int argc, _TCHAR* argv[])
{
	CString path="E://Demo//Demo//WdfEx//scene.wdf";
	CFile file;
	file.Open((LPCTSTR)path,CFile::modeRead|CFile::typeBinary);
	GetHeader(&file);
	GetFileList(&file);
	return 0;
}


void GetHeader(CFile* file)
{
	HEADER m_Header;
	file->Read(&m_Header,sizeof(HEADER));
	
	unsigned int Flag = m_Header.Flag;
	switch(Flag)
	{
	case 0x57444650: // WDFP
		m_FileType=1;
		break;
	case 0x57444658: // WDFX
		m_FileType=2;
		break;
	case 0x57444648: // WDFH
		m_FileType=3;
		break;
	default:
		m_FileType=0;
	}
	m_Number = m_Header.Number;
	m_Offset = m_Header.Offset;
}

void GetFileList(CFile* file)
{
	m_FileList = new FILELIST[m_Number];
	m_FileListXml = new FILELIST_XML[m_Number];
	memset(m_FileList,0,sizeof(FILELIST)*m_Number);
	memset(m_FileListXml,0,sizeof(FILELIST_XML)*m_Number);

	file->Seek(m_Offset,CFile::begin);
	file->Read(m_FileList,sizeof(FILELIST)*m_Number);

	// 如果文件类型为WDFX和WDFH时
	if (m_FileType==2 || m_FileType==3)
	{
		//DecodeList((char *)m_FileList,m_Number*16); // 解密文件列表数据
	}	
	
	for (unsigned int i=0;i<m_Number;i++)
	{
		m_FileListXml[i].FileList.Hash = m_FileList[i].Hash;
		m_FileListXml[i].FileList.Offset = m_FileList[i].Offset;
		m_FileListXml[i].FileList.Size = m_FileList[i].Size;
		m_FileListXml[i].FileList.Spaces = m_FileList[i].Spaces;
	}
}
