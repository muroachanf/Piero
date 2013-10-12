#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <assert.h>


void last_error_message(DWORD error )
{
   LPVOID lpMsgBuf;
   FormatMessage( 
       FORMAT_MESSAGE_ALLOCATE_BUFFER | 
         FORMAT_MESSAGE_FROM_SYSTEM | 
         FORMAT_MESSAGE_IGNORE_INSERTS,
       NULL,
       error,
       // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
       MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // English language
       (LPTSTR) &lpMsgBuf,
       0,
       NULL 
   );
   MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );   
   LocalFree( lpMsgBuf );
}
int main(int argc, char* argv[])
{
   OVERLAPPED overlapIn;
   HANDLE tHandle;
   char *buf1, *buf2;
   LARGE_INTEGER fileSize;
   if(1)
   {
      tHandle = CreateFile("NOT EXISTS.cab", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
      DWORD error = GetLastError();         
      if (0!=error)
         last_error_message(error);     
   }
}