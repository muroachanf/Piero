//g++.exe -mconsole 5.cpp -static 
// STATIC -- overlapped ,passed 
#define _WIN32_WINNT 0x0501
#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include <windows.h>
#include <string>
#include <winbase.h>
#include <iostream>
using namespace std;
#include <psapi.h>
// #pragma comment(lib,"psapi.lib")
#define BUF_SIZE 8192*100*100 // BuffSize for read file
int  x=0; 
int Dootherwork();
int  read(char *file){
  HANDLE hIFile=CreateFile((LPCSTR)file,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL); //FILE_FLAG_OVERLAPPED-To process input or output asynchronously    
  if (INVALID_HANDLE_VALUE==hIFile) 
  {
    printf("Unable to open file %s.  Error=%d\n",file, GetLastError());
    return 1;
  }
  OVERLAPPED iAIO;  
  ZeroMemory (&iAIO,sizeof(iAIO));
  DWORD dwBytesRead=0;
  DWORD dwTotalBytesRead=0;
  BOOL RF;
  BOOL OLR=0;
  DWORD *lpFileSizeHigh;
  DWORD rfs; 

  lpFileSizeHigh = new DWORD;
  LARGE_INTEGER *fsize;
  fsize =new LARGE_INTEGER;
  rfs=GetFileSizeEx (hIFile,fsize);
  LONGLONG BuffSize=0;
  HANDLE hEvent;
  hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if(hEvent)
    iAIO.hEvent = hEvent;   
  else  
    printf("\nCreate event failed with error:%d",GetLastError());  
  // 决定 read buffer size
  if (fsize->QuadPart==0)
  {
      printf("\nUnable to get the size of file. Error:%d ",GetLastError());
      return 1;
  }else { 
    printf("\nFile size is: %lld Bytes",fsize->QuadPart);
    if (fsize->QuadPart > BUF_SIZE)
      BuffSize=BUF_SIZE;    
    else
      BuffSize=fsize->QuadPart;    
  }

  wchar_t *IBuffer = (wchar_t *)HeapAlloc(GetProcessHeap(),  HEAP_ZERO_MEMORY,  BuffSize);
  while (fsize->QuadPart>dwTotalBytesRead)
  {
      SetLastError(0);      
      RF=ReadFile(hIFile,IBuffer,BuffSize, NULL,&iAIO); // pass  a pointer to an OVERLAPPED structure (iAIO)        
      if ((RF==0) && GetLastError()==ERROR_IO_PENDING) 
      {
            printf ("\nAsynch readfile started. I can do other operations now");             
            while( !GetOverlappedResult( hIFile,&iAIO,&dwBytesRead,FALSE))
            {
                if (GetLastError()==ERROR_IO_INCOMPLETE)
                {
                  printf("\nI/O pending: %d .",GetLastError());
                  Dootherwork();
                }
                else if  (GetLastError()==ERROR_HANDLE_EOF)
                { 
                  printf("\nEnd of file reached.");
                  break;
                } 
                else
                {
                  printf("GetOverlappedResult failed with error:%d",GetLastError());
                  break;
                }
            } 
        }
        else if ((RF==0)  && GetLastError()!=997 )
        {
            printf ("Error reading file :%d",GetLastError());
            return 0;
        }
        dwTotalBytesRead=dwTotalBytesRead + iAIO.InternalHigh;
        iAIO.Offset=dwTotalBytesRead;
        printf("\nReadFile operation completed for %lld bytes",dwTotalBytesRead);
    
  }
  printf("\nReadFile  completed.  %d bytes read",dwTotalBytesRead);
 ResetEvent(iAIO.hEvent);
 HeapFree(GetProcessHeap(),0,IBuffer);
 CloseHandle(hEvent);
 CloseHandle(hIFile);
}

void GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = GetLastError();
    if(errorMessageID == 0)
        return ;

    // LPSTR messageBuffer = NULL;
    char   messageBuffer[256] = {0};
    size_t size = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM ,NULL, 
        errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), messageBuffer, 0, NULL);

    printf("error mesg:",messageBuffer);
    printf("%10.10s",messageBuffer);

    //Free the buffer.
    LocalFree(messageBuffer);

    // return message;
}
int  read1(char *file){
  HANDLE hIFile=CreateFile((LPCSTR)file,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL); //FILE_FLAG_OVERLAPPED-To process input or output asynchronously    
  if (INVALID_HANDLE_VALUE==hIFile) 
  {
    printf("Unable to open file %s.  Error=%d\n",file, GetLastError());
    return 1;
  }
  OVERLAPPED iAIO;  
  ZeroMemory (&iAIO,sizeof(iAIO));
  DWORD dwBytesRead=0;
  DWORD dwTotalBytesRead=0;
  BOOL RF;
  BOOL OLR=0;
  DWORD *lpFileSizeHigh;
  DWORD rfs; 

  lpFileSizeHigh = new DWORD;
  LARGE_INTEGER *fsize;
  fsize =new LARGE_INTEGER;
  rfs=GetFileSizeEx (hIFile,fsize);
  LONGLONG BuffSize=0;
  HANDLE hEvent;
  hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if(hEvent)
    iAIO.hEvent = hEvent;   
  else  
    printf("\nCreate event failed with error:%d",GetLastError());  
  // 决定 read buffer size
  if (fsize->QuadPart==0)
  {
      printf("\nUnable to get the size of file. Error:%d ",GetLastError());
      return 1;
  }
  BuffSize= 1024*20; 
  printf("\nnew size:%d ",  BuffSize);
  wchar_t *IBuffer = (wchar_t *)HeapAlloc(GetProcessHeap(),  HEAP_ZERO_MEMORY,  BuffSize);
  // while (fsize->QuadPart>dwTotalBytesRead)
  while (1)
  {
      SetLastError(0);      
      RF=ReadFile(hIFile,IBuffer,BuffSize, NULL,&iAIO); // pass  a pointer to an OVERLAPPED structure (iAIO)        
      if ((RF==0) && GetLastError()==ERROR_IO_PENDING) 
      {
            printf ("\nAsynch readfile started. I can do other operations now");             
            while( !GetOverlappedResult( hIFile,&iAIO,&dwBytesRead,FALSE))
            {
                if (GetLastError()==ERROR_IO_INCOMPLETE)
                {
                  printf("\nI/O pending: %d .",GetLastError());
                  Dootherwork();
                }
                else if  (GetLastError()==ERROR_HANDLE_EOF)
                { 
                  printf("\nEnd of file reached.");
                  break;
                } 
                else
                {
                  printf("GetOverlappedResult failed with error:%d",GetLastError());
                  break;
                }
            } 
        }
        else if ((RF==0)  && GetLastError()!=997 )
        {
            printf ("Error reading file :%d",GetLastError());
          GetLastErrorAsString();
            return 0;
        }
        dwTotalBytesRead=dwTotalBytesRead + iAIO.InternalHigh;
        iAIO.Offset=dwTotalBytesRead;
        printf("\nReadFile operation completed for %lld bytes",dwTotalBytesRead);
        printf("\nfirst two chars:%2.2s",IBuffer);
    
  }
  printf("\nReadFile  completed.  %d bytes read",dwTotalBytesRead);
 ResetEvent(iAIO.hEvent);
 HeapFree(GetProcessHeap(),0,IBuffer);
 CloseHandle(hEvent);
 CloseHandle(hIFile);
}
int main(int argc, char* argv[])
{
  if (argc<2)
  {
        printf("Usage is: To stimulate asynch I/O");
        return 1;
  }

  read1(argv[1]);
 return 0;
}


int Dootherwork()
{

    x=x+1;
    printf("\nWe are doing other work when overlapped I/O read is in progress-%d -Sleeping for 1000 Milli second",x);
    Sleep(1000);
    return 0;
}