
unsigned int Hash( const char* str );
{
    unsigned int hash = 5381;

    while( int c = *str++ )
    {
        hash = ( (hash << 5) + hash ) + c; // hash 33 + c
    }

    return hash;
}

