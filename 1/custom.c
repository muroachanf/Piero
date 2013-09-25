/* File custom.c
 * (custom control implementation)
 */


#include <stdio.h>
#include <windows.h>
#include "custom.h"
#include "logger.h"

int start_line = 0;
int PAGE_COUNT = 1;

void _rectangle(HDC hdc,RECT rect){
	Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
}
static void 
CustomPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;
	int i,y ,base_x,base_y;
	char buf[100];
	int last_count = 0 ;
    GetClientRect(hwnd, &rect);
    PAGE_COUNT = (rect.bottom - rect.top)/line_height;	
    // _log("PAGE_COUNT:%d",PAGE_COUNT);
    hdc = BeginPaint(hwnd, &ps);
	FillRect(hdc,&rect,0);
    SetTextColor(hdc, RGB(0,0,0));
	SetBkMode(hdc, TRANSPARENT);
	// DrawText(hdc, _T("Hello World!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    //FillRect(hdc, &rect,0);        
	
	for( i =0 ;i<line_cell_xcount;i++)
		for( y =start_line ;y<line_cell_ycount;y++){
			base_x = i*line_width;
			base_y = (y-start_line)*line_height;
			rect.left = base_x;
			rect.top = base_y;
			rect.right = base_x+line_width;
			rect.bottom = base_y+line_height;
			
			_rectangle(hdc,rect);			
			sprintf(buf,"%d",y);
			DrawText(hdc, _T(buf), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			if (y == line_cell_ycount -1)
				last_count++;
		}
    EndPaint(hwnd, &ps);	
	hdc = GetDC(hwnd);
    ReleaseDC(hwnd, hdc);
}

static void 
on_vscroll(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	int pos ;
	RECT rect;
	pos = GetWindowLong((HWND)lParam,GWL_ID);
	switch(LOWORD(wParam))
	{
	case SB_LINEDOWN:
		start_line++;
		if (start_line>line_cell_ycount-1)
			start_line=line_cell_ycount-1;
		break;
	case SB_LINEUP:
		start_line--;
		if (start_line<0)
			start_line = 0;
		break;
	case SB_PAGEDOWN:
		start_line+=PAGE_COUNT;
		if (start_line>line_cell_ycount-1)
			start_line=line_cell_ycount-1;
		break;
	case SB_PAGEUP:
		start_line-= PAGE_COUNT;
		if (start_line<0)
			start_line = 0;
		break;
	}
	GetClientRect(hwnd, &rect);
	_log("start_line:%d",start_line);
	SetScrollPos(hwnd,SB_VERT,start_line,TRUE);
	InvalidateRect(hwnd,&rect,TRUE);
}
static LRESULT CALLBACK
CustomProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_PAINT:
            CustomPaint(hwnd);
            return 0;
		case WM_VSCROLL:
			on_vscroll(hwnd,uMsg,wParam,lParam);
			return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void
CustomRegister(void)
{
    WNDCLASS wc = { 0 };

    wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW ;
    wc.lpfnWndProc = CustomProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CUSTOM_WC;
    RegisterClass(&wc);
}

void
CustomUnregister(void)
{
    UnregisterClass(CUSTOM_WC, NULL);
}
