// 0 -failure 1-sucess 
HWND get_rootwindow();
int create_class(HINSTANCE hInst,char* AppTitle,WNDPROC  WindowProc);
HWND  create_win(HINSTANCE hInst,char* AppTitle,int nCmdShow,int x,int y,int w,int h);
void loop(HWND hwnd);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 
HWND create_edit(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id);
HWND create_button(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id);
HWND create_label(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id);

//

int get_badrobot(char *word,char*buffer,int size);

//
void create_link(BOOL force);
// HRESULT CreateShellLink(LPCSTR pszShortcutFile, LPSTR pszLink, LPSTR pszDesc);
// BOOL AddNewGroup(LPSTR GroupName, char szPath [MAX_PATH]) ;
// BOOL FileExists(LPCTSTR szPath);

int file_exists(TCHAR * file);
