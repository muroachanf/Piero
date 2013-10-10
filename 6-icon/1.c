//http://www.paulgriffiths.net/program/c/srcs/winhellosrc.html
// g++.exe -mwindows  -static  1.c 1.res
#include <windows.h>
#include "resource.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/*  WinMain(), our entry point  */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		   LPSTR szCmdLine, int iCmdShow) {
    static char szAppName[] = "winhello";
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass;


    /*  Fill in WNDCLASSEX struct members  */

    wndclass.cbSize         = sizeof(wndclass);
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = hInstance;
    // wndclass.hIcon          = NULL;
    // wndclass.hIconSm         = NULL;
    // wndclass.hIcon          =  LoadIcon(hInstance, IDI_APPLICATION);
    // wndclass.hIconSm         = LoadIcon(hInstance, IDI_APPLICATION);
    // 弄个icon进去，有这么难吗
    wndclass.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(app_icon_id));
    wndclass.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(app_icon_id));
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName  = szAppName;
    wndclass.lpszMenuName   = NULL;

    
    /*  Register a new window class with Windows  */

    RegisterClassEx(&wndclass);


    /*  Create a window based on our new class  */

    hwnd = CreateWindow(szAppName, "Hello, world!",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, hInstance, NULL);


    /*  Show and update our window  */

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);


    /*  Retrieve and process messages until we get WM_QUIT  */

    while ( GetMessage(&msg, NULL, 0, 0) ) {
	TranslateMessage(&msg);    /*  for certain keyboard messages  */
	DispatchMessage(&msg);     /*  send message to WndProc        */
    } 


    /*  Exit with status specified in WM_QUIT message  */

    return msg.wParam;
}


/*  Window procedure  */

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC         hdc;
    switch ( iMsg ) {
        case WM_PAINT:
        {
                hdc = BeginPaint(hwnd, &ps);
                TextOut(hdc, 100, 100, "Hello, dfadfdafaworld!", 13);
                EndPaint(hwnd, &ps);
                return 0;
        }
        case WM_DESTROY:    	
        {
                PostQuitMessage(0);
                return 0;
        }
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
