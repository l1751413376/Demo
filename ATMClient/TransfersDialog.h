#pragma once


// CTransfersDialog �Ի���

class CTransfersDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTransfersDialog)

public:
	CTransfersDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTransfersDialog();

// �Ի�������
	enum { IDD = IDD_Transfers };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
