////////////////////////////////////////////////////////////////////////////////
#if defined( WIN32 )

#pragma comment( lib, "ModuleRTTI.lib" )

#include <windows.h>

BOOL APIENTRY DllMain( HMODULE, DWORD, LPVOID )
{
    return TRUE;
}

#endif


////////////////////////////////////////////////////////////////////////////////

#include "ModuleA.h"
#include "gwRTTI/gwRTTI.h"

using namespace gw::RTTI;

////////////////////////////////////////////////////////////////////////////////

MODULEA_API void ModuleA()
{
    const TypeInfo* type;
    type = Type< float >();         // defined in RTTI module
    type = Type< VectorA >();       // defined in this module
    type = Type< class ObjectB >(); // defined in ModuleB - no linkage or headers but we can still retrieve type
}


////////////////////////////////////////////////////////////////////////////////
// Vector3

template<> void TypeInfoImpl< VectorA >::Create()
{
    //
    // base classes
    //

    static const TypeInfo* bases[] = { this, nullptr };
    BaseClasses = bases;


    //
    // class fields
    //

    static Field fields[3];

    // 0. x

    fields[0].Name = "x";
    fields[0].Type = Type< float >();
    fields[0].Get = []( void* o ) -> void* { return &reinterpret_cast<VectorA*>(o)->x; };


    // 1. y

    fields[1].Name = "y";
    fields[1].Type = Type< float >();
    fields[1].Get = []( void* o ) -> void* { return &reinterpret_cast<VectorA*>(o)->y; };


    // 2. z

    fields[2].Name = "z";
    fields[2].Type = Type< float >();
    fields[2].Get = []( void* o ) -> void* { return &reinterpret_cast<VectorA*>(o)->z; };


    // set fields

    Fields = fields;
    NumMembers = 3;
}

gwRTTI_REGISTER( VectorA );
