
// Page_SMDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPage_SMDlg �Ի���
class CPage_SMDlg : public CDialogEx
{
// ����
public:
	CPage_SMDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PAGE_SM_DIALOG };

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
	afx_msg void Random();
	static UINT threadFIFO(LPVOID lpParam);
	static UINT threadLRU(LPVOID lpParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton2();
//	CListBox m_list3;
	CListBox m_list1;
	CListBox m_list2;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	CListBox m_list3;
	CListBox m_list5;
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnKillfocusEdit3();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnEnKillfocusEdit5();
	CListCtrl m_ctllist;
};
