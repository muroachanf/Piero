//g++.exe -mwindows 5-async-read.c -static 
#include "windows.h"
#include "stdlib.h"
#include <windows.h>
#include <string>
#include <winbase.h>
#include <iostream>
using namespace std;
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
#define BUF_SIZE 8192*100*100 // BuffSize for read file
int  x=0; 
int Dootherwork();

int main(int argc, char* argv[])
{
  if (argc<2)
  {
        printf("Usage is: To stimulate asynch I/O");
        return 1;
  }

HANDLE hIFile=CreateFile((LPCSTR)argv[1],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL); //FILE_FLAG_OVERLAPPED-To process input or output asynchronously
    
 if (INVALID_HANDLE_VALUE==hIFile) 
          {
            printf("Unable to open file %s.  Error=%d\n",argv[1], GetLastError());
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
      {
      iAIO.hEvent = hEvent; 
      }
      else
      {
      printf("\nCreate event failed with error:%d",GetLastError());
      }
  
    if (fsize->QuadPart==0)
      {
      printf("\nUnable to get the size of file. Error:%d ",GetLastError());
      return 1;
      }
    else 
      { 
          printf("\nFile size is: %lld Bytes",fsize->QuadPart);
          if (fsize->QuadPart > BUF_SIZE)
          {
          BuffSize=BUF_SIZE;
          }
          else
          {
          BuffSize=fsize->QuadPart;
          }
      }
    wchar_t *IBuffer = (wchar_t *)HeapAlloc(GetProcessHeap(),  HEAP_ZERO_MEMORY,  BuffSize);
  

  while (fsize->QuadPart>dwTotalBytesRead)
  {

      SetLastError(0);
      
      RF=ReadFile(hIFile,IBuffer,BuffSize, NULL,&iAIO); // pass  a pointer to an OVERLAPPED structure (iAIO)
        
      if ((RF==0) && GetLastError()==997)      //ERROR_IO_PENDING                 997L   
        {
            printf ("\nAsynch readfile started. I can do other operations now");
             
            while( !GetOverlappedResult( hIFile,&iAIO,&dwBytesRead,FALSE))
                {
                    if (GetLastError()==996)//ERROR_IO_INCOMPLETE  (Not signaled)            996L 
                    {
                    printf("\nI/O pending: %d .",GetLastError());
                    Dootherwork();
                    }
                    else if  (GetLastError()==38) //ERROR_HANDLE_EOF                 38L
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
 return 0;
}


int Dootherwork()
{

    x=x+1;
    printf("\nWe are doing other work when overlapped I/O read is in progress-%d -Sleeping for 1000 Milli second",x);
    Sleep(1000);
    return 0;
}