
// KASSINNIDlg.h : header file
//



#pragma once

#include "clipper.hpp"



// CKASSINNIDlg dialog
class CKASSINNIDlg : public CDialogEx
{
// Construction
public:
	CKASSINNIDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KASSINNI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void DrawOvalCas();
	afx_msg void make_net();
	afx_msg void Clip_it_and_count();
	CSliderCtrl RotAngle;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl RSize;
	CEdit max_dots_text;
	CEdit max_angle_text;
	CEdit max_X_text;
	CEdit max_Y_text;
	CSliderCtrl X_Slider;
	CSliderCtrl Y_Slider;
	CEdit curr_dot;
	CEdit curr_ang;
	CEdit curr_X;
	CEdit curr_Y;
	CEdit curr_R;
	CEdit max_R;
	afx_msg void analysis();
	afx_msg void oval_from_file();
	afx_msg void save_coords_of_max_dots();
	afx_msg void vokrug_ovala();
	CSliderCtrl A_slider;
	CSliderCtrl B_Slider;
	CEdit S_obolochki;
	CEdit S_Kas;
	CEdit S_obol_plus_kas;
	CEdit a_cuurent;
	CEdit b_current;
	afx_msg void make_response_coords();
	afx_msg void formula_square();
	CEdit FSquare_res;
	CButton RadioPoly;
	CButton RadioPoints;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
