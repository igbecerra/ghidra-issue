#!/bin/sh
rm mainGhTest.o libGhTest.dll
i686-w64-mingw32-gcc -Wall -m32 -O2 -c main.c -o mainGhTest.o
i686-w64-mingw32-gcc -shared mainGhTest.o -o libGhTest.dll -L. -lBase -luser32 -static-libstdc++ -static-libgcc -m32 -s
# x64 build
mkdir x64
rm x64/mainGhTest.o x64/libGhTest.dll
x86_64-w64-mingw32-gcc -Wall -m64 -O2 -c main.c -o x64/mainGhTest.o
x86_64-w64-mingw32-gcc -Wall -shared x64/mainGhTest.o -o x64/libGhTest.dll -L./x64 -lBase -luser32 -static-libstdc++ -static-libgcc -m64 -s