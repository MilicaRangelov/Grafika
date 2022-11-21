
// Klk2015View.cpp : implementation of the CKlk2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2015.h"
#endif

#include "Klk2015Doc.h"
#include "Klk2015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKlk2015View

IMPLEMENT_DYNCREATE(CKlk2015View, CView)

BEGIN_MESSAGE_MAP(CKlk2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKlk2015View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2015View construction/destruction

CKlk2015View::CKlk2015View() noexcept
{
	// TODO: add construction code here
	background = new DImage();
	background->Load((CString)"blue.png");

	position = 0;
	open = true;
	left = false;

}

CKlk2015View::~CKlk2015View()
{
}

BOOL CKlk2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2015View drawing

void CKlk2015View::Translation(CDC* pDC, double dX, double dY, bool right)
{
	XFORM form = { 1,0,0,1,dX,dY };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2015View::Scale(CDC* pDC, double sX, double sY, bool right)
{
	XFORM form = { sX,0,0,sY,0,0 };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2015View::DrawPacman(CDC* pDC, CRect rect, float angle)
{

	double r = rect.Width()/2;
	double b = sin(angle) * r;

	CBrush brush (RGB(255,255,0));
	CBrush* oldBrush = pDC->SelectObject(&brush);

	if (open) {
		int direction = pDC->GetArcDirection();
		pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->Pie(-r, -r, r, r, rect.Width() / 2, -b, rect.Width() / 2, b);
		pDC->SetArcDirection(direction);
	}
	else {
		pDC->Ellipse(-r, -r, r, r);
	}
	XFORM old;
	pDC->GetWorldTransform(&old);
	Translation(pDC, -b / 4 , 2*b / 3);

	CBrush oko1(RGB(0,0,0));

	pDC->SelectObject(oko1);
	pDC->Ellipse(-5, -5, 5, 5);
	
	pDC->SetWorldTransform(&old);
	pDC->SelectObject(oldBrush);
}

void CKlk2015View::DrawGhost(CDC* pDC, CRect rect)
{
	CBrush brush(RGB(255, 0, 0));
	
	CBrush*oldBrush = pDC->SelectObject(&brush);
	int direction = pDC->GetArcDirection();
	pDC->SetArcDirection(AD_CLOCKWISE);

	CRect pom1;
	pom1.bottom = rect.bottom;
	pom1.top = rect.top -  10* rect.top / 6;
	pom1.left = rect.left;
	pom1.right = rect.left + rect.Width() / 6 ;

	CRect pom2;
	pom2.bottom = rect.bottom;
	pom2.top = rect.top - 10 * rect.top / 6;
	pom2.left = pom1.left + rect.Width() / 6;
	pom2.right = pom1.right + rect.Width() / 6;

	CRect pom3;
	pom3.bottom = rect.bottom;
	pom3.top = rect.top - 10 * rect.top / 6;
	pom3.left = pom2.left + rect.Width() / 6;
	pom3.right = pom2.right + rect.Width() / 6;

	CRect pom4;
	pom4.bottom = rect.bottom;
	pom4.top = rect.top - 10 * rect.top / 6;
	pom4.left = pom3.left + rect.Width() / 6;
	pom4.right = pom3.right + rect.Width() / 6;

	CRect pom5;
	pom5.bottom = rect.bottom;
	pom5.top = rect.top - 10 * rect.top / 6;
	pom5.left = pom4.left + rect.Width() / 6;
	pom5.right = pom4.right + rect.Width() / 6;

	CRect pom6;
	pom6.bottom = rect.bottom;
	pom6.top = rect.top - 10 * rect.top / 6;
	pom6.left = pom5.left + rect.Width() / 6;
	pom6.right = pom5.right + rect.Width() / 6;

	CRect po7;
	po7.left = rect.left;
	po7.right = rect.right;
	po7.top = rect.top + rect.Height() / 2;
	po7.bottom = pom1.top ;

	pDC->Rectangle(po7);

	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Arc(pom2.left, pom2.top, pom2.right, pom2.bottom, pom2.left, pom2.top + pom2.Height() / 2, pom2.right, pom2.top + pom2.Height() / 2);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Arc(pom4.left, pom4.top, pom4.right, pom4.bottom, pom4.left, pom4.top + pom4.Height() / 2, pom4.right, pom4.top + pom4.Height() / 2);
		pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Arc(pom6.left, pom6.top, pom6.right, pom6.bottom, pom6.left, pom6.top + pom6.Height() / 2, pom6.right, pom6.top + pom6.Height() / 2);

	pDC->BeginPath();
	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, rect.left , rect.top + rect.Height() / 2, rect.right, rect.top + rect.Height() /2);
	pDC->SetArcDirection(direction);
	pDC->Arc(pom1.left, pom1.top, pom1.right, pom1.bottom, pom1.left, pom1.top + pom1.Height() / 2, pom1.right, pom1.top + pom1.Height() / 2);
	


	pDC->SetArcDirection(direction);
	pDC->Arc(pom3.left, pom3.top, pom3.right, pom3.bottom, pom3.left, pom3.top + pom3.Height() / 2, pom3.right, pom3.top + pom3.Height() / 2);



	pDC->SetArcDirection(direction);
	pDC->Arc(pom5.left, pom5.top, pom5.right, pom5.bottom, pom5.left, pom5.top + pom5.Height() / 2, pom5.right, pom5.top + pom5.Height() / 2);



	pDC->MoveTo(-rect.Width() / 2, 0);
	pDC->LineTo(rect.left, rect.bottom - 5);
	pDC->MoveTo(rect.Width() / 2, 0);
	pDC->LineTo(rect.right, rect.bottom - 5);

	pDC->EndPath();
	pDC->StrokeAndFillPath();

	CBrush oko1(RGB(255, 255, 255));
	CBrush oko2(RGB(64,0,128));
	pDC->SelectObject(oko1);

	XFORM old;
	pDC->GetWorldTransform(&old);

	Translation(pDC, 0, -rect.Height() / 4);
	pDC->Ellipse(-8, -8, 8, 8);
	pDC->SelectObject(oko2);
	pDC->Ellipse(-5, -5, 5, 5);
	Translation(pDC, rect.Width() / 4, 0);
	pDC->SelectObject(oko1);
	pDC->Ellipse(-8, -8, 8, 8);
	pDC->SelectObject(oko2);
	pDC->Ellipse(-5, -5, 5, 5);
	pDC->SetWorldTransform(&old);

	pDC->SelectObject(&oldBrush);

}

void CKlk2015View::OnDraw(CDC* pDC)
{
	CKlk2015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap* map = new CBitmap();
	map->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	
	memDC->SelectObject(map);
	background->Draw(memDC, CRect(0, 0, background->Width(), background->Height()), rect);

	int prevMode = memDC->GetGraphicsMode();
	memDC->SetGraphicsMode(GM_ADVANCED);

	XFORM globalForm;
	memDC->GetWorldTransform(&globalForm);

	CBrush* brush = (CBrush*)memDC->SelectStockObject(HOLLOW_BRUSH);
	if (left) {
		Scale(memDC, -1, 1);
		Translation(memDC, -(400 + position), 400);
	}
	else {
		Translation(memDC, (400 + position), 400);
	}

	CRect packaman(-50, -50, 50, 50);

	//memDC->Rectangle(packaman);
	DrawPacman(memDC, packaman, 5);
	
	memDC->SetWorldTransform(&globalForm);

	Translation(memDC, 148, 168);
	CRect ghost(-48, -48, 48, 48);
	memDC->Rectangle(ghost);
	DrawGhost(memDC, ghost);
	memDC->SelectObject(brush);
	memDC->SetWorldTransform(&globalForm);
	memDC->SetGraphicsMode(prevMode);

	

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
	delete memDC;

	// TODO: add draw code for native data here
}


// CKlk2015View printing


void CKlk2015View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKlk2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CKlk2015View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKlk2015View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKlk2015View diagnostics

#ifdef _DEBUG
void CKlk2015View::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2015Doc* CKlk2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2015Doc)));
	return (CKlk2015Doc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2015View message handlers


BOOL CKlk2015View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CKlk2015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_LEFT) {

		left = true;
		position -= 10;
		open = !open;
		Invalidate();
	}
	else if(nChar == VK_RIGHT){
		left = false;
		position += 10;
		open = !open;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
