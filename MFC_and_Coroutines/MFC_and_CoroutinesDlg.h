
// MFC_and_CoroutinesDlg.h : header file
//

#pragma once
#include "MyCoroutines.h"


// CMFCandCoroutinesDlg dialog
class CMFCandCoroutinesDlg : public CDialogEx
{
// Construction
public:
	CMFCandCoroutinesDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_AND_COROUTINES_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCoroutineTest();
protected:
	afx_msg LRESULT OnUserCoroutine(WPARAM wParam, LPARAM lParam);

private:
	FireAndForget TestCoroutine();
};
