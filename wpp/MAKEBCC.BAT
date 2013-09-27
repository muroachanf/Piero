@echo off
set path=%path%;c:\Borland\BCC55\Bin
make -f makefile.bcc DLL=1
rem 原来，一个bat里面做了两个make。继续...
rem cd samples
rem make -f makefile.bcc DLL=1
rem copy ..\wpp.dll
rem cd ..
