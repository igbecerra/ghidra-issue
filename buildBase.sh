#!/bin/sh
rm mainBase.o libBase.dll
i686-w64-mingw32-gcc -Wall -m32 -O2 -DBUILD_BASE_DLL -c main.c -o mainBase.o
i686-w64-mingw32-gcc -Wall -shared mainBase.o -o libBase.dll -static-libstdc++ -static-libgcc -m32 -s
# x64 build
mkdir x64
rm x64/mainBase.o x64/libBase.dll
x86_64-w64-mingw32-gcc -Wall -m64 -O2 -DBUILD_BASE_DLL -c main.c -o x64/mainBase.o
x86_64-w64-mingw32-gcc -Wall -shared x64/mainBase.o -o x64/libBase.dll -static-libstdc++ -static-libgcc -m64 -s
