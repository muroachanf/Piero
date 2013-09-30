#include <windows.h>

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, 
    WPARAM wParam, LPARAM lParam)
{
  HBRUSH hBrushYellow, hBrushRed, holdBrush;
  HPEN hPen, holdPen;
  HFONT hFont, holdFont;
  PAINTSTRUCT ps;
  RECT rect, rect2;
  
  wchar_t *cap[] = { L"75", L"150", L"225", L"300", L"375", L"450", 
      L"525", L"600", L"675"};

  HDC hdc;
  int till;
  int step, full;
  int i;
  LRESULT g_pos = 225;

  switch(msg)  
  {
    case WM_PAINT:
    
        hdc = BeginPaint(hwnd, &ps);

        GetClientRect(hwnd, &rect);

        till = (rect.right / 750.0) * g_pos;
        step = rect.right / 10.0;
        full = (rect.right / 750.0) * 700;
        
        hBrushYellow = CreateSolidBrush(RGB(255, 255, 184));
        hBrushRed = CreateSolidBrush(RGB(255, 110, 110));

        hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
        holdPen = (HPEN)SelectObject(hdc, hPen);

        hFont = CreateFontW(13, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 
                0, 0, 0, 0, L"Tahoma");

        holdFont = (HFONT)SelectObject(hdc, hFont);
        
        if(till > full) {
        
            SelectObject(hdc, hBrushYellow);
            Rectangle(hdc, 0, 0, full, 30);
            holdBrush = (HBRUSH)SelectObject(hdc, hBrushRed);
            Rectangle(hdc, full, 0, till, 30);

        } else {
        
            holdBrush = (HBRUSH)SelectObject(hdc, hBrushYellow);
            Rectangle(hdc, 0, 0, till, 30);
        }

        SelectObject(hdc, holdPen);

        for ( i = 1; i < 10; i++) {

          MoveToEx(hdc, i*step, 0, NULL);
          LineTo(hdc, i*step, 7);

          rect2.bottom = 28;
          rect2.top = 8;
          rect2.left = i*step-10;
          rect2.right = i*step+10;

          SetBkMode(hdc, TRANSPARENT) ;
          DrawTextW(hdc, cap[i-1], wcslen(cap[i-1]), &rect2, DT_CENTER);
        }

        SelectObject(hdc, holdBrush);
        DeleteObject(hBrushYellow);
        DeleteObject(hBrushRed);

        DeleteObject(hPen);

        SelectObject(hdc, holdFont);
        DeleteObject(hFont);
                    
        EndPaint(hwnd, &ps);
        break;
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void register_burning_ctl(){
  WNDCLASSW rwc = {0};
  rwc.lpszClassName = L"BurningControl";
  rwc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
  rwc.style         = CS_HREDRAW;
  rwc.lpfnWndProc   = PanelProc;
  rwc.hCursor       = LoadCursor(0, IDC_ARROW);
  RegisterClassW(&rwc);
}
HWND create_burning_ctl(HWND hwnd,int x,int y,int w,int h){
  return CreateWindowExW(WS_EX_STATICEDGE , L"BurningControl", NULL,WS_CHILD | WS_VISIBLE, x,y,w,h, hwnd, (HMENU)1, NULL, NULL);
}