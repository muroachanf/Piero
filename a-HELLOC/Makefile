all:bigbang.exe t.exe
hello.res:hello.rc
	windres hello.rc -O coff -o hello.res
t.exe: hello.cpp hello.res u_win.cpp logger.c
	g++.exe -Wno-write-strings -mwindows  -static  hello.cpp hello.res logger.c -o $@
bigbang.exe: big_bang.cpp
	g++.exe -Wno-write-strings -mconsole  -static  big_bang.cpp -o $@
clean:
	rm *.exe	
	rm *.res
