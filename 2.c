
/*
set path=%path%;c:\mingw\bin\
g++.exe -mwindows 2.c l.c -o 2  -static -Wno-write-strings
*/
#include <windows.h> 
#include "l.h"


int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{   
	char *AppTitle="Dictionary"; 
	if (0==create_class(hInst,AppTitle,WindowProc))
		return 0;  
	create_win(hInst,AppTitle,nCmdShow);	
	loop(); 
} 

const int id_edit = 5;
const int id_query = 1;
const int id_quit = 2;
const int id_static = 3;
void on_click(int id,HWND hwnd){      
    if(id==id_query){
       HWND hwndEdit = GetDlgItem(hwnd,id_edit);
       int len = GetWindowTextLengthW(hwndEdit) + 1;         
       wchar_t text[len];
       GetWindowTextW(hwndEdit, text, len);
       SetWindowTextW(hwnd, text);          
    }else{
       PostQuitMessage(0);
    }
}
void on_paint(HWND hwnd){
      PAINTSTRUCT ps; 
      HDC dc; 
      RECT r; 
      GetClientRect(hwnd,&r); 
      dc=BeginPaint(hwnd,&ps); 
      DrawText(dc,"Hello",-1,&r,DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
      EndPaint(hwnd,&ps); 
}
void on_create(HWND  hwnd){
    create_label(L"word",20, 20, 100, 20,hwnd, id_static);
    create_edit(L"单词...",20, 40, 100, 20,hwnd, id_edit);        
    create_button(L"查询",20, 60, 80, 25,hwnd, id_query);
    create_button(L"退出",20, 80, 80, 25,hwnd, id_quit);      
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{ 
  switch (msg) 
  { 
  	case WM_CREATE:
  	{      
        on_create(hwnd);        
        break;
    }
    case WM_PAINT: 
    { 
      on_paint(hwnd);
      break; 
    } 
    case WM_DESTROY: 
      PostQuitMessage(0); 
      break; 
    case WM_COMMAND:
          if (HIWORD(wparam) == BN_CLICKED) {
             on_click(LOWORD(wparam),hwnd);  
          }
          break;
    default: 
      return DefWindowProc(hwnd, msg, wparam, lparam); 
  } 
  return 0; 
} 
