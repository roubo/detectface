// cameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "camera.h"
#include "cameraDlg.h"
#include "highgui.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraDlg dialog

CCameraDlg::CCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCameraDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_iWidth = 320 ;
	m_iHeight = 240 ;
	m_bCameraOpen = false ;
	m_bImageShow = false ;

}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCameraDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCameraDlg, CDialog)
	//{{AFX_MSG_MAP(CCameraDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Open_Camera, OnOpenCamera)
	ON_BN_CLICKED(IDC_Image_Show, OnImageShow)
	ON_BN_CLICKED(IDC_Image_Stop, OnImageStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraDlg message handlers

BOOL CCameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	
	

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCameraDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCameraDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCameraDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



UINT CCameraDlg::ReceiveThread(void *param)
{



	const char* cascade_name = "haarcascade_frontalface_alt2.xml";
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
    storage = cvCreateMemStorage(0);

	CCameraDlg *pDlg = (CCameraDlg*)param ;
	while(1)
	{
		if(!pDlg->m_bImageShow)
		{
			return 0 ;
		}

		//��ȡһ֡
		IplImage *pFrame = pDlg->camera.QueryFrame();
		
		//�����ʾ���ڣ���ʾԭʼͼ��
		pDlg->m_imageL.CopyOf(pFrame);
		SetRect(pDlg->rectL, 0,0,pFrame->width,pFrame->height);	
		pDlg->m_imageL.DrawToHDC(pDlg->hDCL,&(pDlg->rectL));

		//�������һ����������ͼ�����������Ľ����ʾ���ұߴ���

		pDlg->ImageThreshold(pFrame) ;

		//�ұ���ʾ���ڣ���ʾ������ͼ��
		pDlg->m_imageR.CopyOf(pFrame);
		SetRect(pDlg->rectR, 0,0,pFrame->width,pFrame->height);
		pDlg->m_imageR.DrawToHDC(pDlg->hDCR,&(pDlg->rectR));
		
		//��ʱ200����
		Sleep(200) ;
	
		
	}	

	return 0 ;
}

void CCameraDlg::OnOpenCamera() 
{
	if(m_bCameraOpen)
	{
		return ;
	}
	cam_count = CCameraDS::CameraCount();	
	if(!cam_count)
	{
		MessageBox("δ���ҵ�����ͷ!","",MB_ICONINFORMATION);
	}
	else
	{
		//��ȡ��������ͷ������
		CString str ;
		for(int i=0; i < cam_count; i++)
		{
			char camera_name[1024];  
			int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name) );
			
			if(retval >0)
			{
				str.Format("���Ϊ%d�����������Ϊ%s",i,camera_name) ;
				//MessageBox(str,"",MB_ICONINFORMATION);
			}
			else
			{
				str.Format("���ܻ�ñ��Ϊ%d�����������",i) ;
				MessageBox(str,"",MB_ICONINFORMATION);
			}		
		}
		//�����ʾ�������ù���
		pDCL = GetDlgItem(IDC_PICL)->GetDC() ;
		hDCL= pDCL->GetSafeHdc();
		GetDlgItem(IDC_PICL)->GetClientRect(&rectL);

		//�ұ���ʾ�������ù���
		pDCR = GetDlgItem(IDC_PICR)->GetDC() ;
		hDCR= pDCR->GetSafeHdc();
		GetDlgItem(IDC_PICR)->GetClientRect(&rectR);
		
		
		if(!camera.OpenCamera(0,0,320,240))
		{
			MessageBox("���ܴ�����ͷ!","",MB_ICONINFORMATION);			
		}
		else
		{
			MessageBox("����ͷ��ʼ���!","",MB_ICONINFORMATION);
			m_bCameraOpen = true ;
		}
	}	
}

void CCameraDlg::OnImageShow() 
{
	if(!m_bCameraOpen)
	{
		return ;
	}
	
	m_bImageShow = true ;
	AfxBeginThread(ReceiveThread, this) ;
}

void CCameraDlg::OnImageStop() 
{
	m_bImageShow = false ;
}

void CCameraDlg::ImageThreshold(IplImage* img )
{
	static CvScalar colors[] = 
	{
		{{0,0,255}},
		{{0,128,255}},
		{{0,255,255}},
		{{0,255,0}},
		{{255,128,0}},
		{{255,255,0}},
		{{255,0,0}},
		{{255,0,255}}
	};

	double scale = 1.3;
	IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
	IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
		cvRound (img->height/scale)),          //cvRound ���غͲ�����ӽ�������ֵ��
		8, 1 );
	int i;

	cvCvtColor( img, gray, CV_BGR2GRAY );      //CvtColorɫ�ʿռ�ת��,�Ӳ�ɫͼ��img ����Ҷ�ͼ�� gray
	cvResize( gray, small_img, CV_INTER_LINEAR );      //���� cvResize ��ͼ��gray  �ı�ߴ�õ���small_img  ͬ����С
	cvEqualizeHist( small_img, small_img );            //�Ҷ�ͼ��ֱ��ͼ���⻯,�÷�����һ��ͼ�����Ⱥ���ǿ�Աȶȡ�
	cvClearMemStorage( storage );         //���������ͷ��ڴ棨������ڴ棩��

	if( cascade )
	{
		double t = (double)cvGetTickCount();            //�ú�����������������/�û������ִ��ʱ��
		CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
			1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
			cvSize(30, 30) );//ĩ����Ϊ��ⴰ�ڵ���С�ߴ�
		for( i = 0; i < (faces ? faces->total : 0); i++ )
		{
			CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
			CvPoint center;
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*scale);
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
			//Ϊ��֮ǰ��Ҫ����scale��ͼ���ȱ�С�ټ�⣬�Ƿ������²��������ã�
		}
	}

}
