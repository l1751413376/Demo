#include "StdAfx.h"
#include "WDF.h"

CWDF::CWDF(void)
{
	m_Header = new HEADER;
	m_HeaderXml = new HEADER_XML;
	memset(m_Header,0,sizeof(HEADER));
	memset(m_HeaderXml,0,sizeof(HEADER_XML));
	isWriteXml = TRUE;
	isUnpack = TRUE;
	isLstFile = TRUE;
	isDefFile = TRUE;
	isWriteDat = TRUE;
}

CWDF::~CWDF(void)
{
}

CStringList* CWDF::GetWdfList(CString path)
{
	CStringList *pFileList=new CStringList[1];
	CStringList *pFileNameList=new CStringList[1]; 
	CStringList *pShowNameList=new CStringList[1];
	BrowseFile(path,L"wd",pFileList,pFileNameList,pShowNameList); // ��������*.wd*���ļ�·��
	m_WdfFileList = pFileNameList;
	return pFileList;
}

CStringList* CWDF::GetFileName()
{
	return m_WdfFileList;
}

BOOL CWDF::BrowseFile(CString strFile, CString strFilter, CStringList *pFileList, CStringList *pFileNameList, CStringList *pShowNameList)
{
	CFileFind   FileFind;   
	CString   SearchTarget   =   strFile;//   +   "\\"   +strFilter;   
	if(SearchTarget.Right(1)   !=   "\\")   
	SearchTarget   +=   "\\";   
	SearchTarget   +=   "*.*";
	BOOL res = FileFind.FindFile(SearchTarget);
	while(res)   
	{   
		res = FileFind.FindNextFile(); // ��һ���ļ�
		if(FileFind.IsDirectory() && !FileFind.IsDots()) // �����Ŀ¼�Ͳ��ǵ�
		{   
			//�����һ����Ŀ¼���õݹ��������һ����   
			CString strPath = FileFind.GetFilePath();   
			CString strTitle = FileFind.GetFileTitle();   
			BrowseFile(strPath, strFilter, pFileList, pFileNameList, pShowNameList); // �ݹ�
		}
		else   if(!FileFind.IsDirectory() && !FileFind.IsDots()) // �������Ŀ¼�Ͳ��ǵ�
		{   
			//��ʾ��ǰ���ʵ��ļ�   
			CString strExtName; // ��׺��
			CString strName;    // �ļ���
			strName = FileFind.GetFileName(); // ����ļ��� 
			strExtName = strName.Right(strName.GetLength() - strName.Find('.')-1); // ��ú�׺�� 
			strExtName = strExtName.Left(strFilter.GetLength()); // Ϊ��ƥ����˵ĺ�׺��
			strExtName.MakeUpper(); // ת���ɴ�д  
			if(!strExtName.CompareNoCase(strFilter))//��չ����ȷ   
			{
				CString strPath = FileFind.GetFilePath();   
				pFileList->AddTail(strPath); 
				pFileNameList->AddTail(strName);   
				strName = strName.Left(strName.Find('.'));   
				pShowNameList->AddTail(strName);   
			}   
		}   
	}   
	FileFind.Close();//�ر�   
	return   TRUE; 
}

CWDF::uint32 CWDF::GetGameType(CString path)
{
	// ��Ϸ���ͣ�
	// Type = 1 : �λ�����
	// Type = 2 : ������2
	// Type = 3 : ������3
	// Type = 4 : ���⴫
	//
	uint32 type=0;
	CString Value;
	CString strUpdatePath = path;
	strUpdatePath += "//update.ini";
		::GetPrivateProfileString(L"Setting",L"Version",L"Default",Value.GetBuffer(10),10,strUpdatePath);
		
	char*   p=(LPSTR)(LPCTSTR)Value;
	char temp='0';
	memcpy((void *)&temp,p,1);
	type = atoi(&temp);
	m_GameType = type;
	return type;
}

// �ж��ļ��Ƿ����
bool CWDF::FileExist(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName); 
}

// �ж�Ŀ¼�Ƿ����
bool CWDF::FolderExists(CString s)
{
    DWORD attr; 
    attr = GetFileAttributes(s); 
    return (attr != (DWORD)(-1) ) &&
        ( attr & FILE_ATTRIBUTE_DIRECTORY); 
}

