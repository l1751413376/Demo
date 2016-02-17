// WdfToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WdfTool.h"
#include "WdfToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CWdfToolDlg 对话框




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


// CWdfToolDlg 消息处理程序

BOOL CWdfToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_IsUnPack.SetCheck(TRUE);
	m_IsXml.SetCheck(TRUE);
	m_IsFileExist.SetCheck(TRUE);
	m_IsDefFile.SetCheck(TRUE);
	m_IsDat.SetCheck(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWdfToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CWdfToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWdfToolDlg::OnBnClickedGameBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi; // 浏览的信息
	memset(&bi, 0, sizeof(BROWSEINFO));   

	char Buffer[MAX_PATH]="";

	//初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot =NULL;//初始化制定的root目录很不容易，
	//bi.pszDisplayName = *Buffer;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = L"请选择网易西游系列安装目录：";
	//bi.ulFlags = BIF_BROWSEINCLUDEFILES;//包括文件
	bi.ulFlags = BIF_EDITBOX;//包括文件

	bi.lpfn = NULL;
	bi.iImage=IDR_MAINFRAME;
	//初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, (LPWSTR)&Buffer); //取得文件夹路径到Buffer里
	}
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) return;
	//释放内存
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	m_GamePath.SetWindowTextW((LPCTSTR)Buffer);
}

void CWdfToolDlg::OnBnClickedFileBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
		BROWSEINFO bi; // 浏览的信息
	memset(&bi, 0, sizeof(BROWSEINFO));   

	char Buffer[MAX_PATH]="";

	//初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot =NULL;//初始化制定的root目录很不容易，
	//bi.pszDisplayName = *Buffer;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = L"请选择包裹的目录：";
	//bi.ulFlags = BIF_BROWSEINCLUDEFILES;//包括文件
	bi.ulFlags = BIF_EDITBOX;//包括文件

	bi.lpfn = NULL;
	bi.iImage=IDR_MAINFRAME;
	//初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, (LPWSTR)&Buffer); //取得文件夹路径到Buffer里
	}
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) return;
	//释放内存
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	m_FilePath.SetWindowTextW((LPCTSTR)Buffer);
}

void CWdfToolDlg::OnBnClickedPack()
{
	// TODO: 在此添加控件通知处理程序代码
	// 打包操作
	MessageBox(L"这个版本中无此功能",L"提示",0);
}

void CWdfToolDlg::OnBnClickedUnpack()
{
	// TODO: 在此添加控件通知处理程序代码
	// 解包操作
	m_UnPack_Button.EnableWindow(FALSE); // 禁用按钮
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
		MessageBox(L"请选择游戏安装目录的路径",L"提示",0);
		m_UnPack_Button.EnableWindow(TRUE); // 启用按钮
		return;
	}
	if(m_sFilePath=="")
	{
		MessageBox(L"请选择输出文件目录的路径",L"提示",0);
		m_UnPack_Button.EnableWindow(TRUE); // 启用按钮
		return;
	}

	
	unsigned int type = Wdf->GetGameType(m_sGamePath);
	if(type==0) 
	{
		MessageBox(L"请选择正确的游戏安装目录的路径\n未找到游戏目录下的Update.ini文件",L"提示",0);
		m_UnPack_Button.EnableWindow(TRUE); // 启用按钮
		return;
	}

	CStringList* WdfList = Wdf->GetWdfList(m_sGamePath); // 获得文件列表
	
	

	POSITION pos = WdfList->GetHeadPosition();
	POSITION pos1 = Wdf->GetFileName()->GetHeadPosition();
	while (pos!=NULL)
	{
		CString path = WdfList->GetNext(pos);
		CString name = Wdf->GetFileName()->GetNext(pos1);
		Wdf->OpenWdf(path,name,m_sFilePath);
	}
	Wdf->~CWDF();
	m_UnPack_Button.EnableWindow(TRUE); // 启用按钮
}
