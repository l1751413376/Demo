#pragma once
#import <msxml3.dll>
using namespace MSXML2;

class CWDF
{
public:
	CWDF(void);
	~CWDF(void);

	typedef unsigned int uint32;
	struct HEADER
	{
		uint32 Flag; // �����ı�ǩ
		uint32 Number; // �����е��ļ�����
		uint32 Offset; // �����е��ļ��б�ƫ��λ��
	};

	struct FILELIST
	{
		uint32 Hash; // �ļ�������ɢ��
		uint32 Offset; // �ļ���ƫ��
		uint32 Size; // �ļ��Ĵ�С
		uint32 Spaces; // �ļ��Ŀռ�
	};

	struct HEADER_XML
	{
		HEADER Header; // WDF�ļ�ͷָ��
		
		uint32 Type; // ����������
		uint32 Size; // ������С
		char Hash[16]; // ������ɢ��
		char Name[20]; // ����������
		char Desc[512]; // ����������
	};

	struct FILELIST_XML
	{
		FILELIST FileList; // �ļ��б��ָ��

		unsigned char Key; // WDFH �ļ�����Key
		char Name[20]; // ��������
		char Path[260]; // ·��
		char Desc[512]; // ����
	};

	struct HEADER_DAT 
	{
		uint32 Flag; // �ļ���ǩ
		uint32 Type; // ��Ϸ���� 0:�ޡ�1:�λá� 2:��2�� 3:��3�� 4:���⴫
		uint32 Number; // �ļ�����
		uint32 Finish; // ���������
		uint32 Offset; // ƫ��ֵ
		char Name[20]; // ��������
	};

	struct FILELIST_DAT 
	{
		uint32 Type; // �������
		uint32 Hash; // ɢ��
		char Path[260]; // ·����
	};

	CString m_GameName; // ��Ϸ����
	uint32 m_GameType; // ��Ϸ����
	uint32 m_FileType; // �ļ�����
	HEADER *m_Header; // WDF�ļ�ͷ
	uint32 m_Flag;
	uint32 m_Number;
	uint32 m_Offset;

	FILELIST *m_FileList; // WDF�ļ��б�
	BOOL isWriteXml; // �Ƿ�ҪдXML�ļ�
	BOOL isUnpack; // �Ƿ����ļ�
	BOOL isFileExist; // �ļ��Ƿ����
	BOOL isLstFile; // �Ƿ�ʹ��LST�ļ�
	BOOL isDefFile; // �Ƿ�ʹ��Ĭ��LST�ļ�
	BOOL isWriteDat; // �Ƿ�ҪдDAT�ļ�

	HEADER_XML *m_HeaderXml; // XML���ļ�ͷ
	FILELIST_XML *m_FileListXml; // XML���ļ��б�

	uint32 GetGameType(CString path); // �����Ϸ������
	CStringList* GetWdfList(CString path); // ���WDF�ļ����б�
	CStringList* m_WdfFileList;
	CStringList* GetFileName(); // ����ļ���
	CString m_WdfFileName; // WDF���ļ���
	CString m_XmlFileName; // XML���ļ���
	CString m_OutFileName; // ������ļ���
	CString m_FileName; // �����ļ�����

	void SetIsUnPack(BOOL b);
	void SetIsXmlFile(BOOL b);
	void SetIsFileExist(BOOL b);
	void SetIsDefFile(BOOL b);
	void SetIsDatFile(BOOL b);

	void OpenWdf(CString path, CString filename, CString DstPath); // ��WDF�ļ�
	void GetHeader(CFile* file); // ���WDF�ļ�ͷ����
	void GetFileList(CFile* file); // ���WDF�ļ��б������
	void SaveXml(); // �����XML�ļ�

	void DecodeList(char *Data, int Size);
	unsigned char DecodeData(char *Data, int Size);
	unsigned long string_id(const char *str);
	char *string_adjust(char *p);

	BOOL BrowseFile(CString strFile, CString strFilter, CStringList *pFileList, CStringList *pFileNameList, CStringList *pShowNameList);//�����ļ����� 
	bool FileExist(CString strFileName); // �ж��ļ��Ƿ����
	bool FolderExists(CString s); // �ж�Ŀ¼�Ƿ����
	bool SuperMkDir(CString P); // �������Ŀ¼

	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr  xmlRoot;
	MSXML2::IXMLDOMProcessingInstructionPtr pIInstruction ;
};
