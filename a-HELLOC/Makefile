all:t.exe
hello.res:hello.rc
	windres hello.rc -O coff -o hello.res
t.exe: hello.c hello.res	
	g++.exe -mwindows  -static  hello.c hello.res -o $@
clean:
	rm *.exe	
	rm *.res
