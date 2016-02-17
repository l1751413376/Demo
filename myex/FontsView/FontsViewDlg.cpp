// FontsViewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FontsView.h"
#include "FontsViewDlg.h"
#include ".\fontsviewdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
int BITSTABLE[]={2,5,17,65};

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFontsViewDlg �Ի���



CFontsViewDlg::CFontsViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFontsViewDlg::IDD, pParent)
	, m_strEntered(_T(""))
	, m_strFontName(_T(""))
	, m_intBold(0)
	, m_intAngle(0)
	, m_intHeight(0)
	, m_intPixX(0)
	, m_intPixY(0)
	, m_strStatus(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFontsViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EENTERED, m_strEntered);
	DDX_Control(pDX, IDC_LFONTS, m_ctlFontList);
	DDX_LBString(pDX, IDC_LFONTS, m_strFontName);
	DDX_Control(pDX, IDC_STATIC_PIC, m_ctlDisplay);

	DDX_Control(pDX, IDC_SLIDER_HEIGHT, m_slider_height);
	DDX_Control(pDX, IDC_SLIDER_BOLD, m_slider_bold);
	DDX_Control(pDX, IDC_SLIDER_ANGLE, m_slider_angle);
	DDX_Text(pDX, IDC_EDIT_BOLD, m_intBold);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_intAngle);
	DDX_Control(pDX, IDC_CHECK_ITALIC, m_ctlCheckItalic);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_intHeight);
	DDX_Text(pDX, IDC_EDIT_PIXX, m_intPixX);
	DDX_Text(pDX, IDC_EDIT_PIXY, m_intPixY);
	DDX_Control(pDX, IDC_CHECK_REVERSE, m_ctlBtnReverse);
	DDX_Control(pDX, IDC_SCROLLBAR_H, m_ctlScrollH);
	DDX_Control(pDX, IDC_SCROLLBAR_V, m_ctlScrollV);
	DDX_Control(pDX, IDC_COMBO_NBITS, m_ctlComboNbits);
	DDV_MinMaxInt(pDX, m_intHeight, 0, 30000);
	DDV_MinMaxInt(pDX, m_intBold, 0, 900);
	DDV_MinMaxInt(pDX, m_intAngle, 0, 3599);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_strStatus);
	DDV_MinMaxInt(pDX, m_intPixY, 0, INT_MAX);
}

BEGIN_MESSAGE_MAP(CFontsViewDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BEXIT, OnBnClickedBexit)
	ON_EN_CHANGE(IDC_EENTERED, OnEnChangeEentered)
	ON_LBN_SELCHANGE(IDC_LFONTS, OnLbnSelchangeLfonts)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BOLD, OnNMReleasedcaptureSliderBold)
	
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_ANGLE, OnNMReleasedcaptureSliderAngle)
	ON_BN_CLICKED(IDC_CHECK_ITALIC, OnBnClickedCheckItalic)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HEIGHT, OnNMReleasedcaptureSliderHeight)
	
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_CHECK_REVERSE, OnBnClickedCheckReverse)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_NBITS, OnCbnSelchangeComboNbits)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, OnEnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_BOLD, OnEnChangeEditBold)
	ON_EN_CHANGE(IDC_EDIT_ANGLE, OnEnChangeEditAngle)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_PIXX, OnEnChangeEditPixx)
END_MESSAGE_MAP()


// CFontsViewDlg ��Ϣ�������

