// TransfersDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ATMClient.h"
#include "TransfersDialog.h"
#include "afxdialogex.h"


// CTransfersDialog �Ի���

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


// CTransfersDialog ��Ϣ�������
