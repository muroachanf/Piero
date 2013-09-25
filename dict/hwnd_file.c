#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <Shlwapi.h>
#include "share/common.h"

char hwnd_file[256];
BOOL hwnd_is_running(){
   return file_exists(hwnd_file);
}

HWND hwnd_get(){
  FILE *fp = fopen (hwnd_file,"r");
  char line[1024];  
  memset(line,0,sizeof(line));
  if( fp == NULL ) 
      return (HWND)(LONG_PTR)0;  
  if ( fgets(line,1024,fp) ) {
    return (HWND)(LONG_PTR)atoi(line);
  }    
  fclose(fp);
  return 0;
}
void hwnd_write(HWND hwnd){
  DeleteFile(hwnd_file);
  FILE *fp = fopen (hwnd_file,"w");
  fprintf(fp,"%d",hwnd);
  fclose(fp);
}
void hwnd_ready()
{
  memset(hwnd_file,0,sizeof(hwnd_file));
  GetModuleFileName(GetModuleHandle (NULL),hwnd_file,sizeof(hwnd_file));
  // _log(hwnd_file);
  PathRemoveFileSpec(hwnd_file);
  strcat(hwnd_file,"\\hwnd.txt");
  // _log(hwnd_file);
}
BOOL check_instance_should_exit(){
  hwnd_ready();  
  if (hwnd_is_running()){
     SetForegroundWindow(hwnd_get());
     return TRUE;
  }
  return FALSE;
}
void hwnd_clear(){
    DeleteFile(hwnd_file);  
}