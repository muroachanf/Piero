_flag=-Wno-write-strings
_lib=-lws2_32 -lole32 -luuid -lShlwapi
_include=-Ishare
gcc=g++
_head=hwnd_file.h share/logger.h share/common.h

default: dict
dict.o: dict.c $(_head)
	$(gcc) -c dict.c -o dict.o $(_flag) $(_include)
common.o: share/common.c 
	$(gcc) -c share/common.c  $(_flag) $(_include)
logger.o: share/logger.c 
	$(gcc) -c share/logger.c  $(_flag) $(_include)
hwnd_file.o:hwnd_file.c 
	$(gcc) -c hwnd_file.c  $(_flag) $(_include)  
dict: dict.o common.o logger.o hwnd_file.o
	$(gcc)   dict.o common.o logger.o hwnd_file.o -o dict $(_lib)  -mwindows -static $(_flag) -s
clean: 
	rm -f *.o


# test 	
4.o:test/4.c
	$(gcc) -c test/4.c -o 4.o  $(_flag) $(_include)
test:4.o	
	$(gcc)  4.o -static -o test/4  $(_lib)  -mwindows -static $(_flag)
