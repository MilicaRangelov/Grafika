
// IND_17905View.h : interface of the CIND17905View class
//

#pragma once
#include "DImage.h"
#define PI 3.14159265

class CIND17905View : public CView
{
protected: // create from serialization only

	DImage* part;
	int angle;
	CIND17905View() noexcept;
	DECLARE_DYNCREATE(CIND17905View)

// Attributes
public:
	CIND17905Doc* GetDocument() const;

// Operations
public:

	void Translate(CDC* pDC, int width, int height, bool right = false);
	void Rotate(CDC* pDC, double angle, bool right = false);
	void Scale(CDC* pDC, int dX, int dY, bool right = false);
	void Mirror(CDC* pDC, int dX, int dY, bool right = false);

	void DrawPart(CDC* pDC,int ind, int dX, int dY,double angle, bool blue = false);
	CBitmap* DrawTransparent(CDC* pDC, CBitmap* image, int width, int height);
	CBitmap* MakeImageGrayscale(CDC* pDC,int ind, bool blue=false);
	void MemPdC(CDC* pDC, CRect rect);
	void DrawGrid(CDC* pDC);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIND17905View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in IND_17905View.cpp
inline CIND17905Doc* CIND17905View::GetDocument() const
   { return reinterpret_cast<CIND17905Doc*>(m_pDocument); }
#endif

