// Lab10.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab10.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_LAB10, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB10));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB10));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB10);
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            TCHAR szHello[] = L"Hello World!!";
            RECT rt, rt1, rt2;

            GetClientRect(hWnd, &rt);
            rt1 = rt;
            rt1.bottom = 20;
            Rectangle(hdc, rt1.left, rt1.top, rt1.right, rt1.bottom);

            DrawText(hdc, szHello, ARRAYSIZE(szHello), &rt1, DT_CENTER);

            rt2 = rt1;
            rt2.top = rt1.bottom + 1;
            rt2.bottom = rt2.top + 60;

            if (rt2.bottom > rt.bottom && rt.bottom > rt2.top)
                rt2.bottom = rt.bottom;

            Rectangle(hdc, rt2.left, rt2.top, rt2.right, rt2.bottom);

            SetTextColor(hdc, RGB(0xFF, 0x00, 0x00)); //red
            DrawText(hdc, szHello, ARRAYSIZE(szHello), &rt2, NULL);
            SetTextColor(hdc, RGB(0x00, 0xFF, 0x00)); //green
            DrawText(hdc, szHello, ARRAYSIZE(szHello), &rt2, DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);

            TCHAR szIcan[] = L"I can paint";
            SetBkColor(hdc, RGB(0xFF, 0x66, 0xFF)); // pink
            TextOut(hdc, 150, 100, szIcan, ARRAYSIZE(szIcan));
            SetBkColor(hdc, RGB(0xFF, 0xFF, 0xFF)); // restore white

            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // black
            SelectObject(hdc, hPen);

            HBRUSH hBrush1 = CreateSolidBrush(RGB(0xFF, 0xFF, 0x00));
            SelectObject(hdc, hBrush1);

            Rectangle(hdc, 134, 456, 324, 190);
            Rectangle(hdc, 450, 150, 650, 350);

            // ellipse
            HBITMAP hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
            HBRUSH hBrush2 = CreatePatternBrush(hBmp);
            SelectObject(hdc, hBrush2);
            Ellipse(hdc, 134, 456, 324, 190);

            HBRUSH hBrush3 = CreateHatchBrush(HS_CROSS, RGB(0x88, 0x88, 0x20));
            SelectObject(hdc, hBrush3);
            Ellipse(hdc, 450, 150, 650, 350);

            
            POINT pp[4];
            HBITMAP hBmp1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
            HBRUSH hBrush14 = CreatePatternBrush(hBmp1);
            SelectObject(hdc, hBrush14);
            pp[0].x = 700; pp[0].y = 200;
            pp[1].x = 900; pp[1].y = 100;
            pp[2].x = 900; pp[2].y = 300;
            pp[3].x = 700; pp[3].y = 400;
            Polygon(hdc, pp, 4);



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
