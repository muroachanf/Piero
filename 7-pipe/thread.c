#include<windows.h>
#include<stdio.h>
#include<conio.h>

#include "thread.h"

DWORD WINAPI c_thread_proc( LPVOID lpParam ) 
{
    thread * t = (thread*)lpParam;
    t->proc();
}

thread::~thread(){
    CloseHandle(hMutex);
    CloseHandle(hThread);
}
void thread::run(){
        
        hThread = CreateThread(NULL, // security attributes ( default if NULL )
                            0, // stack SIZE default if 0
                            c_thread_proc, // Start Address
                            this, // input data
                            0, // creational flag ( start if  0 )
                            &threadID); // thread ID
        printf("Other business in Main\n"); 
          }
void thread::proc(){
    printf("thread::proc\n");
}
BOOL thread::state_mutex(){
    hMutex = ::CreateMutex(NULL, TRUE, "state_mutex");
}
BOOL thread::query_state_mutex(){
    HANDLE hMutex = ::OpenMutex(SYNCHRONIZE, FALSE, "state_mutex");
    return (hMutex != NULL);
}
void thread::close_state_mutex(){
    CloseHandle(hMutex);
}
BOOL thread::should_exit(){
    HANDLE hMutex = ::OpenMutex(SYNCHRONIZE, FALSE, "MY_MUTEX_NAME");
    return (hMutex != NULL);
}
void thread::notify_exit(){
    hMutex = ::CreateMutex(NULL, TRUE, "MY_MUTEX_NAME");
}