#include <windows.h>
#include <Shellapi.h>
/*
set path=%path%;c:\mingw\bin\
set prompt=%
g++.exe  4.c  -static -Wno-write-strings -lShlwapi -mwindows
*/
void run(LPCTSTR path)
{
	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = path;
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;
	BOOL ret = ShellExecuteEx(&ShExecInfo);	
}

void run1(LPCTSTR path)
{
	ShellExecute(0, "", path, NULL, NULL, SW_SHOW);
}
int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow) 
{
	run("..\\dict.exe");
	// run1("..\\dict.exe");
}