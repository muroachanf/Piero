// set path=%path%;c:\mingw\bin\
// set prompt=%
// g++.exe -mwindows -static 2-liu.cpp

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
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam);
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
  return hwndedit;
}

LONG point2logical(HDC hdc,LONG point){
  // The value 72 is significant because one inch contains 72 points. 
  if (point <0 )
    return -MulDiv(point,72, GetDeviceCaps(hdc, LOGPIXELSY));
  else
    return point;
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms644997(v=vs.85).aspx
// Immediately following each DLGITEMTEMPLATE structure is a class array that specifies the window class of the control. If the first element of this array is any value other than 0xFFFF, the system treats the array as a null-terminated Unicode string that specifies the name of a registered window class. If the first element is 0xFFFF, the array has one additional element that specifies the ordinal value of a predefined system class. The ordinal can be one of the following atom values.
// Value Meaning
// 0x0080  Button
// 0x0081  Edit
// 0x0082  Static
// 0x0083  List box
// 0x0084  Scroll bar
// 0x0085  Combo box

DWORD get_control_type(const char *name){
  // *name = 1;
  if(strcmp(name,"button") == 0)
    return 0x0080FFFF ;
  else if(strcmp(name,"button") == 0)
    return 0x0080FFFF ;
  else if(strcmp(name,"edit") == 0)
    return 0x0081FFFF ;
  else if(strcmp(name,"static") == 0)
    return 0x0082FFFF ;
  else if(strcmp(name,"listbox") == 0)
    return 0x0083FFFF ;
  else if(strcmp(name,"scrollbar") == 0)
    return 0x0084FFFF ;
  else if(strcmp(name,"combobox") == 0)
    return 0x0085FFFF ;
  else return 0x0 ;
}
void add_child(DialogTemplate *tmp,const char *c_type,const wchar_t  *c_name ,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE,DWORD ID){
   tmp->AlignToDword();
   tmp->Write<DWORD>(0); // help id
   tmp->Write<DWORD>(0); // window extended style
   tmp->Write<DWORD>(STYLE); // style
   tmp->Write<WORD>(X); // x
   tmp->Write<WORD>(Y); // y
   tmp->Write<WORD>(W); // width
   tmp->Write<WORD>(H); // height
   tmp->Write<DWORD>(ID); // control ID

   tmp->Write<DWORD>(get_control_type(c_type)); // Button
   tmp->WriteString(c_name); // text
   tmp->Write<WORD>(0); // no extra data 
}
BOOL Dialog(HWND hwnd, LPCWSTR pszMessage, LPCWSTR pszTitle,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE,WORD c_count)
{
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
                                hwnd, DlgProc) >= 0;
  }
  ReleaseDC(NULL, hdc); // fixed 11 May
 }
 return fSuccess;
}


BOOL dialog_modal(HWND hwnd,LPCWSTR pszTitle,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE)
{
 WORD c_count = 0;

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
   fSuccess = DialogBoxIndirect(g_hinst(), tmp.Template(),hwnd, DlgProc) >= 0;
  }
  ReleaseDC(NULL, hdc); // fixed 11 May
 }
 return fSuccess;
}

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

int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{
  dialog_modal(NULL,L"Title ",200,32,200,180,WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME); 
}

