
// KASSINNI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CKASSINNIApp:
// See KASSINNI.cpp for the implementation of this class
//

class CKASSINNIApp : public CWinApp
{
public:
	CKASSINNIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	afx_msg void save_coords_of_max();
};

extern CKASSINNIApp theApp;
