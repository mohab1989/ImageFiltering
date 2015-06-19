// ProFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProFilter.h"
#include "ProFilterDlg.h"
#include "afxdialogex.h"
#include "Filters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CProFilterDlg dialog



CProFilterDlg::CProFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProFilterDlg::IDD, pParent)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BMP, m_BMP);
	DDX_Control(pDX, IDC_FilterSelect, m_FilterSelect);
	DDX_Control(pDX, IDC_KernelRank, m_KernelRank);
	DDX_Control(pDX, IDC_KernelRankBuddy, m_KernelRankBuddy);
	DDX_Control(pDX, IDC_ApplyFilter, m_ApplyFilter);
	DDX_Control(pDX, IDC_FilterSizeMessege, m_FilterSizeMessege);
	DDX_Control(pDX, IDC_Message, m_Messege);
	DDX_Control(pDX, IDC_SigmaValue, m_SigmaValue);
	DDX_Control(pDX, IDC_SigmaValueSpin, m_SigmaValueSpin);
	DDX_Control(pDX, IDC_GroupBox, m_GroupBox);
	DDX_Control(pDX, IDC_Save, m_btnSave);
}

BEGIN_MESSAGE_MAP(CProFilterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BrowseBtn, &CProFilterDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_ApplyFilter, &CProFilterDlg::OnBnClickedApplyfilter)
	ON_CBN_SELCHANGE(IDC_FilterSelect, &CProFilterDlg::OnCbnSelchangeFilterselect)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_Save, &CProFilterDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_Undo, &CProFilterDlg::OnBnClickedUndo)
END_MESSAGE_MAP()


// CProFilterDlg message handlers

