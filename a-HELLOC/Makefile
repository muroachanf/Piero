all:t.exe
hello.res:hello.rc
	windres hello.rc -O coff -o hello.res
t.exe: hello.cpp hello.res u_win.cpp logger.c
	g++.exe -Wno-write-strings -mwindows  -static  hello.cpp hello.res logger.c -o $@
clean:
	rm *.exe	
	rm *.res
