
// CefMfcDemoDlg.h : ͷ�ļ�
//
// http://blog.csdn.net/zhuhongshu/article/details/70159672
// �ںܶ�汾��Cef���������˶��߳���Ϣѭ�����ᵼ�³����ڽ���ʱ�����жϣ�������Cef��bug��������release�汾��Cef��û�����⡣
// ��2623��2526�汾��Debugģʽ�У����������multi_threaded_message_loop���������˳�ʱ���ض��ᴥ���жϡ��������Cef��bug���ڹٷ�demo��Ҳ��������⣬������Releaseģʽ����û������ġ�
// �����ʹ��Cefģ��������һЩ�������������쳣��������ʹ��releaseģʽ+���������ģʽ������һ�Σ��ܶ����ⶼ��debugģʽ���ߵ�����ģʽ���µ�

#pragma once

#include "SimpleClient.h"

// CCefMfcDemoDlg �Ի���
class CCefMfcDemoDlg : public CDialogEx
{
// ����
public:
	CCefMfcDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CEFMFCDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	CefRefPtr<CSimpleClient>  m_simpleClient;

	BOOL PreTranslateMessage(MSG* pMsg);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnGo();
	afx_msg void OnBnClickedBtnGoford();
	afx_msg void OnBnClickedBtnGoback();
};