BOOL CProFilterDlg::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	draw = false;
	m_KernelRank.SetPos(3);
	m_KernelRank.SetRange32(3,100);
	m_SigmaValueSpin.SetPos(1);
	m_SigmaValueSpin.SetRange32(1,100);

	m_FilterSelect.EnableWindow(0);
	m_KernelRank.EnableWindow(0);
	m_KernelRankBuddy.EnableWindow(0);
	m_SigmaValue.EnableWindow(0);
	m_ApplyFilter.EnableWindow(0);
	m_btnSave.EnableWindow(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// a function that displays text on static text control ( messege to user)
void CProFilterDlg::Message(CString Error)
{
	m_Messege.SetWindowTextW(Error);
}

//Event handler for click to add button
void CProFilterDlg::OnBnClickedBtnBrowse()
{
	CString ImagePath;
	CString msg;
	msg.Format(_T(""));
	Message(msg);
	ImagePath="";
	CFileDialog Browse(true);
	if(Browse.DoModal() == IDOK)
	{
		 ImagePath = Browse.GetPathName();
	}
	//check if there is a path selected
	if(ImagePath!="")
	{
		CString ext;
		ext=ImagePath.Right(4);
		ext.MakeLower();
		//check if the input file is an image
			if (ext == ".bmp" || ext == ".jpg" || ext == ".png" ) 
			{
				image.Destroy();
				image.Load(ImagePath);
				//check if image is bigger than group box (frame)
				if (image.GetWidth()<=1030 && image.GetHeight()<=966)
				{
					TopDown();
					show(image);
					draw=true;
					m_btnSave.EnableWindow(1);
					m_FilterSelect.EnableWindow(1);
					undo.FreeUnDo(image);
					msg.Format(_T("Image Loaded .. You can draw lines over the image (click left mouse drag and release)"));
					Message(msg);
				}
				else
				{
					msg.Format(_T("Image Is Too Big.. Maximum Resolution is (1030x966)Pixels"));
					Message(msg);
				}
				
				
			}
			else
			{
				msg.Format(_T("The Selected File Is No An Image"));
				Message(msg);
			}
	}
}

// A function that converts all images to 32 bits top-down bitmap (first element is top left)
void CProFilterDlg::TopDown()
{
	unsigned long nx, ny;
	unsigned long ImageLength;
	if (image.GetBPP() < 8) return;
	CString msg;
	byte *t, r, g, b;			
	int *p, *q,*buf;
	unsigned long i, j;
	RGBQUAD *pRGB = new RGBQUAD[256];	// For GetDIBColorTable()

	nx = image.GetWidth();
	ny = image.GetHeight();
	ImageLength = nx * ny;	// No. of pixels


	// Allocate n sized buffer for temp storage

	if (!(buf=new int [ImageLength * sizeof(int)])) 
	{	
		msg.Format(_T("could not allocate memory for image buffer"));
		Message(msg);
		free(pRGB);
		return;
	}

	BeginWaitCursor();
	
	switch (image.GetBPP()) 
	{
		case 8:
			// GetDIBColorTable returns zero only if it fails
			if (!(i = GetDIBColorTable(image.GetDC(), 0, 256, pRGB))) 
			{
				msg.Format(_T("could not GetDIBColorTable()"));
				Message(msg);
				//device context (drawing)
				image.ReleaseDC();
				goto End;
			}
			
			image.ReleaseDC();

			for (j = 0, q = buf; j < ny; j++) 
			{
				// t points to the address of the pixel with the x and y co-ordinates
				t = (byte *) image.GetPixelAddress(0, j);	 
				for (i = 0; i < nx; i++, t++, q++) 
				{
					r = pRGB[*t].rgbRed;
					g = pRGB[*t].rgbGreen;
					b = pRGB[*t].rgbBlue;
					*q = RGB(b, g, r);	// CImage is BGR
				}
			}
			break;

		case 24:
			for (j = 0, q = buf; j < ny; j++) 
			{
				t = (byte *) image.GetPixelAddress(0, j);	 
				for (i = 0; i < nx; i++, t++, q++) 
				{
					b = *t;			// CImage is BGR
					g = *(++t);
					r = *(++t);
					*q = RGB(b, g, r);
				}
			}
			break;
		case 32:	// Just need to make top-down
			for (j = 0, q = buf; j < ny; j++) 
			{
				//used the p pointer instead of the t because the p pointer is of type int wich is 32 bits we dont need to break the colors down to bytes 8 bits. 
				p = (int *) image.GetPixelAddress(0, j);	 
				for (i = 0; i < nx; i++, p++, q++) {
					*q = *p;
				}
			}
			break;
			default:
			goto End;
	}

	// Start a new CImage
	image.Destroy();
	// convert all image to 32 and create a top down DIP last parameter is zero because alfa (transperacy) is zero
	if (!image.Create(nx, -(int)ny, 32, 0)) 
	{
		msg.Format(_T("Failed at creating bitmap"));
		Message(msg);
		goto End;
	}
	p = (int *) image.GetBits();	// Ptr to new bitmap (top-down DIB)
	memcpy_s(p, ImageLength * sizeof(int), buf, ImageLength * sizeof(int)); // Copy buf to bitmap

End:
	EndWaitCursor();
	free(pRGB);
	free(buf);

}

//function that displays the Cimage object on the Cstatic control (bitmap)
void CProFilterDlg::show(CMyImage &image)
{	
	m_BMP.SetBitmap(image);
	//gets the rect of the image with respect to client
	CWnd *pWnd = GetDlgItem(IDC_BMP);
	pWnd->GetWindowRect(&Boundry);
	ScreenToClient(&Boundry);	
}

// apply filter event handler
void CProFilterDlg::OnBnClickedApplyfilter()
{
		CString msg;
		msg.Format(_T(""));
		Message(msg);
		CString Value;
		m_KernelRankBuddy.GetWindowText(Value);
		KernelRank= _wtoi(Value);
	//if even number turn to odd
		if ((KernelRank % 2==0))
	{
		KernelRank -= 1;
	}
	
		if ((KernelRank<3)||(KernelRank> 100))
	{
		msg.Format(_T("Wrong filter size, Enter Odd Number that is more than 3 "));
		goto End;
	}
		
	if (SelectedFilter=="Average")
	{
		msg.Format(_T("Applying Average..Wait!!"));	
		Message(msg);
		BeginWaitCursor();
		//Add image before change to undo stack
		undo.OnDo(image);

		Filters Filter;
		Filter.AverageFilter(image,KernelRank);
		show(image);
		msg.Format(_T("Average Done!!"));	
		Message(msg);
		EndWaitCursor();
	}
	else if (SelectedFilter=="Median")
	{
		msg.Format(_T("Applying Median..Wait!!"));	
		Message(msg);
		BeginWaitCursor();

		//Add image before change to undo stack
		undo.OnDo(image);

		Filters Filter;
		Filter.MedianFilter(image,KernelRank);
		show(image);
		msg.Format(_T("Median Done!!"));	
		Message(msg);
		EndWaitCursor();
	}
	else if (SelectedFilter=="Gaussian")
	{
		float Sigma=0;
		CString Value;
		m_SigmaValue.GetWindowText(Value);

		//check that the entered value is a float
		for (int i = 0; i < Value.GetLength(); i++){
			if (  Value.GetAt(i)!='0'&& Value.GetAt(i)!='1' && Value.GetAt(i)!='2'&& Value.GetAt(i)!='3'
			    &&Value.GetAt(i)!='4'&& Value.GetAt(i)!='5'&& Value.GetAt(i)!='6'&& Value.GetAt(i)!='7'
				&&Value.GetAt(i)!='8'&&Value.GetAt(i)!='9'&&Value.GetAt(i)!='.')
			{
				msg.Format(_T("Wrong sigma value, Enter a positive float"));		
				goto End;
			}
		}

		msg.Format(_T("Applying Gaussian..Wait!!"));	
		Message(msg);
		BeginWaitCursor();

		//Add image before change to undo stack
		undo.OnDo(image);

		Sigma= _wtof(Value);
		Filters Filter;
		Filter.GaussianFilter(image,KernelRank,Sigma);
		show(image);
		msg.Format(_T("Gaussian Done!!"));	
		Message(msg);
		EndWaitCursor();
	}
	else
	{
		msg.Format(_T("Error Choosing Filter"));
		goto End;
End:
		Message(msg);
	}
}

//on changing the drop down (combobox)
void CProFilterDlg::OnCbnSelchangeFilterselect()
{

	int index= m_FilterSelect.GetCurSel();
	m_FilterSelect.GetLBText(index,SelectedFilter);
	m_KernelRank.EnableWindow(1);
	m_KernelRankBuddy.EnableWindow(1);
	m_ApplyFilter.EnableWindow(1);
	m_SigmaValue.EnableWindow(0);
	// activate sigma selection only when gaussian is selected
	if (SelectedFilter=="Gaussian")
	{
		m_SigmaValue.EnableWindow(1);
	}

}

//on left mouse button down
void CProFilterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	BfirstPoint=point;
	//draw only if there is an image and the cursor is on the image
	if (draw && Boundry.PtInRect(point) )
	{	
		// shift co-ordinates of point to fit the co-ordinates of the bitmap on screen
		CPoint topLeft;
		topLeft=Boundry.TopLeft();
		point=point-topLeft;
		firstPoint=point;
		CDialogEx::OnLButtonDown(nFlags, point);
	}	
}

