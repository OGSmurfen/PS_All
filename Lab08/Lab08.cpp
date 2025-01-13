// Lab08.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab08.h"
#include "commctrl.h"

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
INT_PTR CALLBACK DialogExtra(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
    LoadStringW(hInstance, IDC_LAB08, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB08));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB08));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB08);
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
            case IDM_FILE_DLGEXTRA:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_EXTRA), hWnd, DialogExtra);
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

INT_PTR CALLBACK DialogExtra(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int iSetIndex = 0, iProgress = 0, iDeltaStep = 0;
    int index;
    WCHAR szText[MAX_LOADSTRING];

    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETSTEP, (WPARAM)-1, NULL);
        SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETRANGE, NULL, MAKELPARAM(0, 10));
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, NULL, (LPARAM)L"Eleven");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, NULL, (LPARAM)L"Twenty two");
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, NULL, (LPARAM)L"Thirty three");
        return (INT_PTR)TRUE;
    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_DELTA:
            iDeltaStep = iDeltaStep != 3 ? 3 : -3;
            SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_DELTAPOS, (WPARAM)iDeltaStep,
                NULL);
            return (INT_PTR)TRUE;
        case IDC_BUTTON_DECREMENT:
            SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_STEPIT, NULL, NULL);
            return (INT_PTR)TRUE;
        case IDC_BUTTON_INCREMENT:
            iProgress = (iProgress + 1) % 11;
            SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETPOS, (WPARAM)iProgress,
                NULL);
            return (INT_PTR)TRUE;
        case IDC_BUTTON_DC:
            index = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, (WPARAM)NULL,
                (LPARAM)NULL);
            if (index != CB_ERR)
                SendDlgItemMessage(hDlg, IDC_COMBO1, CB_DELETESTRING, (WPARAM)index,
                    (LPARAM)NULL);
            return (INT_PTR)TRUE;

        case IDC_BUTTON_SC:
            if (iSetIndex == -1)
                iSetIndex = 3;
            iSetIndex--;
            SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, (WPARAM)iSetIndex,
                (LPARAM)NULL);
            return (INT_PTR)TRUE;

        case IDC_BUTTON_GC:
            index = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, (WPARAM)NULL,
                (LPARAM)NULL);
            if (index != CB_ERR)
            {
                index = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETLBTEXT,
                    (WPARAM)index, (LPARAM)szText);
                if (index != CB_ERR)
                    MessageBox(hDlg, szText, L"Selected Item is", MB_OK);
                else
                    MessageBox(hDlg, L"Error getting the selected item",
                        L"ERROR!", MB_OK | MB_ICONERROR);
            }
            return (INT_PTR)TRUE;
        case IDC_BUTTON_INSERT:
            SendDlgItemMessage(hDlg, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)1,
                (LPARAM)L"Twelve");
            return (INT_PTR)TRUE;
        case IDC_BUTTON_RESET:
            MessageBeep(MB_ICONERROR);
            SendDlgItemMessage(hDlg, IDC_COMBO1, CB_RESETCONTENT, (WPARAM)NULL, (LPARAM)NULL);
            return (INT_PTR)TRUE;
        case IDC_BUTTON_APPEND:
            index = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCOUNT, (WPARAM)NULL,
                (LPARAM)NULL);
            if (index == 0)
            {
                SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, NULL,
                    (LPARAM)L"Eleven");
                SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, NULL,
                    (LPARAM)L"Twenty two");
                SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, NULL,
                    (LPARAM)L"Thirthy three");
            }
            index = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_FINDSTRING, NULL,
                (LPARAM)L"Fourty");
            if (index == -1)
                SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, NULL,
                    (LPARAM)L"Fourty four");
            return (INT_PTR)TRUE;

      
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)TRUE;
        break;

    default:
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
