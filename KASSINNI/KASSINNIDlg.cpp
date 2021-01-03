
// KASSINNIDlg.cpp : implementation file
//
#include "stdafx.h"
#include "pch.h"
#include "framework.h"
#include "KASSINNI.h"
#include "KASSINNIDlg.h"
#include "afxdialogex.h"
#include <math.h>
#include <fstream>
#include <string>

#include "clipper.hpp"

using namespace ClipperLib;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CKASSINNIDlg dialog



CKASSINNIDlg::CKASSINNIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KASSINNI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKASSINNIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, RotAngle);
	DDX_Control(pDX, IDC_SLIDER2, RSize);
	DDX_Control(pDX, IDC_EDIT1, max_dots_text);
	DDX_Control(pDX, IDC_EDIT2, max_angle_text);
	DDX_Control(pDX, IDC_EDIT3, max_X_text);
	DDX_Control(pDX, IDC_EDIT4, max_Y_text);
	DDX_Control(pDX, IDC_SLIDER3, X_Slider);
	DDX_Control(pDX, IDC_SLIDER4, Y_Slider);
	DDX_Control(pDX, IDC_EDIT5, curr_dot);
	DDX_Control(pDX, IDC_EDIT6, curr_ang);
	DDX_Control(pDX, IDC_EDIT7, curr_X);
	DDX_Control(pDX, IDC_EDIT8, curr_Y);
	DDX_Control(pDX, IDC_EDIT9, curr_R);
	DDX_Control(pDX, IDC_EDIT10, max_R);
	DDX_Control(pDX, IDC_SLIDER5, A_slider);
	DDX_Control(pDX, IDC_SLIDER6, B_Slider);
	DDX_Control(pDX, IDC_EDIT11, S_obolochki);
	DDX_Control(pDX, IDC_EDIT12, S_Kas);
	DDX_Control(pDX, IDC_EDIT13, S_obol_plus_kas);
	DDX_Control(pDX, IDC_EDIT14, a_cuurent);
	DDX_Control(pDX, IDC_EDIT15, b_current);
	DDX_Control(pDX, IDC_EDIT16, FSquare_res);
}

BEGIN_MESSAGE_MAP(CKASSINNIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CKASSINNIDlg::DrawOvalCas)
	ON_BN_CLICKED(IDC_BUTTON2, &CKASSINNIDlg::make_net)
	ON_BN_CLICKED(IDC_BUTTON4, &CKASSINNIDlg::Clip_it_and_count)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON6, &CKASSINNIDlg::analysis)
	ON_BN_CLICKED(IDC_BUTTON7, &CKASSINNIDlg::oval_from_file)
	ON_BN_CLICKED(IDC_BUTTON8, &CKASSINNIDlg::save_coords_of_max_dots)
	ON_BN_CLICKED(IDC_BUTTON9, &CKASSINNIDlg::vokrug_ovala)
	ON_BN_CLICKED(IDC_BUTTON10, &CKASSINNIDlg::make_response_coords)
	ON_BN_CLICKED(IDC_BUTTON11, &CKASSINNIDlg::formula_square)
END_MESSAGE_MAP()


// CKASSINNIDlg message handlers

CRect rectDraw;
int cx, cy;
double R = 1;
int coef = 100; /////////////////


BOOL CKASSINNIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}

		RotAngle.SetRange(0, 360, 1);
		RotAngle.SetPos(0);

		RSize.SetRange(1, 10, 1);
		RSize.SetPos(1);

		X_Slider.SetRange(-1 * R*coef, R*coef, 1);
		X_Slider.SetPos(0);

		Y_Slider.SetRange(-1 * (0.5*R*sqrt(3)) * coef, (0.5*R*sqrt(3)) * coef, 1);
		int j = (0.5*R*sqrt(3)) * coef;
		Y_Slider.SetPos(0);

		GetClientRect(&rectDraw);
		cx = rectDraw.Width() / 2;
		cy = rectDraw.Height() / 2;

		rectDraw.SetRect(cx - 175, cy - 120, cx + 175, cy + 120);

		A_slider.SetRange(1, 1000, 1);
		A_slider.SetPos(500);

		B_Slider.SetRange(1, 1000, 1);
		B_Slider.SetPos(499);

	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKASSINNIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

