# ghidra-issue

Minimal repro for a ghidra 10.3+ bug. Stack depth is lost on certain calls to Win32 x86 DLLs, regardless of calling convention used. The bytes emitted for the ```CALL``` instruction matter somehow. See the comments in ```main.c``` for more info. I'm unable to trigger the bug on x64 DLLs, it may not exist or maybe the compilers use a different ```CALL``` instruction...

# Important notes (most also as comments in main.c)
- To reproduce the bug, it's important that the imported functions are marked as ```__declspec(dllimport)```.
- Same code was also tested as built by MSVC 2019, producing the same results.
- Bug was found on an 2001 DLL built using MSVC 6 calling standard Windows APIs like ```LoadLibraryA``` or ```GetProcAddress```. These functions can also reproduce the bug.

# Build env:
```
OS: Arch Linux
i686-w64-mingw32-gcc (GCC) 13.1.0
x86_64-w64-mingw32-gcc (GCC) 13.1.0
```

Details on how it was build can be found on the build scripts...

## main.c

Contains code that builds ```libBase.dll``` and ```libGhTest.dll``` under many compilers.
Even when we only care about ```libGhTest.dll```, ```libBase.dll``` allows us to fully control what code we're actually building.

#### Code walkthrough:

```
#if defined(_MSC_VER)
    ...
#endif
```
Compiler agnostic definition of helper macros for calling conventions

```
#if (defined(_WIN32) && !defined(__CYGWIN__)) // Targeting Windows
    ...
#endif
```
OS-agnostic definition of the helper macro EXPORT_THIS. On Win32 and Win64 platforms turns to ```__declspec(dllexport)```.

```
#if defined(BUILD_BASE_DLL) // Building Base, export Base funcs
    ...
#endif
```
OS-agnostic definition of the helper macro BASE_EXPORTS_THIS. On Win32 and Win64 platforms, this is defined to ```EXPORT_THIS``` when building ```libBase.dll``` or to ```__declspec(dllimport)``` when building ```libGhTest.dll```. This last one is *really important* for MinGW-GCC to emit the buggy ```CALL``` instruction.

```
BASE_EXPORTS_THIS int C_ [...]Func(int a, int b, [...] int e);
...
```
Declarations of the functions exported from ```libBase.dll```. These apply when building either DLL, but the meaning of BASE_EXPORTS_THIS changes.

```
#if defined(BUILD_BASE_DLL)
BASE_EXPORTS_THIS int C_STDCALL StdCallFunc(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}
...
```
Implementation of the functions for ```libBase.dll```. Only compiled when building it.

```
#else // Building GhTest
EXPORT_THIS int TestFunctionStdCallBase(int a, int b, int c, int d, int e)
{
    // Whatever code to convince compilers
    // to actually CALL
    if (StdCallFunc(a, b, c, d, e) == 0xc0ffee)
    {
        return 0x99aabb;
    }
    return 0x33;
}
...
#endif
```
Implementation of the functions for ```libGhTest.dll```, calling the respective function from ```libBase.dll``` and doing something with the result so the compiler doesn't just ```JMP```.

## build
Main build script. Just calls buildBase.sh amd buildGhTest.sh *NOTE THAT THE SCRIPTS DO CALL RM*

## buildBase.sh && buildGhTest.sh
Build scripts for each DLL with the necessary defines. *NOTE THAT THE SCRIPTS DO CALL RM*
