
// Klk2019_Again.h : main header file for the Klk2019_Again application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2019AgainApp:
// See Klk2019_Again.cpp for the implementation of this class
//

class CKlk2019AgainApp : public CWinAppEx
{
public:
	CKlk2019AgainApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKlk2019AgainApp theApp;