// �ݹ鴴��Ŀ¼�� ���Ŀ¼�Ѿ����ڻ��ߴ����ɹ�����TRUE
bool CWDF::SuperMkDir(CString P)
{
    int len=P.GetLength();
    if ( len <2 ) return false; 
    
    if('\\'==P[len-1])
    {
        P=P.Left(len-1);
        len=P.GetLength();
    }
    if ( len <=0 ) return false;
    
    if (len <=3) 
    {
        if (FolderExists(P))return true;
        else return false; 
    }
    
    if (FolderExists(P))return true;

    
    
    CString Parent;
    Parent=P.Left(P.ReverseFind('\\') );
    
    if(Parent.GetLength()<=0)return false; 
    
    bool Ret=SuperMkDir(Parent); 
    
    if(Ret) 
    {
        SECURITY_ATTRIBUTES sa;
        sa.nLength=sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor=NULL;
        sa.bInheritHandle=0;
        Ret=(CreateDirectory(P,&sa)==TRUE);
        return Ret;
    }
    else
        return false;
  }

void CWDF::OpenWdf(CString path, CString filename, CString DstPath)
{
	// 
	m_WdfFileName = path;
	m_FileName = filename;
	switch(m_GameType)
	{
	case 1: 
		m_GameName="xyq"; // �λ�����
		break;
	case 2:
		m_GameName="xy2"; // ������2
		break;
	case 3:
		m_GameName="xy3"; // ������3
		break;
	case 4:
		m_GameName="xyw"; // ���⴫
		break;
	}

	DstPath += "\\NetEase\\\\";
	DstPath += m_GameName;
	DstPath += "\\\\";
	DstPath += filename;
	m_OutFileName = DstPath;
	m_XmlFileName = DstPath;
	m_XmlFileName += ".xml";

	CFile file; // �ļ�����
	file.Open((LPCTSTR)path,CFile::modeRead|CFile::typeBinary); // ��WDF�ļ�
	SuperMkDir(DstPath); // �������Ŀ¼
	GetHeader(&file);
	GetFileList(&file);
	file.Close();
	SaveXml();
	
}

void CWDF::GetHeader(CFile* file)
{
	file->Read(m_Header,sizeof(HEADER));
	m_HeaderXml->Header.Flag=m_Header->Flag;
	m_HeaderXml->Header.Number=m_Header->Number;
	m_HeaderXml->Header.Offset=m_Header->Offset;

	unsigned int Flag = m_Header->Flag;
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
	m_Number = m_Header->Number;
	m_Offset = m_Header->Offset;
}

void CWDF::GetFileList(CFile* file)
{
	m_FileList = new FILELIST[m_Number];
	m_FileListXml = new FILELIST_XML[m_Number];
	memset(m_FileList,0,sizeof(FILELIST)*m_Number);
	memset(m_FileListXml,0,sizeof(FILELIST_XML)*m_Number);

	file->Seek(m_Offset,CFile::begin);
	file->Read(m_FileList,sizeof(FILELIST)*m_Number);

	// ����ļ�����ΪWDFX��WDFHʱ
	if (m_FileType==2 || m_FileType==3)
	{
		DecodeList((char *)m_FileList,m_Number*16); // �����ļ��б�����
	}	
	
	for (unsigned int i=0;i<m_Number;i++)
	{
		m_FileListXml[i].FileList.Hash = m_FileList[i].Hash;
		m_FileListXml[i].FileList.Offset = m_FileList[i].Offset;
		m_FileListXml[i].FileList.Size = m_FileList[i].Size;
		m_FileListXml[i].FileList.Spaces = m_FileList[i].Spaces;
	}

	// �������ﴦ��LST�ļ�
	if (isLstFile)
	{
		CStdioFile  infile;
		CString str;
		CString TempName;
		TempName = m_WdfFileName;
		TempName = TempName.Left(TempName.Find('.',0));
		TempName += ".lst"; 

		if(infile.Open((LPCTSTR)TempName,CFile::modeRead|CFile::typeText)) // �ļ��Ƿ��
		{
			while (infile.ReadString(str)) // ֱ���ļ�β
			{
				int len = str.GetLength()+1;
				DWORD dwLen = WideCharToMultiByte(CP_OEMCP,NULL,str,-1,NULL,0,NULL,FALSE);
				char* p = new char[dwLen];
				if(!p)
				{
					delete []p;
				}
				WideCharToMultiByte (CP_OEMCP,NULL,str,-1,p,len,NULL,FALSE);

				uint32 hash = string_id((char*)string_adjust((char*)p)); // ɢ�л�
				
				for (unsigned int i=0;i<m_Number;i++)
				{
					if ((uint32)hash==m_FileListXml[i].FileList.Hash) // �ж��ַ���ɢ���Ƿ����
					{
						memcpy(m_FileListXml[i].Path,p,len);
					}
				}
			}
		infile.Close();
		}

		if (isDefFile)
		{
			TempName = m_WdfFileName;
			TempName = TempName.Left(TempName.ReverseFind('\\'));
			TempName += "\\Default.lst"; // Ĭ��lst�ļ�

			if(infile.Open((LPCTSTR)TempName,CFile::modeRead|CFile::typeText)) // �ļ��Ƿ��
			{
				while (infile.ReadString(str)) // ֱ���ļ�β
				{
					int len = str.GetLength()+1;
					DWORD dwLen = WideCharToMultiByte(CP_OEMCP,NULL,str,-1,NULL,0,NULL,FALSE);
					char* p = new char[dwLen];
					if(!p)
					{
						delete []p;
					}
					WideCharToMultiByte (CP_OEMCP,NULL,str,-1,p,len,NULL,FALSE);

					uint32 hash = string_id((char*)string_adjust((char*)p)); // ɢ�л�

					for (unsigned int i=0;i<m_Number;i++)
					{
						if ((uint32)hash==m_FileListXml[i].FileList.Hash) // �ж��ַ���ɢ���Ƿ����
						{
							memcpy(m_FileListXml[i].Path,p,len);
						}
					}
				}
				infile.Close();
			}
		}
	}
}

