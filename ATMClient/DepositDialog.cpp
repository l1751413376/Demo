// DepositDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ATMClient.h"
#include "DepositDialog.h"
#include "afxdialogex.h"


// CDepositDialog �Ի���

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


// CDepositDialog ��Ϣ�������
