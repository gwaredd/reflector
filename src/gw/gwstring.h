////////////////////////////////////////////////////////////////////////////////
//
// portable versions of the string_s functions
//
// behaves slightly differently - validates inputs and does 'what it can'
// rather than throw a wobbly!
//

#pragma once

#include <stdlib.h>
#include "gw.h"


////////////////////////////////////////////////////////////////////////////////
// strings

extern int gw_strcpy(  char* dst, int size, const char *src );
extern int gw_strncpy( char* dst, int size, const char *src, int num );
extern int gw_strcat(  char* dst, int size, const char *src );
extern int gw_strncat( char* dst, int size, const char *src, int num );

template< size_t size > inline int gw_strcpy(  char (&dst)[size], const char* src )            { return gw_strcpy( dst, size, src ); }
template< size_t size > inline int gw_strncpy( char (&dst)[size], const char* src, int num )   { return gw_strncpy( dst, size, src, num ); }
template< size_t size > inline int gw_strcat(  char (&dst)[size], const char* src )            { return gw_strcat( dst, size, src ); }
template< size_t size > inline int gw_strncat( char (&dst)[size], const char* src, int num )   { return gw_strncat( dst, size, src, num ); }


#ifdef gwCOMPILER_MSVC

    #define gw_strlen           strlen
    #define gw_strcmp           strcmp
    #define gw_strncmp          strncmp
    #define gw_stricmp          _stricmp
    #define gw_strnicmp         _strnicmp

    #define gw_sprintf          sprintf_s
    #define gw_vsprintf         vsprintf_s
    #define gw_vsprintfc        _vscprintf

#else

    #define gw_strlen(s)        int( strlen(s) )
    #define gw_strcmp           strcmp
    #define gw_stricmp          strcasecmp
    #define gw_strncmp          strncmp
    #define gw_strnicmp         strncasecmp

    #define gw_sprintf          snprintf
    #define gw_vsprintf         vsnprintf
    #define gw_vsprintfc(f,a)   vsnprintf( nullptr, 0, (f), (a) )

#endif
