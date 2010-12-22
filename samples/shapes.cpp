//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "shapes.h"

//////////////////////////////////////////////////////////////////////////
// djb2 hash

static unsigned int Hash( const char* str )
{
    unsigned int hash = 5381;

    while( int c = *str++ )
    {
        hash = ( (hash << 5) + hash ) + c; // hash 33 + c
    }

    return hash;
}

//////////////////////////////////////////////////////////////////////////

Shape* Shape::Create( const char* name )
{
    switch( Hash( name ) )
    {
        case 0:
            return new Box();

        default:
            fprintf_s( stderr, "Unknown type '%s'\n", name );
            return NULL;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////

void Shape::Set( Shape* pShape, const char* name, const char* value )
{
    if( pShape == NULL || name == NULL || value == NULL )
    {
        return;
    }

}

