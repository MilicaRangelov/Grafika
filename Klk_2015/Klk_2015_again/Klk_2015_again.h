
// Klk_2015_again.h : main header file for the Klk_2015_again application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKlk2015againApp:
// See Klk_2015_again.cpp for the implementation of this class
//

class CKlk2015againApp : public CWinAppEx
{
public:
	CKlk2015againApp() noexcept;


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

extern CKlk2015againApp theApp;
