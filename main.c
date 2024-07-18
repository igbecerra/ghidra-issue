#if defined(_MSC_VER)
    #define C_CDECL __cdecl
    #define C_STDCALL __stdcall
    #define C_FASTCALL __fastcall
    #define C_THISCALL __thiscall
#else // Almost any compiler but MSVC supports __attribute((whatever))
    #define C_CDECL __attribute__((cdecl))
    #define C_STDCALL __attribute__((stdcall))
    #define C_FASTCALL __attribute__((fastcall))
    #define C_THISCALL __attribute__((thiscall))
#endif

#if (defined(_WIN32) && !defined(__CYGWIN__)) // Targeting Windows
    #define EXPORT_THIS __declspec(dllexport)
#else // Here's hoping your compiler support this!
    #define EXPORT_THIS __attribute__ ((visibility("default")))
#endif

#if defined(BUILD_BASE_DLL) // Building Base, export Base funcs
    #define BASE_EXPORTS_THIS EXPORT_THIS
#else // Building GhTest, import from Base if Windows
    #if (defined(_WIN32) && !defined(__CYGWIN__)) // Targeting Windows
    // IMPORTANT!!!
    // At least on i686-w64-mingw32-gcc 13.1.0 this causes a
    // different call instruction to be emitted, which triggers
    // the Ghidra bug
        #define BASE_EXPORTS_THIS __declspec(dllimport)
    #else // Just in case I try this on non-Windows
        #define BASE_EXPORTS_THIS
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Base.dll declarations...
// These use 4+ args to semi-guarantee stack use on x64
BASE_EXPORTS_THIS int C_STDCALL StdCallFunc(int a, int b, int c, int d, int e);
BASE_EXPORTS_THIS int C_CDECL CDeclFunc(int a, int b, int c, int d, int e);
BASE_EXPORTS_THIS int C_FASTCALL FastCallFunc(int a, int b, int c, int d, int e);

#if defined(BUILD_BASE_DLL)
BASE_EXPORTS_THIS int C_STDCALL StdCallFunc(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}
BASE_EXPORTS_THIS int C_CDECL CDeclFunc(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}
BASE_EXPORTS_THIS int C_FASTCALL FastCallFunc(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}
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
EXPORT_THIS int TestFunctionCDeclBase(int a, int b, int c, int d, int e)
{
    // Whatever code to convince compilers
    // to actually CALL
    if (CDeclFunc(a, b, c, d, e) == 0xc0ffee)
    {
        return 0x99aabb;
    }
    return 0x33;
}
EXPORT_THIS int TestFunctionFastCallBase(int a, int b, int c, int d, int e)
{
    // Whatever code to convince compilers
    // to actually CALL
    if (FastCallFunc(a, b, c, d, e) == 0xc0ffee)
    {
        return 0x99aabb;
    }
    return 0x33;
}
#endif
#ifdef __cplusplus
}
#endif
