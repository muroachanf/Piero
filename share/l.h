// 0 -failure 1-sucess 
int create_class(HINSTANCE hInst,char* AppTitle,WNDPROC  WindowProc);
int  create_win(HINSTANCE hInst,char* AppTitle,int nCmdShow);
void loop();
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 
HWND create_edit(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id);
HWND create_button(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id);
HWND create_label(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id);

//

void get_badrobot(char *word,char*buffer,int size);