
// ATMClientDlg.h : ͷ�ļ�
//

#pragma once


// CATMClientDlg �Ի���
class CATMClientDlg : public CDialogEx
{
// ����
public:
	CATMClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ATMCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void Deposit();
	afx_msg void Withdrawals();
	afx_msg void TransfersDialog();
	afx_msg void OnBnClickedCancel2();
};
