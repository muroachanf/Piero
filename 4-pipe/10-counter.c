
/*
set path=%path%;c:\mingw\bin\
set prompt=%
g++.exe 10-counter.c
*/
#include <windows.h> 
#include <stdio.h> 

int main(int argc, char *argv[]) {
  int c=100;
  int t =0;
  if (argc >=2)
    c = atoi(argv[1]);
  for(int i= 1;i<c;i++){
    printf("%d\n",i);
    if (i % 50 ==0)  {  
      printf("------------more?----------\n");
      scanf("%d",&t);
    }
  }
}
