
// IND_17905View.h : interface of the CIND17905View class
//

#pragma once


class CIND17905View : public CView
{

private:
	void DrawTrianglePolygon(POINT p1, POINT p2, POINT p3, CDC* pDC);
	void DrawSolidTriangle(POINT p1, POINT p2, POINT p3, COLORREF color, CDC* pDC);
	void DrawHatchTriangle(POINT p1, POINT p2, POINT p3, COLORREF bk, int pattern, COLORREF color, CDC* pDC);
	void DrawQuadricPolygon(POINT p1, POINT p2, POINT p3, POINT p4, COLORREF color, CDC* pDC);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawGrid(CDC* pDC);

protected: // create from serialization only
	
	bool keyHit;
	CIND17905View() noexcept;
	DECLARE_DYNCREATE(CIND17905View)

// Attributes
public:
	CIND17905Doc* GetDocument() const;

// Operations
public:

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
};

#ifndef _DEBUG  // debug version in IND_17905View.cpp
inline CIND17905Doc* CIND17905View::GetDocument() const
   { return reinterpret_cast<CIND17905Doc*>(m_pDocument); }
#endif

