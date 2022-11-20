
// Klk2013View.cpp : implementation of the CKlk2013View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2013.h"
#endif

#include "Klk2013Doc.h"
#include "Klk2013View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKlk2013View

IMPLEMENT_DYNCREATE(CKlk2013View, CView)

BEGIN_MESSAGE_MAP(CKlk2013View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKlk2013View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2013View construction/destruction

CKlk2013View::CKlk2013View() noexcept
{
	// TODO: add construction code here
	angle = 0;
	position = 0;

	tree = new DImage();
	tree->Load((CString)"bark.jpg");

	background = new DImage();
	background->Load((CString)"sky.jpg");

}

CKlk2013View::~CKlk2013View()
{
	delete tree;
	delete background;
}

BOOL CKlk2013View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2013View drawing

void CKlk2013View::Translate(CDC* pDC, double dX, double dY, bool right)
{
	XFORM form = { 1,0,0,1,dX, dY };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2013View::Rotate(CDC* pDC, double angle, bool right)
{
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle), 0,0 };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2013View::DrawBlade(CDC* pDC, int size)
{
	CPen* pen = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* brush = new CBrush(RGB(195,195,195));

	CPen* oldPen = pDC->SelectObject(pen);
	CBrush* oldBrush = pDC->SelectObject(brush);
	
	POINT* part1 = new POINT[3];
	part1[0].x = 0; part1[0].y = 0;
	part1[1].x = size * cos(M_PI / 12); part1[1].y = -size / cos(M_PI/12);
	part1[2].x = 4 * size; part1[2].y = 0;
	pDC->Polygon(part1, 3);
	
	delete[] part1;
	delete brush;

	brush = new CBrush(RGB(138,138,138));
	oldBrush = pDC->SelectObject(brush);

	POINT* part2 = new POINT[3];
	part2[0].x = 0; part2[0].y = 0;
	part2[1].x = size * cos(M_PI / 12); part2[1].y = size / cos(M_PI / 12);
	part2[2].x = 4 * size; part2[2].y = 0;
	pDC->Polygon(part2, 3);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	
	Text(pDC, size);
	delete brush;
	delete pen;
}

void CKlk2013View::Text(CDC* pDC, int size)
{
	CFont* font = new CFont();
	COLORREF clrOld = pDC->SetTextColor(RGB(255,255,255));
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	CString szMsg = (CString)"Shuricane";

	font->CreateFontW(0.7 * size, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, (CString)"Monotype Corsiva");

	CFont* old = pDC->SelectObject(font);

	
	pDC->TextOutW(size-1, -size / 2 -1, szMsg);

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOutW(size, -size / 2 , szMsg);

	pDC->SetTextColor(clrOld);
	pDC->SetBkMode(nOldMode);

	pDC->SelectObject(old);

	font->DeleteObject();
	delete font;
}

void CKlk2013View::OnDraw(CDC* pDC)
{
	CKlk2013Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	
	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap* map = new CBitmap();
	map->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* oldMap = memDC->SelectObject(map);

	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM globalMode, oldForm;
	memDC->GetWorldTransform(&globalMode);

	background->Draw(memDC, CRect(0, 0, background->Width(), background->Height()), rect);

	int size = 16;
	

	Translate(memDC, 100 + position, 200);
	Rotate(memDC, angle);
	memDC->GetWorldTransform(&oldForm);

	DrawBlade(memDC, size);

	memDC->SetWorldTransform(&oldForm);
	Rotate(memDC, M_PI);
	DrawBlade(memDC, size);

	memDC->SetWorldTransform(&oldForm);
	Rotate(memDC, M_PI / 2);
	DrawBlade(memDC, size);

	memDC->SetWorldTransform(&oldForm);
	Rotate(memDC, -M_PI / 2);
	DrawBlade(memDC, size);

	memDC->SetWorldTransform(&globalMode);
	Translate(memDC, rect.Width(), 0);
	tree->Draw(memDC, CRect(0, 0, tree->Width(), tree->Height()), CRect(-rect.Width() / 4, 0, rect.Width() / 4, rect.Height()));
	memDC->SetWorldTransform(&globalMode);

	memDC->SetWorldTransform(&oldForm);
	memDC->SetWorldTransform(&globalMode);
	memDC->SetGraphicsMode(oldMode);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	memDC->SelectObject(oldMap);
	delete map;
	memDC->DeleteDC();
	delete memDC;
}


// CKlk2013View printing


void CKlk2013View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKlk2013View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2013View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2013View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CKlk2013View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKlk2013View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKlk2013View diagnostics

#ifdef _DEBUG
void CKlk2013View::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2013View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2013Doc* CKlk2013View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2013Doc)));
	return (CKlk2013Doc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2013View message handlers


void CKlk2013View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == 'W') {

		CRect rect;
		GetClientRect(&rect);

		double desetProcenata = 16 * 0.9;
		int pocetakStabla = rect.Width() - rect.Width() / 4;
		
		if ((pocetakStabla - (position + 100 + 32) > desetProcenata)) {
			position += 20;
			angle += 10;
			Invalidate();
		}
	
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
