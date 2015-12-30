////////////////////////////////////////////////////////////////////////////////

#pragma once


////////////////////////////////////////////////////////////////////////////////

#if defined( _WIN32 )

    #include <windows.h>

    #define LoadModule(...) LoadLibrary( "ModuleB.dll" )
    #define UnloadModule    FreeLibrary
    #define FindSymbol      GetProcAddress

#else

    #include <dlfcn.h>

    #ifdef __MACH__
        #define LoadModule(...) dlopen( "libModuleB.dylib", RTLD_LAZY | RTLD_LOCAL );
    #else
        #define LoadModule(...) dlopen( "libModuleB.so", RTLD_LAZY | RTLD_LOCAL );
    #endif

    #define UnloadModule    dlclose
    #define FindSymbol      dlsym

#endif



////////////////////////////////////////////////////////////////////////////////


#if defined( _MSC_VER )

    #define MODULE_EXPORT   __declspec( dllexport )
    #define MODULE_IMPORT   __declspec( dllimport )

#elif defined( __GNUC__ )

    #define MODULE_EXPORT   __attribute__ ((visibility ("default")))
    #define MODULE_IMPORT   __attribute__ ((visibility ("default")))

#else

    #error unknown compiler

#endif

