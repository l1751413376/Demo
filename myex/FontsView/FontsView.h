// FontsView.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CFontsViewApp:
// �йش����ʵ�֣������ FontsView.cpp
//

class CFontsViewApp : public CWinApp
{
public:
	CFontsViewApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFontsViewApp theApp;
