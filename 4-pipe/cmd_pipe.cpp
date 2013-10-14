
#include <windows.h> 
#include <stdio.h> 
#include "cmd_pipe.h"


class context_{
public:
   virtual HANDLE get_stdout_handle()=0;
   virtual void set_stdout_handle(HANDLE a)=0;
   virtual void set_lastcmd(char * lastcmd)=0;
   virtual void on_data(char *buffer)=0;
};

class context :public context_{
  private:
    HANDLE h;
    data_notify_proc notify;
    char * lastcmd;
  public:
    context(data_notify_proc proc){
      notify = proc ;
      lastcmd = NULL;
      // notify_count = 0;
    }
    HANDLE get_stdout_handle()  {
      return h;
    }
    void set_stdout_handle(HANDLE a)  {
      h =a ;
    }
    void on_data(char *buffer) {
      if (lastcmd !=NULL){
        // printf("last command :%s\n",lastcmd);
        // printf("current buffer:%s\n",buffer);
        // 去掉 cmd 本身的回显
        char *result = strstr(buffer, lastcmd);
        int position = result - buffer;
        // printf("postion:%d\n",position);
        if (position==0)
          // + 1 ,so as to cut line carriage with cmd 
          notify(buffer+strlen(lastcmd)+1);        
        else
          notify(buffer);      
      }
      else
        notify(buffer);      
    }
    void set_lastcmd(char * l){
      lastcmd = l;
    }
};

DWORD WINAPI thread_proc(LPVOID lpvParam){
      // HANDLE hStdOutRead =(HANDLE)lpvParam;
      context_ *ctx = (context_ *)lpvParam ;
      HANDLE hStdOutRead =ctx->get_stdout_handle();
      DWORD d;
      char buffer[512];
      while(1){
        ReadFile( hStdOutRead, buffer, sizeof(buffer)-1, &d, 0);
        buffer[d]='\0';
        ctx->on_data(buffer);
        // printf("%s", buffer);
      }
}
class cmd{
  private:
    ULONG PipeSerialNumber ;
    HANDLE hStdInRead, hStdInWrite;
    HANDLE hStdOutRead, hStdOutWrite;
    HANDLE hStdErrRead, hStdErrWrite;
    PROCESS_INFORMATION ps;
    HANDLE  child_stdout_watcher_thread;
    HANDLE  child_stderr_watcher_thread;
    context_ *ctx ;
    context_ *ctx_err ;
    void destroy_cmd (){
        // Close all handles
        CloseHandle( child_stdout_watcher_thread);
        CloseHandle( child_stderr_watcher_thread);
        CloseHandle( ps.hProcess);
        CloseHandle( ps.hThread);
        CloseHandle(  hStdInRead); CloseHandle(  hStdInWrite);
        CloseHandle( hStdOutRead); CloseHandle( hStdOutWrite);
        CloseHandle( hStdErrRead); CloseHandle( hStdErrWrite);
      }  
    void set_context(context_ *ctx_){
      ctx = ctx_ ;
    }
    void set_context_err(context_ *ctx_){
      ctx_err = ctx_ ;
    }
    cmd(){
      
      // context_ *ctx = new context ();
    }
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
      // sti.hStdError = hStdOutWrite;
      // Create the process.
      char cmd[]="rtconsole.exe cmd.exe ";
      CreateProcess( 0, cmd, 0, 0, TRUE,
              // NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
              NORMAL_PRIORITY_CLASS,
              0, 0, &sti, &ps);
    }
    void rec_blocked(){
        DWORD d;
        char buffer[512];
        ReadFile( hStdOutRead, buffer, sizeof(buffer)-1, &d, 0);
        buffer[d]='\0';
        printf("%s", buffer);
    }
    void wait_rec_forever(){
        for( ; ;){
          rec_blocked();        
          // scanf("%s",buffer);
        }
    }  
    HANDLE create_watcher(HANDLE h_stdio,context_ *ctx){
          ctx->set_stdout_handle (h_stdio);
          DWORD  dwThreadId = 0; 
          HANDLE hThread = CreateThread( 
              NULL,              // no security attribute 
              0,                 // default stack size 
              ::thread_proc,    // thread proc
              (LPVOID) ctx,    // thread parameter 
              0,                 // not suspended 
              &dwThreadId);      // returns thread ID 

           if (hThread == NULL) 
           {
              printf("CreateThread failed, GLE=%d.\n", GetLastError()); 
              return NULL;
           }
           else CloseHandle(hThread); 
           return hThread;
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
    void ready_wait(){
      fork_cmd();
      child_stdout_watcher_thread = create_watcher(hStdOutRead,ctx);   
      child_stdout_watcher_thread = create_watcher(hStdErrRead,ctx_err);   
    }  
    void send(const char *s){
      DWORD d;
      WriteFile( hStdInWrite, s, strlen(s), &d, 0);
    }    
    char last_cmd[1024];
    public:
    cmd(data_notify_proc proc){
      set_context(new context(proc));  
      set_context_err(new context(proc));
      ready_wait();
    }
    
    ~cmd(){
      destroy_cmd();  
      delete ctx;
      delete ctx_err;    
    }
    void send_cmd(const char *s){
      send(s);
      strcpy(last_cmd,s);
      ctx->set_lastcmd(last_cmd);
      ctx_err->set_lastcmd(last_cmd);
      send("\n");
    }    
};



cmd* cmd_init(data_notify_proc proc){
  return new cmd(proc);
}
void cmd_run(cmd * c ,const char *buff){
  c->send_cmd(buff);
}
void cmd_free(cmd *c){
  delete c ;
}