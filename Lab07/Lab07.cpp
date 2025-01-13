// Lab07.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab07.h"
#include <string>
#include <windows.h>
#include <strsafe.h>

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
INT_PTR CALLBACK DialogTemperature(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
    LoadStringW(hInstance, IDC_LAB07, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB07));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB07));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB07);
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
    HMENU hMenu;
    HMENU hSubMenu;

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
            case ID_FILE_DEGREECONVERTER:
            case ID_POPUP_DEGREECONVERTRER:
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_TEMPERATURE), hWnd, DialogTemperature);
                break;
            }
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

// Message handler for Temperature Dialog box.
INT_PTR CALLBACK DialogTemperature(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL bSigned = true, bSuccess;
    int intVal;
    WCHAR buffer[50];

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BUTTON1:
        {
            intVal = GetDlgItemInt(hDlg, IDC_EDIT1, &bSuccess, bSigned);
            SetDlgItemText(hDlg, IDC_EDIT2, L"");
            if (!bSuccess)
            {
                MessageBox(hDlg, L"Error extracting integer value", L"Error!", MB_OK);
                return (INT_PTR)TRUE;
            }

            if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
            {
                if (IsDlgButtonChecked(hDlg, IDC_RADIO5) == BST_CHECKED)
                {
                    float res = intVal * 1.8 + 32;
                    
                    HRESULT result = StringCbPrintf(buffer, sizeof(buffer), L"%.2f", res);

                    if (SUCCEEDED(result)) {
                        // Successfully formatted the string into buffer
                        SetDlgItemText(hDlg, IDC_EDIT2, buffer);  // Set the formatted string in the dialog control
                    }
                    
                }
                if (IsDlgButtonChecked(hDlg, IDC_RADIO6) == BST_CHECKED)
                {

                }
            }else
            if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
            {
                if (IsDlgButtonChecked(hDlg, IDC_RADIO4) == BST_CHECKED)
                {

                }
                if (IsDlgButtonChecked(hDlg, IDC_RADIO6) == BST_CHECKED)
                {

                }
            }else
            if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
            {
                if (IsDlgButtonChecked(hDlg, IDC_RADIO4) == BST_CHECKED)
                {

                }
                if (IsDlgButtonChecked(hDlg, IDC_RADIO5) == BST_CHECKED)
                {

                }
            }
            else
            {
                MessageBox(hDlg, L"Check some boxes!", L"Error!", MB_OK);
                return (INT_PTR)TRUE;
            }


            return (INT_PTR)TRUE;
            break;
        }
        case IDC_RADIO1:
        {
            if (IsDlgButtonChecked(hDlg, IDC_RADIO4) == BST_CHECKED) 
                CheckDlgButton(hDlg, IDC_RADIO4, BST_UNCHECKED);
            break;
        }
        case IDC_RADIO2:
        {
            if (IsDlgButtonChecked(hDlg, IDC_RADIO5) == BST_CHECKED)
                CheckDlgButton(hDlg, IDC_RADIO5, BST_UNCHECKED);
            break;
        }
        case IDC_RADIO3:
        {
            if (IsDlgButtonChecked(hDlg, IDC_RADIO6) == BST_CHECKED)
                CheckDlgButton(hDlg, IDC_RADIO6, BST_UNCHECKED);
            break;
        }
        case IDC_RADIO4:
        {
            if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
                CheckDlgButton(hDlg, IDC_RADIO1, BST_UNCHECKED);
            break;
        }
        case IDC_RADIO5:
        {
            if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
                CheckDlgButton(hDlg, IDC_RADIO2, BST_UNCHECKED);
            break;
        }
        case IDC_RADIO6:
        {
            if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
                CheckDlgButton(hDlg, IDC_RADIO3, BST_UNCHECKED);
            break;
        }
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            break;

        default:
            break;
        }
        
    }
    break;
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
