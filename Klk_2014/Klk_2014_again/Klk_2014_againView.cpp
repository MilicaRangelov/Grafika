
// Klk_2014_againView.cpp : implementation of the CKlk2014againView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk_2014_again.h"
#endif

#include "Klk_2014_againDoc.h"
#include "Klk_2014_againView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKlk2014againView

IMPLEMENT_DYNCREATE(CKlk2014againView, CView)

BEGIN_MESSAGE_MAP(CKlk2014againView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKlk2014againView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2014againView construction/destruction

CKlk2014againView::CKlk2014againView() noexcept
{
	// TODO: add construction code here
	clio = GetEnhMetaFile((CString)"clio.emf");

	ENHMETAHEADER emfHeader;
	GetEnhMetaFileHeader(clio,sizeof(ENHMETAHEADER),&emfHeader);
	w = emfHeader.rclBounds.right - emfHeader.rclBounds.left;
	h = emfHeader.rclBounds.bottom - emfHeader.rclBounds.top;

	DeleteObject(&emfHeader);

	wheel = new  DImage();
	wheel->Load((CString)"Wheel.png");

	angle = 15;
	position = 0;
}

CKlk2014againView::~CKlk2014againView()
{
	DeleteObject(&clio);
	delete wheel;
}

BOOL CKlk2014againView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2014againView drawing

void CKlk2014againView::Translate(CDC* pDC, double dx, double dy, bool right)
{
	XFORM form = { 1,0,0,1,dx,dy };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2014againView::Rotate(CDC* pDC, double angle, bool right)
{
	XFORM form = { cos(angle), sin(angle),-sin(angle),cos(angle),0,0 };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2014againView::Scale(CDC* pDC, double sx, double sy, bool right)
{
	XFORM form = { sx,0,0,sy,0,0};
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2014againView::DrawGround(CDC* pDC, double angle)
{
	CRect rect;
	GetClientRect(&rect);
	POINT points[3];
	points[0].x = rect.left;
	points[0].y = rect.bottom;

	points[1].x = rect.right;
	points[1].y = rect.bottom;

	points[2].x = rect.right;
	points[2].y = rect.bottom - (tan(angle*PI / 180) * rect.Width());

	CBrush* brush = new CBrush(HS_BDIAGONAL, RGB(167, 64, 52));
	CBrush* old = pDC->SelectObject(brush);

	pDC->Polygon(points, 3);

	pDC->SelectObject(old);
	delete brush;

}

void CKlk2014againView::DrawCar(CDC* pDC, int x, int y)
{
	XFORM oldForm;
	

	Translate(pDC, x , y);
	Rotate(pDC, -(PI * angle) / 180);
	pDC->GetWorldTransform(&oldForm);

	pDC->PlayMetaFile(clio,CRect( w / 2, - h / 2, - w / 2, h / 2));

	double alpha = position * 180 / 70 * PI;



	DrawWheel(pDC, -225, 100, 70, alpha);
	pDC->SetWorldTransform(&oldForm);
	DrawWheel(pDC, 190, 100, 70, alpha);
}

void CKlk2014againView::DrawWheel(CDC* pDC, int x, int y, int r, double angle)
{
	Translate(pDC, x, y);
	Rotate(pDC, (PI*angle) / 180);
	Translate(pDC, -r , -r + 10);

	DrawTransparent(pDC, wheel);

}

void CKlk2014againView::DrawTransparent(CDC* pDC, DImage* image)
{
	CBitmap* bitmap = image->GetBitmap();
	int w = image->Width();
	int h = image->Height();

	CBitmap* mask = new CBitmap();
	mask->CreateBitmap(w,h,1,1,NULL);

	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CDC destDC;
	destDC.CreateCompatibleDC(pDC);

	CBitmap* oldSrc= srcDC.SelectObject(bitmap);
	COLORREF color = srcDC.GetPixel(0, 0);
	srcDC.SetBkColor(color);

	CBitmap* oldDest = destDC.SelectObject(mask);

	destDC.BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCCOPY);

	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));

	srcDC.BitBlt(0, 0, w, h, &destDC, 0, 0, SRCAND);

	srcDC.SelectObject(oldSrc);
	destDC.SelectObject(oldDest);

	srcDC.DeleteDC();
	destDC.DeleteDC();


	CDC* mem = new CDC();
	mem->CreateCompatibleDC(pDC);
	mem->SelectObject(mask);

	XFORM form;
	pDC->GetWorldTransform(&form);
	int sw = 140;
	int sh = 140;
	Scale(pDC, (1.0 * sw / w), (1.0 * sh / h));
	

	pDC->BitBlt(0, 0, w, h, mem, 0, 0, SRCAND);
	mem->SelectObject(bitmap);
	pDC->BitBlt(0, 0, w, h, mem, 0, 0, SRCPAINT);
	pDC->SetWorldTransform(&form);
	mem->DeleteDC();
	delete mem;
}

void CKlk2014againView::OnDraw(CDC* pDC)
{
	CKlk2014againDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap* bitmap = new CBitmap();
	bitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC->SelectObject(bitmap);
	memDC->FillRect(rect, new CBrush(RGB(153,217,234)));

	DrawGround(memDC, angle);

	int prevMode = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM global;
	memDC->GetWorldTransform(&global);
	int x = 300 + position;
	int y = rect.bottom - 2*(tan(angle * PI / 180) * rect.Width() /3 ) - position / 6;
	DrawCar(memDC, x, y);
	memDC->SetWorldTransform(&global);
	memDC->SetGraphicsMode(prevMode);
	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	delete bitmap;
	memDC->DeleteDC();
	delete memDC;
}


// CKlk2014againView printing


void CKlk2014againView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKlk2014againView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2014againView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2014againView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CKlk2014againView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKlk2014againView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKlk2014againView diagnostics

#ifdef _DEBUG
void CKlk2014againView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2014againView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2014againDoc* CKlk2014againView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2014againDoc)));
	return (CKlk2014againDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2014againView message handlers


BOOL CKlk2014againView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CKlk2014againView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_LEFT) {
		position -= 10;
		Invalidate();
	}
	else if (nChar == VK_RIGHT) {
		position += 10;
		Invalidate();
	}
	else if (nChar == VK_UP) {
		angle += 10;
		Invalidate();
	}
	else if (nChar == VK_DOWN) {
		angle -= 10;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