//on left mouse button up
void CProFilterDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//draw only if there is an image and the cursor is on the image
	if (draw && Boundry.PtInRect(point) && Boundry.PtInRect(BfirstPoint) )
	{
		//Add image before change to undo stack
		undo.OnDo(image);

		// shift co-ordinates of point to fit the co-ordinates of the bitmap on screen
		CPoint topLeft;
		topLeft=Boundry.TopLeft();
		point=point-topLeft;

		//get the device contet of the Cimage object to draw directly on the image changing its pixels value
		HDC Hdc1 =image.GetDC();
		CDC dc1;
		CDC *pdc1;
		pdc1=dc1.FromHandle(Hdc1);
		pdc1->MoveTo(firstPoint);
		pdc1->LineTo(point);

		//get the device contet of the Cstatic control so the user can see the lines over the image
		CDC *pdc2;
		pdc2 = m_BMP.GetDC();
		pdc2->MoveTo(firstPoint);
		pdc2->LineTo(point);

		//releasing the Device contet for both the cimage and the cstatic control
		image.ReleaseDC();
		m_BMP.ReleaseDC(pdc2);
		CDialogEx::OnLButtonUp(nFlags, point);
	}
}

// event handler for save button click
void CProFilterDlg::OnBnClickedSave()
{
	CString msg;
	CFileDialog Save(true);
	if(Save.DoModal() == IDOK)
	{
		CString ImagePath;
		ImagePath = Save.GetPathName();
		image.Save(ImagePath,Gdiplus::ImageFormatBMP);
		msg.Format(_T("Image Saved"));
		Message(msg);
	}
	else
	{
		msg.Format(_T(""));
		Message(msg);
	}
}


void CProFilterDlg::OnBnClickedUndo()
{
	CString msg;
	// check if there's action to be undone
	if (undo.undoCounter>0)
	{
		undo.OnUnDo(image);
		show(image);
		msg.Format(_T("Undo Done!!"));
		Message(msg);
	}
	else
	{
		msg.Format(_T("Nothing To Undo"));
		Message(msg);
	}
}