// �ļ��б�Ľ����㷨
void CWDF::DecodeList(char *Data, int Size)
{
	unsigned char Key[257] = {
		0x8E,0x38,0x8F,0x75,0x83,0xB7,0x43,0x0D,0xDD,0x88,0xAD,0x29,0x4C,0xF9,0xB4,0xE4,\
		0x44,0xF4,0xA3,0x64,0x7D,0x4B,0x5C,0x0B,0x34,0x09,0x12,0x5A,0x14,0xB4,0x98,0xB5,\
		0x85,0x55,0x61,0x0D,0x0A,0x12,0x62,0x61,0xE0,0x02,0x74,0x78,0x7A,0x19,0xC7,0x0B,\
		0x09,0xAC,0x94,0x30,0x60,0x46,0xE1,0xB3,0x02,0xB8,0xCC,0x4A,0xF7,0xD3,0x6B,0xAD,\
		0x49,0x63,0x76,0x83,0x6C,0xC1,0x8F,0x30,0xB0,0xB0,0x92,0xF3,0x3C,0x15,0xFE,0xF3,\
		0x0C,0x2A,0x1B,0x65,0x8C,0x98,0xE3,0xA5,0x92,0x1A,0x3A,0x05,0x2E,0x61,0xC0,0x63,\
		0x0B,0x55,0x60,0x4A,0x4C,0xC5,0x29,0x90,0x1E,0x64,0x8C,0xBA,0xDF,0xD7,0x4F,0xD7,\
		0xBB,0xB8,0xA6,0xB2,0x85,0xA5,0xC7,0xE9,0x7E,0x73,0x28,0xBF,0x75,0x8C,0xDE,0x5C,\
		0x28,0x5A,0xFE,0x31,0x11,0x7E,0x19,0x45,0x11,0x94,0xF4,0x9E,0x4C,0xE6,0x6E,0x58,\
		0x48,0x6F,0xC9,0xEC,0x36,0x61,0xBB,0x9C,0x03,0x57,0x67,0xF1,0x0C,0xC4,0x75,0xF7,\
		0xCA,0xAF,0x98,0x31,0xCF,0xD7,0xF8,0x1A,0x2F,0x35,0x14,0xDB,0x75,0x19,0x93,0x0F,\
		0xA4,0x83,0x27,0x7D,0x67,0x67,0x74,0x93,0x49,0x63,0x08,0x67,0xB1,0x54,0xEA,0xD7,\
		0xD9,0x34,0x03,0x0B,0x70,0x09,0xD9,0x33,0x85,0xA4,0x79,0x1F,0x57,0xA0,0x4B,0x88,\
		0x02,0x11,0x79,0x4F,0x8A,0x6E,0x0F,0x95,0xBF,0x08,0x5F,0x4F,0x74,0xAE,0x19,0xDA,\
		0xBB,0x80,0xC9,0x3A,0x3A,0x94,0xBA,0x11,0x18,0x46,0xFC,0x8C,0x33,0x48,0x0D,0x18,\
		0x10,0x5A,0xF1,0x9C,0x57,0xCF,0x2A,0xCB,0xB6,0x5D,0x3C,0x19,0x0B,0x3B,0xFC,0xDD,\
		0x20
	};

	int Key_Step = 0;
	for (unsigned int i=0;i<Size;i++)
	{
		*Data ^= Key[Key_Step]; // ����
		*Data ++;
		Key_Step++;

		if (Key_Step==33)
		{
			Key_Step++;
		}

		if (Key_Step==257)
		{
			Key_Step = 0;
		}
	}
}

