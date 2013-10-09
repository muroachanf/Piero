// set path=%path%;c:\mingw\bin\
// set prompt=%
// g++.exe -mwindows 1-raymond-chen-onthefly.cpp
// ref: http://blogs.msdn.com/b/oldnewthing/archive/2005/04/29/412577.aspx
#include <windows.h>
#include <vector>
using namespace std;
HINSTANCE g_hinst(){return GetModuleHandle(NULL);}
class DialogTemplate {
public:
 LPCDLGTEMPLATE Template() 
 { 
    return (LPCDLGTEMPLATE)&v[0]; 
 }
 void AlignToDword()
 { 
    if (v.size() % 4) 
        Write(NULL, 4 - (v.size() % 4)); 
 }
 void Write(LPCVOID pvWrite, DWORD cbWrite) {
    v.insert(v.end(), cbWrite, 0);
    if (pvWrite) 
      CopyMemory(&v[v.size() - cbWrite], pvWrite, cbWrite);
 }
 template<typename T> void Write(T t) 
 {
   Write(&t, sizeof(T)); 
 }
 void WriteString(LPCWSTR psz)
 { 
    Write(psz, (lstrlenW(psz) + 1) * sizeof(WCHAR)); 
 }
 private:
    vector<BYTE> v;
};

void set_font(HWND hwnd,int height){
    HFONT hf;
    HDC hdc;
    long lfHeight;
    
    hdc = GetDC(NULL);
    lfHeight = -MulDiv(height, GetDeviceCaps(hdc, LOGPIXELSY), 40);
    ReleaseDC(NULL, hdc);    
    hf = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
    SendMessage (hwnd, WM_SETFONT, WPARAM (hf), TRUE);
}
HWND create_edit(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
    HWND hwndedit = CreateWindowW(L"EDIT", text, WS_CHILD | WS_VISIBLE | SS_LEFT |WS_BORDER|WS_TABSTOP  ,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
    set_font(hwndedit,12);
    return hwndedit;
}

HWND create_button(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
  return CreateWindowW(L"button", text, WS_CHILD | WS_VISIBLE | SS_LEFT|WS_TABSTOP ,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
}

HWND create_label(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
  HWND hwndedit = CreateWindowW(L"STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
  set_font(hwndedit,8);
  SetWindowPos(hwndedit, NULL, 0, 0, 100, 200, 0);
  return hwndedit;
}
HWND hwnd_button;
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{
 switch (wm) {
 case WM_INITDIALOG: {
  // 为什么不按我指定的位置来呢？？？很费解。
  create_label(L"准备...",20, 7, 50, 20,hwnd, 0);
  create_button(L"OK...",120, 7+30, 50, 20,hwnd, 0);
  hwnd_button = create_button(L"case...",20, 7+30+30, 50, 20,hwnd, 0);
  return TRUE;
}
 case WM_SIZE:{
  SetWindowPos(hwnd_button, NULL, 0, 0, 100, 200, 0);
 }
 case WM_COMMAND:
  // if (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL) EndDialog(hwnd, 0);
  EndDialog(hwnd, 0);
  break;
 }
 return FALSE;
}

LONG point2logical(HDC hdc,LONG point){
  // The value 72 is significant because one inch contains 72 points. 
  if (point <0 )
    return -MulDiv(point,72, GetDeviceCaps(hdc, LOGPIXELSY));
  else
    return point;
}

BOOL Dialog(HWND hwnd,LPCWSTR pszTitle,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE,DLGPROC proc)
{
 WORD c_count = 0 ;
 BOOL fSuccess = FALSE;
 HDC hdc = GetDC(NULL);
 if (hdc) {
  NONCLIENTMETRICSW ncm = { sizeof(ncm) };
  if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0)) {
   DialogTemplate tmp;

   // Write out the extended dialog template header
   tmp.Write<WORD>(1); // dialog version
   tmp.Write<WORD>(0xFFFF); // extended dialog template
   tmp.Write<DWORD>(0); // help ID
   tmp.Write<DWORD>(0); // extended style
   tmp.Write<DWORD>(STYLE);
   tmp.Write<WORD>(c_count); // number of controls
   tmp.Write<WORD>(X); // X
   tmp.Write<WORD>(Y); // Y
   tmp.Write<WORD>(W); // width
   tmp.Write<WORD>(H); // height
   tmp.WriteString(L""); // no menu
   tmp.WriteString(L""); // default dialog class
   tmp.WriteString(pszTitle); // title

   ncm.lfMessageFont.lfHeight = point2logical(hdc,ncm.lfMessageFont.lfHeight);
   
   tmp.Write<WORD>((WORD)ncm.lfMessageFont.lfHeight); // point
   tmp.Write<WORD>((WORD)ncm.lfMessageFont.lfWeight); // weight
   tmp.Write<BYTE>(ncm.lfMessageFont.lfItalic); // Italic
   tmp.Write<BYTE>(ncm.lfMessageFont.lfCharSet); // CharSet
   tmp.WriteString(ncm.lfMessageFont.lfFaceName);
 
   // add_child(&tmp,"static",pszMessage,7,7,200-14,80-7-14-7,WS_CHILD | WS_VISIBLE ,IDCANCEL);   
   // add_child(&tmp,"button",L"1",75,59,50,14,WS_CHILD | WS_VISIBLE |WS_GROUP | WS_TABSTOP | BS_DEFPUSHBUTTON,IDCANCEL);
   // add_child(&tmp,"button",L"2",75,73,50,14,WS_CHILD | WS_VISIBLE |WS_GROUP | WS_TABSTOP ,IDCANCEL);
  
   // Template is ready - go display it.
   fSuccess = DialogBoxIndirect(g_hinst(), tmp.Template(),
                                hwnd, proc) >= 0;
  }
  ReleaseDC(NULL, hdc); // fixed 11 May
 }
 return fSuccess;
}

int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{
  Dialog(NULL,L"Title",200,32,200,180,WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME,DlgProc); 
}

