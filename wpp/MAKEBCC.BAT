@echo off
set path=%path%;c:\Borland\BCC55\Bin
make -f makefile.bcc DLL=1
cd samples
make -f makefile.bcc DLL=1
copy ..\wpp.dll
cd ..
