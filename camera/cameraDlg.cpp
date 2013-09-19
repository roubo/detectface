// cameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "camera.h"
#include "cameraDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*内存存储器是一个可用来存储诸如序列，轮廓，图形,子划分等动态增长数据结构的底层结构。它是由一系列以同等
大小的内存块构成，呈列表型 ---bottom 域指的是列首，top 域指的是当前指向的块但未必是列尾.在bottom和top
之间所有的块(包括bottom, 不包括top）被完全占据了空间；在top和列尾之间所有的块（包括块尾，不包括top)则
是空的；而top块本身则被占据了部分空间 -- free_space 指的是top块剩余的空字节数。新分配的内存缓冲区（或
显示的通过cvMemStorageAlloc 函数分配，或隐示的通过 cvSeqPush, cvGraphAddEdge等高级函数分配）总是起
始于当前块（即top块)的剩余那部分，如果剩余那部分能满足要求（够分配的大小）。分配后，free_space 就减少
了新分配的那部分内存大小，外加一些用来保存适当列型的附加大小。当top块的剩余空间无法满足被分配的块（缓冲
区）大小时，top块的下一个存储块被置为当前块（新的top块） -- free_space 被置为先前分配的整个块的大小。
如果已经不存在空的存储块（即：top块已是列尾），则必须再分配一个新的块（或从parent那继承,见
cvCreateChildMemStorage)并将该块加到列尾上去。于是，存储器（memory storage)就如同栈（Stack)那样,
bottom指向栈底，(top, free_space)对指向栈顶。栈顶可通过cvSaveMemStoragePos保存，通过 
cvRestoreMemStoragePos 恢复指向， 通过 cvClearStorage重置。*/
static CvMemStorage* storage = 0;//内存存储器，
static CvHaarClassifierCascade* cascade = 0;//利用分类器文件进行人脸检测

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

	/*属于初始化的一部分，设置了图像的高度、宽度以及摄像头的状态和图像显示的状态*/
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


/*消息映射*/

BEGIN_MESSAGE_MAP(CCameraDlg, CDialog)
	//{{AFX_MSG_MAP(CCameraDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Open_Camera, OnOpenCamera)//前面是控件名称，后面一个参数是控件对应的成员函数
	ON_BN_CLICKED(IDC_Image_Show, OnImageShow)
	ON_BN_CLICKED(IDC_Image_Stop, OnImageStop)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCameraDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCameraDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraDlg message handlers
//初始化对话框
BOOL CCameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);//该函数允许应用程序为复制或修改而访问窗口菜单（系统菜单或控制菜单）
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

		//获取一帧
		IplImage *pFrame = pDlg->camera.QueryFrame();
		
		//左边显示窗口，显示原始图像
		pDlg->m_imageL.CopyOf(pFrame);
		SetRect(pDlg->rectL, 0,0,pFrame->width,pFrame->height);	
		pDlg->m_imageL.DrawToHDC(pDlg->hDCL,&(pDlg->rectL));//CvvImage这个类的成员函数DrawToHDC可将位图整体经缩放后显示到视图窗口中，解决了位图的显示问题，

		//可以添加一个函数用于图像处理，将处理后的结果显示在右边窗口

		pDlg->ImageThreshold(pFrame) ;

		//右边显示窗口，显示处理后的图像
		pDlg->m_imageR.CopyOf(pFrame);
		SetRect(pDlg->rectR, 0,0,pFrame->width,pFrame->height);
		pDlg->m_imageR.DrawToHDC(pDlg->hDCR,&(pDlg->rectR));
		
		//延时200毫秒
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
		MessageBox("未查找到摄像头!","",MB_ICONINFORMATION);
	}
	else
	{
		//获取所有摄像头的名称
		CString str ;
		for(int i=0; i < cam_count; i++)
		{
			char camera_name[1024];  
			int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name) );
			
			if(retval >0)
			{
				str.Format("编号为%d摄像机的名字为%s",i,camera_name) ;
				//MessageBox(str,"",MB_ICONINFORMATION);
			}
			else
			{
				str.Format("不能获得编号为%d摄像机的名字",i) ;
				MessageBox(str,"",MB_ICONINFORMATION);
			}		
		}
		//左边显示窗口设置关联
		pDCL = GetDlgItem(IDC_PICL)->GetDC() ;
		hDCL= pDCL->GetSafeHdc();
		GetDlgItem(IDC_PICL)->GetClientRect(&rectL);

		//右边显示窗口设置关联
		pDCR = GetDlgItem(IDC_PICR)->GetDC() ;
		hDCR= pDCR->GetSafeHdc();
		GetDlgItem(IDC_PICR)->GetClientRect(&rectR);
		
		
		if(!camera.OpenCamera(0,0,320,240))
		{
			MessageBox("不能打开摄像头!","",MB_ICONINFORMATION);			
		}
		else
		{
			MessageBox("摄像头初始完毕!","",MB_ICONINFORMATION);
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
/*图像处理的一个函数（非常重要），CvScalar是一个可以用来存放4个double数值的数组；
一般用来存放像素值，最多可以存放四个通道的，这里设置很多的colors是用不同的颜色表示
不同的人脸，在这个线程中主要完成了图像处理和人脸检测部分，人脸检测是采用Harr分类器，
这个分类器是经过多次训练得到的，在这里我们直接使用。*/
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
		cvRound (img->height/scale)),          //cvRound 返回和参数最接近的整数值。
		8, 1 );
	int i;

	cvCvtColor( img, gray, CV_BGR2GRAY );      //CvtColor色彩空间转换,从彩色图像img 输出灰度图像 gray
	cvResize( gray, small_img, CV_INTER_LINEAR );      //函数 cvResize 将图像gray  改变尺寸得到与small_img  同样大小
	cvEqualizeHist( small_img, small_img );            //灰度图象直方图均衡化,该方法归一化图像亮度和增强对比度。
	cvClearMemStorage( storage );         //函数并不释放内存（仅清空内存）。

	if( cascade )
	{
		double t = (double)cvGetTickCount();            //该函数可用来测量函数/用户代码的执行时间
		CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
			1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
			cvSize(30, 30) );//末参数为检测窗口的最小尺寸
		for( i = 0; i < (faces ? faces->total : 0); i++ )
		{
			CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
			CvPoint center;
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*scale);
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
			//为何之前需要变量scale将图像先变小再检测，是否起到向下采样的作用？
		}
	}

}




