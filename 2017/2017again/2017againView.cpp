
// 2017againView.cpp : implementation of the CMy2017againView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2017again.h"
#endif

#include "2017againDoc.h"
#include "2017againView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2017againView

IMPLEMENT_DYNCREATE(CMy2017againView, CView)

BEGIN_MESSAGE_MAP(CMy2017againView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy2017againView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMy2017againView construction/destruction

CMy2017againView::CMy2017againView() noexcept
{
	// TODO: add construction code here
	leg1 = new DImage();
	leg1->Load((CString)"Leg1.png");

	leg2 = new DImage();
	leg2->Load((CString)"Leg2.png");

	leg3 = new DImage();
	leg3->Load((CString)"Leg3.png");

	body = new DImage();
	body->Load((CString)"Body.png");

	bk = new DImage();
	bk->Load((CString)"Back2.jpg");

	dx = 0;
	dy = 0;
	sx = 1;
	sy = 1;

	angle[0] = 0;
	angle[1] = -10;
	angle[2] = -20;
	angle[3] = 0;
	angle[4] = 10;
	angle[5] = 20;

	l1 = 0;
	l2 = 3;

	l3 = 1;
	l4 = 4;

	lr1 = 0;
	lr2 = 0;
	ud1 = 0;
	ud2 = 0;
}

CMy2017againView::~CMy2017againView()
{
	delete leg1;
	delete leg2;
	delete leg3;
	delete body;
	delete bk;
}

BOOL CMy2017againView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2017againView drawing

void CMy2017againView::Translate(CDC* pDC, float dx, float dy, bool right)
{
	XFORM form = { 1,0,0,1,dx,dy };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;

	pDC->ModifyWorldTransform(&form, mode);
}

void CMy2017againView::Rotate(CDC* pDC, float angle, bool right)
{
	XFORM form = { cos(angle), sin(angle),-sin(angle), cos(angle),0,0};
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;

	pDC->ModifyWorldTransform(&form, mode);
}

void CMy2017againView::Scale(CDC* pDC, float sx, float sy, bool right)
{
	XFORM form = { sx,0,0,sy,0,0 };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;

	pDC->ModifyWorldTransform(&form, mode);
}

void CMy2017againView::DrawTransparent(CDC* pDC, DImage* image)
{
	CBitmap* bitmap = image->GetBitmap();
	CBitmap* mask = new CBitmap();

	int w = image->Width();
	int h = image->Height();

	mask->CreateBitmap(w, h, 1, 1, NULL);

	CDC src, dest;
	src.CreateCompatibleDC(pDC);
	dest.CreateCompatibleDC(pDC);

	src.SelectObject(bitmap);
	dest.SelectObject(mask);

	src.SetBkColor(src.GetPixel(0, 0));

	dest.BitBlt(0, 0, w, h, &src, 0, 0, SRCCOPY);

	src.SetBkColor(RGB(0, 0, 0));
	src.SetTextColor(RGB(255, 255, 255));

	src.BitBlt(0, 0, w, h, &dest, 0, 0, SRCAND);

	src.DeleteDC();
	dest.DeleteDC();

	CDC* mem = new CDC();
	mem->CreateCompatibleDC(pDC);
	mem->SelectObject(mask);

	pDC->BitBlt(0, 0, w, h, mem, 0, 0, SRCAND);
	mem->SelectObject(bitmap);
	pDC->BitBlt(0, 0, w, h, mem, 0, 0, SRCPAINT);
	delete mem;
}

void CMy2017againView::DrawBody(CDC* pDC)
{
	Translate(pDC, 100 + dx, 100 + dy);
	DrawTransparent(pDC, body);

	Translate(pDC, body->Width() / 2 -135, body->Height() / 2 -80);

	DrawLeg(pDC, angle[l1], -72.0, 168.0 );
	DrawLeg(pDC, angle[l2], -72.0, 168.0);
	DrawLeg(pDC, angle[l3], 142.0, 168.0);
	DrawLeg(pDC, angle[l4], 142.0, 168.0);
}

void CMy2017againView::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM form ,form2;
	pDC->GetWorldTransform(&form);
	
	Translate(pDC, dx, dy);
	Rotate(pDC, (PI * alpha) / 180);
	pDC->GetWorldTransform(&form2);
	Translate(pDC, -27, -23);
	DrawTransparent(pDC, leg1);

	pDC->SetWorldTransform(&form2);
	Translate(pDC, 0, 135);
	Rotate(pDC, -(PI * alpha) / 180);
	Translate(pDC, -25, -20);

	DrawTransparent(pDC, leg2);
	Translate(pDC, -35, 90);
	DrawTransparent(pDC, leg3);

	pDC->SetWorldTransform(&form);
}

void CMy2017againView::OnDraw(CDC* pDC)
{
	CMy2017againDoc* pDoc = GetDocument();
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

	int prev = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM global;
	memDC->GetWorldTransform(&global);

	bk->Draw(memDC, CRect(0, 0, bk->Width(), bk->Height()), CRect(0+20 + lr1,0+20 + ud1,rect.Width() - 20 - lr2,  rect.Height() - 20 - ud2));
	Scale(memDC, sx, sy);
	DrawBody(memDC);
	memDC->SetWorldTransform(&global);
	memDC->SetGraphicsMode(prev);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	delete bitmap;
	delete memDC;
}


// CMy2017againView printing


void CMy2017againView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy2017againView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2017againView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2017againView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMy2017againView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy2017againView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy2017againView diagnostics

#ifdef _DEBUG
void CMy2017againView::AssertValid() const
{
	CView::AssertValid();
}

void CMy2017againView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2017againDoc* CMy2017againView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2017againDoc)));
	return (CMy2017againDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy2017againView message handlers


BOOL CMy2017againView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CMy2017againView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_LEFT) {
		dx -= 10;
		l1 = (l1 + 1) % 6;
		l2 = (l2 + 1) % 6;
		l3 = (l3 + 1) % 6;
		l4 = (l4 + 1) % 6;
		Invalidate();
	}
	else if (nChar == VK_RIGHT) {
		dx += 10;
		l1 = (l1 + 1) % 6;
		l2 = (l2 + 1) % 6;
		l3 = (l3 + 1) % 6;
		l4 = (l4 + 1) % 6;
		Invalidate();
	}
	else if (nChar == VK_UP) {
		sx -= 0.5;
		sy -= 0.5;
		Invalidate();
	}
	else if (nChar == VK_DOWN) {
		sx += 0.5;
		sy += 0.5;
		Invalidate();
	}
	else if (nChar == 'W') {
		
		ud1 -= 1;
		ud2 += 1;
		Invalidate();
	}
	else if (nChar == 'S') {
		ud1 += 1;
		ud2 -= 1;
		Invalidate();
	}
	else if (nChar == 'A') {

		lr1 -= 1;
		lr2 += 1;
		Invalidate();
	}
	else if (nChar == 'D') {
		lr1 += 1;
		lr2 -= 1;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
