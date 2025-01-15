// Lab09.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab09.h"
#include "math.h"
#include "strsafe.h"
#include "CommCtrl.h"

#define MAX_LOADSTRING 100

#define TIMER1 5

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgKeyMouTmrFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int LenOfLine(POINT pt1, POINT pt2);

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
    LoadStringW(hInstance, IDC_LAB09, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB09));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB09));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB09);
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
    static POINT ptLMDown = {0, 0};
    WCHAR szText[MAX_LOADSTRING];
    POINT pt;
    int iLen;

    switch (message)
    {
    case WM_KEYDOWN:
        if (wParam == VK_F5)
            ShowWindow(hWnd, SW_MAXIMIZE);
        if (wParam == VK_F6)
            ShowWindow(hWnd, SW_RESTORE);
        if (wParam == VK_F7)
            ShowWindow(hWnd, SW_MINIMIZE);
        break;
    case WM_LBUTTONDOWN:
        ptLMDown = { LOWORD(lParam), HIWORD(lParam) };
        break;
    case WM_LBUTTONUP:
        pt = { LOWORD(lParam), HIWORD(lParam) };
        iLen = LenOfLine(pt, ptLMDown);
        StringCbPrintf(szText, ARRAYSIZE(szText),
            L"Line {%i,%i} - {%i,%i}.\nLength of line %i.",
            ptLMDown.x, ptLMDown.y, pt.x, pt.y, iLen);
        MessageBox(hWnd, szText, L"Info", MB_OK);
        break;
    case WM_RBUTTONUP:
        pt = { LOWORD(lParam), HIWORD(lParam) };
        iLen = LenOfLine(pt, { 100,100 });
        StringCbPrintf(szText, ARRAYSIZE(szText),
            L"Point of click {%i,%i}.\nDistance to target %i.", pt.x, pt.y, iLen);
        MessageBox(hWnd, szText, L"Info", MB_OK);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_TESTDLG:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgKeyMouTmrFunc);
                break;
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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


INT_PTR CALLBACK DlgKeyMouTmrFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT wTmp;
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_LBUTTONDOWN:
        SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETRANGE, NULL, MAKELPARAM(0, 100));
        SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETSTEP, (WPARAM)3, NULL);
        SetTimer(hDlg, TIMER1, 100, NULL);
        return (INT_PTR)TRUE;
        break;
    case WM_LBUTTONUP:
        KillTimer(hDlg, TIMER1);
        return (INT_PTR)TRUE;
    case WM_TIMER:
        SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_STEPIT, NULL, NULL);
        return (INT_PTR)TRUE;
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_EDIT1, EM_SETLIMITTEXT, (WPARAM)16, NULL);
        SendDlgItemMessage(hDlg, IDC_EDIT1, EM_SETPASSWORDCHAR, (WPARAM)'o', NULL);
        return (INT_PTR)TRUE;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            wTmp = SendDlgItemMessage(hDlg, IDC_EDIT1, EM_LINELENGTH, NULL, NULL);
            if (wTmp < 8)
                MessageBox(hDlg, L"The minimum length is 8!", L"Error", MB_OK);
            return (INT_PTR)TRUE;
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            break;

        }

    }
    }
    return (INT_PTR)FALSE;
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


int LenOfLine(POINT pt1, POINT pt2)
{
    int a = abs(pt1.x - pt2.x);
    int b = abs(pt1.y - pt2.y);
    int c = sqrt(pow(a, 2) + pow(b, 2));

    return c;
}