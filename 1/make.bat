set old=%path% 
set path=%path%;c:\mingw\bin\
set prompt=%
mingw32-make %1
echo off
set path =%old% 

 