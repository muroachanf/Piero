// g++.exe -mwindows -static 3-testcase.c dialog_comom.c ../dict/share/common.c -lws2_32 -lole32 -luuid -lShlwapi

#include <windows.h>
#include "dialog_common.h"
#include "../dict/share/common.h"


INT_PTR CALLBACK DlgProc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{
 switch (wm) {
 case WM_INITDIALOG: {
  create_label(L"准备...",20, 7, 350, 20,hwnd, 0);
  create_button(L"OK...",20, 7+20, 350, 20,hwnd, 0);  
  return TRUE;
}
 case WM_COMMAND:
  // if (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL) EndDialog(hwnd, 0);
  EndDialog(hwnd, 0);
  break;
 }
 return FALSE;
}


const int id_about = 1 ;
void on_create(HWND  hwnd){    
    create_button(L"?",0, 0, 50, 25,hwnd, id_about);    
}
void on_about(HWND hwnd){
  // hwnd 不传入的话，是不会模态的！
   dialog_modal(hwnd,L"Title ",200,32,200,180,WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME,DlgProc); 
}
void on_click(int id,HWND hwnd){      
    if(id==id_about){
       on_about(hwnd);
    }else{
       PostQuitMessage(0);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{ 
  // _log("msg:%d", msg);
  switch (msg) 
  { 
    case WM_CREATE:
    {      
        on_create(hwnd);        
        break;
    }
    case WM_DESTROY: 
    {
      PostQuitMessage(0);
      break; 
    }
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

int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{   
  char *title="Dictionary"; 
  if (0==create_class(hInst,title,WindowProc))
    return 0;  
  HWND hwnd = create_win(NULL,title,nCmdShow,CW_USEDEFAULT,CW_USEDEFAULT,400,180);  
  loop(hwnd); 
} 
