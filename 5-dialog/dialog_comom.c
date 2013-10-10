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


BOOL dialog_modal(HWND hwnd,LPCWSTR pszTitle,WORD X,WORD Y,WORD W,WORD H,DWORD STYLE,DLGPROC proc)
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
   fSuccess = DialogBoxIndirect(g_hinst(), tmp.Template(),hwnd, proc) >= 0;
  }
  ReleaseDC(NULL, hdc); // fixed 11 May
 }
 return fSuccess;
}

