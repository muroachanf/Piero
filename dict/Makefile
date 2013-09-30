_flag=-Wno-write-strings
_lib=-lws2_32 -lole32 -luuid -lShlwapi -lcomctl32
_include=-Ishare
gcc=g++
_head=hwnd_file.h share/logger.h share/common.h about_dlg.h burning_ctl.h about_dialogbox.h

default: dict.exe
dict.o: dict.c $(_head)
	$(gcc) -c dict.c -o dict.o $(_flag) $(_include)
common.o: share/common.c 
	$(gcc) -c share/common.c  $(_flag) $(_include)
logger.o: share/logger.c 
	$(gcc) -c share/logger.c  $(_flag) $(_include)
hwnd_file.o:hwnd_file.c 
	$(gcc) -c hwnd_file.c  $(_flag) $(_include)  

burning_ctl.o:burning_ctl.c 
	$(gcc) -c burning_ctl.c  $(_flag) $(_include)  


about_dialogbox.o:about_dialogbox.c  
	$(gcc) -c about_dialogbox.c  $(_flag) $(_include)  
about_dlg.o:about_dlg.c  burning_ctl.o
	$(gcc) -c about_dlg.c  $(_flag) $(_include)  
resource.o: about_rc.rc Application.ico resource.h
	windres  -I. -i about_rc.rc -o resource.o
dict.exe: dict.o common.o logger.o hwnd_file.o about_dlg.o burning_ctl.o about_dialogbox.o resource.o
	$(gcc)   dict.o common.o logger.o hwnd_file.o  about_dlg.o  burning_ctl.o about_dialogbox.o resource.o -o dict $(_lib)  -mwindows -static $(_flag) -s

clean: 	
	rm -f *.o	
	rm piero.txt
	rm hwnd.txt

# test 	
4.o:test/4.c
	$(gcc) -c test/4.c -o 4.o  $(_flag) $(_include)
test:4.o	
	$(gcc)  4.o -static -o test/4  $(_lib)  -mwindows -static $(_flag)
