
// 2018_againView.cpp : implementation of the CMy2018againView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2018_again.h"
#endif

#include "2018_againDoc.h"
#include "2018_againView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2018againView

IMPLEMENT_DYNCREATE(CMy2018againView, CView)

BEGIN_MESSAGE_MAP(CMy2018againView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy2018againView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMy2018againView construction/destruction

CMy2018againView::CMy2018againView() noexcept
{
	// TODO: add construction code here


	
	position = 0;
	arm1 = new DImage();
	arm1->Load((CString)"arm1.png");

	arm2 = new DImage();
	arm2->Load((CString)"arm2.png");

	bager = new DImage();
	bager->Load((CString)"bager.png");

	bk = new DImage();
	bk->Load((CString)"pozadina.png");

	viljuska = GetEnhMetaFile((CString)"viljuska.emf");
	ENHMETAHEADER header;
	GetEnhMetaFileHeader(viljuska, sizeof(ENHMETAHEADER), &header);

	w = header.rclBounds.right - header.rclBounds.left;
	h = header.rclBounds.bottom - header.rclBounds.top;

	angle1Arm1 = -90;
	angleArm2 = -90;
	angleFork = 0;

	angle = 0;
	sx = 1;
	sy = 1;
}

CMy2018againView::~CMy2018againView()
{
	delete arm1;
	delete arm2;
	delete bk;
	delete bager;

	DeleteObject(viljuska);

}

BOOL CMy2018againView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2018againView drawing

void CMy2018againView::Translate(CDC* pDC, float dx, float dy, bool right)
{
	XFORM form = { 1,0,0,1,dx,dy };
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CMy2018againView::Rotate(CDC* pDC, float angle, bool right)
{
	XFORM form = { cos(angle), sin(angle),-sin(angle),cos(angle),0,0};
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CMy2018againView::Scale(CDC* pDC, float sx, float sy, bool right)
{
	XFORM form = { sx,0,0,sy,0,0};
	DWORD mode = right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&form, mode);
}

void CMy2018againView::DrawTransparent(CDC* pDC, DImage* image)
{
	int w = image->Width();
	int h = image->Height();

	CBitmap* bitmap = image->GetBitmap();
	CBitmap* mask = new CBitmap();
	mask->CreateBitmap(w, h, 1, 1, NULL);

	CDC src, dest;
	src.CreateCompatibleDC(pDC);
	src.SelectObject(bitmap);

	dest.CreateCompatibleDC(pDC);
	dest.SelectObject(mask);

	COLORREF color = src.GetPixel(0, 0);
	src.SetBkColor(color);

	dest.BitBlt(0, 0,w, h, &src, 0, 0, SRCCOPY);

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
	delete mask;


}

void CMy2018againView::DrawBackground(CDC* pDC, CRect rect)
{
	int wid = rect.Width();
	int hei = rect.Height();

	int w2 = bk->Width();
	int h2 = bk->Height();

	XFORM form;
	pDC->GetWorldTransform(&form);

	Translate(pDC, wid / 2, hei / 2);

	bk->Draw(pDC, CRect(0,0,w2,h2), CRect(-w2/2, -rect.top - h2/2, w2/2, rect.bottom));

	pDC->SetWorldTransform(&form);
}

void CMy2018againView::DrawBager(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	Translate(pDC, 3* rect.right / 4 + position, rect.Height() / 2);
	DrawTransparent(pDC, bager);
	DrawArm1(pDC);
}

void CMy2018againView::DrawArm1(CDC* pDC)
{

	Translate(pDC, 58,161);
	Rotate(pDC, (PI * angle1Arm1) / 180);
	Translate(pDC, -56, -61);
	DrawTransparent(pDC, arm1);
	DrawArm2(pDC);
}

void CMy2018againView::DrawArm2(CDC* pDC)
{
	Translate(pDC, 309, 61);

	Rotate(pDC, (PI * angleArm2) / 180);
	Translate(pDC, -36, -40);
	DrawTransparent(pDC, arm2);
	DrawFork(pDC);
}

void CMy2018againView::DrawFork(CDC* pDC)
{
	Translate(pDC, 272, 40);
	Rotate(pDC, (PI * angleFork) / 180);
	Scale(pDC, 2.5, 2.5);
	Translate(pDC, -14, -20);

	pDC->PlayMetaFile(viljuska, CRect(0, 0, w, h));
}

void CMy2018againView::OnDraw(CDC* pDC)
{
	CMy2018againDoc* pDoc = GetDocument();
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

	DrawBackground(memDC, rect);
	DrawBager(memDC);

	memDC->SetWorldTransform(&global);
	memDC->SetGraphicsMode(prev);

	int orgmode = pDC->SetGraphicsMode(GM_ADVANCED);
	Translate(pDC, rect.Width() / 2, rect.Height() / 2);
	Rotate(pDC, angle * PI);
	Scale(pDC, sx, sy);
	Translate(pDC, -rect.Width() / 2, -rect.Height() / 2);

	pDC->SetGraphicsMode(orgmode);



	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	memDC->DeleteDC();
	delete memDC;
	delete bitmap;
}


// CMy2018againView printing


void CMy2018againView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy2018againView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2018againView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2018againView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMy2018againView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy2018againView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy2018againView diagnostics

#ifdef _DEBUG
void CMy2018againView::AssertValid() const
{
	CView::AssertValid();
}

void CMy2018againView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2018againDoc* CMy2018againView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2018againDoc)));
	return (CMy2018againDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy2018againView message handlers


BOOL CMy2018againView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CMy2018againView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == '1') {
		angle1Arm1 -= 10;
		Invalidate();
	}
	else if (nChar == '2') {
		angle1Arm1 += 10;
		Invalidate();
	}
	else if (nChar == '3') {
		angleArm2 -= 10;
		Invalidate();
	}
	else if (nChar == '4') {
		angleArm2 += 10;
		Invalidate();
	}
	else if (nChar == '5') {
		angleFork -= 10;
		Invalidate();
	}
	else if (nChar == '6') {
		angleFork += 10;
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
	else if (nChar == 'M') {
		sy = (-1) * sy;
		sx = (-1) * sx;
		Invalidate();
	}
	else if (nChar == 'R') {
		angle= int(angle + 1) % 4;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
