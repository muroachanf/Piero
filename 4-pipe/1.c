#include <stdio.h>
#include <windows.h>
#include <tchar.h>

DWORD execute (HANDLE & hConsoleOutput, char *command) {

	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInformation;
	SECURITY_ATTRIBUTES securityAttributes;

	ZeroMemory( &startupInfo, sizeof(startupInfo));
	ZeroMemory( &processInformation, sizeof(processInformation) );

	securityAttributes.nLength = sizeof (SECURITY_ATTRIBUTES);
	securityAttributes.lpSecurityDescriptor = NULL;
	securityAttributes.bInheritHandle = true;

	startupInfo.cb = sizeof(startupInfo);	
	startupInfo.dwFlags = STARTF_USESTDHANDLES;
	startupInfo.hStdOutput = hConsoleOutput;
	startupInfo.hStdError = NULL;
	startupInfo.hStdInput = NULL;

	if(CreateProcess(NULL, command, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInformation )){
		WaitForSingleObject( processInformation.hProcess, INFINITE );
		DWORD returnCode;
		GetExitCodeProcess(processInformation.hProcess, &returnCode); 
		CloseHandle (processInformation.hProcess);
		CloseHandle (processInformation.hThread);
		return returnCode;
	} else {
		DWORD errorCode = GetLastError ();
		return errorCode;
	}
	return -1;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {

	char title[] = "Title";		char windowClass[] = "Window Class";	

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= NULL;		wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;		wcex.cbWndExtra		= 0;
	wcex.hInstance		= NULL;		wcex.hIcon			= NULL; 
	wcex.hCursor		= NULL;		wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;		wcex.lpszClassName	= windowClass;
	wcex.hIconSm		= NULL; 
	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindow(windowClass, title, WS_VISIBLE, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
	if (!hWnd){
   return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	ShowWindow(hWnd, SW_HIDE);
	AllocConsole();

	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	char commandLine [MAX_PATH];
	sprintf (commandLine, "dir c:");
	execute(hConsoleOutput, commandLine);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}