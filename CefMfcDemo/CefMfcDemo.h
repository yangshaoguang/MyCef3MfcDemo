
// CefMfcDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCefMfcDemoApp: 
// �йش����ʵ�֣������ CefMfcDemo.cpp
//

class CCefMfcDemoApp : public CWinApp
{
public:
	CCefMfcDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCefMfcDemoApp theApp;