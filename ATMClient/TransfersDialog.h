#pragma once


// CTransfersDialog 对话框

class CTransfersDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTransfersDialog)

public:
	CTransfersDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTransfersDialog();

// 对话框数据
	enum { IDD = IDD_Transfers };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