BOOL CFontsViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	FillFontList();
	m_strEntered="��";
	this->SetWindowText("��ģ��ȡ");
	this->m_slider_height.SetRange(0,30000);
	this->m_slider_angle.SetTicFreq(1);
	this->m_intHeight=100;
	this->m_slider_height.SetPos(this->m_intHeight);
	this->m_slider_angle.SetRange(0,3600-1);
	this->m_slider_angle.SetTicFreq(1);
	this->m_intAngle=0;
	this->m_slider_angle.SetPos(this->m_intAngle);
	this->m_slider_bold.SetRange(0,900);
	this->m_slider_bold.SetTicFreq(100);
	this->m_intBold=100;
	this->m_slider_bold.SetPos(this->m_intBold);
	this->m_bCheckItalic=false;
	this->m_bCheckReverse=false;
	this->m_ctlBtnReverse.SetCheck(this->m_bCheckReverse);
	this->m_ctlCheckItalic.SetCheck(this->m_bCheckItalic);
	this->charLineW=0;
	this->pbmpinfo=NULL;
	this->pfileinfo=NULL;
	this->pBuf=NULL;
	this->m_intBits=1;
	this->m_ctlComboNbits.SetCurSel(this->m_intBits-1);
	this->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(0);
	this->m_ctlScrollH.ShowWindow(SW_HIDE);
	this->m_ctlScrollV.ShowWindow(SW_HIDE);
	this->m_intScrollHPos=0;
	this->m_intScrollVPos=0;
	this->m_strStatus="״ָ̬ʾ��";

	UpdateData(FALSE);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CFontsViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFontsViewDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		
		CDC* pdc=this->m_ctlDisplay.GetDC();
		this->ShowFontChar(pdc);
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CFontsViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFontsViewDlg::OnBnClickedBexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
int CALLBACK MyEnumFontProc(ENUMLOGFONTEX* lpelf,NEWTEXTMETRICEX* lpntm,DWORD nFontType,long lParam)
{
	CFontsViewDlg* pWnd=(CFontsViewDlg*) lParam;
	if(pWnd)
	{
		pWnd->m_ctlFontList.AddString(lpelf->elfLogFont.lfFaceName);
		return 1;
	}
	return 0;
}
void CFontsViewDlg::FillFontList(void)
{
	int iCurCount;
	CString strCurFont;
	CString strPrevFont="";
	LOGFONT lf;
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfFaceName[0]=NULL;
	lf.lfPitchAndFamily=0;
	m_ctlFontList.ResetContent();
	CClientDC dc(this);
	::EnumFontFamiliesEx((HDC) dc,&lf,(FONTENUMPROC) MyEnumFontProc,(LPARAM) this,0);
	for (iCurCount=m_ctlFontList.GetCount();iCurCount >0;iCurCount--)
	{
		m_ctlFontList.GetText((iCurCount-1),strCurFont);
		if (strCurFont==strPrevFont)
		{
			m_ctlFontList.DeleteString((iCurCount-1));
		}
		else
		{
			strPrevFont=strCurFont;
		}
	}
	this->m_ctlFontList.SetCurSel(0);
	TRACE("the total is %d\n",this->m_ctlFontList.GetCount());
	this->m_ctlFontList.GetText(this->m_ctlFontList.GetCurSel(),this->m_strFontName);
}

void CFontsViewDlg::OnEnChangeEentered()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	
	
}

