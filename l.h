// 0 -failure 1-sucess 
int create_class(HINSTANCE hInst,char* AppTitle,WNDPROC  WindowProc);
int  create_win(HINSTANCE hInst,char* AppTitle,int nCmdShow);
void loop();
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 
