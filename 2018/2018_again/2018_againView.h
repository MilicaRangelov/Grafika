
// 2018_againView.h : interface of the CMy2018againView class
//

#pragma once
#include "DImage.h"
#define PI 3.1416

class CMy2018againView : public CView
{
protected: // create from serialization only
	CMy2018againView() noexcept;
	DECLARE_DYNCREATE(CMy2018againView)

// Attributes
	DImage* arm1;
	DImage* arm2;
	DImage* bager;
	DImage* bk;
	
	HENHMETAFILE viljuska;
	int w;
	int h;

	int position;
	float angle1Arm1;
	float angleArm2;
	float angleFork;

	int sx, sy;
	float angle;
public:
	CMy2018againDoc* GetDocument() const;

// Operations
public:

	void Translate(CDC* pDC, float dx, float dy, bool right = false);
	void Rotate(CDC* pDC, float angle, bool right = false);
	void Scale(CDC* pDC, float sx, float sy, bool right = false);

	void DrawTransparent(CDC* pDC, DImage* image);

	void DrawBackground(CDC* pDC, CRect rect);
	void DrawBager(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawFork(CDC* pDC);
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
	virtual ~CMy2018againView();
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

#ifndef _DEBUG  // debug version in 2018_againView.cpp
inline CMy2018againDoc* CMy2018againView::GetDocument() const
   { return reinterpret_cast<CMy2018againDoc*>(m_pDocument); }
#endif

