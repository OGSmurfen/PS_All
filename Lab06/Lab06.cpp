// Lab06.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab06.h"

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
    LoadStringW(hInstance, IDC_LAB06, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB06));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB06));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB06);
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
    HMENU hMenu, hSubMenu;
    DWORD dwRes;
    BOOL bRes;
    UINT uRes;
    MENUITEMINFO miinf;
    switch (message)
    {
    case WM_RBUTTONDOWN:
    {
        hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_POPUP));
        hSubMenu = GetSubMenu(hMenu, 0);
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ClientToScreen(hWnd, &pt);
        TrackPopupMenu(hSubMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
        DestroyMenu(hMenu);
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_POPUP_MESSAGEBOX:
                MessageBox(hWnd, L"Message Box from Pop-up Menu!", L"Info", MB_OK);
                break;
            case IDM_ADD_LABEL4:
                hMenu = GetMenu(hWnd);
                hSubMenu = GetSubMenu(hMenu, 3);
                if (GetMenuState(hMenu, IDM_ADD_LABEL4 + 1, MF_BYCOMMAND) == -1)
                {
                    ZeroMemory(&miinf, sizeof(miinf));
                    miinf.cbSize = sizeof(miinf);
                    miinf.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
                    miinf.wID = IDM_ADD_LABEL4 + 1;
                    miinf.fType = MFT_STRING;
                    miinf.dwTypeData = (LPWSTR)_T("New Menu Item");
                    miinf.fState = MFS_ENABLED;
                    bRes = InsertMenuItem(hMenu, IDM_ADD_LABEL2, FALSE, &miinf);
                }
                break;
            case IDM_ADD_LABEL2:
                hMenu = GetMenu(hWnd);
                bRes = DeleteMenu(hMenu, IDM_ADD_LABEL4 + 1, MF_BYCOMMAND);
                break;
            case IDM_ADD_LABEL1:
                hMenu = GetMenu(hWnd);
                hSubMenu = GetSubMenu(hMenu, 3);
                if (GetMenuState(hMenu, IDM_ADD_LABEL4 + 1, MF_BYCOMMAND) == -1)
                    bRes = AppendMenu(hSubMenu, MF_STRING, IDM_ADD_LABEL4 + 1, L"New Menu Item");
                break;
            case IDM_ADD_LABEL4 + 1:
                MessageBox(hWnd, L"New Menu Item is selected", L"Info", MB_OK);
                break;
            case IDM_ACTIVATE_OPT:
                hMenu = GetMenu(hWnd);
                dwRes = CheckMenuItem(hMenu, IDM_OPTION, MF_CHECKED);
                break;
            case IDM_DISABLE_ACT:
                hMenu = GetMenu(hWnd);
                uRes = GetMenuState(hMenu, IDM_ACTION, MF_BYCOMMAND);
                bRes = EnableMenuItem(hMenu, IDM_ACTION,
                    uRes & MF_DISABLED ? MF_ENABLED : MF_DISABLED);
                bRes = ModifyMenu(hMenu, IDM_DISABLE_ACT, MF_BYCOMMAND,
                    IDM_DISABLE_ACT, uRes & MF_DISABLED ? L"Disable action" : L"Enable action");
                break;
            case IDM_OPTION:
                MessageBox(hWnd, L"Clicked on OPTION", L"Info", MB_OK);
                break;
            case IDM_ACTION:
                MessageBox(hWnd, L"Clicked on ACTION", L"Info", MB_OK);
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
