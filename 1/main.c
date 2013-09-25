/* File main.c
 * (application doing actually nothing but creating a main window and
 *  the custom control as its only child)
 */

#include <tchar.h>
#include <windows.h>

#include "custom.h"


static HINSTANCE hInstance;
static HWND hwndCustom;

#define CUSTOM_ID     100
#define MARGIN          7

static LRESULT CALLBACK
MainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_SIZE:
			break;
            if(wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED) {
                WORD cx = LOWORD(lParam);
                WORD cy = HIWORD(lParam);
                SetWindowPos(hwndCustom, NULL, MARGIN, MARGIN,
                             cx-2*MARGIN, cy-2*MARGIN, SWP_NOZORDER);
            }
            break;

        case WM_CREATE:
            hwndCustom = CreateWindow(CUSTOM_WC, NULL, WS_CHILD | WS_VISIBLE |WS_VSCROLL  |ES_AUTOVSCROLL,
                                  0, 0, 100, 100, hwnd, (HMENU) CUSTOM_ID, hInstance, NULL);
			SetScrollRange (hwndCustom,SB_VERT,0,line_cell_ycount,TRUE);			
            break;

        case WM_CLOSE:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY
_tWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, TCHAR* lpszCmdLine, int iCmdShow)
{
    WNDCLASS wc = { 0 };
    HWND hwnd;
    MSG msg;

    hInstance = hInst;

    CustomRegister();

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = _T("MainWindow");
    RegisterClass(&wc);

    hwnd = CreateWindow(_T("MainWindow"), _T("App Name"), WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 350, 250, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, iCmdShow);

    while(GetMessage(&msg, NULL, 0, 0)) {
        if(IsDialogMessage(hwnd, &msg))
            continue;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CustomUnregister();

    return (int)msg.wParam;
}