void CFontsViewDlg::SetMyFont(void)
{
	
	
	if(m_strFontName.GetLength()>0)
	{
		CFont newfont;
		newfont.CreateFont(this->m_intHeight,0,this->m_intAngle,0,this->m_intBold,this->m_bCheckItalic,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,m_strFontName);
		CDC* pdc=this->m_ctlDisplay.GetDC();
		CFont *poldfont=pdc->SelectObject(&newfont);
		MAT2 mmat2={0,1,0,0,0,0,0,1};
		GLYPHMETRICS glpm;
		DWORD nLen;
		UINT chartemp;
		BYTE temp;
		temp=this->m_strEntered.GetAt(0);
		if(temp>=128)
		{
			chartemp=((UINT)temp)<<8;
		    temp=this->m_strEntered.GetAt(1);
			chartemp+=temp;
		}
		else
			chartemp=temp;
		
		TRACE("chartemp is %d, sizeof(uint) is %d\n",chartemp,sizeof(UINT));
		if(this->m_intBits==1)
		  nLen=::GetGlyphOutline(pdc->GetSafeHdc(),chartemp,this->m_intBits,&glpm,0,NULL,&mmat2);
		else
		 nLen=::GetGlyphOutline(pdc->GetSafeHdc(),chartemp,this->m_intBits+2,&glpm,0,NULL,&mmat2);

		        //GGO_BITMAP 1 
		        // GGO_GRAY2_BITMAP   4
                // GGO_GRAY4_BITMAP   5
                //GGO_GRAY8_BITMAP   6
		if((signed)nLen>0)
		{
			if(this->pBuf!=NULL)
				delete []this->pBuf;
			this->pBuf=new BYTE[nLen];
			int nLentemp;
			if(this->m_intBits==1)
			 {
				 nLentemp=::GetGlyphOutline(pdc->GetSafeHdc(),chartemp,this->m_intBits,&glpm,nLen,pBuf,&mmat2);
				 

				 this->charLineW=(glpm.gmBlackBoxX/32+(glpm.gmBlackBoxX%32==0?0:1))*4;
				this->FormBitmapInfo(glpm.gmBlackBoxX,glpm.gmBlackBoxY,this->m_intBits);
				if(nLentemp!=nLen)
				 {
					this->m_strStatus="������󣡲�֧�֣�";
				 }
				 else
				{
					this->m_strStatus="������";
				}
			 }
			else
			{
				nLentemp=::GetGlyphOutline(pdc->GetSafeHdc(),chartemp,this->m_intBits+2,&glpm,nLen,pBuf,&mmat2);
				
				this->charLineW=(glpm.gmBlackBoxX/4+(glpm.gmBlackBoxX%4==0?0:1))*4;
				this->FormBitmapInfo(glpm.gmBlackBoxX,glpm.gmBlackBoxY,this->m_intBits);
				if(nLentemp!=nLen)
				 {
					 this->m_strStatus="������󣡲�֧�֣�";
				 }
				else
				{
					this->m_strStatus="������";
				}

			}	 
			
            this->m_intPixX=glpm.gmBlackBoxX;
			this->m_intPixY=glpm.gmBlackBoxY;
			//TRACE("x is %d,y is %d\n",glpm.gmBlackBoxX,glpm.gmBlackBoxY);
            this->UpdateData(FALSE);
     
			this->ShowFontChar(pdc);
			this->m_intScrollHPos=0;
			this->m_intScrollVPos=0;
			this->ShowVHScrollBar();
			
		

			
				pdc->SelectObject(poldfont);
				newfont.DeleteObject();
				this->GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(1);
				
				
		}
		else
		{
			this->m_strStatus="������󣡲�֧�֣�";
			this->UpdateData(0);
		}
	}
}

void CFontsViewDlg::OnLbnSelchangeLfonts()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	SetMyFont();
}

void CFontsViewDlg::OnNMReleasedcaptureSliderBold(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	this->m_intBold=this->m_slider_bold.GetPos();
	this->UpdateData(0);
	this->OnLbnSelchangeLfonts();
	*pResult = 0;
}

void CFontsViewDlg::OnNMReleasedcaptureSliderAngle(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->m_intAngle=this->m_slider_angle.GetPos();
	this->UpdateData(0);
	this->OnLbnSelchangeLfonts();
	*pResult = 0;
}

void CFontsViewDlg::OnBnClickedCheckItalic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->m_bCheckItalic=this->m_ctlCheckItalic.GetCheck();

	this->OnLbnSelchangeLfonts();
}

void CFontsViewDlg::OnNMReleasedcaptureSliderHeight(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->m_intHeight=this->m_slider_height.GetPos();
	this->UpdateData(0);
	this->OnLbnSelchangeLfonts();
	*pResult = 0;
}


void CFontsViewDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(this->pbmpinfo!=NULL)
		delete []this->pbmpinfo;
	if(this->pfileinfo!=NULL)
		delete []this->pfileinfo;
	if(this->pBuf!=NULL)
		delete []this->pBuf;

	CDialog::OnClose();
}

void CFontsViewDlg::FormBitmapInfo(int nWidth, int nHeight, int nBits)
{
	if(this->pbmpinfo!=NULL)
		delete []this->pbmpinfo;
	this->pbmpinfo=(LPBITMAPINFO)(new BYTE[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*BITSTABLE[nBits-1]]);
	this->pbmpinfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	this->pbmpinfo->bmiHeader.biWidth=nWidth;
	this->pbmpinfo->bmiHeader.biHeight=-nHeight;
	this->pbmpinfo->bmiHeader.biPlanes=1;
	this->pbmpinfo->bmiHeader.biBitCount=nBits==1?1:8;
	this->pbmpinfo->bmiHeader.biCompression=BI_RGB;
	this->pbmpinfo->bmiHeader.biSizeImage=0;
	this->pbmpinfo->bmiHeader.biXPelsPerMeter=0;
	this->pbmpinfo->bmiHeader.biYPelsPerMeter=0;
	this->pbmpinfo->bmiHeader.biClrUsed=BITSTABLE[nBits-1];
	this->pbmpinfo->bmiHeader.biClrImportant=BITSTABLE[nBits-1];
	RGBQUAD* pquad=(new RGBQUAD[BITSTABLE[nBits-1]]);
	if(nBits==1)
	{	if(this->m_bCheckReverse==false)
	  {
		(*pquad).rgbRed=255;
		(*pquad).rgbGreen=255;
		(*pquad).rgbBlue=255;
		(*pquad).rgbReserved=0;
		(*(pquad+1)).rgbRed=0;
		(*(pquad+1)).rgbGreen=0;
		(*(pquad+1)).rgbBlue=0;
		(*(pquad+1)).rgbReserved=0;
	  }
	else
	  {
		(*pquad).rgbRed=0;
		(*pquad).rgbGreen=0;
		(*pquad).rgbBlue=0;
		(*pquad).rgbReserved=0;
		(*(pquad+1)).rgbRed=255;
		(*(pquad+1)).rgbGreen=255;
		(*(pquad+1)).rgbBlue=255;
		(*(pquad+1)).rgbReserved=0;

	  }
	}
	else
	{  if(this->m_bCheckReverse==false)
		{
			for(int i=0;i<BITSTABLE[nBits-1];i++)
			{
				(*(pquad+i)).rgbRed=(*(pquad+i)).rgbGreen=(*(pquad+i)).rgbBlue=255-255*i/(BITSTABLE[nBits-1]-1);
				(*(pquad+i)).rgbReserved=0;

			}
		}
		else
		{
			    for(int i=0;i<BITSTABLE[nBits-1];i++)
			{
				(*(pquad+i)).rgbRed=(*(pquad+i)).rgbGreen=(*(pquad+i)).rgbBlue=255*i/(BITSTABLE[nBits-1]-1);
				
				(*(pquad+i)).rgbReserved=0;

			}

		}
	}
	memcpy(&this->pbmpinfo->bmiColors,pquad,sizeof(RGBQUAD)*BITSTABLE[nBits-1]);
	delete []pquad;
}

