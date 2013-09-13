#include <windows.h> 
#include "l.h"
// LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 
// 0 -failure 1-sucess 
int create_class(HINSTANCE hInst,char* AppTitle,WNDPROC  WindowProc){
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
	    CW_USEDEFAULT,CW_USEDEFAULT,100,300, 
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