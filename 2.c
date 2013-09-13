
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
HWND create_edit(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
  return CreateWindowW(L"EDIT", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
}

HWND create_button(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
  return CreateWindowW(L"button", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{ 
  static HWND hwndEdit;
  switch (msg) 
  { 
  	case WM_CREATE:
  	{      
        CreateWindowW(L"STATIC", L"单词", WS_CHILD | WS_VISIBLE | SS_LEFT,
            20, 20, 100, 20,hwnd, (HMENU) 1, NULL, NULL);
        hwndEdit = create_edit(L"单词...",20, 40, 100, 20,hwnd, 5);        
                   create_button(L"查询",20, 60, 80, 25,hwnd, 1);
                   create_button(L"退出",20, 80, 80, 25,hwnd, 2);      
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
    case WM_COMMAND:

          if (LOWORD(wparam) == 1) {
            Beep(40, 50);
          }
          if (LOWORD(wparam) == 2) {
            PostQuitMessage(0);
          }
          if (HIWORD(wparam) == BN_CLICKED) {

             int len = GetWindowTextLengthW(hwndEdit) + 1;
             wchar_t text[len];
             GetWindowTextW(hwndEdit, text, len);
             SetWindowTextW(hwnd, text);          
          }
          break;
    default: 
      return DefWindowProc(hwnd, msg, wparam, lparam); 
  } 
  return 0; 
} 