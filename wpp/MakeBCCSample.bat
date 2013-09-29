@echo off 
set path=%path%;c:\Borland\BCC55\Bin
cd samples
@echo on
make -f makefile.bcc DLL=0 &&copy ..\wpp.dll 
@echo off 
cd ..


