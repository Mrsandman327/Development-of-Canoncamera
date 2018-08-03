
// Canon TestDlg.h : 头文件
//

#pragma once


// CCanonTestDlg 对话框
class CCanonTestDlg : public CDialogEx
{
// 构造
public:
	CCanonTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CANONTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnShot();
	CComboBox m_Av;
	CComboBox m_Iso;
	CComboBox m_Tv;
	afx_msg void OnCbnSelchangeComboIso();
	afx_msg void OnCbnSelchangeComboAv();
	afx_msg void OnCbnSelchangeComboTv();
	afx_msg void OnBnClickedButtonEnd();
	afx_msg void OnBnClickedButtonStart();
	
	static UINT ThreadPhotograph(LPVOID ThreaParam);
	CDC *pDC;
	CRect rect;
	long ts[10];
	long tt[10];
	
	afx_msg void OnBnClickedButtonStopview();
};
