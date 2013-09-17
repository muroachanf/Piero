
/*
set path=%path%;c:\mingw\bin\
set prompt=%
g++.exe -mwindows 2.c share/l.c -o 2  -static -Wno-write-strings -lws2_32 -lole32 -luuid -lShlwapi -Ishare

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "libole32.a")
#pragma comment(lib, "uuid.lib")
*/
#include <windows.h> 
#include <wchar.h>
#include <stdio.h>
#include "l.h"

void log2(HWND v1, HWND v2){
  char aa[100];
  sprintf(aa,"%d,%d",v1,v2);
  SetWindowText(get_rootwindow(),aa);
}

void log3(HINSTANCE v1, HMODULE v2){
  char aa[100];
  sprintf(aa,"%d,%d",v1,v2);
  SetWindowText(get_rootwindow(),aa);
}
// HWND g_hwnd ;
int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{   
  log3(hInst,GetModuleHandle (NULL));
	char *AppTitle="Dictionary"; 
	if (0==create_class(hInst,AppTitle,WindowProc))
		return 0;  
	create_win(hInst,AppTitle,nCmdShow,CW_USEDEFAULT,CW_USEDEFAULT,400,180);	
	loop(); 
} 

const int id_edit = 5;
const int id_query = 1;
const int id_quit = 2;
const int id_static = 3;

void on_enter(){
       HWND hwnd = get_rootwindow();
       HWND hwndedit = GetDlgItem(hwnd,id_edit);       
       // log2(g_hwnd,get_rootwindow(hwndedit));
       int len = GetWindowTextLengthW(hwndedit) + 1;         
       char text[len];
       GetWindowText(hwndedit, text, len);
       char buffer[1000];
       int rlen = get_badrobot(text,buffer,sizeof(buffer));
       HWND hstatic = GetDlgItem(hwnd,id_static);
       if (rlen >0) {
          // buffer encoding is gb2312 == instance of MBCS 
          SetWindowText(hstatic, buffer);
       }else{                    
          wchar_t buffer[1000];
          if(rlen ==-1)
            wcscpy(buffer,L"要联网哦");// raw string is utf-8 ,because the source code file is utf-8 encoded file 
          else
            wcscpy(buffer,L"好刁钻的单词");
          SetWindowTextW(hstatic,buffer);
       }
      SendMessage(hwndedit, EM_SETSEL, 0, -1);
}

WNDPROC DefEditProc ;
LRESULT CALLBACK MyEditProc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam) {
  switch(message) {
  case WM_CHAR:
    if( wParam == VK_RETURN ) {        
        on_enter();
        return(0);
    }    
  default:
    break;
  }
  return( (LRESULT)CallWindowProc((WNDPROC)DefEditProc,hDlg,message,wParam,lParam) );
}
void on_click(int id,HWND hwnd){      
    if(id==id_query){
       on_enter();
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
    HWND hwndedit = create_edit(L"cat",20, 20, 350, 40,hwnd, id_edit);        
    create_label(L"准备...",20, 60, 350, 40,hwnd, id_static);
    create_button(L"查询",20, 100, 350, 25,hwnd, id_query);
    DefEditProc = (WNDPROC)GetWindowLong(hwndedit,GWL_WNDPROC);
    SetWindowLong(hwndedit,GWL_WNDPROC,(long)MyEditProc);
    SetFocus(hwndedit);
    create_link(TRUE);
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
