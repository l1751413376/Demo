// DepositDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ATMClient.h"
#include "DepositDialog.h"
#include "afxdialogex.h"


// CDepositDialog 对话框

IMPLEMENT_DYNAMIC(CDepositDialog, CDialogEx)

CDepositDialog::CDepositDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDepositDialog::IDD, pParent)
{

}

CDepositDialog::~CDepositDialog()
{
}

void CDepositDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDepositDialog, CDialogEx)
END_MESSAGE_MAP()


// CDepositDialog 消息处理程序
