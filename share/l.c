#include <windows.h> 
#include "l.h"
// LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 
// 0 -failure 1-sucess 
int create_class(HINSTANCE hInst,char* AppTitle,WNDPROC  WindowProc){
	WNDCLASS wc; 
	HWND hwnd; 
	MSG msg; 

	wc.style=CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc=WindowProc; 
	wc.cbClsExtra=0; 
	wc.cbWndExtra=0; 
	wc.hInstance=hInst; 
	wc.hIcon=LoadIcon(NULL,IDI_WINLOGO); 
	wc.hCursor=LoadCursor(NULL,IDC_ARROW); 
	wc.hbrBackground=(HBRUSH)COLOR_WINDOWFRAME; 
	wc.lpszMenuName=NULL; 
	wc.lpszClassName=AppTitle; 
	if (!RegisterClass(&wc)) 
		return 0 ;else return 1;
}
int  create_win(HINSTANCE hInst,char* AppTitle,int nCmdShow){
	HWND hwnd = CreateWindow(AppTitle,AppTitle, 
	    WS_OVERLAPPEDWINDOW, 
	    CW_USEDEFAULT,CW_USEDEFAULT,400,200, 
	    NULL,NULL,hInst,NULL); 
	// ::SendMessage(hwnd, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

	ShowWindow(hwnd,nCmdShow); 
	UpdateWindow(hwnd); 
	return 1;
}
void loop(){
	  MSG msg; 
	  while (GetMessage(&msg,NULL,0,0) > 0) 
	  { 
	    TranslateMessage(&msg); 
	    DispatchMessage(&msg); 
	  } 
}
void set_font(HWND hwnd){
	HFONT hf;
    HDC hdc;
    long lfHeight;
    
    hdc = GetDC(NULL);
    lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 40);
    ReleaseDC(NULL, hdc);

    hf = CreateFont(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
    SendMessage (hwnd, WM_SETFONT, WPARAM (hf), TRUE);
}
HWND create_edit(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
    HWND hwndedit = CreateWindowW(L"EDIT", text, WS_CHILD | WS_VISIBLE | SS_LEFT |WS_BORDER ,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
    set_font(hwndedit);
    return hwndedit;
}

HWND create_button(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
  return CreateWindowW(L"button", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
}

HWND create_label(LPCWSTR text,int x,int y,int w,int h,HWND hwnd,int id){
 	return CreateWindowW(L"STATIC", text, WS_CHILD | WS_VISIBLE | SS_LEFT,
            x,y,w,h,hwnd, (HMENU) id, NULL, NULL);
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
   HTTPRequestPage(s,page,server);
   // req(s);
   int i = recv(s,buffer,max,0);
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
void get_badrobot(char *word,char*buffer,int size){
  memset(buffer,0,sizeof(buffer));
  // char *url = "http://fanyi.youdao.com/openapi.do?keyfrom=badrobot&key=2138134139&type=data&doctype=json&version=1.1&q=you";
  char url[256];
  // strcpy(url,"http://badrobot.sinaapp.com/dict.php?word=");
  // strcat(url,word);  
  snprintf(url, sizeof url, "%s%s", "http://badrobot.sinaapp.com/dict.php?word=", word);
  int len = DownloadToBuffer(url,buffer,size);  
  printf("len:%d\n", len);       
  ignore_header(buffer);  
}