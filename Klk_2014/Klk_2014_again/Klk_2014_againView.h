
// Klk_2014_againView.h : interface of the CKlk2014againView class
//

#pragma once
#include "DImage.h"
#define PI 3.1416
#include <iostream>
#include <cmath>

using namespace std;

class CKlk2014againView : public CView
{
protected: // create from serialization only
	CKlk2014againView() noexcept;
	DECLARE_DYNCREATE(CKlk2014againView)

// Attributes
public:
	CKlk2014againDoc* GetDocument() const;

	HENHMETAFILE clio;
	int w;
	int h;
	DImage* wheel;

	double angle;
	int position;
// Operations
public:

	void Translate(CDC* pDC, double dx, double dy, bool right = false);
	void Rotate(CDC* pDC, double angle, bool right = false);
	void Scale(CDC* pDC, double sx, double sy, bool right = false);

	void DrawGround(CDC* pDC, double angle);

	void DrawCar(CDC* pDC, int x, int y);
	void DrawWheel(CDC* pDC, int x, int y, int r, double angle);
	void DrawTransparent(CDC* pDC, DImage* image);
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
	virtual ~CKlk2014againView();
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

#ifndef _DEBUG  // debug version in Klk_2014_againView.cpp
inline CKlk2014againDoc* CKlk2014againView::GetDocument() const
   { return reinterpret_cast<CKlk2014againDoc*>(m_pDocument); }
#endif

