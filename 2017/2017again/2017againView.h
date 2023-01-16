
// 2017againView.h : interface of the CMy2017againView class
//

#pragma once
#include "DImage.h"
#define PI 3.1416

class CMy2017againView : public CView
{
protected: // create from serialization only
	CMy2017againView() noexcept;
	DECLARE_DYNCREATE(CMy2017againView)

// Attributes
	DImage* leg1;
	DImage* leg2;
	DImage* leg3;
	DImage* bk;
	DImage* body;

	int dx;
	int dy;
	float sx;
	float sy;

	double angle[7];
	int l1;
	int l2;
	int l3;
	int l4;
	int lr1;
	int lr2;
	int ud1;
	int ud2;
public:
	CMy2017againDoc* GetDocument() const;

// Operations
public:

	void Translate(CDC* pDC, float dx, float dy, bool right = false);
	void Rotate(CDC* pDC, float angle, bool right = false);
	void Scale(CDC* pDC, float sx, float sy, bool right = false);

	void DrawTransparent(CDC* pDC, DImage* image);

	void DrawBody(CDC* pDC);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);

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
	virtual ~CMy2017againView();
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in 2017againView.cpp
inline CMy2017againDoc* CMy2017againView::GetDocument() const
   { return reinterpret_cast<CMy2017againDoc*>(m_pDocument); }
#endif

