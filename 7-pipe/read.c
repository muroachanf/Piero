//g++ thread_exit.c -Wno-write-strings
#include <windows.h>
#include <stdio.h>
#include "thread.h"

class my_thread:public thread{
    HANDLE read_stdout;
    public :
    my_thread(HANDLE handle){
        read_stdout = handle;
    }
     #define PROMPT '~'
    int wait_ready(){
        char buf[1024];          
        unsigned long  bread, avail;
        while(1){            
            if (should_exit()){
                printf("%s\n", "your mom call you eating");
                break;
            }
            memset(buf, 0, sizeof(buf));
            PeekNamedPipe(read_stdout, NULL, 0, NULL, &avail, NULL);
            if(avail == 0)
                continue;
            ReadFile(read_stdout, buf, 1023, &bread, NULL); 
            printf("%s", buf);
            if (buf[bread-1] == PROMPT){// run completed
                // notify state is PROMPT
                state_mutex();
            }
        }
        return 0;
    }
    void proc(){                
        wait_ready();
    }
};

class master{
    HANDLE newstdin, newstdout, read_stdout, write_stdin;  //pipe handles
    PROCESS_INFORMATION pi; //child process info
    my_thread *my;
    void ErrorMessage(char *str)
    {
        fprintf(stderr, "%s\n", str);
        exit(-1);
    }

    void create_pipe()
    {
        SECURITY_ATTRIBUTES sa;

        sa.lpSecurityDescriptor = NULL;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = true;         //allow inheritable handles

        if (!CreatePipe(&newstdin, &write_stdin, &sa, 0))   //create stdin pipe
        {
            ErrorMessage("CreatePipe");
        }

        if (!CreatePipe(&read_stdout, &newstdout, &sa, 0))  //create stdout pipe
        {
            CloseHandle(newstdin);
            CloseHandle(write_stdin);
            ErrorMessage("CreatePipe");
        }
    }

    void spawn_cmd()
    {
        STARTUPINFO si;

        GetStartupInfo(&si);      //set startupinfo for the spawned process
        si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
        si.hStdOutput = newstdout;
        si.hStdError = newstdout;     //set the new handles for the child process
        si.hStdInput = newstdin;
        char *app_spawn = "c:\\windows\\system32\\cmd.exe"; 

        //spawn the child process
        if (!CreateProcess(app_spawn, NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
                           NULL, NULL, &si, &pi))
        {
          CloseHandle(newstdin);
          CloseHandle(newstdout);
          CloseHandle(read_stdout);
          CloseHandle(write_stdin);
          ErrorMessage("CreateProcess");
        }
    }
   
    void sendcmd(char*cmd){
        unsigned long  bread;
        char cmnd[256];
        strcpy(cmnd, cmd);
        strcat(cmnd, "\n");
        WriteFile(write_stdin, cmnd, strlen(cmnd), &bread, NULL);
    }
    public:
    master(){
        create_pipe();
        spawn_cmd();                
    }
    ~master(){
        delete my;        
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        CloseHandle(newstdin);            //clean stuff up
        CloseHandle(newstdout);
        CloseHandle(read_stdout);
        CloseHandle(write_stdin);
        // printf("%s","exit destructor");
    }
    
    void notify_exit_cmd(){
        sendcmd("exit");
        // printf("%s","notify_exit_cmd");
        Sleep(1000);
    }
    int is_cmd_runnning(){    
        unsigned long exitcode = 0;   
        GetExitCodeProcess(pi.hProcess,&exitcode); 
        return  (exitcode == STILL_ACTIVE);
    }
    
    void run(){
        // sendcmd("ping 127.0.0.1");
        my = new my_thread(read_stdout);
        my->run();
        // wait prompt 
        while(1){
            Sleep(1000);
            if (my->query_state_mutex()){
                // prompt state
                char buf[255];
                gets(buf);
                sendcmd(buf);
                my->close_state_mutex();
                // printf("query_state_mutex");
                // break;
            }else{
                // char a =getch();
                // if (a=='a')
                    
            }
        }
    }    
};




int main()
{
    master m ;
    m.run();    
    printf("\nexit----\n");
    return 0;
}
