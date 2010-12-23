//////////////////////////////////////////////////////////////////////////

#include "RTTI.h"
#include "utils.h"


//////////////////////////////////////////////////////////////////////////
// defines

#define WORLD_MAX 100.f
#define WORLD_MIN -100.f

// one benefit of annotation style processing instructions is you can
// use them with the preprocessor with a little bit of work ;)

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#ifdef __clang__
    #define attr(x) __attribute__((annotate( "%reflect attribute " x)))
#else
    #define attr(x)
#endif


//////////////////////////////////////////////////////////////////////////

class Shape
{
    public:

        char*   name;
        Colour  colour;
};


//////////////////////////////////////////////////////////////////////////
//
// reflect everything below ...
//
// NB: inheritance of reflector properties currently does not work if 
// the parent class is templated :(
//


// %%reflect

class Sphere : public RTTI::Impl<Sphere, Shape>
{
    public:
        Vector  pos;
        float   radius attr( "max=" STRINGIFY(WORLD_MAX) ";min=0.1f" );

        // %<accessors>
};

class Plane : public RTTI::Impl<Plane, Shape>
{
    public:
        Vector  normal;
        float   d attr( "max=" STRINGIFY(WORLD_MAX) ";min=" STRINGIFY(WORLD_MIN) );

        // %<accessors>
};

class Line : public RTTI::Impl<Line, Shape>
{
    public:
        Vector pos;
        Vector dir;

        // %<accessors>
};

class Box : public RTTI::Impl<Box, Shape>
{
    public:
        Vector max;
        Vector min;

        // %<accessors>
};

