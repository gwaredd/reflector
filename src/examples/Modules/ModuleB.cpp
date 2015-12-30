////////////////////////////////////////////////////////////////////////////////
#if defined( _WIN32 )

#pragma comment( lib, "ModuleRTTI.lib" )

#include <windows.h>

BOOL APIENTRY DllMain( HMODULE, DWORD, LPVOID )
{
    return TRUE;
}

#endif


////////////////////////////////////////////////////////////////////////////////

#include "gwRTTI/gwRTTI.h"
#include "ModuleB.h"

using namespace gw::RTTI;


////////////////////////////////////////////////////////////////////////////////

MODULEB_API void ModuleB()
{
    const TypeInfo* type;
    type = Type< float >();             // defined in RTTI module
    type = Type< ObjectB >();           // defined in this module
    type = Type< struct VectorA >();    // defined in ModuleA - we can still get the type despite no linking to that module
}

////////////////////////////////////////////////////////////////////////////////
// SomeObject

template<> void TypeInfoImpl< ObjectB >::Create()
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

    // 0. someValue

    fields[0].Name = "someValue";
    fields[0].Type = Type< float >();
    fields[0].Get = []( void* o ) -> void* { return &reinterpret_cast<ObjectB*>(o)->someValue; };


    // set fields

    Fields = fields;
    NumMembers = 1;
}

gwRTTI_REGISTER( ObjectB );
