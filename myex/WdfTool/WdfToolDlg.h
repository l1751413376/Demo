// WdfToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "WDF.h"


// CWdfToolDlg 对话框
class CWdfToolDlg : public CDialog
{
// 构造
public:
	CWdfToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WDFUNPACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGameBrowse();
	afx_msg void OnBnClickedFileBrowse();
	// 游戏安装目录路径
	CEdit m_GamePath;
	// 包裹目录路径
	CEdit m_FilePath;
	
	CString m_sGamePath; // 全局游戏安装目录路径 
	CString m_sFilePath; // 全局输出目录路径
	afx_msg void OnBnClickedPack();
	afx_msg void OnBnClickedUnpack();
	// 解包按钮
	CButton m_UnPack_Button;
	// 打包按钮
	CButton m_Pack_Button;
	// 是否提取文件
	CButton m_IsUnPack;
	// 是否生成XML文件
	CButton m_IsXml;
	CButton m_IsFileExist;
	CButton m_IsDefFile;
	CButton m_IsDat;
};
