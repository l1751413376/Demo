#pragma once


// CDepositDialog �Ի���

class CDepositDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDepositDialog)

public:
	CDepositDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDepositDialog();

// �Ի�������
	enum { IDD = IDD_Deposit };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
