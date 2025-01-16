// Lab12.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab12.h"

#define MAX_LOADSTRING 100

#define PI 3.14159265

#include <math.h>

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void CreateSun(HDC h, COLORREF c);
void CreateClouds(HDC h, COLORREF c);
void CreateShip(HDC hdc, COLORREF brown, COLORREF white, COLORREF black);
void CreateSea(HDC hdc, COLORREF color, int startX, int startY, int width, int amplitude, int frequency);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB12, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB12));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB12));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB12);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
        }
        break;
    case WM_PAINT:
        {
            const COLORREF rgbRed = 0x000000FF;
            const COLORREF rgbGreen = 0x0000FF00;
            const COLORREF rgbBlue = 0x00FF0000;
            const COLORREF rgbLightBlue = RGB(176, 216, 230);
            const COLORREF rgbSeaColor = RGB(29, 124, 168);

            const COLORREF rgbBlack = 0x00000000;
            const COLORREF rgbWhite = 0x00FFFFFF;
            const COLORREF rgbYellow = RGB(255, 255, 0);
            const COLORREF rgbBrown = RGB(41, 16, 15);

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            CreateSun(hdc, rgbYellow);

            CreateClouds(hdc, rgbLightBlue);

            CreateSea(hdc, rgbSeaColor, 0, 400, 1600, 30, 01);

            CreateShip(hdc, rgbBrown, rgbWhite, rgbBlack);
           

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

void CreateSun(HDC hdc, COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, 800, 50, 900, 150);
}

void CreateClouds(HDC hdc, COLORREF color)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, color);
    HBRUSH hBrush = CreateSolidBrush(color);
    SelectObject(hdc, hBrush);
    SelectObject(hdc, hPen);

    // Cloud 1
    Ellipse(hdc, 100, 100, 200, 200);
    Ellipse(hdc, 100, 150, 250, 210);
    // Cloud 2
    Ellipse(hdc, 300, 50, 400, 150);
    Ellipse(hdc, 300, 100, 450, 160);
}

void CreateShip(HDC hdc, COLORREF brown, COLORREF white, COLORREF black)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, brown);
    HBRUSH hBrush = CreateSolidBrush(brown);
    SelectObject(hdc, hBrush);
    SelectObject(hdc, hPen);

    POINT boat[4] = {
        {500, 500}, {700, 480}, {650, 530}, {550, 530}
    };

    Polygon(hdc, boat, 4);

    HPEN hPen2 = CreatePen(PS_SOLID, 2, white);
    HBRUSH hBrush2 = CreateSolidBrush(black);
    SelectObject(hdc, hBrush2);
    SelectObject(hdc, hPen2);

    POINT sail[3] = {
        {660, 320}, {650, 485}, {550, 495}
    };

    Polygon(hdc, sail, 3);

}

void CreateSea(HDC hdc, COLORREF color, int startX, int startY, int width, int amplitude, int frequency)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, color);
    HBRUSH hBrush = CreateSolidBrush(color);
    SelectObject(hdc, hBrush);
    SelectObject(hdc, hPen);

    MoveToEx(hdc, startX, startY, NULL);

    for (int x = startX; x < startX + width; x++)
    {
        int y = startY + (int)(amplitude * sin((x - startX) * frequency * PI / 180.0));
        LineTo(hdc, x, y);
    }
    
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

// Message handler for about box.
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
