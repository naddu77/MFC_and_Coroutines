
// MFC_and_CoroutinesDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_and_Coroutines.h"
#include "MFC_and_CoroutinesDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <thread>
#include <format>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCandCoroutinesDlg dialog
#define WM_USER_COROUTINE	(WM_USER + 1)


CMFCandCoroutinesDlg::CMFCandCoroutinesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_AND_COROUTINES_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCandCoroutinesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCandCoroutinesDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_COROUTINE_TEST, &CMFCandCoroutinesDlg::OnBnClickedButtonCoroutineTest)
	ON_MESSAGE(WM_USER_COROUTINE, &CMFCandCoroutinesDlg::OnUserCoroutine)
END_MESSAGE_MAP()


// CMFCandCoroutinesDlg message handlers

BOOL CMFCandCoroutinesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCandCoroutinesDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCandCoroutinesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCandCoroutinesDlg::OnBnClickedButtonCoroutineTest()
{
	TestCoroutine();
}


afx_msg LRESULT CMFCandCoroutinesDlg::OnUserCoroutine(WPARAM wParam, LPARAM lParam)
{
	std::coroutine_handle<>::from_address(reinterpret_cast<void*>(wParam))();

	return 0;
}

FireAndForget CMFCandCoroutinesDlg::TestCoroutine()
{
	auto print_thread_id{ [this](std::wstring_view msg) {
		std::wstringstream ss;

		ss << std::this_thread::get_id();

		AfxMessageBox(std::format(L"{}\nThread ID: {}\n", msg, ss.str()).c_str());
	} };

	print_thread_id(L"Start a coroutine.");

	co_await ResumeBackground();

	print_thread_id(L"Running in the background for 5 seconds.");

	using namespace std::chrono_literals;

	std::this_thread::sleep_for(5s);

	co_await ResumeMfcForeground(this, WM_USER_COROUTINE);

	print_thread_id(L"End of coroutine.");
}
