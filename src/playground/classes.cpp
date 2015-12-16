
#include "classes.h"

using namespace gw;


template<> void RTTI::TypeInfoImpl<Vector3>::Create()
{
    static Property p[3];
    
    p[0].Name = "x";
    p[0].Info = Type<float>();
    p[0].Get  = []( void* o ) -> void* { return &reinterpret_cast<Vector3*>(o)->x; };
    
    p[1].Name = "y";
    p[1].Info = Type<float>();
    p[1].Get  = []( void* o ) -> void* { return &reinterpret_cast<Vector3*>(o)->y; };
    
    p[2].Name = "z";
    p[2].Info = Type<float>();
    p[2].Get  = []( void* o ) -> void* { return &reinterpret_cast<Vector3*>(o)->z; };
    
    Properties      = p;
    NumProperties   = _countof( p );

}

gwRTTI_REGISTER( Vector3 );


template<> void RTTI::TypeInfoImpl<A>::Create()
{
    static const TypeInfo* Base[] { this, nullptr };
    
    static Property p[1];
    p[0].Name = "A";
    p[0].Info = Type<float>();
    p[0].Get  = []( void* o ) -> void* { return &reinterpret_cast<A*>(o)->mA; };
    
    Inherits        = Base;
    Properties      = p;
    NumProperties   = _countof( p );
}

gwRTTI_REGISTER( A );

template<> void RTTI::TypeInfoImpl<B>::Create()
{
    static const TypeInfo* Base[] { this, Type<A>(), nullptr };
    
    static Property p[1];
    p[0].Name = "B";
    p[0].Info = Type<float>();
    p[0].Get  = []( void* o ) -> void* { return &reinterpret_cast<B*>(o)->mB; };
    
    Inherits        = Base;
    Properties      = p;
    NumProperties   = _countof( p );
}

gwRTTI_REGISTER( B );


template<> void RTTI::TypeInfoImpl<C>::Create()
{
    static const TypeInfo* Base[] { this, Type<B>(), Type<A>(), nullptr };
    static Property p[1];
    
    p[0].Name = "C";
    p[0].Info = Type<float>();
    p[0].Get  = []( void* o ) -> void* { return &reinterpret_cast<C*>(o)->mC; };
    
    
    Inherits        = Base;
    Properties      = p;
    NumProperties   = _countof( p );
}

gwRTTI_REGISTER( C );


