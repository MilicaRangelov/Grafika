
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
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND17905View construction/destruction

CIND17905View::CIND17905View() noexcept
{
	// TODO: add construction code here
	this->m_glRenderer = new CGLRenderer();
}

CIND17905View::~CIND17905View()
{
	if (this->m_glRenderer != nullptr)
	{
		delete this->m_glRenderer;
		this->m_glRenderer = nullptr;
	}
}

BOOL CIND17905View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND17905View drawing

void CIND17905View::OnDraw(CDC* pDC)
{
	CIND17905Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	m_glRenderer->DrawScene(pDC);
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


BOOL CIND17905View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CIND17905View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	CDC* pDC = GetDC();
	this->m_glRenderer->DestroyScene(pDC);
	ReleaseDC(pDC);
}


int CIND17905View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CDC* pDC = GetDC();
	this->m_glRenderer->CreateGLContext(pDC);
	ReleaseDC(pDC);
	return 0;
}


void CIND17905View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CDC* pDC = GetDC();
	this->m_glRenderer->PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CIND17905View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CDC* pDC = GetDC();
	this->m_glRenderer->Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CIND17905View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	 if (nChar == 'W') {
		m_glRenderer->RotateView(-5, 0);
		Invalidate();
	}
	else if (nChar == 'S') {
		m_glRenderer->RotateView(5, 0);
		Invalidate();
	}
	else if (nChar == 'A') {
		m_glRenderer->RotateView(0, -5);
		Invalidate();
	}
	else if (nChar == 'D') {
		m_glRenderer->RotateView(0, 5);
		Invalidate();
	}
	else if (nChar == 'R') {
		 m_glRenderer->pink = ! m_glRenderer->pink;
		 Invalidate();
	 }
	else if (nChar == 'G') {
		 m_glRenderer->red = !m_glRenderer->red;
		 Invalidate();
	 }
	else if (nChar == 'B') {
		 m_glRenderer->blue = !m_glRenderer->blue;
		 Invalidate();
	 }
	else if (nChar == 'N') {
		 m_glRenderer->showNormals = !m_glRenderer->showNormals;
		 Invalidate();
	 }
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