// WDFH �ļ����ݵĽ����㷨
unsigned char CWDF::DecodeData(char *Data, int Size)
{
	unsigned char XOR_KEY;
	if (m_FileType==3)
	{
		XOR_KEY = Data[0] ^ 0x1B; // �������Key
		for (int i=0;i<Size;i++)
		{
			*Data ^= XOR_KEY; // ����
			*Data++;
		}
	}
	return XOR_KEY; // ������Կ
}


// �����ļ���ɢ��ֵ
unsigned long CWDF::string_id(const char *str)
{
	int i;
	unsigned int v;
	static unsigned m[70];
	strncpy((char *)m,str,256);
	for (i=0;i<256/4 && m[i];i++) ;
	m[i++]=0x9BE74448,m[i++]=0x66F42C48;
	v=0xF4FA8928;

	__asm {
			mov esi,0x37A8470E		;x0=0x37A8470E
			mov edi,0x7758B42B		;y0=0x7758B42B
			xor ecx,ecx
_loop:
			mov ebx,0x267B0B11		;w=0x267B0B11
			rol v,1
			lea eax,m
			xor ebx,v

			mov eax,[eax+ecx*4]
			mov edx,ebx
			xor esi,eax
			xor edi,eax

			add edx,edi
			or edx,0x2040801		;a=0x2040801
			and edx,0xBFEF7FDF		;c=0xBFEF7FDF

			mov eax,esi
			mul edx
			adc eax,edx
			mov edx,ebx
			adc eax,0

			add edx,esi
			or edx,0x804021			;b=0x804021
			and edx,0x7DFEFBFF		;d=0x7DFEFBFF

			mov esi,eax
			mov eax,edi
			mul edx

			add edx,edx
			adc eax,edx
			jnc _skip
			add eax,2
_skip:
			inc ecx;
			mov edi,eax
			cmp ecx,i
			jnz _loop
			xor esi,edi
			mov v,esi
	}

	return v;
}

// �ַ�ת��
char *CWDF::string_adjust(char *p)
{
	int i;
	for (i=0;p[i];i++) {
		if (p[i]>='A' && p[i]<='Z') p[i]+='a'-'A';
		else if (p[i]=='/') p[i]='\\';
	}
	return p;
}

