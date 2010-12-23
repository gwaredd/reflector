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

        // %<accessors
        Vector&  GetPos()                 { return pos; }
        float&   GetRadius()              { return radius; }

        void     SetPos( Vector& v )      { pos = v; }
        void     SetRadius( float& v )    { radius = v < 0.1f ? 0.1f : (v > 100.f ? 100.f : v); }
        // %>
};

class Plane : public RTTI::Impl<Plane, Shape>
{
    public:
        Vector  normal;
        float   d attr( "max=" STRINGIFY(WORLD_MAX) ";min=" STRINGIFY(WORLD_MIN) );

        // %<accessors
        Vector&  GetNormal()              { return normal; }
        float&   GetD()                   { return d; }

        void     SetNormal( Vector& v )   { normal = v; }
        void     SetD( float& v )         { d = v < -100.f ? -100.f : (v > 100.f ? 100.f : v); }
        // %>
};

class Line : public RTTI::Impl<Line, Shape>
{
    public:
        Vector pos;
        Vector dir;

        // %<accessors
        Vector&  GetPos()                 { return pos; }
        Vector&  GetDir()                 { return dir; }

        void     SetPos( Vector& v )      { pos = v; }
        void     SetDir( Vector& v )      { dir = v; }
        // %>
};

class Box : public RTTI::Impl<Box, Shape>
{
    public:
        Vector max;
        Vector min;

        // %<accessors
        Vector&  GetMax()                 { return max; }
        Vector&  GetMin()                 { return min; }

        void     SetMax( Vector& v )      { max = v; }
        void     SetMin( Vector& v )      { min = v; }
        // %>
};

