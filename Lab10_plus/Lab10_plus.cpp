// Lab10_plus.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab10_plus.h"
#include <random>
#include "strsafe.h"
#include "CommCtrl.h"

#define MAX_LOADSTRING 100
#define TIMER1 555

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void MapBorders(RECT& playerCoords, const int& squareLength);
bool FoodCollision(const RECT& playerCoords, const RECT& foodCoords);
int GetRandomInt(int min, int max);
void AutoMove(RECT& playerCoords, const int direction, HWND& hWnd);

const int topBorder = 0;
const int leftBorder = 0;
const int rightBorder = 1135;
const int bottomBorder = 530;

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
    LoadStringW(hInstance, IDC_LAB10PLUS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB10PLUS));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB10PLUS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB10PLUS);
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
    static int direction = 2; // 0-left; 1-top; 2-right; 3-bot

    static int score = 0;
    const int squareLength = 20;
    const int foodSize = 20;

    static int foodLeftTopCoords = 150;

    static RECT playerCoords = {
        foodLeftTopCoords,
        foodLeftTopCoords,
        playerCoords.left + squareLength, 
        playerCoords.top + squareLength
    };

    static RECT foodCoords = {
        300, 300, foodCoords.left + foodSize, foodCoords.top + foodSize
    };

    static int currentBrush = 0;
    const int maxBrushes = 3;

    static HBRUSH allBrushes[maxBrushes] = {
        CreateSolidBrush(RGB(0xFF, 0xFF, 0x00)), //yellow
        CreateSolidBrush(RGB(15, 252, 3)), // green
        CreateSolidBrush(RGB(255, 0, 251)) // pink
    };

    switch (message)
    {
    case WM_TIMER:
        AutoMove(playerCoords, direction, hWnd);
        return (INT_PTR)TRUE;
    case WM_RBUTTONDOWN:
    {
        HMENU menu = LoadMenuW(hInst, MAKEINTRESOURCE(IDR_MENU1));
        HMENU subMenu = GetSubMenu(menu, 0);
        POINT rClickPt = { LOWORD(lParam), HIWORD(lParam) };
        ClientToScreen(hWnd, &rClickPt);
        TrackPopupMenu(subMenu, TPM_RIGHTBUTTON, rClickPt.x, rClickPt.y, 0, hWnd, NULL);
        DestroyMenu(menu);
    }
        break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
            case 'W':
                playerCoords.top -= 20;
                playerCoords.bottom -= 20;
                InvalidateRect(hWnd, NULL, TRUE);
                direction = 1;
                break;
            case 'A':
                playerCoords.left -= 20;
                playerCoords.right -= 20;
                InvalidateRect(hWnd, NULL, TRUE);
                direction = 0;
                break;
            case 'S':
                playerCoords.top += 20;
                playerCoords.bottom += 20;
                InvalidateRect(hWnd, NULL, TRUE);
                direction = 3;
                break;
            case 'D':
                playerCoords.left += 20;
                playerCoords.right += 20;
                InvalidateRect(hWnd, NULL, TRUE);
                direction = 2;
                break;
        }
        MapBorders(playerCoords, squareLength);
        if (FoodCollision(playerCoords, foodCoords)) {
            //MessageBoxW(hWnd, L"Collision", L"C", MB_OK);
            int newFoodLeft = GetRandomInt(leftBorder, rightBorder - 10);
            int newFoodTop = GetRandomInt(topBorder, bottomBorder - 10);
            foodCoords = { newFoodLeft ,newFoodTop, newFoodLeft + 20, newFoodTop + 20 };
            score++;
        }
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_GAMESETTINGS_AUTOMOVE:
                SetTimer(hWnd, TIMER1, 50, NULL);
                break;
            case ID_GAMESETTINGS_STOPAUTOMOVE:
                KillTimer(hWnd, TIMER1);
                return (INT_PTR)TRUE;
            case ID_POPUP_CHANGECOLOR:
            {
                currentBrush++;
                if (currentBrush >= maxBrushes) {
                    currentBrush = 0;
                }
                InvalidateRect(hWnd, NULL, TRUE);
            }
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
            // star drawing ...

            TCHAR scoreText[3] = L"";
            StringCbPrintfW(scoreText, sizeof(scoreText), L"%i", score);
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            DrawTextW(hdc, scoreText, ARRAYSIZE(scoreText), &clientRect, DT_CENTER);
            
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // black
            SelectObject(hdc, pen);

            SelectObject(hdc, allBrushes[currentBrush]);

            Rectangle(hdc, playerCoords.left, playerCoords.top, playerCoords.right, playerCoords.bottom);

            HBRUSH fruitBrush = CreateSolidBrush(RGB(255, 0, 25));
            SelectObject(hdc, fruitBrush);

            Rectangle(hdc, foodCoords.left, foodCoords.top, foodCoords.right, foodCoords.bottom);


            // end drawing ...
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

void AutoMove(RECT &playerCoords, const int direction, HWND & hWnd)
{
    int moveSpeed = 10;
    // 0-left; 1-top; 2-right; 3-bot
    switch (direction)
    {
    case 0:
        playerCoords.left -= moveSpeed;
        playerCoords.right -= moveSpeed;
        break;
    case 1:
        playerCoords.top -= moveSpeed;
        playerCoords.bottom -= moveSpeed;
        break;
    case 2:
        playerCoords.left += moveSpeed;
        playerCoords.right += moveSpeed;
        break;
    case 3:
        playerCoords.top += moveSpeed;
        playerCoords.bottom += moveSpeed;
        break;
    default:
        break;
    }
    InvalidateRect(hWnd, NULL, TRUE);
}

bool FoodCollision(const RECT &playerCoords, const RECT &foodCoords)
{
    if (playerCoords.right > foodCoords.left && playerCoords.left < foodCoords.right) {
        if (playerCoords.bottom > foodCoords.top && playerCoords.top < foodCoords.bottom) {
            return true;
        }
    }

    return false;
}

void MapBorders(RECT& playerCoords, const int& squareLength)
{
    
    if (playerCoords.left < leftBorder)
    {
        playerCoords.left = leftBorder;
        playerCoords.right = playerCoords.left + squareLength;
    }
    if (playerCoords.right > rightBorder) {
        playerCoords.right = rightBorder;
        playerCoords.left = playerCoords.right - squareLength;
    }
    if (playerCoords.top < topBorder)
    {
        playerCoords.top = topBorder;
        playerCoords.bottom = playerCoords.top + squareLength;
    }
    if (playerCoords.bottom > bottomBorder)
    {
        playerCoords.bottom = bottomBorder;
        playerCoords.top = playerCoords.bottom - squareLength;
    }
}

int GetRandomInt(int min, int max)
{
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(min, max);
    int dice_roll = distribution(generator);

    return dice_roll;
}
