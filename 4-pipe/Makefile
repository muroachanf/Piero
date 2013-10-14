_flag=-Wno-write-strings
_lib=-lws2_32 -lole32 -luuid -lShlwapi -lcomctl32
_include=-Ishare
gcc=g++
_head=hwnd_file.h share/logger.h share/common.h about_dlg.h burning_ctl.h about_dialogbox.h

default: 9.exe counter.exe
9.exe: 9-enc.cpp cmd_pipe.cpp
	$(gcc)  9-enc.cpp cmd_pipe.cpp -o 9.exe
counter.exe:10-counter.c 
	$(gcc)  10-counter.c -o counter.exe

clean: 	
	rm -f *.o	
	rm *.exe	