double PI = 3.141592;

CRect rect;
CPen OvalPen(PS_SOLID, 1, RGB(255, 0, 0)), HexPen(PS_SOLID, 1, RGB(0, 0, 0)), result(PS_SOLID, 1, RGB(255, 0, 0)),
			FromFilePen(PS_SOLID, 1, RGB(0, 0, 255));
CBrush obolochka(RGB(0, 0, 127));
CBrush oval(RGB(255, 255, 255));

std::vector<Path> Oval(1);
POINT *OvalDraw;

std::vector<Paths> HexNet(1);

std::vector<Path> new_dot(1);
POINT **OneHexDraw;

std::vector<Paths> AllRes(1);
POINT **AllResDraw;

std::vector<Path> Rotated(1);

std::vector<Path> TwoParts(2);
POINT **TwoPartsDraw;

Clipper cp;
std::vector<Path> res(1);

std::vector<Paths> achieved_poly(1);
POINT **achieved_poly_draw;


double mult = 10/double(coef); ///////////////


double form_y_for_oval(double a, double b, double x);
std::vector<Path> form_dot(double center_x, double center_y);
void RotatedOval(int Angle, int new_X, int new_Y);

int number_of_points = 0;

int max_dots = 0;
int max_angle = 0;
double max_X = 0;
double max_Y = 0;

int curr_dots = 0;
int curr_angle = 0;
double curr_X_int = 0;
double curr_Y_int = 0;

CString for_controls;

vector<Path> kass_from_file(1);
vector<Paths> MaxCoords(1);

double area_kassini;
double area_kassini_and_achieved;
double area_achieved;

bool TwoPartsIndicate = false;
bool OvalIndicate = false;
bool HexIndicator = false;
bool IntersecIndicator = false;
bool ReachIndicator = false;
bool OvalFileIndicator = false;

double a, b;

void CKASSINNIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		if (ReachIndicator == true) {
			dc.SelectObject(obolochka);
			for (int i = 0; i < achieved_poly[0].size(); i++) {
				dc.Polygon(achieved_poly_draw[i], achieved_poly[0][i].size());
			}

			//for (int i = 0; i < achieved_poly[0].size(); i++) {
			//	for (int j = 0; j < achieved_poly[0][i].size(); j++) {
			//		dc.Ellipse(cx + (achieved_poly[0][i][j].X / (coef / 10) - 1), cy - (achieved_poly[0][i][j].Y / (coef / 10) - 1),
			//			cx + (achieved_poly[0][i][j].X / (coef / 10) + 1), cy - (achieved_poly[0][i][j].Y / (coef / 10) + 1));
			//		//Sleep(10);
			//	}
			//}
		}
		dc.SelectObject(oval); //был OvalPen
		//dc.Rectangle(rectDraw);
		if (OvalIndicate == true) {
			dc.Polygon(OvalDraw, Oval[0].size());
		}

		if (TwoPartsIndicate == true) {
			for (int i = 0; i < TwoParts.size(); i++) {
				dc.Polygon(TwoPartsDraw[i], TwoParts[i].size());
			}
			//for (int i = 0; i < TwoParts.size(); i++) {
			//	for (int j = 0; j < TwoParts[i].size(); j++) {
			//		dc.Ellipse((TwoPartsDraw[i][j].x - 1), TwoPartsDraw[i][j].y - 1, TwoPartsDraw[i][j].x + 1, TwoPartsDraw[i][j].y + 1);
			//		//Sleep(10);
			//	}
			//}
		}

		if (HexIndicator == true) {
			dc.SelectObject(HexPen);
			for (int i = 0; i < HexNet[0].size(); i++) {
				dc.Polygon(OneHexDraw[i], 3);
			}
		}
		if (IntersecIndicator == true) {
			dc.SelectObject(result);
			for (int i = 0; i < AllRes[0].size(); i++) {
				dc.Polygon(AllResDraw[i], 3);
			}
		}

		dc.SelectObject(FromFilePen);
		if (OvalFileIndicator == true) {
			for (int i = 0; i < kass_from_file[0].size(); i++) {
				dc.Ellipse(cx + (kass_from_file[0][i].X / (coef/10) - 3), cy - (kass_from_file[0][i].Y / (coef/10) - 3),
					cx + (kass_from_file[0][i].X / (coef/10) + 3), cy - (kass_from_file[0][i].Y / (coef/10) + 3));
			}
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKASSINNIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int N = 16;
double x_end, y_end;
int current_size;

void CKASSINNIDlg::DrawOvalCas()
{
	a = double(A_slider.GetPos()) / 100;
	b = double(B_Slider.GetPos()) / 100;
	Oval[0].clear();
	for (double x = -N / 2; x < N / 2; x += 0.1) {
		x_end = x;
		y_end = form_y_for_oval(a, b, x);
		if (y_end >= 0) {
			Oval[0] << IntPoint(coef * (x_end), coef * sqrt(y_end));
		}
	}
	current_size = Oval[0].size();
	for (int i = 0; i < current_size; i++) {
		Oval[0] << IntPoint(Oval[0][current_size - i - 1].X, -1 * Oval[0][current_size - i - 1].Y);
	}

	RotatedOval(RotAngle.GetPos(), X_Slider.GetPos(), Y_Slider.GetPos());

	if (a > b) {
		for (int i = 0; i < TwoParts.size(); i++) {
			TwoParts[i].clear();
		}
		for (int i = 0; i < Oval[0].size()*0.25; i++) {
			TwoParts[0] << IntPoint(Oval[0][i].X, Oval[0][i].Y);

		}
		for (int i = Oval[0].size()*0.75; i < Oval[0].size(); i++) {
			TwoParts[0] << IntPoint(Oval[0][i].X, Oval[0][i].Y);
		}

		for (int i = Oval[0].size()*0.25; i < Oval[0].size()*0.75; i++) {
			TwoParts[1] << IntPoint(Oval[0][i].X, Oval[0][i].Y);
		}

		TwoPartsDraw = new POINT*[TwoParts.size()];
		for (int i = 0; i < TwoParts.size(); i++) {
			TwoPartsDraw[i] = new POINT[TwoParts[i].size()];
		}

		for (int i = 0; i < TwoParts.size(); i++) {
			for (int j = 0; j < TwoParts[i].size(); j++) {
				TwoPartsDraw[i][j].x = cx + mult * TwoParts[i][j].X;
				TwoPartsDraw[i][j].y = cy - mult * TwoParts[i][j].Y;
			}
		}

		OvalIndicate = false;
		TwoPartsIndicate = true;
	}
	/////////////сделать возможность поворота и перемещения двухсвязного овала
	else {
		TwoPartsIndicate = false;
		OvalIndicate = true;
	}


	InvalidateRect(rectDraw, 1);
	OnPaint();
}

void RotatedOval(int Angle, int new_X, int new_Y) {

	Rotated[0].clear();
	OvalDraw = new POINT[Oval[0].size()];
	for (int i = 0; i < Oval[0].size(); i++) {
		Rotated[0] << IntPoint(Oval[0][i].X * cos(Angle*PI / 180) - Oval[0][i].Y * sin(Angle * PI / 180) + new_X,
			Oval[0][i].X * sin(Angle*PI / 180) + Oval[0][i].Y * cos(Angle * PI / 180) + new_Y);

		OvalDraw[i].x = cx + mult * Rotated[0][i].X;
		OvalDraw[i].y = cy - mult * Rotated[0][i].Y;
	}
}

double form_y_for_oval(double a, double b, double x) {
	double y = sqrt(pow(b, 4) + 4 * a*a*x*x) - x * x - a * a;
	return y;
}

std::vector<Path> form_dot(double center_x, double center_y) {
	std::vector<Path> new_dot(1);
	double x, y;
	for (int i = 0; i < 3; i++) {
		x = center_x + cos((90 - (120 * i))*PI / 180); /////////////////
 		y = center_y + sin((90 - (120 * i))*PI / 180); /////////////////
		new_dot[0] << IntPoint(round(x), round(y));
	}
	return new_dot;
}

double center_x = 0, center_y = 0, step_right = 1, step_left = -1;
int x_right_bord = 0, x_left_bord = 0, y_top_bord = 0, y_bot_bord = 0, odd = 0;


void CKASSINNIDlg::make_net()
{
	a = double(A_slider.GetPos()) / 100;
	b = double(B_Slider.GetPos()) / 100;
	R = double(RSize.GetPos());
	center_x = 0, center_y = 0, step_right = 1, step_left = -1;
	x_right_bord = 0, x_left_bord = 0, y_top_bord = 0, y_bot_bord = 0, odd = 0;

	HexNet[0].clear();
	HexNet[0] << form_dot(center_x, center_y)[0];

	while (y_bot_bord == 0 || y_top_bord == 0) {
		while (x_left_bord == 0 || x_right_bord == 0) {
			if (abs(step_right * R * coef) < rectDraw.Width() * coef / 2 && x_right_bord == 0) { /////////////////
				center_x = (step_right * R + odd * 0.5 * R)* coef;
				if (sqrt(pow(center_x, 2) + pow(center_y, 2)) <= (pow(b, 2) + 9.5*coef)) { ////////////////
					HexNet[0] << form_dot(center_x, center_y)[0];
					if (center_y != 0) {
						HexNet[0] << form_dot(center_x, -1 * center_y)[0];
					}
				}
				else {
					break;
				}
				step_right++;
			}
			else {
				x_right_bord = 1;
			}

			if (abs(step_left * R * coef) < rectDraw.Width() * coef / 2 && x_left_bord == 0) { //////////////////
				center_x = (step_left * R + odd * 0.5 * R) * coef;
				HexNet[0] << form_dot(center_x, center_y)[0];
				if (sqrt(pow(center_x, 2) + pow(center_y, 2)) <= (pow(b, 2) +9.5*coef)) { ///////////////
					if (center_y != 0) {
						HexNet[0] << form_dot(center_x, -1 * center_y)[0];
					}
				}
				else {
					break;
				}
				step_left--;
			}
			else {
				x_left_bord = 1;
			}
		}
		center_y += (R * sqrt(3) / 2)*coef;
		if (center_y < coef * rectDraw.Height()/2) { ////////////////////
			if (odd == 0) {
				odd = 1;
			}
			else {
				odd = 0;
			}

			center_x = R * odd * 0.5 * coef;
			if (sqrt(pow(center_x, 2) + pow(center_y, 2)) <= (pow(b, 2) + 9.5*coef)) { //////////////
				HexNet[0] << form_dot(center_x, center_y)[0];
				HexNet[0] << form_dot(center_x, -1 * center_y)[0];
			}

			x_left_bord = 0;
			x_right_bord = 0;
			step_right = 1;
			step_left = -1;
		}
		else {
			y_bot_bord = 1;
			y_top_bord = 1;
		}
	}

	OneHexDraw = new POINT*[HexNet[0].size()];
	for (int i = 0; i < HexNet[0].size(); i++) {
		OneHexDraw[i] = new POINT[HexNet[0][i].size()];
	}
	for (int i = 0; i < HexNet[0].size(); i++) {
		for (int j = 0; j < HexNet[0][i].size(); j++) {
			OneHexDraw[i][j].x = cx + mult * HexNet[0][i][j].X;
			OneHexDraw[i][j].y = cy - mult * HexNet[0][i][j].Y;
		}
	}
	HexIndicator = true;
	InvalidateRect(rectDraw, 1);
	OnPaint();
}


void CKASSINNIDlg::Clip_it_and_count()
{
	number_of_points = 0;

	AllRes[0].clear();

	for (int i = 0; i < HexNet[0].size(); i++) {
		cp.Clear();
		res.clear();

		cp.AddPath(HexNet[0][i], ptSubject, true);
		cp.AddPath(Rotated[0], ptClip, true);
		cp.Execute(ctIntersection, res, pftNonZero, pftNonZero);
		if (res.size() > 0) {
			number_of_points++;
			AllRes[0] << HexNet[0][i];
		}
	}
	AllResDraw = new POINT*[AllRes[0].size()];
	for (int i = 0; i < AllRes[0].size(); i++) {
		AllResDraw[i] = new POINT[AllRes[0][i].size()];
	}
	for (int i = 0; i < AllRes[0].size(); i++) {
		for (int j = 0; j < AllRes[0][i].size(); j++) {
			AllResDraw[i][j].x = cx + mult * AllRes[0][i][j].X;
			AllResDraw[i][j].y = cy - mult * AllRes[0][i][j].Y;
		}
	}

	if (number_of_points > max_dots) {
		MaxCoords[0] = AllRes[0];

		for_controls.Format(_T("%d"), int(R));
		max_R.SetWindowTextW(for_controls);

		max_dots = number_of_points;
		for_controls.Format(_T("%d"), max_dots);
		max_dots_text.SetWindowTextW(for_controls);

		max_angle = RotAngle.GetPos();
		for_controls.Format(_T("%d"), max_angle);
		max_angle_text.SetWindowTextW(for_controls);

		max_X = X_Slider.GetPos();
		for_controls.Format(_T("%f"), max_X/coef);
		max_X_text.SetWindowTextW(for_controls);

		max_Y = Y_Slider.GetPos();
		for_controls.Format(_T("%f"), max_Y/coef);
		max_Y_text.SetWindowTextW(for_controls);
	}

	for_controls.Format(_T("%d"), int(R));
	curr_R.SetWindowTextW(for_controls);

	curr_dots = number_of_points;
	for_controls.Format(_T("%d"), curr_dots);
	curr_dot.SetWindowTextW(for_controls);

	curr_angle = RotAngle.GetPos();
	for_controls.Format(_T("%d"), curr_angle);
	curr_ang.SetWindowTextW(for_controls);

	curr_X_int = X_Slider.GetPos();
	for_controls.Format(_T("%f"), curr_X_int/coef);
	curr_X.SetWindowTextW(for_controls);

	curr_Y_int = Y_Slider.GetPos();
	for_controls.Format(_T("%f"), curr_Y_int/coef);
	curr_Y.SetWindowTextW(for_controls);

	IntersecIndicator = true;
	InvalidateRect(rectDraw, 1);
	OnPaint();
}

void CKASSINNIDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl *pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
	if (pSlider == &RotAngle) {
		delete[] OvalDraw;
		delete[] TwoPartsDraw;
		delete[] AllResDraw;
		RotatedOval(RotAngle.GetPos(), X_Slider.GetPos(), Y_Slider.GetPos());
		Clip_it_and_count();
		//AfxMessageBox(_T(":)"), MB_ICONINFORMATION);
		InvalidateRect(rectDraw, 1);
		OnPaint();
	}

	if (pSlider == &RSize) {
		X_Slider.SetRange(-1 * R*coef, R*coef, 1);
		Y_Slider.SetRange(-1 * (0.5*R*sqrt(3)) * coef, (0.5*R*sqrt(3)) * coef, 1);
		delete[] OneHexDraw;
		delete[] AllResDraw;
		make_net();
		Clip_it_and_count();
		InvalidateRect(rectDraw, 1);
		OnPaint();
		HexIndicator == true;
	}
	if (pSlider == &X_Slider || pSlider == &Y_Slider) {
		delete[] OvalDraw;
		delete[] TwoPartsDraw;
		delete[] AllResDraw;
		RotatedOval(RotAngle.GetPos(), X_Slider.GetPos(), Y_Slider.GetPos());
		Clip_it_and_count();
		InvalidateRect(rectDraw, 1);
		OnPaint();
	}

	if (pSlider == &A_slider || pSlider == &B_Slider) {
		delete[] OvalDraw;
		delete[] TwoPartsDraw;
		DrawOvalCas();
		vokrug_ovala();
		formula_square();
		InvalidateRect(rectDraw, 1);
		OnPaint();
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CKASSINNIDlg::analysis()
{
	for (int x = X_Slider.GetRangeMin(); x < X_Slider.GetRangeMax() + 1; x++) {
		for (int y = 0; y < Y_Slider.GetRangeMax() + 1; y++) {
			for (int angle = 0; angle < 180; angle++) {
				delete[] OvalDraw;
				delete[] AllResDraw;
				X_Slider.SetPos(x / 10);
				Y_Slider.SetPos(y / 10);
				RotAngle.SetPos(angle);
				RotatedOval(RotAngle.GetPos(), X_Slider.GetPos(), Y_Slider.GetPos());
				Clip_it_and_count();
			}
		}
	}
}

void CKASSINNIDlg::oval_from_file()
{
	int counter_dim = 0;
	int counter_dots = 0;
	int error = 0;

	CString fName = _T("");


	if (fName == "") {
		CFileDialog fileDialog(true);	//объект класса выбора файла
		int result = fileDialog.DoModal();	//запустить диалоговое окно
		if (result == IDOK)	//если файл выбран
		{
			fName = fileDialog.GetPathName();
		}
	}
	ifstream out(fName);
	if (out.is_open()) {
		double coords[2];
		string digits;
		string s;
		int j;
		while (!out.eof()) {
			getline(out, s);
			if (s.size() == 0) {
				break;
			}
			j = 0;
			for (int i = 0; i < 2; i++) {
				while (s[j] != ' ') {
					digits.push_back(s[j]);
					j++;
					if (j == s.size()) {
						break;
					}
				}
				coords[i] = atof(digits.c_str());
				j++;
				digits.clear();
			}
			if (counter_dots <= 5) {
				kass_from_file[0] << IntPoint(coef*coords[0], coef*coords[1]);
				counter_dots++;
			}
			else {
				error = 0;
				for (int k = counter_dots - 5; k < counter_dots; k++) {
					if ((int(coef * coords[0]) == kass_from_file[0][k].X) && (int(coef * coords[1]) == kass_from_file[0][k].Y)) {
						error = 1;
						break;
					}
				}
				if (error == 0) {
					kass_from_file[0] << IntPoint(coef*coords[0], coef*coords[1]);
					counter_dots++;
				}
			}
		}
		out.close();
	}

	OvalFileIndicator = true;

	InvalidateRect(rectDraw, 1);
	OnPaint();

	//AfxMessageBox(fileDialog.GetPathName()); // показать полный путь
}

void CKASSINNIDlg::save_coords_of_max_dots()
{
	if (a > b){
		AfxMessageBox(_T("Ошибка!\nМножество неодносвязно"), MB_ICONWARNING);
	}
	else {
		CString name_of_save_file, coords;
		double x, y;
		name_of_save_file.Format(_T("%d_%d_%.2f_%.2f.txt"), max_dots, max_angle, max_X / coef, max_Y / coef);
		ofstream wr;
		wr.open(name_of_save_file);
		if (wr.is_open()) {
			for (int i = 0; i < MaxCoords[0].size(); i++) {
				x = double(MaxCoords[0][i][0].X) / double(coef);
				y = double(MaxCoords[0][i][0].Y) / double(coef);
				coords.Format(_T("%.2f %.2f"), x, y);
				wr << CT2A(coords) << endl;
			}
		}
		wr.close();
	}
}

ClipperOffset co;
double epsilon = 0.5;

void CKASSINNIDlg::vokrug_ovala()
{
	co.Clear();
	achieved_poly[0].clear();
	area_kassini_and_achieved = 0;

	if (a > b) {
		for (int i = 0; i < TwoParts.size(); i++) {
			co.AddPath(TwoParts[i], jtRound, etClosedPolygon);
		}
	}
	else {
		co.AddPath(Rotated[0], jtRound, etClosedPolygon);
	}
	co.Execute(achieved_poly[0], epsilon * coef);

	area_kassini = abs(Area(Rotated[0])/pow(coef, 2));
	for (int i = 0; i < achieved_poly[0].size(); i++) {
		area_kassini_and_achieved += abs(Area(achieved_poly[0][i]));
	}
	area_kassini_and_achieved /= pow(coef, 2);
	area_achieved = area_kassini_and_achieved - area_kassini;

	delete[] achieved_poly_draw;
	achieved_poly_draw = new POINT*[achieved_poly[0].size()];
	for (int i = 0; i < achieved_poly[0].size(); i++) {
		achieved_poly_draw[i] = new POINT[achieved_poly[0][i].size()];
	}
	for (int i = 0; i < achieved_poly[0].size(); i++) {
		for (int j = 0; j < achieved_poly[0][i].size(); j++) {
			achieved_poly_draw[i][j].x = cx + mult * achieved_poly[0][i][j].X;
			achieved_poly_draw[i][j].y = cy - mult * achieved_poly[0][i][j].Y;
		}
	}

	for_controls.Format(_T("%.2f"), area_kassini);
	S_Kas.SetWindowTextW(for_controls);

	for_controls.Format(_T("%.2f"), area_kassini_and_achieved);
	S_obol_plus_kas.SetWindowTextW(for_controls);
	
	for_controls.Format(_T("%.2f"), area_achieved);
	S_obolochki.SetWindowTextW(for_controls);

	for_controls.Format(_T("%.2f"), double(A_slider.GetPos()) / 100);
	a_cuurent.SetWindowTextW(for_controls);

	for_controls.Format(_T("%.2f"), double(B_Slider.GetPos()) / 100);
	b_current.SetWindowTextW(for_controls);

	ReachIndicator = true;
	InvalidateRect(rectDraw, 1);
	OnPaint();
}


void CKASSINNIDlg::make_response_coords()
{
	int init = 300;
	A_slider.SetPos(300);

	CString name_of_save_file, coords;
	name_of_save_file.Format(_T("%.2f.txt"), epsilon);
	ofstream wr;
	wr.open(name_of_save_file);
	if (wr.is_open()) {
		for (int i = 0; i < 150; i++) {
			B_Slider.SetPos(300 + i);
			//////////////////////сделать для TwoParts
			delete[] OvalDraw;
			DrawOvalCas();
			vokrug_ovala();
			coords.Format(_T("%.2f %.2f"), double(B_Slider.GetPos() - A_slider.GetPos())/100, area_achieved);
			wr << CT2A(coords) << endl;
		}
	}
	wr.close();
}

double FSquare = 0;
double KassLength = 0;
double first, second;

void CKASSINNIDlg::formula_square()
{
	KassLength = 0;
	FSquare = 0;

	for (int i = 0; i < Oval[0].size() - 1; i++) {
		first = pow((Oval[0][i + 1].X - Oval[0][i].X), 2);
		second = pow((Oval[0][i + 1].Y - Oval[0][i].Y), 2);
		KassLength += sqrt(first + second);
	}
	FSquare = (KassLength/coef) * PI * pow(epsilon, 2);

	for_controls.Format(_T("%.2f"), FSquare);
	FSquare_res.SetWindowTextW(for_controls);
}
