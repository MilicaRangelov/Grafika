
// Klk_2015_againView.cpp : implementation of the CKlk2015againView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk_2015_again.h"
#endif

#include "Klk_2015_againDoc.h"
#include "Klk_2015_againView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKlk2015againView

IMPLEMENT_DYNCREATE(CKlk2015againView, CView)

BEGIN_MESSAGE_MAP(CKlk2015againView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKlk2015againView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2015againView construction/destruction

CKlk2015againView::CKlk2015againView() noexcept
{
	// TODO: add construction code here
	bk = new DImage();
	bk->Load((CString)"blue.png");

	xP = 400;
	yP = 400;
	rP = 50;

	xG = 148;
	yG = 168;
	rG = 78;

	sx = 1;
	sy = 1;

	angle = 10.0;
	rotate = 0;
	move = true;
}

CKlk2015againView::~CKlk2015againView()
{
	delete bk;
}

BOOL CKlk2015againView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2015againView drawing

void CKlk2015againView::Translation(CDC* pDC, float dx, float dy, bool right)
{
	XFORM form = { 1,0,0,1,dx,dy };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2015againView::Rotate(CDC* pDC, float angle, bool right)
{
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle),0,0 };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2015againView::Scale(CDC* pDC, float sx, float sy, bool right)
{
	XFORM form = { sx,0,0,sy,0,0};
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2015againView::DrawPackman(CDC* pDC, CRect rect, float angle)
{
	XFORM oldF;
	pDC->GetWorldTransform(&oldF);
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brush(RGB(255,255,0));

	CPen* oldPen = pDC->SelectObject(&pen);
	CBrush* old = pDC->SelectObject(&brush);

	if ((xP - rP +27 <= xG + rG) && (yP - rP +27 <= yG + rG)) {

		CFont font;
		font.CreateFont(40, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (CString)"Arial");
		pDC->SetBkColor(RGB(0, 0, 255));
		CRect client;
		GetClientRect(&client);
		pDC->SelectObject(&font);
		pDC->TextOut(client.left + client.Width() / 3, client.top + client.Height() / 2, (CString)"EndOfTheGame");
		move = false;
	}

	Translation(pDC, xP, yP);
	Rotate(pDC, (PI * rotate / 180));
	Scale(pDC, sx, sy);

	float s = rP * (PI * angle) / 180;
	pDC->Pie(-rect.Width() / 2, -rect.Height() / 2, rect.Width() / 2, rect.Height() / 2, rP, -s, rP, s);

	CBrush b(RGB(0, 0, 0));
	pDC->SelectObject(&b);

	Translation(pDC, rP / 2, -rP / 2);
	pDC->Ellipse(-5, -5, 5, 5);

	pDC->SetWorldTransform(&oldF);

		
	pDC->SelectObject(oldPen);
	pDC->SelectObject(old);
}

void CKlk2015againView::DrawGhost(CDC* pDC, CRect rect)
{
	int a = rect.top - rect.bottom;
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
	CBrush brush(RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(&pen);
	CBrush* oldBrush = pDC->SelectObject(&brush);
	int right = 196;
	int left = right - 8;

	int top = 216;
	int bottom = 200;

	pDC->BeginPath();
	int val = pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->MoveTo(rect.left, rect.bottom );
	pDC->ArcTo(rect.left, rect.bottom, rect.right, rect.top, rect.left, rect.bottom + a / 2, rect.right, rect.bottom + a / 2);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->ArcTo(left, bottom, right, top, right, bottom + 8, left, bottom + 8);
	for (int i = 0; i < 5; i++) {
		right = left;
		left -= 16;
		val = pDC->SetArcDirection(val);
		pDC->ArcTo(left, bottom, right, top, right, bottom + 8, left, bottom + 8);
	}
	right = left;
	left -= 8;
	val = pDC->SetArcDirection(val);
	pDC->ArcTo(left, bottom, right, top, right, bottom + 8, left, bottom + 8);
	

	pDC->EndPath();

	pDC->StrokeAndFillPath();
	pDC->SetArcDirection(val);

	CBrush b(RGB(255, 255, 255));
	pDC->SelectObject(&b);

	CRect client;
	GetClientRect(&client);

	pDC->MoveTo(client.left, client.top);
	XFORM oldF;
	pDC->GetWorldTransform(&oldF);

	Translation(pDC, rect.left - 3*a/4 , rect.top - a/4);
	pDC->Ellipse(-9, -9, 9, 9);
	CBrush b2(RGB(128, 0, 255));
	CBrush* oldB = pDC->SelectObject(&b2);
	pDC->Ellipse(-6, -6, 6, 6);
	pDC->SelectObject(oldB);
	Translation(pDC, a / 4, 0);
	pDC->Ellipse(-9, -9, 9, 9);
	oldB = pDC->SelectObject(&b2);
	pDC->Ellipse(-6, -6, 6, 6);
	pDC->SelectObject(oldB);
	pDC->SetWorldTransform(&oldF);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CKlk2015againView::OnDraw(CDC* pDC)
{
	CKlk2015againDoc* pDoc = GetDocument();
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

	int prevMode = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM global;
	memDC->GetWorldTransform(&global);

	bk->Draw(memDC, CRect(0, 0, bk->Width(), bk->Height()), rect);
	DrawGhost(memDC, CRect(100, 120, 196, 216));
	DrawPackman(memDC, CRect(0, 0, 100, 100), angle);

	memDC->SetWorldTransform(&global);
	memDC->SetGraphicsMode(prevMode);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	delete bitmap;
	memDC->DeleteDC();
	delete memDC;
}


// CKlk2015againView printing


void CKlk2015againView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKlk2015againView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2015againView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2015againView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CKlk2015againView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKlk2015againView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKlk2015againView diagnostics

#ifdef _DEBUG
void CKlk2015againView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2015againView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2015againDoc* CKlk2015againView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2015againDoc)));
	return (CKlk2015againDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2015againView message handlers


BOOL CKlk2015againView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CKlk2015againView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_RIGHT && move) {
		xP += 10;
		sx = 1;
		sy = 1;
		rotate = 0;
		if (angle == 10.0)
			angle = 0.0;
		else
		{
			angle = 10.0;
		}
		Invalidate();
	}
	else if (nChar == VK_LEFT && move) {

		xP -= 10;
		sx = -1;
		sy = 1;
		rotate = 0;
		if (angle == 10.0)
			angle = 0.0;
		else
		{
			angle = 10.0;
		}
		Invalidate();
	}
	else if (nChar == VK_UP && move) {

		yP -= 10;
		sx = 1;
		rotate = -90;
		sy = -1;

		if (angle == 10.0)
			angle = 0.0;
		else
		{
			angle = 10.0;
		}
		Invalidate();
	}
	else if (nChar == VK_DOWN && move) {

		yP += 10;
		rotate = 90;
		sx = 1;
		sy = 1;
		if (angle == 10.0)
			angle = 0.0;
		else
		{
			angle = 10.0;
		}
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
