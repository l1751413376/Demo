// WithdrawalsDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ATMClient.h"
#include "WithdrawalsDialog.h"
#include "afxdialogex.h"


// CWithdrawalsDialog �Ի���

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


// CWithdrawalsDialog ��Ϣ�������
