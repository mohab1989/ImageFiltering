
// ProFilterDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"
#include "Undo.h"

// CProFilterDlg dialog
class CProFilterDlg : public CDialogEx
{
// Construction
public:
	CProFilterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ImageFiltering};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	
	Undo undo;
	CString SelectedFilter;

	CMyImage image;


	CComboBox m_FilterSelect;

	unsigned long KernelRank;
	
	CEdit m_KernelRankBuddy;
	CEdit m_SigmaValue;

	CButton m_ApplyFilter;
	CButton m_btnSave;

	CStatic m_FilterSizeMessege;
	CStatic m_BMP;
	CStatic m_GroupBox;
	CStatic m_Messege;

	CSpinButtonCtrl m_KernelRank;
	CSpinButtonCtrl m_SigmaValueSpin;

	CPoint firstPoint;
	CPoint BfirstPoint;

	boolean draw;
	
	CRect Boundry;
	
	void CProFilterDlg::Message(CString Error);
	void show(CMyImage &image);
	void TopDown();
	
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedApplyfilter();	
	afx_msg void OnCbnSelchangeFilterselect();
	afx_msg void OnBnClickedPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedUndo();
};
