
// Page_SM.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPage_SMApp: 
// �йش����ʵ�֣������ Page_SM.cpp
//

class CPage_SMApp : public CWinApp
{
public:
	CPage_SMApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPage_SMApp theApp;