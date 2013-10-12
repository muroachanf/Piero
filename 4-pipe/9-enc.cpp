
/*
set path=%path%;c:\mingw\bin\
set prompt=%
g++.exe 9-enc.cpp cmd_pipe.cpp
*/
#include <windows.h> 
#include <stdio.h> 
#include "cmd_pipe.h" 


// test case simplier !

void on_data(char *buffer) {
      printf("%s", buffer);
      // delete buffer;
}

int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) {
  cmd *c = cmd_init(&on_data);
  // input dealer
  char buff[1024]="";    
  while(1){    
      gets(buff);      
      if (strcmp(buff,"quit")==0)
        break;      
     cmd_run(c,buff);
      // c->send("\n");
  }
  cmd_run(c,"exit");
  cmd_free(c);
}
