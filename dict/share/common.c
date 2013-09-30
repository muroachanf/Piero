#include <windows.h> 

#include <shlobj.h>
#include <objbase.h>
#include <Shlwapi.h>
#include <objidl.h>
//
//

HWND get_rootwindow(){
  HWND w =GetActiveWindow();
  HWND last ;
  while(w !=NULL){
    last = w;
    w= GetParent(last);
  }
  return last ;
}

// LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 
// 0 -failure 1-sucess 
int create_class(HINSTANCE hInst,char* AppTitle,WNDPROC  WindowProc){
	WNDCLASS wc; 
	HWND hwnd; 
	MSG msg; 

	wc.style=CS_HREDRAW | CS_VREDRAW|WS_EX_TOOLWINDOW; 
	wc.lpfnWndProc=WindowProc; 
	wc.cbClsExtra=0; 
	wc.cbWndExtra=0; 
	wc.hInstance=hInst; 
	wc.hIcon=LoadIcon(NULL,IDI_WINLOGO); 
	wc.hCursor=LoadCursor(NULL,IDC_ARROW); 
	// wc.hbrBackground=(HBRUSH)COLOR_WINDOWFRAME; 
  wc.hbrBackground=(HBRUSH)GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName=NULL; 
	wc.lpszClassName=AppTitle; 
	if (!RegisterClass(&wc)) 
		return 0 ;else return 1;
}
HWND  create_win(HINSTANCE hInst,char* AppTitle,int nCmdShow,int x,int y,int w,int h){
  // DWORD style = WS_OVERLAPPEDWINDOW ;
  DWORD style = WS_OVERLAPPED  | WS_SYSMENU | WS_THICKFRAME | WS_EX_TOOLWINDOW ;
	// HWND hwnd = CreateWindow(AppTitle,AppTitle, style, x,y,w,h,NULL,NULL,hInst,NULL); 
  HWND hwnd = CreateWindowEx(WS_EX_TOOLWINDOW,AppTitle,AppTitle, style, x,y,w,h,NULL,NULL,hInst,NULL); 
	// ::SendMessage(hwnd, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

	ShowWindow(hwnd,nCmdShow); 
	UpdateWindow(hwnd); 
	return hwnd;
}
void loop(HWND hwnd){    
	  MSG msg; 
	  while (GetMessage(&msg,NULL,0,0) > 0) 
	  { 
      if(!IsDialogMessage(hwnd,&msg)){
  	    TranslateMessage(&msg); 
  	    DispatchMessage(&msg); 
      }
	  } 
}
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

// winsock net

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <strings.h> 

SOCKET HTTPConnectToServer(char* server){
     SOCKADDR_IN serverInfo;
     SOCKET sck; 
     WSADATA wsaData; 
     LPHOSTENT hostEntry; 
     WSAStartup(MAKEWORD(2,2),&wsaData);
     hostEntry = gethostbyname(server);
     if(!hostEntry){  
       WSACleanup();  
       return 0; 
     } 
     sck = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
     if(sck==INVALID_SOCKET){
       WSACleanup(); 
       puts("Failed to setup socket");
       getchar(); 
       return 0; 
     } 
     serverInfo.sin_family = AF_INET;
     serverInfo.sin_addr   = *((LPIN_ADDR)*hostEntry->h_addr_list); 
     serverInfo.sin_port   = htons(80); 
     int i = connect(sck,(LPSOCKADDR)&serverInfo,sizeof(struct sockaddr));
   
     if(sck==SOCKET_ERROR) return 0;
     if(i!=0) return 0;
   
     return sck;
}
// void req(SOCKET s){
//   char *message = "Accept:text/html,application/xhtml+xml,application/xml,application/json;q=0.9,*/*;q=0.8 \r\n\
// Accept-Encoding:gzip,deflate,sdch \r\n\
// Accept-Language:en-US,en;q=0.8    \r\n\
// Cache-Control:max-age=0  \r\n\
// Connection:keep-alive \r\n\
// Host:fanyi.youdao.com \r\n\
// User-Agent:Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.66 Safari/537.36\r\n\
// GET / HTTP/1.1 \r\n\
// ";
//   char *a="GET /openapi.do?keyfrom=badrobot&key=2138134139&type=data&doctype=json&version=1.1&q=scandal HTTP/1.1 \r\n";
//   send(s,message,strlen(message),0);
// }
void HTTPRequestPage(SOCKET s,char *page,char *host){
   unsigned int len;
   if(strlen(page)>strlen(host)){
    len=strlen(page);
   }else len = strlen(host);
   len = 1000;
   char message[20+len];
   if(strlen(page)<=0){
     strcpy(message,"GET / HTTP/1.1\r\n");
   }else 
     sprintf(message,"GET %s HTTP/1.1\r\n",page);
   send(s,message,strlen(message),0);
   memset(message,0,sizeof(message));
   sprintf(message,"Host: %s\r\n\r\n",host);
   send(s,message,strlen(message),0);
   // printf("%s\n",message );
}

