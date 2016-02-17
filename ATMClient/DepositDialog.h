#pragma once


// CDepositDialog 对话框

class CDepositDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDepositDialog)

public:
	CDepositDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDepositDialog();

// 对话框数据
	enum { IDD = IDD_Deposit };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
