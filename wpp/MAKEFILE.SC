# Windows++ Makefile for SYMANTEC
# (C) Paul DiLascia 1993.
#
# Contains all build instructions for Windows++ sample programs
# Requires Symantec C++ Compiler Version 6.0
#
# To build Windows++, issue one of the following DOS commands.
#
#	make		 		make large model
#	make MODEL=m 		make medium model
#
# 	$(WINDOWS) is where DLL is copied to

CL = sc
CFLAGS = -c -W2 -W3 -w[11] -m$(MODEL) $(ENVFLAGS)

WPPOBJ = WPAPP.obj WPCTRL.OBJ WPDLFILE.obj WPDLG.obj WPDLPRT.obj \
	WPDLSTR.obj WPGDI.obj WPGLOB.obj WPHEAP.obj WPMAIN.obj WPMDI.obj \
	WPMENU.obj WPODRAW.obj WPRECT.obj WPWIN.obj GPP.OBJ 

.asm.obj:
	$(SCASM) $*.asm

.cpp.obj:
	$(CL) $(CFLAGS) $*

all: wpp.lib

lib.rsp: 
	echo +WINMAIN+WPAPP+WPCTRL & >> $@
	echo +WPDLFILE+WPDLG+WPDLPRT+WPDLSTR+WPGDI & >> $@
	echo +WPGLOB+WPHEAP+WPMAIN+WPMDI+WPMENU & >> $@
	echo +WPODRAW+WPRECT+WPWIN+GPP  >> $@
	echo WPP.LST;

wpp.lib: $(WPPOBJ) $(SCOBJ) winmain.obj lib.rsp
	del $*.lib
	lib /c /n /b /noi $*.lib @lib.rsp
	del lib.rsp

clean:
	del *.obj
	del *.map
	del *.lst
	del *.lib
	del *.dll

protect-obj:
	attrib +r setds.obj

GPP.obj : wpp.h c:\wpp\gpp.h GPP.CPP
LIBMAIN.obj : wpp.h LIBMAIN.CPP
WINMAIN.obj : wpp.h WINMAIN.CPP
WPAPP.obj : wpp.h WPAPP.CPP
WPCTRL.obj : wpp.h WPCTRL.CPP
WPDLFILE.obj : wpp.h WPDLFILE.CPP
WPDLG.obj : wpp.h WPDLG.CPP
WPDLPRT.obj : wpp.h WPDLPRT.CPP
WPDLSTR.obj : wpp.h WPDLSTR.CPP
WPGDI.obj : wpp.h WPGDI.CPP
WPGLOB.obj : wpp.h WPGLOB.CPP
WPHEAP.obj : wpp.h WPHEAP.CPP
WPMAIN.obj : wpp.h WPMAIN.CPP
WPMDI.obj : wpp.h WPMDI.CPP
WPMENU.obj : wpp.h WPMENU.CPP
WPODRAW.obj : wpp.h WPODRAW.CPP
WPRECT.obj : wpp.h WPRECT.CPP
WPWIN.obj : wpp.h WPWIN.CPP
