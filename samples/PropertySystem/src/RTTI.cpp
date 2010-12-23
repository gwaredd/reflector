//////////////////////////////////////////////////////////////////////////
// DO NOT EDIT - this file is automatically generated
//
// creates

#include "Shapes.h"

namespace RTTI {

            
// name TypeSphere

class TypeSphere : public Type
{
    virtual const char*     Name()      { return "Sphere"; };
    virtual unsigned int    ClassID()   { return 0x7F58D132; }
};

static TypeSphere sTypeSphere;
Type* Sphere::spType = &sTypeSphere;

                
// name TypePlane

class TypePlane : public Type
{
    virtual const char*     Name()      { return "Plane"; };
    virtual unsigned int    ClassID()   { return 0xE2E73057; }
};

static TypePlane sTypePlane;
Type* Plane::spType = &sTypePlane;

                
// name TypeLine

class TypeLine : public Type
{
    virtual const char*     Name()      { return "Line"; };
    virtual unsigned int    ClassID()   { return 0x28582E99; }
};

static TypeLine sTypeLine;
Type* Line::spType = &sTypeLine;

                
// name TypeBox

class TypeBox : public Type
{
    virtual const char*     Name()      { return "Box"; };
    virtual unsigned int    ClassID()   { return 0x98F1686; }
};

static TypeBox sTypeBox;
Type* Box::spType = &sTypeBox;

    

} // ns RTTI

