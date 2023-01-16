
// Klk_2014_again.h : main header file for the Klk_2014_again application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2014againApp:
// See Klk_2014_again.cpp for the implementation of this class
//

class CKlk2014againApp : public CWinAppEx
{
public:
	CKlk2014againApp() noexcept;


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

extern CKlk2014againApp theApp;
