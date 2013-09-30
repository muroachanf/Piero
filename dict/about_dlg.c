#include <windows.h>
#include "burning_ctl.h"

LRESULT CALLBACK DialogProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch(msg)  
  {
    case WM_CREATE:
      CreateWindow(TEXT("static"), TEXT("say bye to MFC !"), WS_VISIBLE | WS_CHILD ,
        50, 20, 150, 25,        
        hwnd, (HMENU) 1, NULL, NULL); 
      SetFocus(
        CreateWindow(TEXT("button"), TEXT("Ok"), WS_VISIBLE | WS_CHILD ,
        50, 70, 100, 25,        
        hwnd, (HMENU) 1, NULL, NULL));  
      register_burning_ctl();
      create_burning_ctl(hwnd,0,100,200,25);
      break;
    case WM_COMMAND:
      DestroyWindow(hwnd);
      break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;       
  }
  return (DefWindowProc(hwnd, msg, wParam, lParam));

}

void register_dlg(HINSTANCE ghInstance,HWND hwnd) 
{
  WNDCLASSEX wc = {0};
  wc.cbSize           = sizeof(WNDCLASSEX);
  wc.lpfnWndProc      = (WNDPROC) DialogProc;
  wc.hInstance        = ghInstance;
  wc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
  wc.lpszClassName    = TEXT("DialogClass");
  RegisterClassEx(&wc);

}

void create_dlg(HINSTANCE ghInstance,HWND hwnd)
{
  CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,  TEXT("DialogClass"), TEXT("?"), 
        WS_VISIBLE | WS_SYSMENU | WS_CAPTION , 100, 100, 200, 150, 
        NULL, NULL, ghInstance,  NULL);
}