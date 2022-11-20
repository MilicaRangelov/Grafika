
// Klk2013View.h : interface of the CKlk2013View class
//

#pragma once
#include "DImage.h"
#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>

class CKlk2013View : public CView
{
protected: // create from serialization only
	CKlk2013View() noexcept;
	DECLARE_DYNCREATE(CKlk2013View)

	int position;
	double angle;

	DImage* background;
	DImage* tree;

// Attributes
public:
	CKlk2013Doc* GetDocument() const;

// Operations
public:

// Overrides

	void Translate(CDC* pDC, double dX, double dY, bool right = false);
	void Rotate(CDC* pDC, double angle, bool right = false);

	void DrawBlade(CDC* pDC, int size);

	void Text(CDC* pDC, int size);
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2013View();
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
};

#ifndef _DEBUG  // debug version in Klk2013View.cpp
inline CKlk2013Doc* CKlk2013View::GetDocument() const
   { return reinterpret_cast<CKlk2013Doc*>(m_pDocument); }
#endif

