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

void HTTPRequestPage(SOCKET s,char *page,char *host){
   unsigned int len;
   if(strlen(page)>strlen(host)){
    len=strlen(page);
   }else len = strlen(host);
   
   char message[20+len];
   if(strlen(page)<=0){
    strcpy(message,"GET / HTTP/1.1\r\n");
   }else sprintf(message,"GET %s HTTP/1.1\r\n",page);
   send(s,message,strlen(message),0);
   
   memset(message,0,sizeof(message));
   sprintf(message,"Host: %s\r\n\r\n",host);
   send(s,message,strlen(message),0);
}

BOOL DownloadToBuffer(char * webpage,char * buffer,unsigned long max){
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
   
   int i = recv(s,buffer,max,0);
   closesocket(s);
   
   if(i<=0) return FALSE;
   return TRUE;
}

int main(int argc,char *argv[]){
  char buffer[2000];
  memset(buffer,0,sizeof(buffer));
  char *url = "http://fanyi.youdao.com/openapi.do?keyfrom=badrobot&key=2138134139&type=data&doctype=json&version=1.1&q=scandal";
  DownloadToBuffer(url,buffer,sizeof(buffer));  
  // 要打印中文，需要chcp 65001,并改字体为true type类型,比如 lucida console 
  printf(buffer);    
  return 0;
}