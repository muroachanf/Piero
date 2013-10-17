#include<windows.h>
#include<stdio.h>
#include<conio.h>

#include "thread.h"
class my_thread:public thread{
    public :
    void proc(){        
        int i ;
        while(1){            
            if (should_exit()){
                printf("%s\n", "your mom call you eating");
                break;
            }
            printf("%s\n", "ancestor proc");
            ::Sleep(1000);
        }
    }
};
int main()
{
    printf("begining\n");
    my_thread t ;
    t.run();    
    while(1){
        ::Sleep(3000);
        t.notify_exit();
        break;
    }
    printf("exiting\n");    
    getch();
    return 0;
}