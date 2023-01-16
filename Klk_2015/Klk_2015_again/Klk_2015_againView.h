
// Klk_2015_againView.h : interface of the CKlk2015againView class
//

#pragma once
#include "DImage.h"
#define PI 3.1416

class CKlk2015againView : public CView
{
protected: // create from serialization only
	CKlk2015againView() noexcept;
	DECLARE_DYNCREATE(CKlk2015againView)

	DImage* bk;
	int xP;
	int yP;
	int rP;


	int xG;
	int yG;
	int rG;

	int sx;
	int sy;

	float angle;
	float rotate;

	bool move;

// Attributes
public:
	CKlk2015againDoc* GetDocument() const;

// Operations
public:

	void Translation(CDC* pDC, float dx, float dy, bool right = false);
	void Rotate(CDC* pDC, float angle, bool right = false);
	void Scale(CDC* pDC, float sx, float sy, bool right = false);

	void DrawPackman(CDC* pDC, CRect rect, float angle);
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
	virtual ~CKlk2015againView();
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

#ifndef _DEBUG  // debug version in Klk_2015_againView.cpp
inline CKlk2015againDoc* CKlk2015againView::GetDocument() const
   { return reinterpret_cast<CKlk2015againDoc*>(m_pDocument); }
#endif

