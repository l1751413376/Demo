// TransfersDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ATMClient.h"
#include "TransfersDialog.h"
#include "afxdialogex.h"


// CTransfersDialog 对话框

IMPLEMENT_DYNAMIC(CTransfersDialog, CDialogEx)

CTransfersDialog::CTransfersDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTransfersDialog::IDD, pParent)
{

}

CTransfersDialog::~CTransfersDialog()
{
}

void CTransfersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTransfersDialog, CDialogEx)
END_MESSAGE_MAP()


// CTransfersDialog 消息处理程序
