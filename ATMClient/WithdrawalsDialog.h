#pragma once


// CWithdrawalsDialog 对话框

class CWithdrawalsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CWithdrawalsDialog)

public:
	CWithdrawalsDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWithdrawalsDialog();

// 对话框数据
	enum { IDD = IDD_Withdrawals };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
