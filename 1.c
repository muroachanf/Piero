

// set path=%path%;c:\mingw\bin\
// g++.exe -mwindows c:\win1\hello.c -o h  -static -Wno-write-strings
#include <windows.h> 


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 
// 0 -failure 1-sucess 
int create_class(HINSTANCE hInst,char* AppTitle){
	WNDCLASS wc; 
	HWND hwnd; 
	MSG msg; 

	wc.style=CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc=WindowProc; 
	wc.cbClsExtra=0; 
	wc.cbWndExtra=0; 
	wc.hInstance=hInst; 
	wc.hIcon=LoadIcon(NULL,IDI_WINLOGO); 
	wc.hCursor=LoadCursor(NULL,IDC_ARROW); 
	wc.hbrBackground=(HBRUSH)COLOR_WINDOWFRAME; 
	wc.lpszMenuName=NULL; 
	wc.lpszClassName=AppTitle; 
	if (!RegisterClass(&wc)) 
		return 0 ;else return 1;
}
int  create_win(HINSTANCE hInst,char* AppTitle,int nCmdShow){
	HWND hwnd = CreateWindow(AppTitle,AppTitle, 
	    WS_OVERLAPPEDWINDOW, 
	    CW_USEDEFAULT,CW_USEDEFAULT,100,100, 
	    NULL,NULL,hInst,NULL); 
	ShowWindow(hwnd,nCmdShow); 
	UpdateWindow(hwnd); 
	return 1;
}
void loop(){
	  MSG msg; 
	  while (GetMessage(&msg,NULL,0,0) > 0) 
	  { 
	    TranslateMessage(&msg); 
	    DispatchMessage(&msg); 
	  } 
}
int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{   
	char *AppTitle="Win2"; 
	if (0==create_class(hInst,AppTitle))
		return 0;  
	create_win(hInst,AppTitle,nCmdShow);	
	loop(); 
} 

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{ 
  switch (msg) 
  { 
    case WM_PAINT: 
    { 
      PAINTSTRUCT ps; 
      HDC dc; 
      RECT r; 
      GetClientRect(hwnd,&r); 
      dc=BeginPaint(hwnd,&ps); 
      DrawText(dc,"Hello World",-1,&r,DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
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