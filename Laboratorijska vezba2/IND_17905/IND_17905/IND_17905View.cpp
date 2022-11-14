
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
	beta = 45;
	alfa = 0;
	grid = true;
}

CIND17905View::~CIND17905View()
{
}

BOOL CIND17905View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND17905View drawing

void CIND17905View::Translate(CDC* pDC, double Dx, double Dy, bool right)
{
	XFORM xform = { 1,0,0,1,Dx,Dy };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xform, mode);
}

void CIND17905View::Scale(CDC* pDC, double Sx, double Sy, bool right)
{
	XFORM xform = { Sx,0,0,Sy,0,0 };
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

void CIND17905View::Text(CDC* pDC)
{
	COLORREF clrOld = pDC->SetTextColor(RGB(252, 186, 3));
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	CString szMsg = (CString)"17905 Milica Rangelov";
	CFont font;
	CString sFaceName = (CString)"Arial";

	font.CreateFont(20,15,-900,0,400,0,0,0,0,0,0,0,0,sFaceName);

	CFont* old = pDC->SelectObject(&font);

	pDC->TextOutW(19*25, 25, szMsg);
	pDC->SetTextColor(clrOld);
	pDC->SetBkMode(nOldMode);

	pDC->SelectObject(old);

	font.DeleteObject();

}

void CIND17905View::DrawCactus(CDC* pDC)
{
	XFORM oldForm, globalForm, saveForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, (double)10 * 25, (double)20 * 25);
	Scale(pDC, 1, -1);
	pDC->GetWorldTransform(&globalForm);

	//static cactus
	Translate(pDC, 0, 3 * 25);

	XFORM GreenCyrclePosition, centerCyrcle;
	pDC->GetWorldTransform(&GreenCyrclePosition);

	Rotate(pDC, (PI * alfa) / 180);
	Translate(pDC, 0, dY);
	DrawCactusScales(pDC, dX4, dY, true);
	DrawGreenCircle(pDC, GreenCyrclePosition, Radius);
	Translate(pDC, 0, dY);

	pDC->GetWorldTransform(&centerCyrcle);

	Translate(pDC, 0, dY);
	DrawCactusScales(pDC, dX2, dY, false);

	pDC->GetWorldTransform(&GreenCyrclePosition);


	//branch
	DrawRightBranch(pDC);
	DrawLeftBranch(pDC);

	DrawGreenCircle(pDC, centerCyrcle, Radius);
	pDC->SetWorldTransform(&globalForm);

	DrawPot(pDC, 120, 70);

	pDC->SetWorldTransform(&oldForm);
}

void CIND17905View::DrawCactusScales(CDC* pDC, int width, int height, bool light)
{
	int w = 20;
	int h = 20;
	XFORM old;
	pDC->GetWorldTransform(&old);

	HENHMETAFILE mf = light ? GetEnhMetaFile(L"RESURSI/cactus_part_light.emf") : GetEnhMetaFile(L"RESURSI/cactus_part.emf");
	pDC->PlayMetaFile(mf, CRect(-width, -height, width, height));
	DeleteEnhMetaFile(mf);

	pDC->SetWorldTransform(&old);
}

void CIND17905View::DrawLeftBranch(CDC* pDC)
{
	XFORM saveForm;
	pDC->GetWorldTransform(&saveForm);

	Rotate(pDC, (PI / 4));
	Translate(pDC, -30, 10);
	DrawCactusScales(pDC, dX2, dY, false);
	Translate(pDC, 0, dY);

	XFORM GreenCyrclePosition;
	pDC->GetWorldTransform(&GreenCyrclePosition);

	Rotate(pDC, (PI / 4));
	Translate(pDC, 0, 38);
	DrawCactusScales(pDC, dX3, dY, false);

	pDC->SetWorldTransform(&GreenCyrclePosition);

	Rotate(pDC, -(PI / 4));
	Translate(pDC, 0, dY);
	DrawCactusScales(pDC, dX3, dY, false);
	Translate(pDC, 0, dY);

	XFORM GreenCyrclePosition2;
	pDC->GetWorldTransform(&GreenCyrclePosition2);
	Translate(pDC, 0, dY);
	DrawCactusScales(pDC, dX4, dY, true);
	DrawGreenCircle(pDC, GreenCyrclePosition2, Radius);
	DrawGreenCircle(pDC, GreenCyrclePosition, Radius);

	pDC->SetWorldTransform(&GreenCyrclePosition);
	pDC->SetWorldTransform(&saveForm);
}

