////////////////////////////////////////////////////////////////////////////////
//
// basic vanilla implementations here. maybe able to improve speed by moving
// to instrinsic or asm implementations (e.g. __asm repne scas)
//

#include "gwstring.h"

#include <errno.h>
#include <algorithm>


////////////////////////////////////////////////////////////////////////////////

int gw_strcpy( char* dst, int size, const char *src )
{
    if( dst == nullptr || size <= 0 )
    {
        return EINVAL;
    }

    if( src == nullptr )
    {
        *dst = '\0';
        return EINVAL;
    }

    // copy as much as we can

    while( ( *dst = *src ) && --size > 0 )
    {
        ++dst;
        ++src;
    }

    if( *src == '\0' )
    {
        return 0;
    }


    // we ran out of buffer

    *dst = '\0';

    return ERANGE;
}


////////////////////////////////////////////////////////////////////////////////

int gw_strncpy( char* dst, int size, const char *src, int num )
{
    if( dst == nullptr || size <= 0 )
    {
        return EINVAL;
    }

    if( src == nullptr || num <= 0 )
    {
        *dst = '\0';
        return EINVAL;
    }

    auto count = std::min( size, num + 1 );

    // copy as much as we can

    while( ( *dst = *src ) && --count > 0 )
    {
        ++dst;
        ++src;
    }

    if( *src != '\0' )
    {
        *dst = '\0';
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////

int gw_strcat( char* dst, int size, const char *src )
{
    if( dst == nullptr || size <= 0 || src == nullptr )
    {
        return EINVAL;
    }

    // find end of string

    while( size > 0 && *dst )
    {
        ++dst;
        --size;
    }

    if( size == 0 )
    {
        return ERANGE;
    }

    // copy

    while( --size > 0 && ( *dst = *src ) )
    {
        ++dst;
        ++src;
    }

    if( *src == '\0' )
    {
        return 0;
    }


    // we ran out of buffer

    *dst = '\0';

    return ERANGE;
}


////////////////////////////////////////////////////////////////////////////////

int gw_strncat( char* dst, int size, const char *src, int num )
{
    if( dst == nullptr || size <= 0 || src == nullptr || num <= 0 )
    {
        return EINVAL;
    }

    // find end of string

    while( size > 0 && *dst )
    {
        ++dst;
        --size;
    }

    if( size == 0 )
    {
        return ERANGE;
    }

    // copy
    
    auto enough = size > num;
    auto count = enough ? num : size - 1;

    while( count > 0 && ( *dst = *src ) )
    {
        ++dst;
        ++src;
        --count;
    }

    *dst = '\0';

    return enough ? 0 : ERANGE;
}


