//////////////////////////////////////////////////////////////////////////
// DO NOT EDIT - this is generated automatically

#include "hash.h"

namespace Type {

//////////////////////////////////////////////////////////////////////////

// TypeShape

class TypeShape : public Info
{
    public:
        TypeShape() : type( kTypeShape ) {}
        virtual const char* GetName() { return "Shape" };
}

static TypeShape s_typeShape;


// TypeSphere

class TypeSphere : public Info
{
    public:
        TypeSphere() : type( kTypeSphere ) {}
        virtual const char* GetName() { return "Sphere" };
}

static TypeSphere s_typeSphere;


// TypePlane

class TypePlane : public Info
{
    public:
        TypePlane() : type( kTypePlane ) {}
        virtual const char* GetName() { return "Plane" };
}

static TypePlane s_typePlane;


// TypeLine

class TypeLine : public Info
{
    public:
        TypeLine() : type( kTypeLine ) {}
        virtual const char* GetName() { return "Line" };
}

static TypeLine s_typeLine;


// TypeBox

class TypeBox : public Info
{
    public:
        TypeBox() : type( kTypeBox ) {}
        virtual const char* GetName() { return "Box" };
}

static TypeBox s_typeBox;


//////////////////////////////////////////////////////////////////////////

static Info* s_types[ kNumTypes ] =
{
    &s_typeShape,
    &s_typeSphere,
    &s_typePlane,
    &s_typeLine,
    &s_typeBox,
};

Info* Get( eType type )
{
    return s_types[ type ];
}

Info* Get( const char* name )
{
    switch( Hash( name ) )
    {
        case 0xE30FA59C: return s_types[ kTypeShape ];       // Shape
        case 0x7F58D132: return s_types[ kTypeSphere ];      // Sphere
        case 0xE2E73057: return s_types[ kTypePlane ];       // Plane
        case 0x28582E99: return s_types[ kTypeLine ];        // Line
        case 0x098F1686: return s_types[ kTypeBox ];         // Box
    }

    return 0;
}

} // Type
