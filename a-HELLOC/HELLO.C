//////////////////////////////////////////////////////////
// WINDOWS++ CLASS LIBRARY.  Copyright 1992,1996 Paul DiLascia.
// FILE: HELLO.C
//
/******************************************************/
/* This is the basic C version of the hello program.  */
/******************************************************/

#include <windows.h>
#include "u_win.c"

int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
	win *w = new win(hinst, pinst,show);
	// w->set_rect(CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT);
	w->set_rect(100,100,300,200);
	app a (w);
	return a.main(hinst,cmdline,show);
}
