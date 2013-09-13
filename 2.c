
/*
set path=%path%;c:\mingw\bin\
g++.exe -mwindows 2.c l.c -o 2  -static -Wno-write-strings
*/
#include <windows.h> 
#include "l.h"


int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{   
	char *AppTitle="Win2"; 
	if (0==create_class(hInst,AppTitle,WindowProc))
		return 0;  
	create_win(hInst,AppTitle,nCmdShow);	
	loop(); 
} 

static wchar_t *lyrics =  L"I know you told me I should stay away\n\
I know you said he's just a dog astray\n\
";
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{ 
  switch (msg) 
  { 
  	case WM_CREATE:
  	{      
        CreateWindowW(L"STATIC", L"lyrics", WS_CHILD | WS_VISIBLE | SS_LEFT,
            20, 20, 100, 20,hwnd, (HMENU) 1, NULL, NULL);
        CreateWindowW(L"EDIT", L"lyrics", WS_CHILD | WS_VISIBLE | SS_LEFT,
            20, 40, 100, 20,hwnd, (HMENU) 1, NULL, NULL);
        CreateWindowW(L"button", L"Beep",
          WS_VISIBLE | WS_CHILD ,
            20, 60, 80, 25,
          hwnd, (HMENU) 1, NULL, NULL);
      CreateWindowW(L"button", L"Quit",
          WS_VISIBLE | WS_CHILD ,
          20, 80, 80, 25,
          hwnd, (HMENU) 2, NULL, NULL);
        break;
    }
    case WM_PAINT: 
    { 
      PAINTSTRUCT ps; 
      HDC dc; 
      RECT r; 
      GetClientRect(hwnd,&r); 
      dc=BeginPaint(hwnd,&ps); 
      DrawText(dc,"Hello",-1,&r,DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
      EndPaint(hwnd,&ps); 
      break; 
    } 

    case WM_DESTROY: 
      PostQuitMessage(0); 
      break; 

    default: 
      return DefWindowProc(hwnd, msg, wparam, lparam); 
  } 
  return 0; 
} 