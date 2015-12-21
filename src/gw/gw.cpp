////////////////////////////////////////////////////////////////////////////////

#include "gw.h"

#ifdef gwDEBUG

#include "gwstring.h"

#include <stdio.h>
#include <stdarg.h>


////////////////////////////////////////////////////////////////////////////////

bool gw_gEnableTrace = true;


////////////////////////////////////////////////////////////////////////////////

void gwDebugPrintf( const char* format, ... )
{
    va_list args;
    
    va_start( args, format );
    auto len = gw_vsprintfc( format, args ) + 1;
    va_end( args );
    
    auto buffer = (char*) gw_malloc( len );
    
    va_start( args, format );
    gw_vsprintf( buffer, len, format, args );
    va_end( args );
    
    gwDebugWrite( buffer );
    gwDebugWrite( "\n" );
    
    gw_free( buffer );
}


////////////////////////////////////////////////////////////////////////////////

bool gwDebugAssertHandler( const char* assert, const char* file, int line )
{
    static bool skipAsserts     = false;
    static bool printAsserts    = true;
    
    if( printAsserts )
    {
        gwDebugPrintf( "%s(%i): [ASSERT] %s", file, line, assert );
    }
    
    return skipAsserts == false;
}


////////////////////////////////////////////////////////////////////////////////

void gwDebugError( const char* type, const char* file, int line )
{
    char buffer[ 256 ];
    gw_sprintf( buffer, 256, "%s(%i): [%s] ", file, line, type );
    gwDebugWrite( buffer );
}


////////////////////////////////////////////////////////////////////////////////

#ifdef gwPLATFORM_WINDOWS

    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    void gwDebugWrite( const char* str )
    {
        OutputDebugStringA( str );
    }

#else

    #include <iostream>

    void gwDebugWrite( const char* str )
    {
        std::cerr << str;
    }

#endif


////////////////////////////////////////////////////////////////////////////////
#endif // gwDEBUG

