// WdfToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "WDF.h"


// CWdfToolDlg �Ի���
class CWdfToolDlg : public CDialog
{
// ����
public:
	CWdfToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WDFUNPACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGameBrowse();
	afx_msg void OnBnClickedFileBrowse();
	// ��Ϸ��װĿ¼·��
	CEdit m_GamePath;
	// ����Ŀ¼·��
	CEdit m_FilePath;
	
	CString m_sGamePath; // ȫ����Ϸ��װĿ¼·�� 
	CString m_sFilePath; // ȫ�����Ŀ¼·��
	afx_msg void OnBnClickedPack();
	afx_msg void OnBnClickedUnpack();
	// �����ť
	CButton m_UnPack_Button;
	// �����ť
	CButton m_Pack_Button;
	// �Ƿ���ȡ�ļ�
	CButton m_IsUnPack;
	// �Ƿ�����XML�ļ�
	CButton m_IsXml;
	CButton m_IsFileExist;
	CButton m_IsDefFile;
	CButton m_IsDat;
};
