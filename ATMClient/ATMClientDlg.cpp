
// ATMClientDlg.cpp : ʵ���ļ�
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


// CATMClientDlg �Ի���

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


// CATMClientDlg ��Ϣ�������

BOOL CATMClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CATMClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CATMClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CATMClientDlg::Deposit()
{
	CDepositDialog Deposit;
	Deposit.DoModal();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CATMClientDlg::Withdrawals()
{
	CWithdrawalsDialog Withdrawals;
	Withdrawals.DoModal();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CATMClientDlg::TransfersDialog()
{
	CTransfersDialog TransfersDialog;
	TransfersDialog.DoModal();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
