all:read.exe t.exe
t.exe:thread_exit_test.c thread.c
	g++ thread_exit_test.c thread.c -o t.exe
read.exe:read.c thread.c
	g++ -Wno-write-strings read.c thread.c -o read.exe