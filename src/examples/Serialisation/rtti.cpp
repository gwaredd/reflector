////////////////////////////////////////////////////////////////////////////////
// Automatically generated RTTI information
// source  : example.json
// date    : Thu Dec 24 2015 21:50:24 GMT+0000 (GMT)

#include <vector>
#include <string>

#include "gwRTTI/gwRTTI.h"
#include "classes.h"

namespace gw { namespace RTTI {

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
    BaseClasses = bases;


    //
    // class fields
    //

    static Field fields[ 3 ];

    // 0. x

    fields[ 0 ].Name     = "x";
    fields[ 0 ].Type     = Type< float >();
    fields[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Vector3*>(o)->x; };


    // 1. y

    fields[ 1 ].Name     = "y";
    fields[ 1 ].Type     = Type< float >();
    fields[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Vector3*>(o)->y; };


    // 2. z

    fields[ 2 ].Name     = "z";
    fields[ 2 ].Type     = Type< float >();
    fields[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Vector3*>(o)->z; };


    // set fields

    Fields          = fields;
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
    BaseClasses = bases;


    //
    // class fields
    //

    static Field fields[ 2 ];

    // 0. Name

    fields[ 0 ].Name     = "Name";
    fields[ 0 ].Type     = Type< std::string >();
    fields[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Component*>(o)->Name; };


    // 1. Enabled

    fields[ 1 ].Name     = "Enabled";
    fields[ 1 ].Type     = Type< bool >();
    fields[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<Component*>(o)->Enabled; };


    // set fields

    Fields          = fields;
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
    BaseClasses = bases;


    //
    // class fields
    //

    static Field fields[ 3 ];

    // 0. Name (inhertied from Component)

    fields[ 0 ].Name     = "Name";
    fields[ 0 ].Type     = Type< std::string >();
    fields[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<RenderComponent*>(o)->Name; };


    // 1. Enabled (inhertied from Component)

    fields[ 1 ].Name     = "Enabled";
    fields[ 1 ].Type     = Type< bool >();
    fields[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<RenderComponent*>(o)->Enabled; };


    // 2. Mesh

    fields[ 2 ].Name     = "Mesh";
    fields[ 2 ].Type     = Type< RenderMesh >();
    fields[ 2 ].IsPointer= true;
    fields[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<RenderComponent*>(o)->Mesh; };


    // set fields

    Fields          = fields;
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
    BaseClasses = bases;


    //
    // class fields
    //

    static Field fields[ 3 ];

    // 0. Name (inhertied from Component)

    fields[ 0 ].Name     = "Name";
    fields[ 0 ].Type     = Type< std::string >();
    fields[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<PhysicsComponent*>(o)->Name; };


    // 1. Enabled (inhertied from Component)

    fields[ 1 ].Name     = "Enabled";
    fields[ 1 ].Type     = Type< bool >();
    fields[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<PhysicsComponent*>(o)->Enabled; };


    // 2. CollisionType

    fields[ 2 ].Name     = "CollisionType";
    fields[ 2 ].Type     = Type< PhysicsComponent::Type >();
    fields[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<PhysicsComponent*>(o)->CollisionType; };


    // set fields

    Fields          = fields;
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
    BaseClasses = bases;


    //
    // class fields
    //

    static Field fields[ 4 ];

    // 0. Name (inhertied from Component)

    fields[ 0 ].Name     = "Name";
    fields[ 0 ].Type     = Type< std::string >();
    fields[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<CollisionComponent*>(o)->Name; };


    // 1. Enabled (inhertied from Component)

    fields[ 1 ].Name     = "Enabled";
    fields[ 1 ].Type     = Type< bool >();
    fields[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<CollisionComponent*>(o)->Enabled; };


    // 2. CollisionType (inhertied from PhysicsComponent)

    fields[ 2 ].Name     = "CollisionType";
    fields[ 2 ].Type     = Type< PhysicsComponent::Type >();
    fields[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<CollisionComponent*>(o)->CollisionType; };


    // 3. CollisionResonse

    fields[ 3 ].Name     = "CollisionResonse";
    fields[ 3 ].Type     = Type< CollisionComponent::Response >();
    fields[ 3 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<CollisionComponent*>(o)->CollisionResonse; };


    // set fields

    Fields          = fields;
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
    BaseClasses = bases;


    //
    // class fields
    //

    static Field fields[ 4 ];

    // 0. Name (inhertied from Component)

    fields[ 0 ].Name     = "Name";
    fields[ 0 ].Type     = Type< std::string >();
    fields[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<ScriptComponent*>(o)->Name; };


    // 1. Enabled (inhertied from Component)

    fields[ 1 ].Name     = "Enabled";
    fields[ 1 ].Type     = Type< bool >();
    fields[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<ScriptComponent*>(o)->Enabled; };


    // 2. Script

    fields[ 2 ].Name     = "Script";
    fields[ 2 ].Type     = Type< std::string >();
    fields[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<ScriptComponent*>(o)->Script; };


    // 3. SomeValue

    fields[ 3 ].Name     = "SomeValue";
    fields[ 3 ].Type     = Type< float >();
    fields[ 3 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<ScriptComponent*>(o)->SomeValue; };

    static Attr field3_attrs[ 2 ];

    field3_attrs[ 0 ].Key     = "min";
    field3_attrs[ 0 ].Value   = "5";

    field3_attrs[ 1 ].Key     = "max";
    field3_attrs[ 1 ].Value   = "31";

    fields[3].Attrs       = field3_attrs;
    fields[3].NumAttrs    = 2;


    // set fields

    Fields          = fields;
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
    BaseClasses = bases;


    //
    // class fields
    //

    static Field fields[ 4 ];

    // 0. Name

    fields[ 0 ].Name     = "Name";
    fields[ 0 ].Type     = Type< std::string >();
    fields[ 0 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<GameObject*>(o)->Name; };


    // 1. Position

    fields[ 1 ].Name     = "Position";
    fields[ 1 ].Type     = Type< Vector3 >();
    fields[ 1 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<GameObject*>(o)->Position; };


    // 2. Components

    fields[ 2 ].Name     = "Components";
    fields[ 2 ].Type     = Type< Component >();
    fields[ 2 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<GameObject*>(o)->Components; };
    fields[ 2 ].IsArray  = true;
    fields[ 2 ].Iterator = []( void* o ) -> std::function< std::pair< void*,const TypeInfo* >() >
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
    fields[ 2 ].Inserter = []( void* o, int size ) -> std::function< bool(void*) >
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

    fields[ 3 ].Name     = "Children";
    fields[ 3 ].Type     = this;
    fields[ 3 ].Get      = []( void* o ) -> void* { return &reinterpret_cast<GameObject*>(o)->Children; };
    fields[ 3 ].IsArray  = true;
    fields[ 3 ].Iterator = []( void* o ) -> std::function< std::pair< void*,const TypeInfo* >() >
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
    fields[ 3 ].Inserter = []( void* o, int size ) -> std::function< bool(void*) >
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

    Fields          = fields;
    NumMembers      = 4;
}

gwRTTI_REGISTER( GameObject );


}}
