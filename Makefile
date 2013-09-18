_flag=-Wno-write-strings
_lib=-lws2_32 -lole32 -luuid -lShlwapi
_include=-Ishare
HEADERS = l.h logger.h
gcc=g++

default: dict
2.o: 2.c 
	$(gcc) -c 2.c -o 2.o $(_flag) $(_include)
l.o: share/l.c 
	$(gcc) -c share/l.c -o l.o $(_flag) $(_include)
logger.o: share/logger.c 
	$(gcc) -c share/logger.c -o logger.o  $(_flag) $(_include)
dict: 2.o l.o logger.o
	$(gcc)   2.o l.o logger.o -o dict $(_lib)  -mwindows -static $(_flag)
clean: 
	rm -f *.o

