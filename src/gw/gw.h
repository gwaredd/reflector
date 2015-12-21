////////////////////////////////////////////////////////////////////////////////
//
// a small and lightweight configuration for gw libraries that attempts to 
// unify the basic environment across platforms and projects to assist portability
//

#pragma once


////////////////////////////////////////////////////////////////////////////////
// target platform

#if defined( WIN32 )

    #define gwPLATFORM_WINDOWS

    #include <intrin.h>
    #define gwDebugBreak __debugbreak

#elif defined( __MACH__ )

    #define gwPLATFORM_OSX
    #define gwDebugBreak abort

#else

    #error unknown platform

#endif


////////////////////////////////////////////////////////////////////////////////
// compiler

#if defined( _MSC_VER )

    #define gwCOMPILER_MSVC
    #include <stdint.h> // for uint8_t etc

    #define gwDLL_EXPORT __declspec( dllexport )
    #define gwDLL_IMPORT __declspec( dllimport )

#elif defined( __GNUC__ )

    #define gwCOMPILER_GNU
    #include <inttypes.h>
    #include <stdlib.h>

    #ifdef __cplusplus

        template< typename T, size_t N > inline int _countof( T (&arr)[N] )
        {
            return (int) N;
            // return std::extent< T[ N ] >::value;
        }

    #endif

    #define gwDLL_EXPORT __attribute__ ((visibility ("default")))
    #define gwDLL_IMPORT __attribute__ ((visibility ("default")))

#else

    #error unknown compiler

#endif


////////////////////////////////////////////////////////////////////////////////
// compilation options

#ifdef _USRDLL
    #define gwDLL
#endif


#ifdef _UNICODE
    // NB: not supported atm!
    //#define gwENABLE_UNICODE
#endif


#if defined( _DEBUG ) || ( defined(DEBUG) && DEBUG == 1 )
    #define gwDEBUG
#endif


////////////////////////////////////////////////////////////////////////////////
// types

#ifdef gwENABLE_UNICODE
    typedef wchar_t gwchar;
    typedef unsigned wchar_t gwuchar;
#else
    typedef char gwchar;
    typedef unsigned char gwuchar;
#endif


////////////////////////////////////////////////////////////////////////////////
// incase we want to redirect the memory allocation

#include <stdlib.h>

#define gw_malloc   malloc
#define gw_free     free


////////////////////////////////////////////////////////////////////////////////
// incase we want to redirect debug output

#ifdef gwDEBUG

    inline void gwDebugEnableOutput( bool enable )   { extern bool gw_gEnableTrace; gw_gEnableTrace = enable; }
    inline bool gwDebugIsOutputEnabled()             { extern bool gw_gEnableTrace; return gw_gEnableTrace; }

    extern void gwDebugError( const char*, const char*, int );
    extern void gwDebugPrintf( const char*, ... );
    extern void gwDebugWrite( const char* );
    extern bool gwDebugAssertHandler( const char* assert, const char* file, int line );

    #define gwTrace(...)    if( gwDebugIsOutputEnabled() ) { gwDebugPrintf( __VA_ARGS__ ); }
    #define gwWarning(...)  if( gwDebugIsOutputEnabled() ) { gwDebugError( "WARNING", __FILE__, __LINE__ ); gwDebugPrintf( __VA_ARGS__ ); }
    #define gwError(...)    if( gwDebugIsOutputEnabled() ) { gwDebugError( "ERROR", __FILE__, __LINE__ ); gwDebugPrintf( __VA_ARGS__ ); }
    #define gwAssert(c) \
        do { \
            static bool skipThisAssert = false; \
            if( !skipThisAssert && !(c) && gwDebugAssertHandler( #c, __FILE__, __LINE__ ) ) \
                gwDebugBreak(); \
        } \
        while( false )

#else

    #define gwDebugEnableOutput(...)
    #define gwDebugIsOutputEnabled()    false

    #define gwTrace(...)
    #define gwWarning(...)
    #define gwError(...)
    #define gwAssert(...)
    //#define gwAssert(c)     do { (void)sizeof(c); } while(0)

#endif

