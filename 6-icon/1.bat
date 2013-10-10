set path=%path%;c:\mingw\bin\
set prompt=%
windres 1.rc -O coff -o 1.res
g++.exe -mwindows  -static  1.c 1.res