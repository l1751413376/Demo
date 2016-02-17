// WdfToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WdfTool.h"
#include "WdfToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CWdfToolDlg �Ի���




CWdfToolDlg::CWdfToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWdfToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWdfToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAME_PATH, m_GamePath);
	DDX_Control(pDX, IDC_FILE_PATH, m_FilePath);
	DDX_Control(pDX, IDC_UNPACK, m_UnPack_Button);
	DDX_Control(pDX, IDC_PACK, m_Pack_Button);
	DDX_Control(pDX, IDC_IS_UNPACK, m_IsUnPack);
	DDX_Control(pDX, IDC_IS_XML, m_IsXml);
	DDX_Control(pDX, IDC_IS_EXIST, m_IsFileExist);
	DDX_Control(pDX, IDC_IS_DEFFILE, m_IsDefFile);
	DDX_Control(pDX, IDC_IS_DAT_FILE, m_IsDat);
}

BEGIN_MESSAGE_MAP(CWdfToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_GAME_BROWSE, &CWdfToolDlg::OnBnClickedGameBrowse)
	ON_BN_CLICKED(IDC_FILE_BROWSE, &CWdfToolDlg::OnBnClickedFileBrowse)
	ON_BN_CLICKED(IDC_PACK, &CWdfToolDlg::OnBnClickedPack)
	ON_BN_CLICKED(IDC_UNPACK, &CWdfToolDlg::OnBnClickedUnpack)
END_MESSAGE_MAP()


// CWdfToolDlg ��Ϣ�������

BOOL CWdfToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_IsUnPack.SetCheck(TRUE);
	m_IsXml.SetCheck(TRUE);
	m_IsFileExist.SetCheck(TRUE);
	m_IsDefFile.SetCheck(TRUE);
	m_IsDat.SetCheck(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWdfToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWdfToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CWdfToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWdfToolDlg::OnBnClickedGameBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi; // �������Ϣ
	memset(&bi, 0, sizeof(BROWSEINFO));   

	char Buffer[MAX_PATH]="";

	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot =NULL;//��ʼ���ƶ���rootĿ¼�ܲ����ף�
	//bi.pszDisplayName = *Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = L"��ѡ����������ϵ�а�װĿ¼��";
	//bi.ulFlags = BIF_BROWSEINCLUDEFILES;//�����ļ�
	bi.ulFlags = BIF_EDITBOX;//�����ļ�

	bi.lpfn = NULL;
	bi.iImage=IDR_MAINFRAME;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, (LPWSTR)&Buffer); //ȡ���ļ���·����Buffer��
	}
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) return;
	//�ͷ��ڴ�
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	m_GamePath.SetWindowTextW((LPCTSTR)Buffer);
}

void CWdfToolDlg::OnBnClickedFileBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		BROWSEINFO bi; // �������Ϣ
	memset(&bi, 0, sizeof(BROWSEINFO));   

	char Buffer[MAX_PATH]="";

	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot =NULL;//��ʼ���ƶ���rootĿ¼�ܲ����ף�
	//bi.pszDisplayName = *Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = L"��ѡ�������Ŀ¼��";
	//bi.ulFlags = BIF_BROWSEINCLUDEFILES;//�����ļ�
	bi.ulFlags = BIF_EDITBOX;//�����ļ�

	bi.lpfn = NULL;
	bi.iImage=IDR_MAINFRAME;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, (LPWSTR)&Buffer); //ȡ���ļ���·����Buffer��
	}
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) return;
	//�ͷ��ڴ�
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	m_FilePath.SetWindowTextW((LPCTSTR)Buffer);
}

void CWdfToolDlg::OnBnClickedPack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// �������
	MessageBox(L"����汾���޴˹���",L"��ʾ",0);
}

void CWdfToolDlg::OnBnClickedUnpack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// �������
	m_UnPack_Button.EnableWindow(FALSE); // ���ð�ť
	CWDF *Wdf=new CWDF();

	BOOL b1 = m_IsUnPack.GetCheck();
	BOOL b2 = m_IsXml.GetCheck();
	BOOL b3 = m_IsFileExist.GetCheck();
	BOOL b4 = m_IsDefFile.GetCheck();
	BOOL b5 = m_IsDat.GetCheck();
	Wdf->SetIsUnPack(b1);
	Wdf->SetIsXmlFile(b2);
	Wdf->SetIsFileExist(b3);
	Wdf->SetIsDefFile(b4);
	Wdf->SetIsDatFile(b5);

	m_GamePath.GetWindowTextW(m_sGamePath);
	m_FilePath.GetWindowTextW(m_sFilePath);
	if(m_sGamePath=="")
	{
		MessageBox(L"��ѡ����Ϸ��װĿ¼��·��",L"��ʾ",0);
		m_UnPack_Button.EnableWindow(TRUE); // ���ð�ť
		return;
	}
	if(m_sFilePath=="")
	{
		MessageBox(L"��ѡ������ļ�Ŀ¼��·��",L"��ʾ",0);
		m_UnPack_Button.EnableWindow(TRUE); // ���ð�ť
		return;
	}

	
	unsigned int type = Wdf->GetGameType(m_sGamePath);
	if(type==0) 
	{
		MessageBox(L"��ѡ����ȷ����Ϸ��װĿ¼��·��\nδ�ҵ���ϷĿ¼�µ�Update.ini�ļ�",L"��ʾ",0);
		m_UnPack_Button.EnableWindow(TRUE); // ���ð�ť
		return;
	}

	CStringList* WdfList = Wdf->GetWdfList(m_sGamePath); // ����ļ��б�
	
	

	POSITION pos = WdfList->GetHeadPosition();
	POSITION pos1 = Wdf->GetFileName()->GetHeadPosition();
	while (pos!=NULL)
	{
		CString path = WdfList->GetNext(pos);
		CString name = Wdf->GetFileName()->GetNext(pos1);
		Wdf->OpenWdf(path,name,m_sFilePath);
	}
	Wdf->~CWDF();
	m_UnPack_Button.EnableWindow(TRUE); // ���ð�ť
}
