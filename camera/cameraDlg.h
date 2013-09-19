// cameraDlg.h : header file
//

#if !defined(AFX_CAMERADLG_H__EA6457DF_4CBA_4422_A6F1_C3F5F3D700CB__INCLUDED_)
#define AFX_CAMERADLG_H__EA6457DF_4CBA_4422_A6F1_C3F5F3D700CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////



#include "camerads.h"
#include "highgui.h"
#include <cv.h>

// CCameraDlg dialog

class CCameraDlg : public CDialog
{
// Construction
public:
	void ImageThreshold(IplImage* img);
	static UINT ReceiveThread(void *param);
	CCameraDlg(CWnd* pParent = NULL);	// standard constructor

	CDC *pDCL, *pDCR ;			//图形设备环境指针
	HDC hDCL, hDCR ;			//设备上下文的句柄
	CRect rectL, rectR;			//窗口区域
	CCameraDS camera ;	//CCameraDS类的对象
	CvvImage m_imageL ;//左窗口图像数据
	CvvImage m_imageR ;//右窗口图像数据
	int m_iWidth;		//图像宽度
	int m_iHeight;		//图像高度
	bool m_bCameraOpen ;//判断图像是否已经打开，标志变量
	int cam_count;		//摄像头数目
	bool m_bImageShow ; //图像是否正在显示

// Dialog Data
	//{{AFX_DATA(CCameraDlg)
	enum { IDD = IDD_CAMERA_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCameraDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenCamera();
	afx_msg void OnImageShow();
	afx_msg void OnImageStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERADLG_H__EA6457DF_4CBA_4422_A6F1_C3F5F3D700CB__INCLUDED_)
