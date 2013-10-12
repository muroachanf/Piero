
/*
g++.exe 7.c
*/
#include <windows.h> 
#include <stdio.h> 


ULONG PipeSerialNumber;

BOOL
APIENTRY
MyCreatePipeEx(
    OUT LPHANDLE lpReadPipe,
    OUT LPHANDLE lpWritePipe,
    IN LPSECURITY_ATTRIBUTES lpPipeAttributes,
    IN DWORD nSize,
    DWORD dwReadMode,
    DWORD dwWriteMode
    )

/*++

Routine Description:

    The CreatePipeEx API is used to create an anonymous pipe I/O device.
    Unlike CreatePipe FILE_FLAG_OVERLAPPED may be specified for one or
    both handles.
    Two handles to the device are created.  One handle is opened for
    reading and the other is opened for writing.  These handles may be
    used in subsequent calls to ReadFile and WriteFile to transmit data
    through the pipe.

Arguments:

    lpReadPipe - Returns a handle to the read side of the pipe.  Data
        may be read from the pipe by specifying this handle value in a
        subsequent call to ReadFile.

    lpWritePipe - Returns a handle to the write side of the pipe.  Data
        may be written to the pipe by specifying this handle value in a
        subsequent call to WriteFile.

    lpPipeAttributes - An optional parameter that may be used to specify
        the attributes of the new pipe.  If the parameter is not
        specified, then the pipe is created without a security
        descriptor, and the resulting handles are not inherited on
        process creation.  Otherwise, the optional security attributes
        are used on the pipe, and the inherit handles flag effects both
        pipe handles.

    nSize - Supplies the requested buffer size for the pipe.  This is
        only a suggestion and is used by the operating system to
        calculate an appropriate buffering mechanism.  A value of zero
        indicates that the system is to choose the default buffering
        scheme.

Return Value:

    TRUE - The operation was successful.

    FALSE/NULL - The operation failed. Extended error status is available
        using GetLastError.

--*/

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

  MyCreatePipeEx(  &hStdInRead,  &hStdInWrite, &saAttr, 0,FILE_FLAG_OVERLAPPED,FILE_FLAG_OVERLAPPED);
  MyCreatePipeEx( &hStdOutRead, &hStdOutWrite, &saAttr, 0,FILE_FLAG_OVERLAPPED,FILE_FLAG_OVERLAPPED);
  MyCreatePipeEx( &hStdErrRead, &hStdErrWrite, &saAttr, 0,FILE_FLAG_OVERLAPPED,FILE_FLAG_OVERLAPPED);

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
  // Sleep(1000);
  // Read response
  for( ; ;){
    ReadFile( hStdOutRead, buffer, sizeof(buffer)-1, &d, 0);
    buffer[d]='\0';
    printf("%s", buffer);
    // scanf("%s",buffer);
  }
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
  char *s = "dir c:\\ \n";
  echo(s);
}