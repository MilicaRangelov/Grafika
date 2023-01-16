
// Klk2019_AgainView.h : interface of the CKlk2019AgainView class
//

#pragma once
#include "DImage.h"
#define PI 3.1416

class CKlk2019AgainView : public CView
{
protected: // create from serialization only
	CKlk2019AgainView() noexcept;
	DECLARE_DYNCREATE(CKlk2019AgainView)

// Attributes

	DImage* arm1;
	DImage* arm2;
	DImage* leg1;
	DImage* leg2;
	DImage* body;
	DImage* bk;

	float angleArm1;
	float angleArm2;

	float angleLeg1;
	float angleLeg2;

	float angleBody;
	int position;
	
public:
	CKlk2019AgainDoc* GetDocument() const;

// Operations
public:

	void Translate(CDC* pDC, float dx, float dy, bool right = false);
	void Rotate(CDC* pDC, float angle, bool right = false);
	void Scale(CDC* pDC, float sx, float sy, bool right = false);

	void DrawTransparent(CDC* pDC, DImage* image);

	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);

	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);

	void DrawBody(CDC* pDC);

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
	virtual ~CKlk2019AgainView();
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

#ifndef _DEBUG  // debug version in Klk2019_AgainView.cpp
inline CKlk2019AgainDoc* CKlk2019AgainView::GetDocument() const
   { return reinterpret_cast<CKlk2019AgainDoc*>(m_pDocument); }
#endif

