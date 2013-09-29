@echo off
set old=%path% 
set path=%path%;c:\mingw\bin\
set prompt=#
@echo on
mingw32-make %1
@echo off
set path =%old% 

 