void CFontsViewDlg::FormFileHeader(int nWidth, int nHeight,int nBits)
{
	
	
		if(this->pfileinfo!=NULL)
			delete []this->pfileinfo;
		this->pfileinfo=(LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
		this->pfileinfo->bfType=*(WORD*)(&"BM");
		this->pfileinfo->bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*BITSTABLE[nBits-1];
		this->pfileinfo->bfSize=this->pfileinfo->bfOffBits+this->charLineW*nHeight;
	
	
}

void CFontsViewDlg::OnBnClickedButtonSave()
{
    
	CString strfile; 
	CFileDialog cfd(FALSE,".bmp","δ����.bmp",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"BMP files(*.bmp)|*.bmp|All files(*.*)|*.*||",this);
	 if(cfd.DoModal()==IDOK)
	 {
		 strfile=cfd.GetPathName();

	 }
	 if(strfile.IsEmpty())
	 {
		 ::AfxMessageBox("��������Ҫ������ļ�����");
		 return ;
	 }
	 CFile cf;
	 cf.Open(strfile,CFile::modeCreate|CFile::modeWrite);
	 this->FormFileHeader(this->m_intPixX,this->m_intPixY,this->m_intBits);
	 cf.Write(this->pfileinfo,sizeof(BITMAPFILEHEADER));
	 cf.Write(this->pbmpinfo,sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*BITSTABLE[this->m_intBits-1]);
	 cf.Write(this->pBuf,this->charLineW*this->m_intPixY);
	 cf.Close();
	 
	 


	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CFontsViewDlg::ShowFontChar(CDC* pdc)
{
	if(this->pBuf!=NULL)
	{
	        CRect rect;
			this->GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);
			CDC memdc;
			memdc.CreateCompatibleDC(pdc);
			CBitmap nbitmap,*poldbitmap;
			nbitmap.CreateCompatibleBitmap(pdc,rect.Width(),rect.Height());
			poldbitmap=memdc.SelectObject(&nbitmap);
			if(this->m_bCheckReverse==false)
			memdc.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));
			::StretchDIBits(memdc.m_hDC,-this->m_intScrollHPos,-this->m_intScrollVPos,this->m_intPixX,this->m_intPixY,0,0,this->m_intPixX,this->m_intPixY,this->pBuf,this->pbmpinfo,DIB_RGB_COLORS,SRCCOPY);
			
			pdc->BitBlt(0,0,rect.Width(),rect.Height(),&memdc,0,0,SRCCOPY);
			memdc.DeleteDC();
			
	}
}

void CFontsViewDlg::OnBnClickedCheckReverse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->m_bCheckReverse=(bool)this->m_ctlBtnReverse.GetCheck();
	this->OnLbnSelchangeLfonts();
}

void CFontsViewDlg::ShowVHScrollBar(void)
{
	CRect rect;
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);
	if(rect.Width()<this->m_intPixX) 
	{	
		SCROLLINFO info;
   info.cbSize = sizeof(SCROLLINFO);     
   info.fMask = SIF_ALL;     
   info.nMin = 0;     
   info.nMax = this->m_intPixX-rect.Width()+50; 
   info.nPage = 50;     
   info.nPos = 0;    
   info.nTrackPos = 10;


		
   this->m_ctlScrollH.SetScrollInfo(&info);
   this->m_ctlScrollH.SetScrollPos(this->m_intScrollHPos);
		this->m_ctlScrollH.EnableScrollBar(ESB_ENABLE_BOTH );
		this->m_ctlScrollH.ShowWindow(SW_SHOW);
	}
	else
         this->m_ctlScrollH.ShowWindow(SW_HIDE);
	if(rect.Height()<this->m_intPixY)
	{
		SCROLLINFO info;
   info.cbSize = sizeof(SCROLLINFO);     
   info.fMask = SIF_ALL;     
   info.nMin = 0;     
   info.nMax = this->m_intPixY-rect.Height()+50; 
   info.nPage = 50;     
   info.nPos = 0;    
   info.nTrackPos = 10;
   this->m_ctlScrollV.SetScrollInfo(&info);
		this->m_ctlScrollV.SetScrollPos(this->m_intScrollHPos);
		this->m_ctlScrollV.EnableScrollBar(ESB_ENABLE_BOTH );
		this->m_ctlScrollV.ShowWindow(SW_SHOW);
	}
	else
		 this->m_ctlScrollV.ShowWindow(SW_HIDE);

}

void CFontsViewDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(pScrollBar==&this->m_ctlScrollH)
	{  
		
		 int minpos;
        int maxpos;
        pScrollBar->GetScrollRange(&minpos, &maxpos); 
       maxpos = pScrollBar->GetScrollLimit();

       // Get the current position of scroll box.
       int curpos = pScrollBar->GetScrollPos();

      // Determine the new position of scroll box.
   switch (nSBCode)
   {
   case SB_LEFT:      // Scroll to far left.
      curpos = minpos;
      break;

   case SB_RIGHT:      // Scroll to far right.
      curpos = maxpos;
      break;

   case SB_ENDSCROLL:   // End scroll.
      break;

   case SB_LINELEFT:      // Scroll left.
      if (curpos > minpos)
         curpos--;
      break;

   case SB_LINERIGHT:   // Scroll right.
      if (curpos < maxpos)
         curpos++;
      break;

   case SB_PAGELEFT:    // Scroll one page left.
   {
      // Get the page size. 
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
   
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;

   case SB_PAGERIGHT:      // Scroll one page right.
   {
      // Get the page size. 
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);

      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;

   case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
      curpos = nPos;      // of the scroll box at the end of the drag operation.
      break;

   case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
      curpos = nPos;     // position that the scroll box has been dragged to.
      break;
   }

   // Set the new position of the thumb (scroll box).
   pScrollBar->SetScrollPos(curpos);
   this->m_intScrollHPos=curpos;
  this->OnPaint();
		
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFontsViewDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(pScrollBar==&this->m_ctlScrollV)
	{  
		
		 int minpos;
        int maxpos;
        pScrollBar->GetScrollRange(&minpos, &maxpos); 
       maxpos = pScrollBar->GetScrollLimit();

       // Get the current position of scroll box.
       int curpos = pScrollBar->GetScrollPos();

      // Determine the new position of scroll box.
   switch (nSBCode)
   {
   case SB_LEFT:      // Scroll to far left.
      curpos = minpos;
      break;

   case SB_RIGHT:      // Scroll to far right.
      curpos = maxpos;
      break;

   case SB_ENDSCROLL:   // End scroll.
      break;

   case SB_LINELEFT:      // Scroll left.
      if (curpos > minpos)
         curpos--;
      break;

   case SB_LINERIGHT:   // Scroll right.
      if (curpos < maxpos)
         curpos++;
      break;

   case SB_PAGELEFT:    // Scroll one page left.
   {
      // Get the page size. 
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
   
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;

   case SB_PAGERIGHT:      // Scroll one page right.
   {
      // Get the page size. 
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);

      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;

   case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
      curpos = nPos;      // of the scroll box at the end of the drag operation.
      break;

   case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
      curpos = nPos;     // position that the scroll box has been dragged to.
      break;
   }

   // Set the new position of the thumb (scroll box).
   pScrollBar->SetScrollPos(curpos);
   this->m_intScrollVPos=curpos;

  this->OnPaint();
		
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFontsViewDlg::OnCbnSelchangeComboNbits()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int i=this->m_ctlComboNbits.GetCurSel();
	if(i!=CB_ERR)
		this->m_intBits=i+1;
	this->OnLbnSelchangeLfonts();
}

void CFontsViewDlg::OnEnChangeEditHeight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(1);
	this->m_slider_height.SetPos(this->m_intHeight);
	this->OnLbnSelchangeLfonts();
}

void CFontsViewDlg::OnEnChangeEditBold()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	this->UpdateData(1);
	this->m_slider_bold.SetPos(this->m_intBold);
	this->OnLbnSelchangeLfonts();
}

void CFontsViewDlg::OnEnChangeEditAngle()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(1);
	this->m_slider_angle.SetPos(this->m_intAngle);
	this->OnLbnSelchangeLfonts();
}

HBRUSH CFontsViewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_STATUS)
	{
		if(this->m_strStatus.Compare("������")!=0)
		pDC->SetTextColor(RGB(255,0,0));
		
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CFontsViewDlg::OnEnChangeEditPixx()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
