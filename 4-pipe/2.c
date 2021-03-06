

// BOOL WINAPI ReadFileEx(
//   _In_       HANDLE hFile,
//   _Out_opt_  LPVOID lpBuffer,
//   _In_       DWORD nNumberOfBytesToRead,
//   _Inout_    LPOVERLAPPED lpOverlapped,
//   _In_       LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
// );

// BOOL WINAPI ReadFile(
//   _In_         HANDLE hFile,
//   _Out_        LPVOID lpBuffer,
//   _In_         DWORD nNumberOfBytesToRead,
//   _Out_opt_    LPDWORD lpNumberOfBytesRead,
//   _Inout_opt_  LPOVERLAPPED lpOverlapped
// );

/*
ref:
Programmatically controlling another application - redirecting input and output
http://www.bolekvrany.cz/index.php?lang=en&what=articles&subsection=control_another_app
cd "My documents\github\piero"
set path=%path%;c:\mingw\bin\
set prompt=%

g++.exe -mconsole 2.c -static 
*/
#include <windows.h> 
#include <stdio.h> 

void echo(char *s){
  SECURITY_ATTRIBUTES saAttr;
  STARTUPINFO sti;
  PROCESS_INFORMATION ps;

  // THE HANDLES MUST BE INHERITABLE -
  // create the security descriptor
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = NULL;

  // Create three pipes STDIN, STDOUT, STDERR
  // Pass the read end of STDIN and write ends of
  // STDOUT and STDERR to the process.
  // Use the write end of STDIN to command the process
  // Use the read ends STDOUT and STDERR to get response
  // from the process.
  HANDLE hStdInRead, hStdInWrite;
  HANDLE hStdOutRead, hStdOutWrite;
  HANDLE hStdErrRead, hStdErrWrite;

  CreatePipe(  &hStdInRead,  &hStdInWrite, &saAttr, 0);
  CreatePipe( &hStdOutRead, &hStdOutWrite, &saAttr, 0);
  CreatePipe( &hStdErrRead, &hStdErrWrite, &saAttr, 0);

  // Create startup information. Must specify
  // STARTF_USESTDHANDLES and the handles
  sti.cb = sizeof(sti);
  sti.lpReserved = 0;
  sti.lpDesktop= NULL;
  sti.lpTitle = "Bla";
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

  sti.hStdInput = hStdInRead;
  sti.hStdOutput = hStdOutWrite;
  sti.hStdError = hStdErrWrite;

  // Create the process. If you want its window to be
  // visible, do not use the CREATE_NO_WINDOW attribute
  // Starts a new command interpreter.  Note that using
  // command.com dos not work for unknown reasons
  CreateProcess( 0, "cmd.exe", 0, 0, TRUE,
                 NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
                 0, 0, &sti, &ps);

  // Give some command to the interpreter
  
  char buffer[512];
  DWORD d;

  WriteFile( hStdInWrite, s, strlen(s), &d, 0);
  // Give it some time to initialize and process the command
  Sleep(1000);
  // Read response
  ReadFile( hStdOutRead, buffer, sizeof(buffer)-1, &d, 0);
  buffer[d]='\0';
  // Display the response
  MessageBox( 0, buffer, "Result", MB_OK);

  // Instruct the interpreter to quit
  s = "exit\n";
  WriteFile( hStdInWrite, s, strlen(s), &d, 0);

  // Close all handles
  CloseHandle( ps.hProcess);
  CloseHandle( ps.hThread);
  CloseHandle(  hStdInRead); CloseHandle(  hStdInWrite);
  CloseHandle( hStdOutRead); CloseHandle( hStdOutWrite);
  CloseHandle( hStdErrRead); CloseHandle( hStdErrWrite);
}
int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) {
  // char *s = "echo Blah Blah Blah\n";
  char *s = "dir c:\\windows \n";
  echo(s);
}
int WinMain1(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{
    SECURITY_ATTRIBUTES saAttr;
    STARTUPINFO sti;
    PROCESS_INFORMATION ps;

    // THE HANDLES MUST BE INHERITABLE -
    // create the security descriptor
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create three pipes STDIN, STDOUT, STDERR
    // Pass the read end of STDIN and write ends of
    // STDOUT and STDERR to the process.
    // Use the write end of STDIN to command the process
    // Use the read ends STDOUT and STDERR to get response
    // from the process.
    HANDLE hStdInRead, hStdInWrite;
    HANDLE hStdOutRead, hStdOutWrite;
    HANDLE hStdErrRead, hStdErrWrite;

    CreatePipe(  &hStdInRead,  &hStdInWrite, &saAttr, 0);
    CreatePipe( &hStdOutRead, &hStdOutWrite, &saAttr, 0);
    CreatePipe( &hStdErrRead, &hStdErrWrite, &saAttr, 0);

    // Create startup information. Must specify
    // STARTF_USESTDHANDLES and the handles
    sti.cb = sizeof(sti);
    sti.lpReserved = 0;
    sti.lpDesktop= NULL;
    sti.lpTitle = "Bla";
    sti.dwX = 0;
    sti.dwY = 0;
    sti.dwXSize = 0;
    sti.dwYSize = 0;
    // 设置一个 字符界面范围，看看more是否可以分批送来？ 哦，不行。
    sti.dwXCountChars = 80;
    sti.dwYCountChars = 20;
    sti.dwFillAttribute = 0;
    sti.dwFlags = STARTF_USESTDHANDLES|STARTF_USECOUNTCHARS;// 设置一个 字符界面范围，看看more是否可以分批送来？ 哦，不行。
    sti.wShowWindow = 0;
    sti.cbReserved2 = 0;
    sti.lpReserved2 = 0;

    sti.hStdInput = hStdInRead;
    sti.hStdOutput = hStdOutWrite;
    sti.hStdError = hStdErrWrite;

    // Create the process. If you want its window to be
    // visible, do not use the CREATE_NO_WINDOW attribute
    // Starts a new command interpreter.  Note that using
    // command.com dos not work for unknown reasons
    CreateProcess( 0, "cmd.exe ", 0, 0, TRUE,
                   NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW ,
                   0, 0, &sti, &ps);

    // Give some command to the interpreter
    char *s = "dir c:\\windows |more  \n";
    char buffer[1024*1024];
    DWORD d;

    WriteFile( hStdInWrite, s, strlen(s), &d, 0);
    // Give it some time to initialize and process the command
    Sleep(1000);
    // Read response
    COMMTIMEOUTS tTimeout; 
    tTimeout.ReadIntervalTimeout = 100; 
    tTimeout.ReadTotalTimeoutMultiplier = 0; 
    tTimeout.ReadTotalTimeoutConstant = 500; // pas de time out = 0 
    tTimeout.WriteTotalTimeoutMultiplier = 0; 
    tTimeout.WriteTotalTimeoutConstant = 0; 
    SetCommTimeouts(hStdOutRead,&tTimeout);
    for (;;) 
    { 
      printf("begin reading...\n");
      BOOL bSuccess = ReadFile( hStdOutRead, buffer, sizeof(buffer)-1, &d, 0);
      printf("reading...%d\n",d);
      if( !bSuccess) 
        break;     
      if( d == 0 ) 
        break;     
      buffer[d]='\0';
      printf("%s\n",buffer );            
    }
  
    // Display the response
    // ::MessageBox( 0, buffer, "Result", MB_OK);
    
    printf("exit ...\n");
    // Instruct the interpreter to quit
    s = "exit\n";
    WriteFile( hStdInWrite, s, strlen(s), &d, 0);
    printf("exit really...\n");
    // Close all handles
    CloseHandle( ps.hProcess);
    CloseHandle( ps.hThread);
    CloseHandle(  hStdInRead); CloseHandle(  hStdInWrite);
    CloseHandle( hStdOutRead); CloseHandle( hStdOutWrite);
    CloseHandle( hStdErrRead); CloseHandle( hStdErrWrite);
}