
// Page_SMDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Page_SM.h"
#include "Page_SMDlg.h"
#include "afxdialogex.h"
#include<iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CPage_SMDlg * pp;
CWinThread* mythreadURL;
CString *psc;//����ָ��
int *psi;
int m_size;//פ������
int M_time; //�ڴ��ȡʱ��
int Q_time;//ȱҳ�ж�ʱ��
int K_time;//���ʱ��
struct mem//����LRU
{
	int num;
	int count;//δʹ�õĴ���
}memBlock[3] = { 0, 0, 0, 0, 0, 0 };//�ṹ�������ʼ�����ڴ�ҳ����Ϊ3
int memBlockF[3] = { 0, 0, 0 };//����FIFO

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPage_SMDlg �Ի���



CPage_SMDlg::CPage_SMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPage_SMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPage_SMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST3, m_list3);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST3, m_list3);
	DDX_Control(pDX, IDC_LIST5, m_list5);
	DDX_Control(pDX, IDC_LIST7, m_ctllist);
}

BEGIN_MESSAGE_MAP(CPage_SMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_SMDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CPage_SMDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CPage_SMDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage_SMDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CPage_SMDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage_SMDlg::OnBnClickedButton3)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CPage_SMDlg::OnEnKillfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CPage_SMDlg::OnEnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CPage_SMDlg::OnEnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT5, &CPage_SMDlg::OnEnKillfocusEdit5)
END_MESSAGE_MAP()


// CPage_SMDlg ��Ϣ�������

