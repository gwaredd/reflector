////////////////////////////////////////////////////////////////////////////////

#pragma once


////////////////////////////////////////////////////////////////////////////////

#if defined( WIN32 )

    #include <windows.h>

    #define LoadModule(...) LoadLibrary( "ModuleB.dll" )
    #define UnloadModule    FreeLibrary
    #define FindSymbol      GetProcAddress

#elif defined( __MACH__ )

    #include <dlfcn.h>

    #define LoadModule(...) dlopen( "libModuleB.dylib", RTLD_LAZY | RTLD_LOCAL );
    #define UnloadModule    dlclose
    #define FindSymbol      dlsym

#else

    #error unknown platform

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

