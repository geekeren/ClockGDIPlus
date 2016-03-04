// ClockForGDIPlus.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ClockForGDIPlus.h"
#include <math.h>
#include <iostream>
#include <ObjIdl.h>
#include <stdio.h>
#include <gdiplus.h>  
#pragma comment(lib, "gdiplus.lib")  

#include "mmsystem.h"//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库
using namespace Gdiplus;


#define MAX_LOADSTRING 100
#define PI  3.14159265359

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
static CachedBitmap* cachedBitmap;


//int scale;
static int scale = 1;
//static int imageId = 0;
static int clickCount = 0;
static Image* image;
static Graphics* bgGraphics;
static Graphics* graphics;
static Bitmap* bmp;
struct vector
{
	double h_x, h_y, m_x, m_y, s_x, s_y;

} vector;

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void refreshTime();
void drawBackGround();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLOCKFORGDIPLUS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLOCKFORGDIPLUS));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLOCKFORGDIPLUS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP | WS_SIZEBOX,
	   ::GetSystemMetrics(SM_CXSCREEN) - 410, 10, 300, 300, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
// 窗口客户区的大小
static int s_cxClient;
static int s_cyClient;
//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	switch (message)
	{
	case WM_TIMER:
		refreshTime();

		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(0), SND_ASYNC | SND_RESOURCE | SND_ASYNC);
		
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_CREATE:{
					   char path[50];
					   sprintf_s(path, "clockbg%d.bmp", clickCount % 5);
					   // string i = "1" + "2";
					   WCHAR pathstr[100];
					   swprintf(pathstr, L"%S", path);

					   GdiplusStartupInput gdiplusStartupInput;
					   ULONG_PTR m_giplusToken;
					   Status sResult;
					   sResult = GdiplusStartup(&m_giplusToken, &gdiplusStartupInput, NULL);
					   image = image->FromFile(pathstr, true);
					   
					   //::SetMapMode(hdc, MM_ANISOTROPIC);
					   //::SetWindowExtEx(hdc, s_cxClient, -s_cyClient, NULL);
					   //::SetViewportExtEx(hdc, s_cxClient, -s_cyClient, NULL);

					   //::SetViewportOrgEx(hdc, s_cxClient / 2, s_cyClient / 2, NULL);//原点
					   //::GetClientRect(hWnd, &rt);//获取客户区大小


					   drawBackGround();


					   //触发计时器
		 ::SetTimer(hWnd, 25, 1000, NULL);
	}
		
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:
	{
					s_cxClient = LOWORD(lParam);
					s_cyClient = HIWORD(lParam);
					drawBackGround();
					return 0;
	}
	case WM_RBUTTONUP:
		if (scale==1)
		scale = 2;
		else if (scale==2)
			scale = 1 ;
		drawBackGround();
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_LBUTTONUP:
		clickCount++;
	case WM_NCHITTEST:
	{
						 UINT nHitTest;
						 nHitTest = ::DefWindowProc(hWnd, WM_NCHITTEST, wParam, lParam);
						 if (nHitTest == HTCLIENT &&
							 ::GetAsyncKeyState(MK_LBUTTON) < 0) // 如果鼠标左键按下，GetAsyncKeyState函数的返回值小于0
							 nHitTest = HTCAPTION;
						 return nHitTest;
	}
	case WM_PAINT:{
					  if (::IsIconic(hWnd))
						  break;
					  hdc = BeginPaint(hWnd, &ps);

					  

					  ::SetMapMode(hdc, MM_ANISOTROPIC);
					  ::SetWindowExtEx(hdc, s_cxClient, -s_cyClient, NULL);
					  ::SetViewportExtEx(hdc, s_cxClient, -s_cyClient, NULL);

					  ::SetViewportOrgEx(hdc, s_cxClient / 2, s_cyClient / 2, NULL);//原点



					  
					  graphics=new Graphics(hdc);
					  graphics->SetSmoothingMode(SmoothingModeAntiAlias);
					  cachedBitmap=new CachedBitmap(bmp, graphics);
					  


					//  graphics->Clear(Color(255, 255, 255));
					  graphics->DrawCachedBitmap(cachedBitmap, 0 - s_cxClient / 2 / scale, 0 - s_cyClient / 2 / scale);//添加背景

					  //画指针，时针
					  double hour_point_x, minute_point_x, second_point_x,
						  hour_point_y, minute_point_y, second_point_y,r;


					  r = s_cxClient / 2;
					
					  hour_point_y = r * 2 / 5 * vector.h_y;
					  hour_point_x = r * 2 / 5 * vector.h_x;
					  minute_point_y = r * 3 / 5 * vector.m_y;
					  minute_point_x = r * 3 / 5 * vector.m_x;
					  second_point_y = r * 4 / 5 * vector.s_y;
					  second_point_x = r * 4 / 5 * vector.s_x;

					  PointF points[4];
					  points[0].X = second_point_x;
					  points[0].Y = second_point_y;
					/*  points[3].X =hour_point_x;
					  points[3].Y = hour_point_y;*/

			
						  points[1].X =( r* (-15*vector.s_x) + 1.5*r*vector.s_y)/100;
						  points[1].Y = (r* (-15*vector.s_y) -1.5*r* vector.s_x)/100;

						  points[2].X =- 0.35*second_point_x;
						  points[2].Y =- 0.35*second_point_y;

						  points[3].X =(r* (-15*vector.s_x )- 1.5*r*vector.s_y)/100;
						  points[3].Y =( r* (-15*vector.s_y) +1.5*r* vector.s_x)/100;

					  SolidBrush brush(Color(100, 200,100));
					  graphics->FillClosedCurve(&brush, points, 4);
					 


					 // PointF points[4];
					  points[0].X = hour_point_x;
					  points[0].Y = hour_point_y;

					  points[1].X = (r* -5 * vector.h_x + 2.5*r*vector.h_y) / 100;
					  points[1].Y = (r* (-5 * vector.h_y) - 2.5*r* vector.h_x) / 100;

					  points[2].X = -0.35*hour_point_x;
					  points[2].Y = -0.35*hour_point_y;

					  points[3].X = (r* (-5 * vector.h_x) - 2.5*r*vector.h_y) / 100;
					  points[3].Y = (r* (-5 * vector.h_y) + 2.5*r* vector.h_x) / 100;

					  brush.SetColor(Color(100, 100, 200));
					  graphics->FillClosedCurve(&brush, points, 4);


					  ////设置指针的末端的坐标点
					  Pen newPen(Color(100, 100, 200), 3);
					  //graphics.DrawLine(&newPen, (int)(-0.2* hour_point_x), (int)(-0.2* hour_point_y), (int)hour_point_x, (int)hour_point_y);
					  ////graphics.DrawLine(&newPen, 0, 0, 500, 500);



					  //分针
					  points[0].X = minute_point_x;
					  points[0].Y = minute_point_y;

					  points[1].X = (r* -5 * vector.m_x + 2*r*vector.m_y) / 100;
					  points[1].Y = (r* (-5 * vector.m_y) - 2*r* vector.m_x) / 100;

					  points[2].X = -0.35*minute_point_x;
					  points[2].Y = -0.35*minute_point_y;

					  points[3].X = (r* (-5 * vector.m_x) - 2*r*vector.m_y) / 100;
					  points[3].Y = (r* (-5 * vector.m_y) + 2*r* vector.m_x) / 100;

					  brush.SetColor(Color(200, 100, 100));
					  graphics->FillClosedCurve(&brush, points, 4);



					  EndPaint(hWnd, &ps);

	}
	
	
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void refreshTime(){
	SYSTEMTIME time;
	GetLocalTime(&time);

	int year = time.wYear;
	int month = time.wMonth;
	int day = time.wDay;
	int hour = time.wHour;
	int minute = time.wMinute * 60 + time.wSecond;
	int second = time.wSecond * 1000 + time.wMilliseconds;

	double hours = (hour % 12) + ((double)time.wMinute / 60);
	vector.h_x = cos(-PI / 2 + 2 * PI*(hours / 12));
	vector.h_y = sin(-PI / 2 + 2 * PI*(hours / 12));


	vector.m_x = cos(-PI / 2 + 2 * PI*minute / 3600);
	vector.m_y = sin(-PI / 2 + 2 * PI*minute / 3600);

	vector.s_x = cos(-PI / 2 + 2 * PI*(double)time.wSecond / 60);
	vector.s_y = sin(-PI / 2 + 2 * PI*(double)time.wSecond / 60);

	//vector.s_x = cos(-PI / 2 + 2 * PI*(double)second / 60000);
	//vector.s_y = sin(-PI / 2 + 2 * PI*(double)second / 60000);
}

void drawBackGround(){
	delete bmp; delete bgGraphics;
	bmp = new  Bitmap((int)s_cxClient, (int)s_cyClient);
	
	bgGraphics = new  Graphics(bmp);
	
	
	bgGraphics->SetSmoothingMode(SmoothingModeAntiAlias);

	//	  Pen pen(Color(255, 255, 255, 2));


	bgGraphics->DrawImage(image, 0, 0
		, (int)(s_cxClient / scale)
		, (int)(s_cyClient / scale));
	if (graphics != NULL)
	graphics->Clear(Color(255, 255, 255));
}