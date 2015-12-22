////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <stdlib.h>

#include "gwRTTI/gwRTTI.h"


////////////////////////////////////////////////////////////////////////////////
// utility

inline float FRand( float min, float max )
{
    return min + ( max - min ) * static_cast< float >( rand() ) / static_cast< float >( RAND_MAX );
}


////////////////////////////////////////////////////////////////////////////////
// not reflected

class RenderMesh
{
    public:

        std::string Name;
};


////////////////////////////////////////////////////////////////////////////////
///%%

struct Vector3
{
    float x,y,z;

    Vector3()
        : x( FRand( -100.f, 100.f ) )
        , y( FRand( -100.f, 100.f ) )
        , z( FRand( -100.f, 100.f ) )
    {
    }
};


////////////////////////////////////////////////////////////////////////////////
// components

///%% -- reflection is inherited by subclasses

class Component
{
    gwRTTI( Component );
    
    public:

        std::string Name;
        bool        Enabled;

        Component()
            : Enabled( true )
        {
        }

        virtual ~Component()
        {
        }
};

class RenderComponent : public Component
{
    gwRTTI( RenderComponent );

    public:

        RenderMesh* Mesh; // unreflected 'opaque' class

        void LoadMesh( const char* name )
        {
            Mesh = new RenderMesh();
            Mesh->Name = name;
        }

        RenderComponent()
            : Mesh( nullptr )
        {
            Name = "RenderComponent";
        }

        virtual ~RenderComponent()
        {
            delete Mesh;
        }
};

class PhysicsComponent : public Component
{
    gwRTTI( PhysicsComponent );

    public:

        enum Type
        {
            Sphere,
            AABB,
            ConvexHull
        };

        Type CollisionType;

        PhysicsComponent()
        {
            Name = "PhysicsComponent";
            CollisionType = Type( rand() % 3 );
        }
};

class CollisionComponent : public PhysicsComponent
{
    gwRTTI( CollisionComponent );

    public:

        enum Response
        {
            Bouncy,
            Hard,
            Soft
        };

        Response CollisionResonse;

        CollisionComponent()
        {
            Name = "CollisionComponent";
            CollisionResonse = Response( rand() % 3 );
        }
};

class ScriptComponent : public Component
{
    gwRTTI( ScriptComponent );

    public:

        std::string Script;

        ///%% min=5, max=31
        float       SomeValue;

        ///%! -- don't reflect this
        float       InternalValue;

        ScriptComponent()
            : Script( "myscript" )
            , SomeValue( FRand( 5.f, 31.f ) )
            , InternalValue( -999.f )
        {
            Name = "ScriptComponent";
        }
};


////////////////////////////////////////////////////////////////////////////////
// game object

///%%

class GameObject
{
    gwRTTI( GameObject );

    public:

        std::string                 Name;
        Vector3                     Position;
        std::vector<Component*>     Components;
        std::vector<GameObject*>    Children;

        GameObject();
        virtual ~GameObject();
        GameObject* CreateRandom( bool = true );

    private:

        // private members are removed from the TypeInfo

        float SecretValue;
};


