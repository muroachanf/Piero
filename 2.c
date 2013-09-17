
/*
set path=%path%;c:\mingw\bin\
set prompt=%
g++.exe -mwindows 2.c share/l.c -o 2  -static -Wno-write-strings -lws2_32 -Ishare
*/
#include <windows.h> 
#include <wchar.h>
#include "l.h"


int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{   
	char *AppTitle="Dictionary"; 
	if (0==create_class(hInst,AppTitle,WindowProc))
		return 0;  
	create_win(hInst,AppTitle,nCmdShow,CW_USEDEFAULT,CW_USEDEFAULT,400,200);	
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
       char text[len];
       GetWindowText(hwndEdit, text, len);
       char buffer[1000];
       int rlen = get_badrobot(text,buffer,sizeof(buffer));
       HWND hstatic = GetDlgItem(hwnd,id_static);
       if (rlen >0) {
         // wchar_t ws[1000];
         // swprintf (ws,L"%hs",buffer);         
         SetWindowText(hstatic, buffer);
       }else{
          if(rlen ==-1)
             SetWindowTextW(hstatic, L"要联网哦");
          else
            SetWindowTextW(hstatic, L"好刁钻的单词");
       }

    }else{
       PostQuitMessage(0);
    }
}
void on_paint(HWND hwnd){
      // PAINTSTRUCT ps; 
      // HDC dc; 
      // RECT r; 
      // GetClientRect(hwnd,&r); 
      // dc=BeginPaint(hwnd,&ps); 
      // DrawText(dc,"Hello",-1,&r,DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
      // EndPaint(hwnd,&ps); 
      UpdateWindow(hwnd);
}
void on_create(HWND  hwnd){
    create_edit(L"cat",20, 20, 350, 40,hwnd, id_edit);        
    create_label(L"---",20, 60, 350, 40,hwnd, id_static);
    create_button(L"查询",20, 100, 350, 25,hwnd, id_query);
    // create_button(L"退出",20, 80, 80, 25,hwnd, id_quit);      
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
