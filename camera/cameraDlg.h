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

	CDC *pDCL, *pDCR ;			//ͼ���豸����ָ��
	HDC hDCL, hDCR ;			//�豸�����ĵľ��
	CRect rectL, rectR;			//��������
	CCameraDS camera ;	//CCameraDS��Ķ���
	CvvImage m_imageL ;//�󴰿�ͼ������
	CvvImage m_imageR ;//�Ҵ���ͼ������
	int m_iWidth;		//ͼ����
	int m_iHeight;		//ͼ��߶�
	bool m_bCameraOpen ;//�ж�ͼ���Ƿ��Ѿ��򿪣���־����
	int cam_count;		//����ͷ��Ŀ
	bool m_bImageShow ; //ͼ���Ƿ�������ʾ

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
