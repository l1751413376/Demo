// WdfTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWdfToolApp:
// �йش����ʵ�֣������ wdfUnpack.cpp
//

class CWdfToolApp : public CWinApp
{
public:
	CWdfToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWdfToolApp theApp;