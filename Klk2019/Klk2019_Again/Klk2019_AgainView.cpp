
// Klk2019_AgainView.cpp : implementation of the CKlk2019AgainView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2019_Again.h"
#endif

#include "Klk2019_AgainDoc.h"
#include "Klk2019_AgainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKlk2019AgainView

IMPLEMENT_DYNCREATE(CKlk2019AgainView, CView)

BEGIN_MESSAGE_MAP(CKlk2019AgainView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKlk2019AgainView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2019AgainView construction/destruction

CKlk2019AgainView::CKlk2019AgainView() noexcept
{
	// TODO: add construction code here
	arm1 = new DImage();
	arm1->Load((CString)"arm1.png");

	arm2 = new DImage();
	arm2->Load((CString)"arm2.png");

	leg1 = new DImage();
	leg1->Load((CString)"leg1.png");

	leg2 = new DImage();
	leg2->Load((CString)"leg2.png");

	body = new DImage();
	body->Load((CString)"body1.png");

	bk = new DImage();
	bk->Load((CString)"background.jpg");

	angleArm1 = 0.0;
	angleArm2 = 0.0;

	angleLeg1 = -60.0;
	angleLeg2 = -90.0;

	angleBody = -90.0;
	position = 0;
}

CKlk2019AgainView::~CKlk2019AgainView()
{
	delete arm1;
	delete arm2;
	delete leg1;
	delete leg2;
	delete body;
	delete bk;
}

BOOL CKlk2019AgainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2019AgainView drawing

void CKlk2019AgainView::Translate(CDC* pDC, float dx, float dy, bool right)
{
	XFORM form = { 1,0,0,1,dx,dy };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form,mode);
}

void CKlk2019AgainView::Rotate(CDC* pDC, float angle, bool right)
{
	XFORM form = {cos(angle), sin(angle),-sin(angle),cos(angle),0,0 };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2019AgainView::Scale(CDC* pDC, float sx, float sy, bool right)
{
	XFORM form = { sx,0,0,sy,0,0 };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CKlk2019AgainView::DrawTransparent(CDC* pDC, DImage* image)
{
	int w = image->Width();
	int h = image->Height();

	CBitmap* bitmap = image->GetBitmap();
	CBitmap* mask = new CBitmap();
	mask->CreateBitmap(w, h, 1, 1, NULL);

	CDC src;
	src.CreateCompatibleDC(pDC);
	src.SelectObject(bitmap);
	CDC dest;
	dest.CreateCompatibleDC(pDC);
	dest.SelectObject(mask);

	COLORREF color = src.GetPixel(0, 0);
	src.SetBkColor(color);

	dest.BitBlt(0, 0, w, h, &src, 0, 0, SRCCOPY);

	src.SetTextColor(RGB(255, 255, 255));
	src.SetBkColor(RGB(0, 0, 0));

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

void CKlk2019AgainView::DrawArm1(CDC* pDC)
{
	Translate(pDC, 90,88);
	Rotate(pDC, (PI * angleArm1) / 180);
	Translate(pDC, -34, -30);
	DrawArm2(pDC);
	DrawTransparent(pDC, arm1);
}

void CKlk2019AgainView::DrawArm2(CDC* pDC)
{
	XFORM form;
	pDC->GetWorldTransform(&form);
	Translate(pDC, 210, 102);
	Rotate(pDC, (PI * angleArm2) / 180);
	Translate(pDC, -23, -61);
	DrawTransparent(pDC, arm2);
	pDC->SetWorldTransform(&form);
}

void CKlk2019AgainView::DrawLeg1(CDC* pDC)
{
	Translate(pDC, 25, 132);
	Rotate(pDC, (PI * angleLeg1) / 180);
	Translate(pDC, -238, -128);
	DrawLeg2(pDC);
	DrawTransparent(pDC, leg1);


}

void CKlk2019AgainView::DrawLeg2(CDC* pDC)
{
	XFORM form;
	pDC->GetWorldTransform(&form);
	Translate(pDC, 27, 125);
	Rotate(pDC, (PI * angleLeg2) / 180);

	Translate(pDC, -35, -60);
	DrawTransparent(pDC, leg2);
	pDC->SetWorldTransform(&form);
}

void CKlk2019AgainView::DrawBody(CDC* pDC)
{
	XFORM form;
	pDC->GetWorldTransform(&form);

	Translate(pDC, 90, 90);

	Rotate(pDC, (PI * angleBody) / 180);
	Translate(pDC, -210, -85);

	DrawTransparent(pDC, body);
	DrawLeg1(pDC);
	pDC->SetWorldTransform(&form);
}

void CKlk2019AgainView::OnDraw(CDC* pDC)
{
	CKlk2019AgainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(memDC);

	CBitmap* bitmap = new CBitmap();
	bitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	memDC->SelectObject(bitmap);

	int prevMode = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM global,old;
	memDC->GetWorldTransform(&global);
	bk->Draw(memDC, CRect(0, 0, bk->Width(), bk->Height()), rect);

	Translate(memDC, position, 0.0);
	memDC->GetWorldTransform(&old);
	DrawBody(memDC);
	DrawArm1(memDC);
	memDC->SetWorldTransform(&global);
	memDC->SetGraphicsMode(prevMode);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	delete bitmap;
	memDC->DeleteDC();
	delete memDC;

}


// CKlk2019AgainView printing


void CKlk2019AgainView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKlk2019AgainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2019AgainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2019AgainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CKlk2019AgainView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKlk2019AgainView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKlk2019AgainView diagnostics

#ifdef _DEBUG
void CKlk2019AgainView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2019AgainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2019AgainDoc* CKlk2019AgainView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2019AgainDoc)));
	return (CKlk2019AgainDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2019AgainView message handlers


BOOL CKlk2019AgainView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CKlk2019AgainView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == 'Q') {
		
		if(angleArm1 < 0)
			angleArm1 += 1;
		if (angleLeg1 < 0)
			angleLeg1 += 1;
		
		Invalidate();
	}
	else if(nChar == 'A') {

		if (angleArm1 > -90)
			angleArm1 -= 1;
		if (angleLeg1 > -60)
			angleLeg1 -= 1;
		Invalidate();
	}
	else if (nChar == 'T' && angleArm2 > -180) {

		angleArm2 -= 1;
		Invalidate();
	}
	else if (nChar == 'G' && angleArm2 < 0) {

		angleArm2 += 1;
		Invalidate();
	}
	else if (nChar == 'R' && angleLeg2 < 0) {

		angleLeg2 += 1;
		Invalidate();
	}
	else if (nChar == 'F' && angleLeg2 > -90) {

		angleLeg2 -= 1;
		Invalidate();
	}
	else if (nChar == 'W' && angleBody > -90) {
		angleBody-= 1;
		Invalidate();
	}
	else if (nChar == 'S' && angleBody < 0.0) {

		angleBody += 1;
		Invalidate();
	}
	else if (nChar == VK_LEFT) {

		position -= 10;
		Invalidate();
	}
	else if (nChar == VK_RIGHT) {

		position += 10;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
