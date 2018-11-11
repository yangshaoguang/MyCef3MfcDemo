
// CefMfcDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CefMfcDemo.h"
#include "CefMfcDemoDlg.h"
#include "afxdialogex.h"

#include "include/cef_app.h"
#include "ClientAppRender.h"
#include "include/cef_runnable.h"
#include "include/cef_cookie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCefMfcDemoDlg �Ի���

CCefMfcDemoDlg::CCefMfcDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCefMfcDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCefMfcDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCefMfcDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_GO, &CCefMfcDemoDlg::OnBnClickedBtnGo)
	ON_BN_CLICKED(IDC_BTN_GOFORD, &CCefMfcDemoDlg::OnBnClickedBtnGoford)
	ON_BN_CLICKED(IDC_BTN_GOBACK, &CCefMfcDemoDlg::OnBnClickedBtnGoback)
END_MESSAGE_MAP()


// CCefMfcDemoDlg ��Ϣ�������


BOOL CCefMfcDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	CefRefPtr<CSimpleClient> client(new CSimpleClient());
	m_simpleClient = client;

	CefRefPtr<ClientAppRender> app(new ClientAppRender());


	CefSettings settings;
	CefSettingsTraits::init(&settings);
	settings.multi_threaded_message_loop = true;
	settings.remote_debugging_port = 8088;//��������壬�������е��Թ���
	settings.single_process = false;

	CefMainArgs mainArgs;

	CefRefPtr<CefApp> cefApp;
	cefApp = app;

	CefInitialize(mainArgs, settings, cefApp, NULL);

	RECT rect;
	GetClientRect(&rect);
	RECT rectnew = rect;
	rectnew.top = rect.top + 50;
	rectnew.bottom = rect.bottom;
	rectnew.left = rect.left;
	rectnew.right = rect.right;

	CefWindowInfo winInfo;
	winInfo.SetAsChild(GetSafeHwnd(), rectnew);

	CefRefPtr<CefSetCookieCallback> callback = NULL;
	std::wstring username_key = L"Username"; //cookie��key
	std::wstring username_value = L"xiaonaiquan";//cookie��value
	std::wstring domain = L"csdn.net";

	CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);
	CefCookie cookie;
	CefString(&cookie.name).FromWString(username_key.c_str());
	CefString(&cookie.value).FromWString(username_value.c_str());
	CefString(&cookie.domain).FromWString(domain.c_str());
	CefString(&cookie.path).FromASCII("/");
	cookie.has_expires = false;

	domain = L"https://" + domain;
	CefPostTask(TID_IO, NewCefRunnableMethod(manager.get(), &CefCookieManager::SetCookie, CefString(domain.c_str()), cookie, callback));

	CefBrowserSettings browserSettings;
	CefBrowserHost::CreateBrowser(winInfo, m_simpleClient, domain.c_str(), browserSettings, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCefMfcDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCefMfcDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCefMfcDemoDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CefShutdown();

	CDialogEx::OnClose();
}



void CCefMfcDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	RECT rect;
	GetClientRect(&rect);

	if (m_simpleClient.get())
	{
		CefRefPtr<CefBrowser> browser = m_simpleClient->GetBrowser();
		if (browser)
		{
			CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
			::MoveWindow(hwnd, 0, 50, rect.right - rect.left, rect.bottom - 50, true);
		}
	}
}

/*
	Go��ť
*/
void CCefMfcDemoDlg::OnBnClickedBtnGo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strUrl;
	GetDlgItem(IDC_EDIT_URL)->GetWindowText(strUrl);
	if (strUrl.Trim().IsEmpty())
	{
		AfxMessageBox(_T("��������ַ"));
		return;
	}
	const CefString cefStrUrl(strUrl);
	m_simpleClient->GetBrowser()->GetMainFrame()->LoadURL(cefStrUrl);
}

/*
	ǰ����ť
*/
void CCefMfcDemoDlg::OnBnClickedBtnGoford()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_simpleClient->GetBrowser()->GoForward();
}

/*
	���˰�ť
*/
void CCefMfcDemoDlg::OnBnClickedBtnGoback()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_simpleClient->GetBrowser()->GoBack();
}

/*
	��Ӧ�س���
*/
BOOL CCefMfcDemoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:    // ���λس�
			OnBnClickedBtnGo();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
