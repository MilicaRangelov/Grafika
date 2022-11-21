
// Klk2015View.h : interface of the CKlk2015View class
//

#pragma once
#include "DImage.h"
#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>



class CKlk2015View : public CView
{
protected: // create from serialization only
	CKlk2015View() noexcept;
	DECLARE_DYNCREATE(CKlk2015View)

	bool open;
	bool left;
	int position;
	DImage* background;
// Attributes
public:
	CKlk2015Doc* GetDocument() const;

// Operations
public:
	

	void Translation(CDC* pDC, double dX, double dY, bool right = false);
	void Scale(CDC* pDC, double sX, double sY, bool right = false);
	void DrawPacman(CDC* pDC, CRect rect, float angle);
	void DrawGhost(CDC* pDC, CRect rect);

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
	virtual ~CKlk2015View();
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

#ifndef _DEBUG  // debug version in Klk2015View.cpp
inline CKlk2015Doc* CKlk2015View::GetDocument() const
   { return reinterpret_cast<CKlk2015Doc*>(m_pDocument); }
#endif

