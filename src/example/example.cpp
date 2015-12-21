////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include "rapidjson/reader.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

#include "gw/gw.h"
#include "gwRTTI/gwRTTI.h"

#include "classes.h"

using namespace rapidjson;


std::vector< GameObject* > GameObjects;


////////////////////////////////////////////////////////////////////////////////
// create some random objects

static int ID = 0;

bool RandomChance()
{
    return ( rand() >> 6 ) % 3 != 0;  // 2 in 3 chance
}

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
        Components.push_back( new PhysicsComponent() );
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

void Create()
{
    for( int i=0; i < 10; i++ )
    {
        GameObjects.push_back( ( new GameObject() )->CreateRandom() );
    }
}


////////////////////////////////////////////////////////////////////////////////

void Load( const char* filename )
{
    
}


////////////////////////////////////////////////////////////////////////////////

void Save()
{
    StringBuffer s;
    PrettyWriter< StringBuffer > writer( s );
    
    writer.StartArray();
    
    for( auto o : GameObjects )
    {
        writer.StartObject();
        writer.String( "name" );
        writer.String( o->Name.c_str() );
        writer.EndObject();
    }
    
    writer.EndArray();
    
    std::cout << s.GetString() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
    Create();
    Save();
    return 0;
}

/*
#include "rapidjson/reader.h"
#include <iostream>

using namespace rapidjson;
using namespace std;

struct MyHandler {
    bool Null() { cout << "Null()" << endl; return true; }
    bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
    bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
    bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
    bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
    bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
    bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
    bool String(const char* str, SizeType length, bool copy) {
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool StartObject() { cout << "StartObject()" << endl; return true; }
    bool Key(const char* str, SizeType length, bool copy) {
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    bool StartArray() { cout << "StartArray()" << endl; return true; }
    bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};

int main() {
    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
    
    MyHandler handler;
    Reader reader;
    StringStream ss(json);
    reader.Parse(ss, handler);
    
    return 0;
}
**/