int DownloadToBuffer(char * webpage,char * buffer,unsigned long max){
   if(webpage==NULL||buffer==NULL||max==0) return FALSE;
   
   unsigned short shift=0;
   if(strncasecmp(webpage,"http://",strlen("http://"))==0){
      shift=strlen("http://");
   }
   if(strncasecmp(webpage+shift,"www.",strlen("www."))==0){
      shift+=strlen("www.");
   }
   char cut[strlen(webpage)-shift+1];
   strcpy(cut,strdup(webpage+shift));
   
   char *server = strtok(cut,"/");
   
   char *page = strdup(webpage+shift+strlen(server));
   
   SOCKET s = HTTPConnectToServer(server);
   if (!s)
     return -1;// network error
   HTTPRequestPage(s,page,server);
   // req(s);
   int i = recv(s,buffer,max,0);
   if (i==0)
     return -2;// no word
   // printf(buffer); 
   closesocket(s);
   return i ;
   // if(i<=0) return FALSE;
   // return TRUE;
}
void ignore_header(char*buffer){
  int i = 0;
  int count = 0;
  while(buffer[i]!='\0'){
    if (buffer[i]=='\r' || buffer[i]=='\n')
        count++;
    else 
        count=0;
    // printf("%d\n", count);
    if (count==4){
      // printf(&buffer[i] );
      i++;
      break;
    }    
    i++;
  }
  strcpy(&buffer[0],&buffer[i]);
}

void cut_first(int lines,char*buffer){
  int i = 0;
  int count = 0;
  // cut first 2 line
  while(buffer[i]!='\0'){
    if (buffer[i]=='\r' || buffer[i]=='\n')
        count++;
    // printf("%d\n", count);
    if (count==lines){
      i++;
      strcpy(&buffer[0],&buffer[i]);
      break;
    }    
    i++;
  }  
}
void cut_last(int lines,char*buffer){
  int i = 0;
  int count = 0;  
  int len = strlen(buffer);
  i = len -1;
  count = 0 ;
  // cut last 3 line
  while(i>=0){
    if (buffer[i]=='\r' || buffer[i]=='\n')
        count++;
    if (count==lines){
      buffer[i] = '\0' ;
      break;
    }    
    i--;
  }
}
void special(char*buffer){  
  cut_last(6,buffer); 
  cut_first(3,buffer);
}

void buffer2file(char*buffer,char*file){
  FILE *fp = fopen(file, "wb");
  if(fp != NULL)
  {
      // fwrite(&buffer[0],strlen(buffer),1,fp);
      fwrite(buffer,strlen(buffer),1,fp);
      fclose(fp);
  }
}
int get_badrobot(char *word,char*buffer,int size){
  memset(buffer,0,size);
  // char *url = "http://fanyi.youdao.com/openapi.do?keyfrom=badrobot&key=2138134139&type=data&doctype=json&version=1.1&q=you";
  char url[256];
  // strcpy(url,"http://badrobot.sinaapp.com/dict.php?word=");
  // strcat(url,word);  
  snprintf(url, sizeof url, "%s%s", "http://badrobot.sinaapp.com/dict.php?word=", word);
  int len = DownloadToBuffer(url,buffer,size);  
  // printf("len:%d\n", len);       
  ignore_header(buffer);  
  return len;
}

// shell 

#include <shlobj.h>
#include <objbase.h>
#include <Shlwapi.h>

#pragma comment( lib, "Shlwapi.lib" )


/***********************************************
//pulled from msdn and debugged by Fallenhobit
//debugged as in, their code would not compile
//I got it too after alot of cleanup....and a few
//changes....and now...com the hard way
***********************************************/

