////////////////////////////////////////////////////////////////////////////////
// Automatically generated RTTI information
// source  : example.json
// date    : Tue Dec 22 2015 01:23:44 GMT+0000 (GMT)

#include <vector>
#include <string>

#include "gwRTTI/gwRTTI.h"
#include "classes.h"

using namespace gw::RTTI;


////////////////////////////////////////////////////////////////////////////////
// common types

gwRTTI_REGISTER( bool );
gwRTTI_REGISTER( char );
gwRTTI_REGISTER( unsigned char );
gwRTTI_REGISTER( int );
gwRTTI_REGISTER( unsigned int );
gwRTTI_REGISTER( float );
gwRTTI_REGISTER( double );
gwRTTI_REGISTER( std::string );
gwRTTI_REGISTER( RenderMesh );


////////////////////////////////////////////////////////////////////////////////
// extras ...

template<> void TypeInfoImpl< std::string >::Create()
{
    ToString = []( void* obj, char* buf, int size )
    {
        if( buf && size > 0 )
        {
            *buf = '\0';
        }

        return obj ? ((std::string*)obj)->c_str() : buf;
    };

    FromString = []( void* obj, const char* buf ) -> bool
    {
        if( obj == nullptr )
        {
            return false;
        }

        auto str = (std::string*)obj;

        if( buf )
        {
            *str = buf;
        }
        else
        {
            str->empty();
        }

        return true;
    };
}



////////////////////////////////////////////////////////////////////////////////
// Vector3

template<> void TypeInfoImpl< Vector3 >::Create()
{
    //
    // base classes
    //
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    
    //
    // class fields
    //

    static Field p[ 3 ];

    // 0. x

    p[ 0 ].Name     = "x";
    p[ 0 ].Type     = Type< float >();
    p[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Vector3*>(o)->x; };
    

    // 1. y

    p[ 1 ].Name     = "y";
    p[ 1 ].Type     = Type< float >();
    p[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Vector3*>(o)->y; };
    

    // 2. z

    p[ 2 ].Name     = "z";
    p[ 2 ].Type     = Type< float >();
    p[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Vector3*>(o)->z; };
    

    // set fields

    Fields          = p;
    NumMembers      = 3;
}

gwRTTI_REGISTER( Vector3 );


////////////////////////////////////////////////////////////////////////////////
// Component

template<> void TypeInfoImpl< Component >::Create()
{
    //
    // base classes
    //
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    
    //
    // class fields
    //

    static Field p[ 2 ];

    // 0. Name

    p[ 0 ].Name     = "Name";
    p[ 0 ].Type     = Type< std::string >();
    p[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Component*>(o)->Name; };
    

    // 1. Enabled

    p[ 1 ].Name     = "Enabled";
    p[ 1 ].Type     = Type< bool >();
    p[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Component*>(o)->Enabled; };
    

    // set fields

    Fields          = p;
    NumMembers      = 2;
}

gwRTTI_REGISTER( Component );


////////////////////////////////////////////////////////////////////////////////
// RenderComponent

template<> void TypeInfoImpl< RenderComponent >::Create()
{
    //
    // base classes
    //
    
    static const TypeInfo* bases[] = { this, Type<Component>(), nullptr };
    Inherits = bases;

    
    //
    // class fields
    //

    static Field p[ 3 ];

    // 0. Name (inhertied from Component)

    p[ 0 ].Name     = "Name";
    p[ 0 ].Type     = Type< std::string >();
    p[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<RenderComponent*>(o)->Name; };
    

    // 1. Enabled (inhertied from Component)

    p[ 1 ].Name     = "Enabled";
    p[ 1 ].Type     = Type< bool >();
    p[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<RenderComponent*>(o)->Enabled; };
    

    // 2. Mesh

    p[ 2 ].Name     = "Mesh";
    p[ 2 ].Type     = Type< RenderMesh >();
    p[ 2 ].IsPointer= true;
    p[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<RenderComponent*>(o)->Mesh; };
    

    // set fields

    Fields          = p;
    NumMembers      = 3;
}

gwRTTI_REGISTER( RenderComponent );


////////////////////////////////////////////////////////////////////////////////
// PhysicsComponent

