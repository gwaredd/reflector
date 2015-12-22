////////////////////////////////////////////////////////////////////////////////

#include "classes.h"


////////////////////////////////////////////////////////////////////////////////

GameObject::GameObject()
    : SecretValue( 64.f )
{
}

GameObject::~GameObject()
{
    for( auto component : Components )
    {
        delete component;
    }
    
    for( auto child : Children )
    {
        delete child;
    }
}



////////////////////////////////////////////////////////////////////////////////

static int ID = 0;

bool RandomChance()
{
    return ( rand() >> 6 ) % 3 != 0;  // 2 in 3 chance
}



////////////////////////////////////////////////////////////////////////////////

GameObject* GameObject::CreateRandom( bool children )
{
    char id[ 8 ];
    sprintf( id, "%i", ++ID );
    
    Name = "Object";
    Name += id;
    
    
    // components
    
    if( RandomChance() )
    {
        std::string mesh = "Mesh";
        mesh += id;
        
        auto comp = new RenderComponent();
        comp->LoadMesh( mesh.c_str() );
        
        Components.push_back( comp );
    }
    
    if( RandomChance() )
    {
        if( RandomChance() )
        {
            Components.push_back( new PhysicsComponent() );
        }
        else
        {
            Components.push_back( new CollisionComponent() );
        }
    }
    
    if( RandomChance() )
    {
        Components.push_back( new ScriptComponent() );
    }
    
    // children
    
    if( children )
    {
        int numChildren = rand() % 5;
        
        for( int i=0; i < numChildren; i++ )
        {
            Children.push_back( ( new GameObject() )->CreateRandom( false ) );
        }
    }
    
    return this;
}

