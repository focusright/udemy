// BasicDx11App3D.cpp : Defines the entry point for the application.
//
// By Geelix School of Serious Games and Edutainment.
//

#include "stdafx.h"
#include "BasicDx11App3D.h"
#include "GxDemoApp.h"

// Global variables
HINSTANCE   g_hInst;                        // current instance
HWND        g_hWnd;                         // main window handle
TCHAR       g_szWndTitle[MAX_LOADSTRING];   // The title bar text
TCHAR       g_szWndClass[MAX_LOADSTRING];   // the main window class name

// Application object
CGxDemoApp  g_demo;

// Function declarations (included in this C++ file)
ATOM                MyRegisterClass(HINSTANCE);
bool                CreateMainWnd(int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


// Main entry point function
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialization
    g_hInst = hInstance;
    g_hWnd = NULL;
    ::LoadString(hInstance, IDS_APP_TITLE, g_szWndTitle, MAX_LOADSTRING);
    ::LoadString(hInstance, IDC_BASICDX11APP3D, g_szWndClass, MAX_LOADSTRING);
    ::MyRegisterClass(hInstance);

    // Create main window
    if (!::CreateMainWnd(nCmdShow))
        return -1;

    // Initialize demo
    if (!g_demo.Initialize(g_hWnd, g_hInst))
        return -1;

    // Main message loop
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        // Update and render
        g_demo.Update();
        g_demo.Render();
    }
    
    // Terminate demo
    g_demo.Terminate();

    return (int)msg.wParam;
}


// Registers the window class
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BASICDX11APP3D));
    wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= g_szWndClass;
    wcex.hIconSm		= ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BASICDX11APP3D));

    return ::RegisterClassEx(&wcex);
}


// Creates the main window
bool CreateMainWnd(int nCmdShow)
{
    // Calculate main window size
    RECT rc = { 0, 0, 640, 480 };
    ::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    
    // Create the main window
    g_hWnd = ::CreateWindow(g_szWndClass, g_szWndTitle, 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        rc.right - rc.left, rc.bottom - rc.top, 
        NULL, NULL, g_hInst, NULL);
    
    // Check window handle
    if (g_hWnd == NULL)
        return false;
    
    ::ShowWindow(g_hWnd, nCmdShow);
    ::UpdateWindow(g_hWnd);
    
    return true;
}


// Processes messages for the main window.
// Note: WM_KEY* and WM_MOUSE* messages are forwarded to the game here.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    
    switch (message)
    {
    case WM_PAINT:
        hdc = ::BeginPaint(hWnd, &ps);
        ::EndPaint(hWnd, &ps);
        break;

    case WM_SIZE:
        // Handled by game
        return g_demo.WndProc(message, wParam, lParam);

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
        // Handled by game
        return g_demo.WndProc(message, wParam, lParam);

    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK:
    case WM_MOUSEWHEEL:
        // Handled by game
        return g_demo.WndProc(message, wParam, lParam);

    case WM_ERASEBKGND:
        // Ignore
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;

    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    
    return 0;
}