template<> void TypeInfoImpl< PhysicsComponent >::Create()
{
    //
    // base classes
    //
    
    static const TypeInfo* bases[] = { this, Type<Component>(), nullptr };
    Inherits = bases;

    
    //
    // class fields
    //

    static Field p[ 3 ];

    // 0. Name (inhertied from Component)

    p[ 0 ].Name     = "Name";
    p[ 0 ].Type     = Type< std::string >();
    p[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<PhysicsComponent*>(o)->Name; };
    

    // 1. Enabled (inhertied from Component)

    p[ 1 ].Name     = "Enabled";
    p[ 1 ].Type     = Type< bool >();
    p[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<PhysicsComponent*>(o)->Enabled; };
    

    // 2. CollisionType

    p[ 2 ].Name     = "CollisionType";
    p[ 2 ].Type     = Type< PhysicsComponent::Type >();
    p[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<PhysicsComponent*>(o)->CollisionType; };
    

    // set fields

    Fields          = p;
    NumMembers      = 3;
}

gwRTTI_REGISTER( PhysicsComponent );


////////////////////////////////////////////////////////////////////////////////
// PhysicsComponent::Type

template<> void TypeInfoImpl< PhysicsComponent::Type >::Create()
{
    //
    // constants
    //
    
    static Constant e[ 3 ];

    e[ 0 ].Name     = "Sphere";
    e[ 0 ].Value    = 0;
    

    e[ 1 ].Name     = "AABB";
    e[ 1 ].Value    = 1;
    

    e[ 2 ].Name     = "ConvexHull";
    e[ 2 ].Value    = 2;
    

    // set constants

    Constants       = e;
    NumMembers      = 3;
}

gwRTTI_REGISTER( PhysicsComponent::Type );


////////////////////////////////////////////////////////////////////////////////
// CollisionComponent

template<> void TypeInfoImpl< CollisionComponent >::Create()
{
    //
    // base classes
    //
    
    static const TypeInfo* bases[] = { this, Type<PhysicsComponent>(), Type<Component>(), nullptr };
    Inherits = bases;

    
    //
    // class fields
    //

    static Field p[ 4 ];

    // 0. Name (inhertied from Component)

    p[ 0 ].Name     = "Name";
    p[ 0 ].Type     = Type< std::string >();
    p[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<CollisionComponent*>(o)->Name; };
    

    // 1. Enabled (inhertied from Component)

    p[ 1 ].Name     = "Enabled";
    p[ 1 ].Type     = Type< bool >();
    p[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<CollisionComponent*>(o)->Enabled; };
    

    // 2. CollisionType (inhertied from PhysicsComponent)

    p[ 2 ].Name     = "CollisionType";
    p[ 2 ].Type     = Type< PhysicsComponent::Type >();
    p[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<CollisionComponent*>(o)->CollisionType; };
    

    // 3. CollisionResonse

    p[ 3 ].Name     = "CollisionResonse";
    p[ 3 ].Type     = Type< CollisionComponent::Response >();
    p[ 3 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<CollisionComponent*>(o)->CollisionResonse; };
    

    // set fields

    Fields          = p;
    NumMembers      = 4;
}

gwRTTI_REGISTER( CollisionComponent );


////////////////////////////////////////////////////////////////////////////////
// CollisionComponent::Response

template<> void TypeInfoImpl< CollisionComponent::Response >::Create()
{
    //
    // constants
    //
    
    static Constant e[ 3 ];

    e[ 0 ].Name     = "Bouncy";
    e[ 0 ].Value    = 0;
    

    e[ 1 ].Name     = "Hard";
    e[ 1 ].Value    = 1;
    

    e[ 2 ].Name     = "Soft";
    e[ 2 ].Value    = 2;
    

    // set constants

    Constants       = e;
    NumMembers      = 3;
}

gwRTTI_REGISTER( CollisionComponent::Response );


////////////////////////////////////////////////////////////////////////////////
// ScriptComponent

