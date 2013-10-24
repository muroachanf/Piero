#include <windows.h>
#include "u_win.cpp"
#include <stdio.h>

class dictwin:public win{
public:
	dictwin(HINSTANCE hinsta, HINSTANCE pinsta,int showa):win(hinsta,pinsta,showa){
  		
  	}

   LRESULT on_paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{
  		PAINTSTRUCT ps;
		HDC hdc;
		RECT rect;
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, "dddHello, world.", -1, &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		return 0;
  	}
  	LRESULT on_crt(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  	{		  		
  		_log("into create");
  		create_label(L"准备...",20, 60, 350, 40,hwnd, 1);
	  	return 0;
	}
	HWND create_label(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
	 	HWND hwndedit = CreateWindowW(L"STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
	            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
	  // set_font(hwndedit,8);
	  return hwndedit;
	}
};


int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
	dictwin *w = new dictwin(hinst, pinst,show);
	// w->set_rect(CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT);
	w->set_rect(100,100,300,200);
	app a (w);
	return a.main(hinst,cmdline,show);
}
