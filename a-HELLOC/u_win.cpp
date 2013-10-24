#include "logger.h"
#include <windows.h>
#include <assert.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
class win{
	WNDPROC winproc;
	HINSTANCE hinst;
	HINSTANCE pinst;
	int show;
	int x,y,w,h;
  protected:
  	virtual LRESULT on_paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)=0;
  	LRESULT on_crt(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
  		_log("base into ");
  		return 0;
  	}
  public:
  	
  	win(HINSTANCE hinsta, HINSTANCE pinsta,int showa){
  		winproc = WndProc ;
  		hinst = hinsta ;
  		show = showa;
  		pinst = pinsta;
  	}
  	void set_rect(int x_,int y_,int w_,int h_){
  		x = x_;
  		y = y_;
  		w = w_;
  		h = h_;
  	}  	
  	LRESULT CALLBACK _proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
		switch(msg) {
		case WM_CREATE:
		{
			_log("before create");
			// assert(this);
			// call 0x1234(this, 1,2);
			// var test = this[100];
			// call test(this, 1, 2)
			on_crt(hwnd,msg,wp,lp);
			_log("after create");
			break;
		}
		case WM_PAINT:
		{
			on_paint(hwnd,msg,wp,lp);
			break;					
		}
		case WM_COMMAND:
		{
			switch (wp) {
			case IDCANCEL:
				SendMessage(hwnd, WM_CLOSE, 0, 0);
				break;
			}
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);			
			break;
		}
		default :
			return DefWindowProc(hwnd, msg, wp, lp);
		}
		return 0;
  	}
  	void main(){
  		_log("win.main");
  		HWND hwnd;
  		if (pinst==NULL) 
		{
			WNDCLASS wndcls;

			wndcls.style = CS_HREDRAW | CS_VREDRAW;
			wndcls.lpfnWndProc = winproc;
			wndcls.cbClsExtra = 0;
			wndcls.cbWndExtra = 0;
			wndcls.hInstance = hinst;
			wndcls.hIcon = LoadIcon(hinst, "appicon");
			wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wndcls.lpszMenuName = "HELLOMENU";
			wndcls.lpszClassName = "HELLOWIN";

			RegisterClass(&wndcls);
		}
		_log("win.main.CreateWindow");
		hwnd = CreateWindow("HELLOWIN",		 /* class name */
			"HELLO--The C version",				 /* title */
			WS_OVERLAPPEDWINDOW,					 /* window style */
			x,y,w,h,
			// 0,0,100,100
			NULL,										 /* parent */
			NULL,										 /* menu */
			hinst,									 /* module instance */
			this);									 /* create param */
		// bind win -- hwnd 
		// _log("win.main.CreateWindow.after");
	 //    assert(this);
	 //    _log("set hwnd:%d",hwnd);
		// SetWindowLong(hwnd,GWL_USERDATA,(LONG)this);
		// SetWindowLong(hwnd,GWL_USERDATA,(LONG)1999);
		ShowWindow(hwnd, show);
		UpdateWindow(hwnd);
  	}
};
class app{
	int loop(){
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return msg.wParam;
	}
	win * w;
	public :
	app(win *a){
		w = a;
	}
	~app(){
		if (NULL!=w)
			delete w;
	}
	int PASCAL main(HINSTANCE hinst, LPSTR cmdline, int show)
	{		
		assert(w);
		_log("app.main");
		w->main();
		return loop();
	}
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{

	// _log("get hwnd:%d",hwnd);
	if (msg == WM_CREATE){
		// _log("mount this with hwnd:%d",hwnd);
		LONG this_ = (LONG) (((LPCREATESTRUCT) lp)->lpCreateParams) ;
		SetWindowLong(hwnd,GWL_USERDATA,this_);
	}
	long v = GetWindowLong(hwnd,GWL_USERDATA);
	// assert(v);
	// _log("v:%d",v);
	win *p = (win*)v;
	// assert(p);	
	return p->_proc(hwnd,msg,wp,lp);	
}


