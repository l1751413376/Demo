#pragma once


// CWithdrawalsDialog �Ի���

class CWithdrawalsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CWithdrawalsDialog)

public:
	CWithdrawalsDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWithdrawalsDialog();

// �Ի�������
	enum { IDD = IDD_Withdrawals };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
