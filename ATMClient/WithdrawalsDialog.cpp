// WithdrawalsDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ATMClient.h"
#include "WithdrawalsDialog.h"
#include "afxdialogex.h"


// CWithdrawalsDialog 对话框

IMPLEMENT_DYNAMIC(CWithdrawalsDialog, CDialogEx)

CWithdrawalsDialog::CWithdrawalsDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWithdrawalsDialog::IDD, pParent)
{

}

CWithdrawalsDialog::~CWithdrawalsDialog()
{
}

void CWithdrawalsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWithdrawalsDialog, CDialogEx)
END_MESSAGE_MAP()


// CWithdrawalsDialog 消息处理程序
