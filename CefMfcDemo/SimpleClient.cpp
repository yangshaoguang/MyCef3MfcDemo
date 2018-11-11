#include "SimpleClient.h"
#include  <tchar.h>

enum { ID_CMD_REFRESH = 0 ,
	ID_SHOW_DEV_TOOLS
};

CSimpleClient::CSimpleClient()
{
	hWnd_ = NULL;
}


CSimpleClient::~CSimpleClient()
{
}

void CSimpleClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	if (!m_cefBrowser.get())
	{
		m_cefBrowser = browser;
		m_BrowserHwnd = browser->GetHost()->GetWindowHandle();
	}
}

//void CSimpleClient::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
//{
//
//	command_line->AppendSwitch("no-proxy-server");
//	command_line->AppendSwitch("--enable-npapi");
//	command_line->AppendSwitch("--disable-web-security");
//	command_line->AppendSwitch("allow-outdated-plugins");
//
//	//manifest.json�е�version
//	command_line->AppendSwitchWithValue("ppapi-flash-version","20.0.0.228");
//
//	//����flash���
//	command_line->AppendSwitchWithValue("ppapi-flash-path", "PepperFlash\\pepflashplayer.dll");
//
//}

bool CSimpleClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message)
{
	const std::string& messageName = message->GetName();
	if (messageName == "login_msg")
	{
		// extract message
		CefRefPtr<CefListValue> args = message->GetArgumentList();
		CefString strUser = args->GetString(0);
		CefString strPassword = args->GetString(1);
		TCHAR szLog[256] = { 0 };
		_stprintf_s(szLog, 256, L"BrowserProcess, user - %s, password - %s\r\n", strUser.c_str(), strPassword.c_str());
		OutputDebugString(szLog);

		//send reply to render process
		CefRefPtr<CefProcessMessage> outMsg = CefProcessMessage::Create("login_reply");

		// Retrieve the argument list object.
		CefRefPtr<CefListValue> replyArgs = outMsg->GetArgumentList();

		// Populate the argument values.
		replyArgs->SetSize(1);
		replyArgs->SetInt(0, 0);

		// Send the process message to the renderer process.
		browser->SendProcessMessage(PID_RENDERER, outMsg);

		return true;
	}
	return false;
}

bool CSimpleClient::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	WindowOpenDisposition target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	switch (target_disposition)
	{
	case WOD_NEW_FOREGROUND_TAB:
	case WOD_NEW_BACKGROUND_TAB:
	case WOD_NEW_POPUP:
	case WOD_NEW_WINDOW:
		browser->GetMainFrame()->LoadURL(target_url);
		return true; //cancel create
	}

	return false;
}

void CSimpleClient::OnBeforeDownload(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback)
{
	callback->Continue(download_item->GetURL(), true);
}

void CSimpleClient::OnDownloadUpdated(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	CefRefPtr<CefDownloadItemCallback> callback)
{
	if (download_item->IsComplete())
	{
		//MessageBox.Show("���سɹ�");
		OutputDebugString(L"���سɹ�");
		if (browser->IsPopup() && !browser->HasDocument())
		{
			//browser->GetHost()->ParentWindowWillClose();
			browser->GetHost()->CloseBrowser(true);
		}
	}
	else
	{
		//���ȡ��Ӧ����ת��һ����ҳ
		browser->GoBack();
	}
}

bool CSimpleClient::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
	const CefKeyEvent& event,
	CefEventHandle os_event,
	bool* is_keyboard_shortcut) 
{
	if (event.type != KEYEVENT_RAWKEYDOWN)
		return false;

	if (event.windows_key_code == 116)//F5ˢ��
		browser->Reload();
	else if (event.windows_key_code == VK_F12)
	{
		ShowDevelopTools(browser, CefPoint(0, 0));
	}
	return false;
}

bool CSimpleClient::OnKeyEvent(CefRefPtr<CefBrowser> browser,
	const CefKeyEvent& event,
	CefEventHandle os_event) {
	return false;
}

//�Ҽ��˵�
void CSimpleClient::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model) 
{
	if (model->GetCount() > 0) {// ˢ�²˵�
		model->AddSeparator();
		model->AddItem(ID_CMD_REFRESH, __TEXT("ˢ��"));
		model->AddItem(ID_SHOW_DEV_TOOLS, __TEXT("������ѡ��"));
	}
}

bool CSimpleClient::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	int command_id, EventFlags event_flags) 
{
	switch (command_id)
	{
	case ID_CMD_REFRESH:
		browser->Reload();
		break;
	case ID_SHOW_DEV_TOOLS:
	{
		ShowDevelopTools(browser, CefPoint(0,0));
		break;
	}
	default:
		break;
	}
	return false;
}

//����ѡ��
void CSimpleClient::ShowDevelopTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at) 
{
	CefWindowInfo windowInfo;
	CefBrowserSettings settings;
	windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), "DevTools");
	browser->GetHost()->ShowDevTools(windowInfo, this, settings, CefPoint());
}

void CSimpleClient::CloseDevelopTools(CefRefPtr<CefBrowser> browser)
{
	browser->GetHost()->CloseDevTools();
}

bool CSimpleClient::DoClose(CefRefPtr<CefBrowser> browser)
{
	////ͬһ��browser�����ж�����������(���µ��Ӵ��ڴ�����,�������ڵ�ǰ������תʱ�����.),��ʱ����ֵ���ܻᱻ������һЩ�ر���.���˽���ο��ٷ�demo.
	//if (CefCurrentlyOn(TID_UI))
	//{
	//	return true;
	//}
	return false;	//����trueȡ���ر�
}
void CSimpleClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	if (m_BrowserHwnd == browser->GetHost()->GetWindowHandle())
	{// ��������ڱ����ٵĻ�,browserָ����ΪNULL.
		m_cefBrowser = NULL;
	}

}







