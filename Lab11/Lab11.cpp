// Lab11.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab11.h"
#include "strsafe.h"
#include "CommCtrl.h"
#include <iostream>
#include <wchar.h>
#include <stdlib.h>
#include <string>

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
INT_PTR CALLBACK Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
    LoadStringW(hInstance, IDC_LAB11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB11));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB11);
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
            case ID_FILE_DIALOG:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dialog);
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

            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
            SelectObject(hdc, hPen);

            POINT hexagon[6];
            // hexagon points starting from bottom most and rotating anti-clockwise
            hexagon[0].x = 100; hexagon[0].y = 200;
            hexagon[1].x = hexagon[0].x + 50; hexagon[1].y = hexagon[0].y - 30;  
            hexagon[2].x = hexagon[0].x + 50; hexagon[2].y = hexagon[1].y - 50;
            hexagon[3].x = hexagon[0].x; hexagon[3].y = hexagon[2].y - 30;
            hexagon[4].x = hexagon[0].x - 50; hexagon[4].y = hexagon[2].y;
            hexagon[5].x = hexagon[0].x - 50; hexagon[5].y = hexagon[1].y;
            Polygon(hdc, hexagon, 6);

            POINT triangle1[3];
            triangle1[0].x = hexagon[5].x; triangle1[0].y = hexagon[5].y;
            triangle1[1].x = hexagon[0].x; triangle1[1].y = hexagon[0].y;
            triangle1[2].x = triangle1[1].x + 90; triangle1[2].y = triangle1[1].y + 80;
            Polygon(hdc, triangle1, 3);

            POINT triangle2[3];
            triangle2[0].x = hexagon[1].x; triangle2[0].y = hexagon[1].y;
            triangle2[1].x = hexagon[0].x; triangle2[1].y = hexagon[0].y;
            triangle2[2].x = triangle1[2].x; triangle2[2].y = triangle1[2].y;
            Polygon(hdc, triangle2, 3);

            POINT triangle3[3];
            triangle3[0].x = hexagon[2].x; triangle3[0].y = hexagon[2].y;
            triangle3[1].x = hexagon[1].x; triangle3[1].y = hexagon[1].y;
            triangle3[2].x = triangle1[2].x; triangle3[2].y = triangle1[2].y;
            Polygon(hdc, triangle3, 3);


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


INT_PTR CALLBACK Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    float floatVal;
    BOOL bSigned, bSuccess;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_TIMER:
    {
        if (IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED)
            SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_STEPIT, NULL, NULL);

        if (IsDlgButtonChecked(hDlg, IDC_CHECK2) == BST_CHECKED)
            SendDlgItemMessage(hDlg, IDC_PROGRESS2, PBM_STEPIT, NULL, NULL);

        return (INT_PTR)TRUE;
    }
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_CHECK1: 
            SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETPOS, 0, 0);
            return (INT_PTR)TRUE;
        case IDC_CHECK2:
            SendDlgItemMessage(hDlg, IDC_PROGRESS2, PBM_SETPOS, 0, 0);
            return (INT_PTR)TRUE;
        case IDC_BUTTON_STOP:
        {
            KillTimer(hDlg, TIMER1);
            return (INT_PTR)TRUE;
        }
        case IDC_BUTTON_START:
        {
            WCHAR szText[MAX_LOADSTRING];

            int index1 = SendDlgItemMessage(hDlg, IDC_LIST1, LB_GETCURSEL, NULL, NULL);
            int index2 = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, NULL, NULL);

            if (index1 == LB_ERR || index2 == CB_ERR) 
            {
                MessageBox(hDlg, L"Select values from both List box AND Combo box", L"ERROR", MB_OK);
                return (INT_PTR)TRUE;
            }
                

            SendDlgItemMessage(hDlg, IDC_LIST1, LB_GETTEXT, (WPARAM)index1, (LPARAM)szText);
            int num1 = _wcstoi64(szText, NULL, 10);

            SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETLBTEXT, (WPARAM)index2, (LPARAM)szText);
            int num2 = _wcstoi64(szText, NULL, 10);

            StringCbPrintf(szText, 32, L"%d; %d", num1, num2);
            MessageBox(hDlg, szText, L"Message", MB_OK);

            SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETRANGE, NULL, MAKELPARAM(0, num1));
            SendDlgItemMessage(hDlg, IDC_PROGRESS2, PBM_SETRANGE, NULL, MAKELPARAM(0, num1));

            SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETSTEP, (WPARAM)num2, NULL);
            SendDlgItemMessage(hDlg, IDC_PROGRESS2, PBM_SETSTEP, (WPARAM)num2, NULL);


            SetTimer(hDlg, TIMER1, 100, NULL);


            return (INT_PTR)TRUE;
        }
        case IDC_BUTTON2:
        {
            int index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_GETCURSEL, NULL, NULL);
            if (index != LB_ERR) 
            {
                WCHAR szText[MAX_LOADSTRING];

                index = SendDlgItemMessage(hDlg, IDC_LIST1, LB_GETTEXT, (WPARAM)index, (LPARAM)szText);

                if (index == LB_ERR)  MessageBox(hDlg, L"Error extracting from list box", L"ERROR", MB_OK);

                SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, NULL, (LPARAM)szText);

                
            }
            else
            {
                MessageBox(hDlg, L"Error extracting from list box", L"ERROR", MB_OK);
            }
            return (INT_PTR)TRUE;
        }
        case IDC_BUTTON1:
        {
            LPWSTR string = new WCHAR[32];
            UINT success = GetDlgItemText(hDlg, IDC_EDIT1, string, 32);
            if (!success) 
            {
                MessageBox(hDlg, L"Error extracting text", L"ERROR", MB_OK);
            }
            else
            {
                float num = wcstof(string, NULL);
                if (num == 0.0f && wcscmp(string, L"0") != 0) {
                    MessageBox(hDlg, L"Invalid float value", L"ERROR", MB_OK);
                }
                else { 
                    wchar_t buffer[32];
                    StringCbPrintf(buffer, 32, L"%.2f", num);
                    SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, NULL, (LPARAM)buffer);
                }
            }
        
            delete[] string;

            return (INT_PTR)TRUE;
        }
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        default:
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