void CIND17905View::DrawRightBranch(CDC* pDC)
{
	XFORM saveForm;
	pDC->GetWorldTransform(&saveForm);

	Rotate(pDC, -(PI / 4));
	Translate(pDC, 30, 10);
	DrawCactusScales(pDC, dX2, dY, false);
	Translate(pDC, 0, dY);

	XFORM GreenCyrclePosition;
	pDC->GetWorldTransform(&GreenCyrclePosition);

	DrawRigtPartBranch(pDC);

	DrawMovingPart(pDC);
	DrawGreenCircle(pDC, GreenCyrclePosition, Radius);

	pDC->SetWorldTransform(&saveForm);
}

void CIND17905View::DrawRigtPartBranch(CDC* pDC)
{
	XFORM saveForm;
	pDC->GetWorldTransform(&saveForm);

	Rotate(pDC, -(PI / 4));
	Translate(pDC, 0, dY);
	DrawCactusScales(pDC, 20, dY, false);
	Translate(pDC, 0, dY);

	XFORM GreenCyrclePosition;
	pDC->GetWorldTransform(&GreenCyrclePosition);

	Translate(pDC, 0, dY);

	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Rotate(pDC, (PI / 4));
	Translate(pDC, -28, 10);
	DrawCactusScales(pDC, 20, dY, false);

	pDC->SetWorldTransform(&oldForm);

	Rotate(pDC, -(PI / 4));
	Translate(pDC, 28, 10);
	DrawCactusScales(pDC, dX3, dY, false);

	pDC->SetWorldTransform(&oldForm);

	DrawGreenCircle(pDC, GreenCyrclePosition, Radius);

	pDC->SetWorldTransform(&saveForm);
}

void CIND17905View::DrawMovingPart(CDC* pDC)
{
	XFORM saveForm;
	pDC->GetWorldTransform(&saveForm);

	Rotate(pDC, (PI * beta / 180));
	Translate(pDC, 0, dY);
	DrawCactusScales(pDC, dX3, dY, false);


	pDC->SetWorldTransform(&saveForm);
}




void CIND17905View::DrawGreenCircle(CDC* pDC, XFORM position ,int radius)
{
	XFORM oldPosition;
	pDC->GetWorldTransform(&oldPosition);
	pDC->SetWorldTransform(&position);
		
	CBrush* brush = new CBrush(RGB(82, 199, 97));
	CBrush* old = pDC->SelectObject(brush);

	pDC->Ellipse(-radius, -radius,radius,radius);

	pDC->SelectObject(old);
	delete brush;

	pDC->SetWorldTransform(&oldPosition);

}

void CIND17905View::DrawPot(CDC* pDC, int width, int height)
{
	CBrush* brush = new CBrush(RGB(205, 130, 0));
	CBrush* oldBrush = pDC->SelectObject(brush);

	POINT* trapezoid = new POINT[4]{
		{width / 3, 0},
		{(width * 5) / 12, (height * 5) / 7},
		{-(width * 5) / 12, (height * 5) / 7},
		{-width / 3, 0} };
	pDC->Polygon(trapezoid, 4);
	pDC->Rectangle(-width / 2, (height * 5) / 7, width / 2, height);
	pDC->SelectObject(oldBrush);
	delete brush;
	delete[] trapezoid;
}

void CIND17905View::DrawGrid(CDC* pDC)
{
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

void CIND17905View::OnDraw(CDC* pDC)
{
	CIND17905Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	pDC->SetBkColor(RGB(155, 155, 155));
	CBrush* bk = new CBrush(RGB(71, 237, 218));
	CRect rect(0, 0, 500, 500);
	pDC->FillRect(rect, bk);


	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	//draw cactus
	DrawCactus(pDC);

	pDC->SetWorldTransform(&oldForm);
	pDC->SetGraphicsMode(prevMode);

	if (grid)
		DrawGrid(pDC);

	Text(pDC);
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
	if (nChar == 'D')
		alfa -= 3;
	else if (nChar == 'A')
		alfa += 3;
	else if (nChar == 39)
		beta -= 3;
	else if (nChar == 37)
		beta += 3;
	else if (nChar == 'G')
		grid = !grid;
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
