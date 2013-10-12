//g++.exe -mconsole 4-async-read.c -static 
//g++.exe -mwindows 4-async-read.c -static -o 4
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <assert.h>

static void WINAPI fileReadComplete(DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo);

void read_async(HANDLE h, char *buf,int len,LPOVERLAPPED_COMPLETION_ROUTINE notify_proc){

}
int main(int argc, char* argv[])
{
   OVERLAPPED overlapIn;
   HANDLE tHandle;
   char *buf1, *buf2;

   LARGE_INTEGER fileSize;

   if(0)
   {
      tHandle = CreateFile("test.cab", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

      if(tHandle == INVALID_HANDLE_VALUE)
      {
         DWORD error = GetLastError();
         assert(0);
      }

      GetFileSizeEx(tHandle,&fileSize);
      DWORD toRead = fileSize.QuadPart/2;

      // read_async(tHandle,)
      
      buf1 = new char[toRead];      
      memset(buf1,0xAB,toRead);     

      overlapIn.Offset = 0;
      overlapIn.OffsetHigh = 0;
      overlapIn.hEvent = NULL;

      //DWORD bytes=0;
      //char buffer[8000000];
      //bool read = ReadFile(tHandle,(LPVOID)buf,toRead,&bytes,NULL);
      bool read1 = ReadFileEx(tHandle, (LPVOID)buf1, toRead, &overlapIn, fileReadComplete);
      // bool read2 = ReadFileEx(tHandle, (LPVOID)buf2, toRead, &overlapIn[1], fileReadComplete);
      // if(read1 || read2)
      // {
      // }
      // else
      // {
      //    DWORD error = GetLastError();
      //    assert(0);
      //    SleepEx(INFINITE,TRUE);
      // }
      DWORD error = GetLastError();

      printf("%s","wool.");
      printf("%s",buf1);
      
      system("pause");
      delete [] buf1;
      delete [] buf2;
      CloseHandle(tHandle);
   }   
   else
   {
      int cc = 1024*1024;
      tHandle = CreateFile("test.cab",GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
      buf1 = new char[cc];
      memset(buf1,0xAB,cc);
      DWORD written=0;
      for (int i=0;i<1000;i++)
         WriteFile(tHandle,buf1,cc,&written,0);
      //WriteFile(tHandle,buf,16000000,&written,0);
      CloseHandle(tHandle);
   }
   return 0;
}

static void WINAPI fileReadComplete(DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo)
{
   dwError;
   lpo;
   printf("File Read Complete: %d bytes read.\n");
   system("pause");
}

