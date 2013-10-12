
/*
g++.exe 8-enc.cpp -Wno-write-strings
*/
#include <windows.h> 
#include <stdio.h> 


class cmd{
  private:
    ULONG PipeSerialNumber ;
    HANDLE hStdInRead, hStdInWrite;
    HANDLE hStdOutRead, hStdOutWrite;
    HANDLE hStdErrRead, hStdErrWrite;
    PROCESS_INFORMATION ps;
  public :
  void fork_cmd(){
    // Create three pipes STDIN, STDOUT, STDERR    
    create_pipe_overlapped(  &hStdInRead,  &hStdInWrite);
    create_pipe_overlapped( &hStdOutRead, &hStdOutWrite);
    create_pipe_overlapped( &hStdErrRead, &hStdErrWrite);
    // Create startup information. Must specify STARTF_USESTDHANDLES and the handles
    STARTUPINFO sti;
    sti.cb = sizeof(sti);
    sti.lpReserved = 0;
    sti.lpDesktop= NULL;
    char title[]="Bla";
    sti.lpTitle = title;
    sti.dwX = 0;
    sti.dwY = 0;
    sti.dwXSize = 0;
    sti.dwYSize = 0;
    sti.dwXCountChars = 0;
    sti.dwYCountChars = 0;
    sti.dwFillAttribute = 0;
    sti.dwFlags = STARTF_USESTDHANDLES;
    sti.wShowWindow = 0;
    sti.cbReserved2 = 0;
    sti.lpReserved2 = 0;
    // bind stdin ,stdout
    sti.hStdInput = hStdInRead;
    sti.hStdOutput = hStdOutWrite;
    sti.hStdError = hStdErrWrite;
    // Create the process.
    CreateProcess( 0, "cmd.exe", 0, 0, TRUE,
            NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
            0, 0, &sti, &ps);
  }
  void send(const char *s){
    DWORD d;
    WriteFile( hStdInWrite, s, strlen(s), &d, 0);
  }
  void rec(){
      DWORD d;
      char buffer[512];
      ReadFile( hStdOutRead, buffer, sizeof(buffer)-1, &d, 0);
      buffer[d]='\0';
      printf("%s", buffer);
  }
  void wait_rec_forever(){
      for( ; ;){
        rec();        
        // scanf("%s",buffer);
      }
  }
  void echo(const char *s){
    fork_cmd();
    char buff[1024]="dir \n";    
    // const char*buff;
    while(1){
       scanf("%s",buff);
      // buff = s;
      send(buff);
      send("\n");
      wait_rec_forever();
      if (strcmp(buff,"exit")==0)
        break;
      break;
    }
    // send("exit\n");
    destroy_cmd();    
  }
  void destroy_cmd (){
    // Close all handles
    CloseHandle( ps.hProcess);
    CloseHandle( ps.hThread);
    CloseHandle(  hStdInRead); CloseHandle(  hStdInWrite);
    CloseHandle( hStdOutRead); CloseHandle( hStdOutWrite);
    CloseHandle( hStdErrRead); CloseHandle( hStdErrWrite);
  }
   BOOL APIENTRY create_pipe_overlapped(
      OUT LPHANDLE lpReadPipe,
      OUT LPHANDLE lpWritePipe          
      )
  {
    SECURITY_ATTRIBUTES saAttr;
    // fill start up info
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    return MyCreatePipeEx(lpReadPipe,lpWritePipe,&saAttr,0,FILE_FLAG_OVERLAPPED,FILE_FLAG_OVERLAPPED);
  }
  BOOL APIENTRY MyCreatePipeEx(
      OUT LPHANDLE lpReadPipe,
      OUT LPHANDLE lpWritePipe,
      IN LPSECURITY_ATTRIBUTES lpPipeAttributes,
      IN DWORD nSize,
      DWORD dwReadMode,
      DWORD dwWriteMode
      )
  {
      HANDLE ReadPipeHandle, WritePipeHandle;
      DWORD dwError;
      CHAR PipeNameBuffer[ MAX_PATH ];

      //
      // Only one valid OpenMode flag - FILE_FLAG_OVERLAPPED
      //

      if ((dwReadMode | dwWriteMode) & (~FILE_FLAG_OVERLAPPED)) {
          SetLastError(ERROR_INVALID_PARAMETER);
          return FALSE;
      }

      //
      //  Set the default timeout to 120 seconds
      //

      if (nSize == 0) {
          nSize = 4096;
          }

      sprintf( PipeNameBuffer,
               "\\\\.\\Pipe\\RemoteExeAnon.%08x.%08x",
               GetCurrentProcessId(),
               PipeSerialNumber++
             );

      ReadPipeHandle = CreateNamedPipeA(
                           PipeNameBuffer,
                           PIPE_ACCESS_INBOUND | dwReadMode,
                           PIPE_TYPE_BYTE | PIPE_WAIT,
                           1,             // Number of pipes
                           nSize,         // Out buffer size
                           nSize,         // In buffer size
                           120 * 1000,    // Timeout in ms
                           lpPipeAttributes
                           );

      if (! ReadPipeHandle) {
          return FALSE;
      }

      WritePipeHandle = CreateFileA(
                          PipeNameBuffer,
                          GENERIC_WRITE,
                          0,                         // No sharing
                          lpPipeAttributes,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL | dwWriteMode,
                          NULL                       // Template file
                        );

      if (INVALID_HANDLE_VALUE == WritePipeHandle) {
          dwError = GetLastError();
          CloseHandle( ReadPipeHandle );
          SetLastError(dwError);
          return FALSE;
      }

      *lpReadPipe = ReadPipeHandle;
      *lpWritePipe = WritePipeHandle;
      return( TRUE );
  }
};


  


int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) {
  // char *s = "echo Blah Blah Blah\n";
  char *s = "echo 1234 \n";
  cmd c;
  c.echo(s);
}