
// IND_17905View.h : interface of the CIND17905View class
//

#pragma once

#define dY 38
#define dX1 0
#define dX2 10
#define dX3 20
#define dX4 30
#define PI 3.14159265
#define Radius 10


class CIND17905View : public CView
{

private:
	int beta;
	int alfa;
	bool grid;
protected: // create from serialization only
	CIND17905View() noexcept;
	DECLARE_DYNCREATE(CIND17905View)

// Attributes
public:
	CIND17905Doc* GetDocument() const;

// Operations
public:

	void Translate(CDC* pDC, double Dx, double Dy, bool right = false);
	void Scale(CDC* pDC, double Sx, double Sy, bool right = false);
	void Rotate(CDC* pDC, double angle,bool right = false);

	void Text(CDC* pDC);

	//draw cactus

	void DrawCactus(CDC* pDC);

	void DrawCactusScales(CDC* pDC, int width, int height, bool light);

	void DrawLeftBranch(CDC* pDC);
	void DrawRightBranch(CDC* pDC);
	void DrawRigtPartBranch(CDC* pDC);

	void DrawMovingPart(CDC* pDC);

	void DrawGreenCircle(CDC* pDC, XFORM position,int radius);

	void DrawPot(CDC* pDC, int width, int height);
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
};

#ifndef _DEBUG  // debug version in IND_17905View.cpp
inline CIND17905Doc* CIND17905View::GetDocument() const
   { return reinterpret_cast<CIND17905Doc*>(m_pDocument); }
#endif