//creates a shell link...in this example a Start Group item
HRESULT CreateShellLink(LPCSTR pszShortcutFile, LPSTR pszLink, LPSTR pszDesc)
{
    HRESULT hres;
    IShellLink* psl;

    // Get a pointer to the IShellLink interface.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
    if (SUCCEEDED(hres))
    {
       IPersistFile* ppf;

       // Query IShellLink for the IPersistFile interface for 
       // saving the shell link in persistent storage.
       hres = psl->QueryInterface(IID_IPersistFile,(void**)&ppf);
       if (SUCCEEDED(hres))
       {   
         WCHAR wsz[MAX_PATH];

         // Set the path to the shell link target.
         hres = psl->SetPath(pszShortcutFile);

         if (!SUCCEEDED(hres))
          MessageBox(NULL,"SetPath failed!","ERROR",MB_OK);

         // Set the description of the shell link.
         hres = psl->SetDescription(pszDesc);

         if (!SUCCEEDED(hres))
            MessageBox(NULL,"Set Description failed!","ERROR",MB_OK);

         // Ensure string is ANSI.
         MultiByteToWideChar(CP_ACP, 0, pszLink, -1, wsz, MAX_PATH);

         // Save the link via the IPersistFile::Save method.
         hres = ppf->Save(wsz, TRUE);
    
         // Release pointer to IPersistFile.
         ppf->Release();
       }
       // Release pointer to IShellLink.
       psl->Release();
    }
    return (hres==S_OK);
}

BOOL FileExists(LPCTSTR szPath)
{
  return PathIsDirectory(szPath);
}

// Fully qualified name of the directory being deleted, without trailing backslash

void silently_remove_directory(LPCTSTR dir) 
{
    SHFILEOPSTRUCT file_op = {
        NULL,
        FO_DELETE,
        dir,
        "",
        FOF_NOCONFIRMATION |
        FOF_NOERRORUI |
        FOF_SILENT,
        false,
        0,
        "" };
    SHFileOperation(&file_op);
}
//adds a program group to te start menu
BOOL AddNewGroup(LPSTR GroupName, char szPath [MAX_PATH],BOOL force) 
{
    LPITEMIDLIST pidlStartMenu;

    // get the pidl for the start menu
    SHGetSpecialFolderLocation( HWND_DESKTOP, CSIDL_PROGRAMS, &pidlStartMenu);

    // transform the PIDL into a path
    SHGetPathFromIDList( pidlStartMenu, szPath);
    strcat (szPath, "\\");
    strcat (szPath, GroupName);
    if (FileExists(szPath) && !force)
      return TRUE;
    else
      silently_remove_directory(szPath);
    // create the folder
    if(!CreateDirectory(szPath, NULL))
    {
    DWORD   dwLastError = ::GetLastError();
    TCHAR   lpBuffer[256] ;
    if(dwLastError != 0)    // Don't want to see a "operation done successfully" error ;-)
      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,                 // It´s a system error
               NULL,                                      // No string to be formatted needed
               dwLastError,                               // Hey Windows: Please explain this error!
               MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),  // Do it in the standard language
               lpBuffer,              // Put the message here
               255,                     // Number of bytes to store the message
               NULL);
         MessageBox( HWND_TOP, lpBuffer, NULL, MB_OK);
              return FALSE ;
    }

    // notify the shell that you made a change
    SHChangeNotify( SHCNE_MKDIR, SHCNF_FLUSH | SHCNF_PATH, szPath, 0);
}

void create_link(BOOL force){
  char szText [MAX_PATH];
  //call this because we are using COM..initializes com
  CoInitialize(NULL);
    if(!AddNewGroup("Piero", szText,force))//create the demo program group
  {
    MessageBox(NULL,"ERROR","ERROR",MB_OK);
    return;
  }
  
  strcat(szText, "\\Piero.lnk");
  if (FileExists(szText))
    return ;
  char filename[255] ;
  GetModuleFileName(0,filename,sizeof(filename));
  if (!CreateShellLink(filename, szText, ""))//create program link
  {
    MessageBox(NULL,"Error Creating Program Link","ERROR",MB_OK);
    return;
  }
  //the companion to CoInitialize()
  CoUninitialize();
}


/* file */

int file_exists(TCHAR * file)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE handle = FindFirstFile(file, &FindFileData) ;
   int found = handle != INVALID_HANDLE_VALUE;
   if(found) 
   {
       //FindClose(&handle); this will crash
       FindClose(handle);
   }
   return found;
}