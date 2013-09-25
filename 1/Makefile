_flag=-Wno-write-strings
_lib=
_include=
gcc=g++
_head=custom.h 

# default: 1
all:1.exe

custom.o: custom.c
	$(gcc) -c custom.c  $(_flag) $(_include)

main.o:main.c custom.h
	$(gcc) -c main.c $(_flag) $(_include)  

logger.o:logger.c 
	$(gcc) -c logger.c $(_flag) $(_include)  

1.exe: main.o custom.o logger.o
	$(gcc)  logger.o main.o custom.o $(_lib) -o 1 -mwindows -static $(_flag) -s

	# $(gcc)  logger.o main.o custom.o $(_lib) -o 1 -mwindows
clean: 
	rm -f *.o