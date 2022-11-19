
// IND_17905View.cpp : implementation of the CIND17905View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_17905.h"
#endif

#include "IND_17905Doc.h"
#include "IND_17905View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND17905View

IMPLEMENT_DYNCREATE(CIND17905View, CView)

BEGIN_MESSAGE_MAP(CIND17905View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIND17905View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND17905View construction/destruction

CIND17905View::CIND17905View() noexcept
{
	// TODO: add construction code here

	part = new DImage[9];

	part[0].Load(CString("res/1.bmp"));
	part[1].Load(CString("res/2.bmp"));
	part[2].Load(CString("res/3.bmp"));
	part[3].Load(CString("res/4.bmp"));
	part[4].Load(CString("res/5.bmp"));
	part[5].Load(CString("res/6.bmp"));
	part[6].Load(CString("res/7.bmp"));
	part[7].Load(CString("res/8.bmp"));
	part[8].Load(CString("res/9.bmp"));

	angle = 0;
	

}

CIND17905View::~CIND17905View()
{
	delete[] part;
}

BOOL CIND17905View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND17905View drawing

void CIND17905View::Translate(CDC* pDC, int width, int height, bool right)
{
	XFORM xform = { 1,0,0,1,width,height };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17905View::Rotate(CDC* pDC, double angle, bool right)
{
	XFORM xform;
	xform.eM11 = xform.eM22 = cos(angle);
	xform.eM12 = sin(angle), xform.eM21 = -xform.eM12;
	xform.eDx = xform.eDy = 0;
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17905View::Scale(CDC* pDC, int dX, int dY, bool right)
{
	XFORM xform = { dX,0,0,dY,0,0 };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17905View::Mirror(CDC* pDC, int dX, int dY, bool right)
{
	XFORM xform = { dX,0,0,dY,0,0 };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17905View::DrawPart(CDC* pDC,int ind, int dX, int dY,double angle, bool blue)
{
	int w = part[ind].Width();
	int h = part[ind].Height();

	XFORM oldTransform;
	GetWorldTransform(pDC->m_hDC, &oldTransform);

	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	CBitmap* bmpImage = MakeImageGrayscale(pDC,ind, blue);

	// transparency
	CBitmap* mask = DrawTransparent(pDC, bmpImage, w, h);
	
	Translate(pDC, dX, dY);
	Mirror(pDC, 1, -1);
	Rotate(pDC, (PI * angle / 180));

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(mask);
	pDC->BitBlt(-w/2, -h/2, w, h, memDC, 0, 0, SRCAND);
	memDC->SelectObject(bmpImage);
	pDC->BitBlt(-w/2, -h/2, w, h, memDC, 0, 0, SRCPAINT);
	memDC->DeleteDC();
	delete memDC;

	mask->DeleteObject();
	delete mask;

	SetWorldTransform(pDC->m_hDC, &oldTransform);
	SetGraphicsMode(pDC->m_hDC, prevMode);
}

CBitmap* CIND17905View::DrawTransparent(CDC* pDC, CBitmap* image, int width, int height)
{
	CBitmap* bmpMask = new CBitmap();
	bmpMask->CreateBitmap(width, height, 1, 1, NULL);
	CDC SrcDC;
	SrcDC.CreateCompatibleDC(pDC);
	CDC DstDC;
	DstDC.CreateCompatibleDC(pDC);
	CBitmap* pOldSrcBmp = SrcDC.SelectObject(image);
	CBitmap* pOldDstBmp = DstDC.SelectObject(bmpMask);

	COLORREF clrTopLeft = SrcDC.GetPixel(0, 0);
	SrcDC.SetBkColor(clrTopLeft);
	DstDC.BitBlt(0, 0, width, height, &SrcDC, 0, 0, SRCCOPY);
	SrcDC.SetTextColor(RGB(255, 255, 255));
	SrcDC.SetBkColor(RGB(0, 0, 0));

	SrcDC.BitBlt(0, 0, width, height, &DstDC, 0, 0, SRCAND);

	SrcDC.DeleteDC();
	DstDC.DeleteDC();

	return bmpMask;
}

CBitmap* CIND17905View::MakeImageGrayscale(CDC*pDC,int ind, bool blue)
{
	BITMAP b;
	CBitmap * bitmap = part[ind].GetBitmap();
	bitmap->GetBitmap(&b);

	u_char* bits = new u_char[b.bmWidthBytes * b.bmHeight];
	bitmap->GetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);
	for (int i = 0; i < b.bmWidthBytes * b.bmHeight; i += 4)
	{
		if (bits[i + 1] == 255)continue;
		u_char newPixel = (bits[i] + bits[i + 1] + bits[i + 2]) / 3;
		bits[i] =  blue? bits[i]:newPixel;
		bits[i + 1] = blue? 0 : newPixel;
		bits[i + 2] = blue? 0 : newPixel;
	}

	CBitmap* bitImg = new CBitmap();
	bitImg->CreateCompatibleBitmap(pDC, part[ind].Width() , part[ind].Height());
	bitImg->SetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);
	delete[] bits;
	return bitImg;
}

void CIND17905View::MemPdC(CDC* pDC, CRect rect)
{
	pDC->SetBkColor(RGB(255, 255, 255));
	CBrush* bk = new CBrush(RGB(255, 255, 255));

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);
	pMemDC->BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pMemDC->SelectObject(&bmp);
	pMemDC->FillRect(rect, bk);
	DrawGrid(pMemDC);

	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	int i = 0;

	DrawPart(pMemDC, i, 4.2*25, 4.3*25, (double)25 + angle);
	i++;
	DrawPart(pMemDC, i, 9.6*25, 9.9*25,(double)-122+ angle);
	i++;
	DrawPart(pMemDC, i, 15.7 * 25, 10 * 25, (double)-108+angle);
	i++;
	DrawPart(pMemDC, i, 3.8 * 25, 15.7 * 25,(double) -160.8 +angle);
	i++;
	DrawPart(pMemDC, i, 16.3* 25, 3.8 * 25,(double) 104 + angle);
	i++;
	DrawPart(pMemDC, i, 9.7 * 25, 16.2 * 25, (double)-67 + angle,true);
	i++;
	DrawPart(pMemDC, i, 10.3 * 25, 4.1 * 25,(double) 55 + angle);
	i++;
	DrawPart(pMemDC, i, 16 * 25, 16.3 * 25, (double)-19 + angle);
	i++;
	DrawPart(pMemDC, i, 3.7 * 25, 10.2* 25, (double)-73 + angle); 
	
	pMemDC->SetGraphicsMode(prevMode);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;
}

void CIND17905View::DrawGrid(CDC* pDC)
{
	CPen* pen = new CPen(PS_SOLID, 1, RGB(121, 121, 121));
	CPen* old = pDC->SelectObject(pen);

	for (int i = 0; i <= 20; i++) {
		pDC->MoveTo(i * 25, 0);
		pDC->LineTo(i * 25, 500);
		pDC->MoveTo(0, i * 25);
		pDC->LineTo(500, i * 25);
	}
	pDC->SelectObject(old);
	delete pen;
}

void CIND17905View::OnDraw(CDC* pDC)
{
	CIND17905Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CRect rect(0, 0, 500, 500);
	MemPdC(pDC,rect);

	
	// TODO: add draw code for native data here
}


// CIND17905View printing


void CIND17905View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIND17905View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND17905View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND17905View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CIND17905View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIND17905View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIND17905View diagnostics

#ifdef _DEBUG
void CIND17905View::AssertValid() const
{
	CView::AssertValid();
}

void CIND17905View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND17905Doc* CIND17905View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND17905Doc)));
	return (CIND17905Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND17905View message handlers


void CIND17905View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == 'A') {

		angle += 10;
		Invalidate();
	}
	else if (nChar == 'D') {
		angle -= 10;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