void CWDF::SaveXml()
{
	uint32 FileSize = 0;
	uint32 FileOffset = 0;
	CString TempName;
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr;
	MSXML2::IXMLDOMElementPtr pe;
	MSXML2::IXMLDOMAttributePtr pa;

	if (isWriteDat)
	{
		CFile outFile;
		HEADER_DAT *Header_Dat = new HEADER_DAT;
		FILELIST_DAT *FileList_Dat = new FILELIST_DAT[m_Number];
		memset(Header_Dat,0,sizeof(HEADER_DAT));
		memset(FileList_Dat,0,sizeof(FILELIST_DAT)*m_Number);
		CString DatFileName = m_XmlFileName;
		DatFileName = DatFileName.Left(DatFileName.Find('.',0));
		DatFileName += ".dat";


		Header_Dat->Flag =0x43464457;
		Header_Dat->Type = m_GameType;
		Header_Dat->Number = m_Number;
		Header_Dat->Finish = 0;
		Header_Dat->Offset = 40;
		//Header_Dat->Name = "Test";
		int len = m_FileName.GetLength()+1;
		DWORD dwLen = WideCharToMultiByte(CP_OEMCP,NULL,m_FileName,-1,NULL,0,NULL,FALSE);
		char* p = new char[dwLen];
		if(!p)
		{
			delete []p;
		}
		WideCharToMultiByte (CP_OEMCP,NULL,m_FileName,-1,p,len,NULL,FALSE);
		memcpy(Header_Dat->Name,p,len);


		for (unsigned int i=0;i<m_Number;i++)
		{
			FileList_Dat[i].Type = 0;
			FileList_Dat[i].Hash = m_FileListXml[i].FileList.Hash;
			memcpy(FileList_Dat[i].Path,m_FileListXml[i].Path,260);
		}

		outFile.Open(DatFileName,CFile::modeWrite|CFile::typeBinary|CFile::modeCreate);
		outFile.Write(Header_Dat,sizeof(HEADER_DAT));
		outFile.Write(FileList_Dat,sizeof(FILELIST_DAT)*m_Number);
		outFile.Close();
		
	}

	if(isWriteXml)
	{
		CoInitialize(NULL);
		hr = pXMLDom.CreateInstance(__uuidof(DOMDocument30));
		if (FAILED(hr)) 
		{
			MessageBox( NULL,L" �޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�! " ,L"��ʾ",0);
			return;
		}
		pXMLDom->preserveWhiteSpace = VARIANT_TRUE;

		// ����XML����
		MSXML2::IXMLDOMProcessingInstructionPtr pi;
		pi = pXMLDom->createProcessingInstruction("xml", "version='1.0'");
		if (pi != NULL) {
			pXMLDom->appendChild(pi);
			pi.Release();
		}

		// ע��XML�ļ�
		MSXML2::IXMLDOMCommentPtr pc;
		pc = pXMLDom->createComment("The xml file created using Dali's WDF Tool.");
		if (pc != NULL) {
			pXMLDom->appendChild(pc);
			pc.Release();
		}
		// ������Ԫ��
		
		pe = pXMLDom->createElement("NetEase");

		// ����Ԫ���������
		
		pa = pXMLDom->createAttribute("GameName"); // ��Ϸ��
		if (pa != NULL) 
		{
			pa->value = (const _variant_t)m_GameName;
			pe->setAttributeNode(pa);
			pa.Release();
		}
		// ���Ԫ�ص���Ԫ��
		pXMLDom->appendChild(pe);
		pe.Release();

		// ������Ԫ��
		pe = pXMLDom->createElement("Package");
		if (pe != NULL)
		{
			pa = pXMLDom->createAttribute("name");
			if (pa != NULL) 
			{
				pa->value = (const _variant_t)m_FileName;
				pe->setAttributeNode(pa);
				pa.Release();
			}

			pXMLDom->documentElement->appendChild(pXMLDom->createTextNode("\n\t"));

			MSXML2::IXMLDOMDocumentFragmentPtr pdf;
			MSXML2::IXMLDOMElementPtr pNode;
			pdf = pXMLDom->createDocumentFragment();
			pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
			pNode = pXMLDom->createElement("Flag");
			pNode->Puttext((_bstr_t)m_HeaderXml->Header.Flag);
			pdf->appendChild(pNode);

			pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
			pNode = pXMLDom->createElement("Number");
			pNode->Puttext((_bstr_t)m_HeaderXml->Header.Number);
			pdf->appendChild(pNode);

			pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
			pNode = pXMLDom->createElement("Offset");
			pNode->Puttext((_bstr_t)m_HeaderXml->Header.Offset);
			pdf->appendChild(pNode);

			pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
			pNode = pXMLDom->createElement("Hash");
			pNode->Puttext((_bstr_t)m_HeaderXml->Hash);
			pdf->appendChild(pNode);

			pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
			pNode = pXMLDom->createElement("Type");
			pNode->Puttext((_bstr_t)m_HeaderXml->Type);
			pdf->appendChild(pNode);

			pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
			pNode = pXMLDom->createElement("Size");
			pNode->Puttext((_bstr_t)m_HeaderXml->Size);
			pdf->appendChild(pNode);

			pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
			pNode = pXMLDom->createElement("Name");
			pNode->Puttext((_bstr_t)m_HeaderXml->Name);
			pdf->appendChild(pNode);

			pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
			pNode = pXMLDom->createElement("Desc");
			pNode->Puttext((_bstr_t)m_HeaderXml->Desc);
			pdf->appendChild(pNode);

			pdf->appendChild(pXMLDom->createTextNode("\n\t"));
			pe->appendChild(pdf);
			pdf.Release();
			pXMLDom->documentElement->appendChild(pe);
			pe.Release();
			pXMLDom->documentElement->appendChild(pXMLDom->createTextNode("\n"));
		}

	}
	for (unsigned int i=0;i<m_Number;i++)
	{
		if(isUnpack)
		{
			//			
			FileSize = m_FileList[i].Size;
			FileOffset = m_FileList[i].Offset;
			char *Data = new char[FileSize];
			memset(Data,0,FileSize);

			CFile inFile;
			inFile.Open(m_WdfFileName,CFile::modeRead|CFile::typeBinary);
			inFile.Seek(FileOffset,CFile::begin);
			inFile.Read(Data,FileSize);
			inFile.Close();

			if (m_FileType==3)
			{
				unsigned char KEY = DecodeData(Data,FileSize); // WDFH ʱ��������
				m_FileListXml[i].Key = KEY; // �˽ű�����Կ
			}

			char *temp = new char[20];
			sprintf(temp,"0x%08X",m_FileList[i].Hash);
			CString temp1(temp);
			TempName = m_OutFileName;
			TempName += "\\\\";
			TempName += temp1;

			if (isFileExist)
			{
				if (!FileExist(TempName))
				{
					CFile outFile;
					outFile.Open((LPCTSTR)TempName,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary);
					outFile.Write(Data,FileSize);
					outFile.Close();
				}
			}
			else
			{
				CFile outFile;
				outFile.Open((LPCTSTR)TempName,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary);
				outFile.Write(Data,FileSize);
				outFile.Close();
			}
			
			delete[] temp;
			delete[] Data;
		}
		
		if(isWriteXml)
		{
			pe=pXMLDom->createElement("File");
			if (pe != NULL)
			{
				pa = pXMLDom->createAttribute("name");
				if (pa != NULL) 
				{
					char *temp = new char[20];
					sprintf(temp,"0x%08X",m_FileListXml[i].FileList.Hash);
					pa->value = (const _variant_t)temp;
					pe->setAttributeNode(pa);
					pa.Release();
				}

				pXMLDom->documentElement->appendChild(pXMLDom->createTextNode("\n\t"));

				MSXML2::IXMLDOMDocumentFragmentPtr pdf;
				MSXML2::IXMLDOMElementPtr pNode;
				pdf = pXMLDom->createDocumentFragment();
				pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
				pNode = pXMLDom->createElement("Hash");
				pNode->Puttext((_bstr_t)m_FileListXml[i].FileList.Hash);
				pdf->appendChild(pNode);

				pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
				pNode = pXMLDom->createElement("Size");
				pNode->Puttext((_bstr_t)m_FileListXml[i].FileList.Size);
				pdf->appendChild(pNode);

				pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
				pNode = pXMLDom->createElement("Offset");
				pNode->Puttext((_bstr_t)m_FileListXml[i].FileList.Offset);
				pdf->appendChild(pNode);

				pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
				pNode = pXMLDom->createElement("Spaces");
				pNode->Puttext((_bstr_t)m_FileListXml[i].FileList.Spaces);
				pdf->appendChild(pNode);

				if (m_FileType==3)
				{
					pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
					pNode = pXMLDom->createElement("Key");
					pNode->Puttext((_bstr_t)m_FileListXml[i].Key);
					pdf->appendChild(pNode);
				}

				pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
				pNode = pXMLDom->createElement("Name");
				pNode->Puttext((_bstr_t)m_FileListXml[i].Name);
				pdf->appendChild(pNode);

				pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
				pNode = pXMLDom->createElement("Path");
				pNode->Puttext((_bstr_t)m_FileListXml[i].Path);
				pdf->appendChild(pNode);

				pdf->appendChild(pXMLDom->createTextNode("\n\t\t"));
				pNode = pXMLDom->createElement("Desc");
				pNode->Puttext((_bstr_t)m_FileListXml[i].Desc);
				pdf->appendChild(pNode);

				pdf->appendChild(pXMLDom->createTextNode("\n\t"));
				pe->appendChild(pdf);
				pdf.Release();
				pXMLDom->documentElement->appendChild(pe);
				pe.Release();
				pXMLDom->documentElement->appendChild(pXMLDom->createTextNode("\n"));
			}
		}
	}
	if(isWriteXml)
	{
		hr = pXMLDom->save((const _variant_t)m_XmlFileName);
		if (FAILED(hr)) 
		{
			MessageBox( NULL,L" ����XML�ļ�����" ,L"��ʾ",0);
		}
		if (pXMLDom) pXMLDom.Release();
		CoUninitialize();
	}
}

void CWDF::SetIsUnPack(BOOL b)
{
	isUnpack = b;
}

void CWDF::SetIsXmlFile(BOOL b)
{
	isWriteXml = b;
}

void CWDF::SetIsFileExist(BOOL b)
{
	isFileExist = b;
}

void CWDF::SetIsDefFile(BOOL b)
{
	isDefFile = b;
}

void CWDF::SetIsDatFile(BOOL b)
{

	isWriteDat = b;
}