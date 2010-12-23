#ifndef utils_h
#define utils_h
////////////////////////////////////////////////////////////////////////////////

// types

struct Vector
{
    float x;
    float y;
    float z;
};

struct Colour
{
    float r;
    float g;
    float b;
    float a;
};

// functions

extern unsigned int Hash( const char* str );

////////////////////////////////////////////////////////////////////////////////
#endif // utils_h