template<> void TypeInfoImpl< ScriptComponent >::Create()
{
    //
    // base classes
    //
    
    static const TypeInfo* bases[] = { this, Type<Component>(), nullptr };
    Inherits = bases;

    
    //
    // class fields
    //

    static Field p[ 4 ];

    // 0. Name (inhertied from Component)

    p[ 0 ].Name     = "Name";
    p[ 0 ].Type     = Type< std::string >();
    p[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<ScriptComponent*>(o)->Name; };
    

    // 1. Enabled (inhertied from Component)

    p[ 1 ].Name     = "Enabled";
    p[ 1 ].Type     = Type< bool >();
    p[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<ScriptComponent*>(o)->Enabled; };
    

    // 2. Script

    p[ 2 ].Name     = "Script";
    p[ 2 ].Type     = Type< std::string >();
    p[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<ScriptComponent*>(o)->Script; };
    

    // 3. SomeValue

    p[ 3 ].Name     = "SomeValue";
    p[ 3 ].Type     = Type< float >();
    p[ 3 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<ScriptComponent*>(o)->SomeValue; };
    
    static Attr p3_attrs[ 2 ];

    p3_attrs[ 0 ].Key     = "min";
    p3_attrs[ 0 ].Value   = "5";

    p3_attrs[ 1 ].Key     = "max";
    p3_attrs[ 1 ].Value   = "31";

    p[3].Attrs       = p3_attrs;
    p[3].NumAttrs    = 2;


    // set fields

    Fields          = p;
    NumMembers      = 4;
}

gwRTTI_REGISTER( ScriptComponent );


////////////////////////////////////////////////////////////////////////////////
// GameObject

template<> void TypeInfoImpl< GameObject >::Create()
{
    //
    // base classes
    //
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    
    //
    // class fields
    //

    static Field p[ 4 ];

    // 0. Name

    p[ 0 ].Name     = "Name";
    p[ 0 ].Type     = Type< std::string >();
    p[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<GameObject*>(o)->Name; };
    

    // 1. Position

    p[ 1 ].Name     = "Position";
    p[ 1 ].Type     = Type< Vector3 >();
    p[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<GameObject*>(o)->Position; };
    

    // 2. Components

    p[ 2 ].Name     = "Components";
    p[ 2 ].Type     = Type< Component >();
    p[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<GameObject*>(o)->Components; };
    p[ 2 ].IsArray  = true;
    p[ 2 ].Iterator = []( void* o ) -> std::function< std::pair< void*,const TypeInfo* >() >
    {
        auto obj = reinterpret_cast<GameObject*>( o );
        auto itr = std::begin( obj->Components );
        auto end = std::end( obj->Components );

        return [=]() mutable -> std::pair< void*,const TypeInfo* >
        {
            if( itr == end ) return std::make_pair( nullptr, nullptr );
            auto cur = *itr++;
            return std::make_pair( cur, cur->GetType() );
        };
    };
    p[ 2 ].Inserter = []( void* o, int size ) -> std::function< bool(void*) >
    {
        auto obj = reinterpret_cast<GameObject*>( o );
        obj->Components.reserve( obj->Components.size() + size );

        return [=]( void* i ) -> bool
        {
            obj->Components.push_back( reinterpret_cast< Component* >( i ) );
            return true;
        };
    };
    

    // 3. Children

    p[ 3 ].Name     = "Children";
    p[ 3 ].Type     = this;
    p[ 3 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<GameObject*>(o)->Children; };
    p[ 3 ].IsArray  = true;
    p[ 3 ].Iterator = []( void* o ) -> std::function< std::pair< void*,const TypeInfo* >() >
    {
        auto obj = reinterpret_cast<GameObject*>( o );
        auto itr = std::begin( obj->Children );
        auto end = std::end( obj->Children );

        return [=]() mutable -> std::pair< void*,const TypeInfo* >
        {
            if( itr == end ) return std::make_pair( nullptr, nullptr );
            auto cur = *itr++;
            return std::make_pair( cur, cur->GetType() );
        };
    };
    p[ 3 ].Inserter = []( void* o, int size ) -> std::function< bool(void*) >
    {
        auto obj = reinterpret_cast<GameObject*>( o );
        obj->Children.reserve( obj->Children.size() + size );

        return [=]( void* i ) -> bool
        {
            obj->Children.push_back( reinterpret_cast< GameObject* >( i ) );
            return true;
        };
    };
    

    // set fields

    Fields          = p;
    NumMembers      = 4;
}

gwRTTI_REGISTER( GameObject );


