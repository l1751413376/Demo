
// ATMClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ATMClient.h"
#include "ATMClientDlg.h"
#include "afxdialogex.h"
#include "TransfersDialog.h"
#include "DepositDialog.h"
#include "WithdrawalsDialog.h"
#include "Initsock.h"
#include "Entity.h"
#include <typeinfo.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CATMClientDlg 对话框

CInitSock InitSock;
SOCKET s;
CATMClientDlg::CATMClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CATMClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CATMClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CATMClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON1, &CATMClientDlg::Deposit)
	ON_BN_CLICKED(IDC_BUTTON2, &CATMClientDlg::Withdrawals)
	ON_BN_CLICKED(IDC_BUTTON3, &CATMClientDlg::TransfersDialog)
	ON_BN_CLICKED(IDCANCEL2, &CATMClientDlg::OnBnClickedCancel2)
END_MESSAGE_MAP()


// CATMClientDlg 消息处理程序

BOOL CATMClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CATMClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CATMClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CATMClientDlg::Deposit()
{
	CDepositDialog Deposit;
	Deposit.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}


void CATMClientDlg::Withdrawals()
{
	CWithdrawalsDialog Withdrawals;
	Withdrawals.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}


void CATMClientDlg::TransfersDialog()
{
	CTransfersDialog TransfersDialog;
	TransfersDialog.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}


void CATMClientDlg::OnBnClickedCancel2()
{
	s=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(s==INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return;
	}

	sockaddr_in servAddr;
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(4567);
	servAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");


	if(::connect(s,(sockaddr*)&servAddr,sizeof(sockaddr_in))==-1)
	{
		printf(" Failed connect() \n");
		return ;
	}

	LoginEntity login;
	login.No=1;
	login.Pwd=1234;
	DataExchange data;
	strcpy_s(data.dataType,typeid(LoginEntity).name());
	memmove(data.data,&login,sizeof(LoginEntity));

	char szTest[256];
	::send(s,(char *)&data,sizeof(DataExchange),0);
}