BOOL CPage_SMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	pp = this;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	DWORD dwStyle = m_ctllist.GetExtendedStyle();//����б���������
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_ctllist.SetExtendedStyle(dwStyle);
	m_ctllist.InsertColumn(0, "", LVCFMT_LEFT, 40);
	m_ctllist.InsertColumn(1, "��ʱ��", LVCFMT_LEFT, 80);  //����б��⣬LVCFMT_LEFT�������ö��뷽ʽ
	m_ctllist.InsertColumn(2, "ƽ��ʱ��", LVCFMT_LEFT, 80);
	m_ctllist.InsertColumn(3, "ȱҳ�ж���Ŀ", LVCFMT_LEFT, 80);
	m_ctllist.InsertItem(0, "FIFO");
	m_ctllist.InsertItem(1, "LRU");
	m_ctllist.InsertItem(2, "LFU");
	m_ctllist.InsertItem(3, "OPT");
	//m_list3.AddString((CString)"Ҫ���������");
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPage_SMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPage_SMDlg::OnPaint()
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
HCURSOR CPage_SMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CPage_SMDlg::Random()
{
	/*CString a;
	CString b;
	b.Format(_T("%.1f"), (rand()+10) % 10);*/
	CString string;
	CString string2;
	/*CString string3;*/
	int b[10];
	psc = new CString[10];
	psi = new int[10];
	for (int i = 0; i < 10; i++)
	{
		b[i] = rand() % 10;
		psi[i] = b[i];
		string.Format(_T("%d"), b[i]);
		psc[i] =string;
		/*string.Format(_T("%d"), ps[i]);*/
		string2 =string2+ string;
		/*string3 = string3 + string;*/
	}
	GetDlgItem(IDC_EDIT1)->SetWindowText(string2);
	/*GetDlgItem(IDC_EDIT2)->SetWindowTextW(string3);*/

	/*for (int i = 0; i < 10; i++)
	{
		CString b;
		b.Format(_T("%.1f"),rand()%10);
		a = a +b ;
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(a);
	}*/
	//return 0;
}
void CPage_SMDlg::OnBnClickedButton1()//���������
{
	Random();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

UINT CPage_SMDlg::threadFIFO(LPVOID lpParam)
{
	int good;
	CPage_SMDlg *ct = (CPage_SMDlg*)lpParam;
	ct->m_list1.ResetContent();
	CProgressCtrl *pProgressCtrl = (CProgressCtrl*)pp->GetDlgItem(IDC_PROGRESS1);
	ct->m_list2.ResetContent();
	//ct->m_list1.AddString(_T("Ҫ���������"));
	//ct->m_list1.AddString((CString)("Ҫ���������"));
	//ct->SetDlgItemTextW(IDC_STATIC, _T("Ҫ���������"));statictext���
	int jindu = 0;
	CString string;
	CString string2;
	CString string3;
	for (int i = 0; i < 10; i++)
	{
		bool bFind = false;
		for (int j = 0; j < 3; j++)
		{
			if (psi[i] == memBlockF[j])
			{
				bFind = true;
			}
		}

		if (!bFind)
		{
			for (int j = 1; i < 3; i++)
			{
				memBlockF[j] = memBlockF[j - 1];
			}
			memBlockF[0] = psi[i];
			
		}
		/*string2 = "";*/
		for (int j = 0; j < 3; j++)//���
		{

			string.Format(_T("%d"), memBlockF[j]);
			string2 = string2 + string;

		}
		string3 = string2.Right(3);
		/*ct->m_list2.AddString('(' + 'a' + ')' + cstring2);*/
		ct->m_list1.InsertString(i,string3);
		jindu = jindu + 10;
		pProgressCtrl->SetPos(jindu);
	}
	//FIFO�㷨��
	return 0;
}
UINT CPage_SMDlg::threadLRU(LPVOID lpParam)
{
	int good=0;
	CPage_SMDlg *ct = (CPage_SMDlg*)lpParam;
	ct->m_list2.ResetContent();
	CProgressCtrl *pProgressCtrl = (CProgressCtrl*)pp->GetDlgItem(IDC_PROGRESS2);//���ƽ���
	int jindu=0;
	CString cstring;
	CString cstring2;
	CString cstring3;
	for (int i = 0; i < 10; i++)
	{
		int maxCount = memBlock[0].count;
		int maxPos = 0;
		bool bFind = false;
		for (int j = 0; j < 3; j++)
		{
			// ��ǳ�countֵ����λ��
			if (maxCount < memBlock[j].count)
			{
				maxCount = memBlock[j].count;
				maxPos = j;
			}

			// �����е�countֵ��+1
			memBlock[j].count++;

			// ������У�����countֵ��Ϊ0
			if (psi[i] == memBlock[j].num)
			{
				memBlock[j].count = 0;
				bFind = true;
				good++;
			}
		}

		// δ���У���count���������滻
		if (!bFind)
		{
			memBlock[maxPos].num = psi[i];
			memBlock[maxPos].count = 0;
			Sleep(Q_time);
		}
		for (int j = 0; j < 3; j++)//���
		{
			
			cstring.Format(_T("%d"), memBlock[j].num);
			/*ct->m_list2.AddString(cstring);*/
			cstring2 = cstring2 + cstring;
			
		}
		/*ct->m_list2.AddString('(' + 'a' + ')' + cstring2);*/


		cstring3= cstring2.Right(3);//ȡ�Ӵ��ұ�3
		ct->m_list2.InsertString(i,cstring3);
		jindu = jindu + 10;
		pProgressCtrl->SetPos(jindu);
		/*cstring2.Empty();*/
	}

	int z_time;//��ʱ��
	int p_time;//ƽ��ʱ��
	int q_num;//ȱҳ�жϸ���
	CString cz_time;
	z_time = (K_time + M_time)*good + (10 - good)*(K_time + Q_time);
	p_time = z_time / 10;
	cz_time.Format(_T("%d"), z_time);
	ct->m_ctllist.SetItemText(0, 1, cz_time);
	cz_time.Format(_T("%d"), p_time);
	ct->m_ctllist.SetItemText(0, 2, cz_time);
	q_num = 10 - good;
	cz_time.Format(_T("%d"), q_num);
	ct->m_ctllist.SetItemText(0, 3, cz_time);
	return 0;
}

void CPage_SMDlg::OnBnClickedOk()//��ʼִ��
{

	for (int i = 0; i < 10; i++)
	{
		m_list5.InsertString(i,'('+ psc[i]+')');
	}
	CString str;
	//FIFO
	CWinThread* mythreadFIFO;
	mythreadFIFO = AfxBeginThread(// mythreadΪȫ�ֱ���
		threadFIFO,//ָ�����̵߳Ŀ��ƺ���
		this,//��Ҫ���͸����ƺ����Ĳ�����this��CThreadDlg���ָ�룬��������ÿؼ���
		THREAD_PRIORITY_NORMAL,//�������ȼ�
		0,//���߳�ʹ�õ�ջ�����ֽ�Ϊ��λ�Ĵ�С�����Ϊ0����ȱʡ��ջ��С�봴�������̵߳�ջ��С��ͬ
		0,//CREATE_SUSPENDED,//���𣨿����̵߳Ĵ������̣�
		0//��ָ�����̵߳İ�ȫ���ԡ����Ϊ�գ���ʹ���봴�������߳���ͬ�İ�ȫ����
		);
	/*CProgressCtrl *pProgressCtrl = (CProgressCtrl*)this->GetDlgItem(IDC_PROGRESS1);*///���ƽ�����
	/*pProgressCtrl->SetPos(10);*/
	//LRU
	mythreadFIFO = AfxBeginThread(
		threadLRU,
		this,
		THREAD_PRIORITY_NORMAL,
		0,0,0);


	//this->GetDlgItem(IDC_EDIT2)->GetWindowText(str);//�õ�ҳ��פ������str

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();�رնԻ���
}


void CPage_SMDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPage_SMDlg::OnBnClickedButton2()//����
{
	//CString strText(_T(""));
	//GetDlgItemText(IDC_LIST2, strText);
	//CStdioFile  file;
	////HANDLE hOpenFile = (HANDLE)CreateFile(L"E:\\����ϵͳ\\������Ŀ\\a.text", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	//file.Open(_T("a.txt"), CFile::modeReadWrite);//modeCreate | CFile::modeWrite |typeText	
	//file.WriteString(strText);//, sizeof(strText));//�ѷ���sд���ļ�
	//file.Close();  //�ر��ļ�
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	/*freopen("a.txt", "w", stdout);
	cout<<strText;*/
	CString str1;
	this->GetDlgItem(IDC_LIST2)->GetWindowText(str1);
	m_list3.AddString(str1);
	CStdioFile file;
	file.Open(_T(".\\a.txt"),CFile::modeNoTruncate | CFile::modeWrite);
	file.SeekToEnd();
	file.WriteString(str1);
	file.WriteString("\r\n");
	file.Flush();
	file.Close();
}

void CPage_SMDlg::OnBnClickedButton4()//����
{
	ResumeThread(mythreadURL->m_hThread);
	/*CString a;
	a.Format(_T("%d"), K_time);
	this->GetDlgItem(IDC_EDIT1)->SetWindowText(a);*/

	//((CListBox*)GetDlgItem(IDC_LIST3))->SetWindowText(psc[j]);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPage_SMDlg::OnBnClickedButton3()//ȫ����ͣ
{
	SuspendThread(mythreadURL->m_hThread);//m_hThread��ǰ�̵߳ľ��
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPage_SMDlg::OnEnKillfocusEdit2()//ҳ��פ�������¼�
{
	CEdit* pEdit;
	CString size;
	this->GetDlgItem(IDC_EDIT2)->GetWindowText(size);
	m_size = _ttoi(size);
	if (m_size<=0||m_size>=10)
	{
		MessageBox( _T("���������뷶Χ1��10������ֵ"));
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
		pEdit->SetFocus();
		pEdit->SetSel(0, size.GetLength());
	}
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPage_SMDlg::OnEnKillfocusEdit3()//�ڴ��ȡʱ���¼�
{
	CEdit* pEdit;
	CString mtime;
	this->GetDlgItem(IDC_EDIT3)->GetWindowText(mtime);
	M_time = _ttoi(mtime);
	if (M_time <= 0 || M_time >= 1000)
	{
		MessageBox(_T("���������뷶Χ1��1000������ֵ"));
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
		pEdit->SetFocus();
		pEdit->SetSel(0, mtime.GetLength());
	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPage_SMDlg::OnEnKillfocusEdit4()//ȱҳ�ж�ʱ���¼�
{
	CEdit* pEdit;
	CString qtime;
	this->GetDlgItem(IDC_EDIT4)->GetWindowText(qtime);
	Q_time = _ttoi(qtime);
	if (Q_time <= 0 || Q_time >= 1000)
	{
		MessageBox(_T("���������뷶Χ1��1000������ֵ"));
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT4);
		pEdit->SetFocus();
		pEdit->SetSel(0, qtime.GetLength());
	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPage_SMDlg::OnEnKillfocusEdit5()//���ʱ��
{
	CEdit* pEdit;
	CString ktime;
	this->GetDlgItem(IDC_EDIT5)->GetWindowText(ktime);
	K_time = _ttoi(ktime);
	if (K_time <= 0 || K_time >= 10)
	{
		MessageBox(_T("���������뷶Χ1��10������ֵ"));
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT5);
		pEdit->SetFocus();
		pEdit->SetSel(0, ktime.GetLength());
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
//��ʽת����
//CString number;
//CString number2;
//CString number3;
//int numb[10];
//int numb2;
//
//this->GetDlgItem(IDC_EDIT1)->GetWindowText(number);
//numb2 = _ttoi(number);//����ת����ȷ
//for (int i = 0; i < 10; i++)
//{
//	char *c = new char[1];
//	c[0] = number.GetAt(i);
//	CString cs = c;
//	numb[i] = atoi(c);
//	number2.Format(_T("%d"), numb[i]);
//	m_list3.InsertString(i, number2);//InsertString��AddString��ͬ
//}
//number3.Format(_T("%d"), numb2);
//this->GetDlgItem(IDC_EDIT2)->SetWindowText(number);
//this->GetDlgItem(IDC_EDIT4)->SetWindowText(number3);