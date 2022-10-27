
// ZadatakView.cpp : implementation of the CZadatakView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Zadatak.h"
#endif

#include "ZadatakDoc.h"
#include "ZadatakView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZadatakView

IMPLEMENT_DYNCREATE(CZadatakView, CView)

BEGIN_MESSAGE_MAP(CZadatakView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CZadatakView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// private methods


void DrawTrianglePolygon(POINT p1, POINT p2, POINT p3, CDC* pDC) {

	POINT* points = new POINT[3];
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;

	pDC->Polygon(points, 3);
}

void DrawSolidTriangle(POINT p1, POINT p2, POINT p3, COLORREF color, CDC* pDC) {

	CBrush* brush = new CBrush(color);
	CBrush* oldBrush = pDC->SelectObject(brush);
	DrawTrianglePolygon(p1, p2, p3, pDC);
	pDC->SelectObject(oldBrush);
	delete brush;

}

void DrawHatchTriangle(POINT p1, POINT p2, POINT p3, COLORREF bk, int pattern, COLORREF color, CDC* pDC) {

	pDC->SetBkColor(bk);
	CBrush* brush = new CBrush(pattern, color);
	CBrush* oldBrush = pDC->SelectObject(brush);
	DrawTrianglePolygon(p1, p2, p3, pDC);
	pDC->SelectObject(oldBrush);
	delete brush;
}
void DrawQuadricPolygon(POINT p1, POINT p2, POINT p3, POINT p4, COLORREF color, CDC* pDC) {

	CBrush* brush = new CBrush(color);
	CBrush* oldBrush = pDC->SelectObject(brush);

	POINT* points = new POINT[4];
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;

	pDC->Polygon(points, 4);
	pDC->SelectObject(oldBrush);
	delete brush;
}

void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {

	CBrush* old = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);
	float rot = 2 * 3.1415926535 / n;
	POINT* points = new POINT[n];

	for (int i = 0; i < n; i++) {
		points[i].x = r * cos(rotAngle) + cx;
		points[i].y = r * sin(rotAngle) + cy;
		rotAngle += rot;
	}
	pDC->Polygon(points, n);
	pDC->SelectObject(old);
}

void DrawGrid(CDC* pDC) {

	CPen* pen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* old = pDC->SelectObject(pen);

	for (int i = 0; i < 20; i++) {
		pDC->MoveTo(i * 25, 0);
		pDC->LineTo(i * 25, 500);
		pDC->MoveTo(0, i * 25);
		pDC->LineTo(500, i * 25);
	}
	pDC->SelectObject(old);
	delete pen;
}


// CZadatakView construction/destruction

CZadatakView::CZadatakView() noexcept
{
	// TODO: add construction code here
	this->keyHit = false;

}

CZadatakView::~CZadatakView()
{
}

BOOL CZadatakView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CZadatakView drawing

void CZadatakView::OnDraw(CDC* pDC)
{
	CZadatakDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetBkColor(RGB(155, 155, 155));
	CBrush* bk = new CBrush(RGB(200, 200, 200));
	CRect rect(0, 0, 500, 500);
	pDC->FillRect(rect, bk);
	
	CPen* pen = new CPen(PS_SOLID, 5, RGB(0, 0, 255));
	CPen* oldPen = pDC->SelectObject(pen);

	DrawSolidTriangle({ 5 * 25,12 * 25 }, { 5 * 25, 16 * 25 }, { 8 * 25, 16 * 25 }, RGB(98, 245, 132), pDC);
	DrawHatchTriangle({ 25, 8 * 25 }, { 6 * 25,(long)(13.3 * 25) }, { (6 * 25), 4 * 25 }, RGB(255, 255, 255), HS_VERTICAL, RGB(0, 0, 255), pDC);
	DrawSolidTriangle({ (6 * 25), 4 * 25 }, { 12 * 25,4 * 25 }, { 6 * 25,(long)(10.8 * 25) }, RGB(139, 81, 168), pDC);
	DrawSolidTriangle({ 12 * 25, 4 * 25 }, { 12 * 25,(long)(8.8 * 25) }, { (long)(7.8 * 25), (long)(8.8 * 25) }, RGB(250, 147, 248), pDC);
	DrawSolidTriangle({ (long)(15.5 * 25), (long)(7.2 * 25) }, { (long)(15.5 * 25), (long)(4 * 25) }, { (long)(19 * 25), (long)(7.2 * 25) },RGB(245, 232, 51), pDC);
	DrawQuadricPolygon({ 25 * 9, (long)(25 * 8.8) }, { 25 * 12, (long)(25 * 8.8) }, { 25 * 15 , 25 * 12 }, { 25 * 12, 25 * 12 }, RGB(255, 156, 43), pDC);
	DrawQuadricPolygon({ 25 * 12, (long)(25 * 4) }, { (long)(15.5*25), (long)(25 * 4) }, { (long)(15.5 * 25), (long)(7.2 * 25) },{ 25 * 12 , (long)(7.2 * 25) }, RGB(255, 0,0), pDC);

	pDC->SelectObject(oldPen);
	delete pen;

	pen = new CPen(PS_SOLID, 3, RGB(0, 0, 255));
	oldPen = pDC->SelectObject(pen);

	DrawRegularPolygon(pDC, 25 * 8, 25 * 6, 25, 6, 0);
	DrawRegularPolygon(pDC, 25 * 4, 25 * 9 - 12, 25, 7, 0);
	DrawRegularPolygon(pDC, 25 * 11 - 10, 25 * 8 - 12, 12, 8, 0);
	DrawRegularPolygon(pDC, 25 * 17 - 15, 25 * 7 - 20, 10, 4, 0);
	DrawRegularPolygon(pDC, 25 * 6, 25 * 15, 12, 5, 30);
	
	pDC->SelectObject(oldPen);
	delete pen;

	if (keyHit) {
		DrawGrid(pDC);
	}


}


// CZadatakView printing


void CZadatakView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CZadatakView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CZadatakView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CZadatakView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CZadatakView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CZadatakView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CZadatakView diagnostics

#ifdef _DEBUG
void CZadatakView::AssertValid() const
{
	CView::AssertValid();
}

void CZadatakView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CZadatakDoc* CZadatakView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZadatakDoc)));
	return (CZadatakDoc*)m_pDocument;
}
#endif //_DEBUG


// CZadatakView message handlers


void CZadatakView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'D') {

		keyHit = !keyHit;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
