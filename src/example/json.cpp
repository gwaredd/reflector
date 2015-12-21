////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include "json.h"


static char buffer[ 256 ];


////////////////////////////////////////////////////////////////////////////////

void ExampleWrite( PrettyWriter< FileWriteStream >& writer, const TypeInfo* info, void* obj )
{
    writer.StartObject();
   
    writer.String( "Type" );
    writer.String( info->Name.c_str() );

    if( info->Fields )
    {
        for( int i=0; i < info->NumMembers; i++ )
        {
            auto& field = info->Fields[i];
            auto ptr    = field.Get( obj );
            
            writer.String( field.Name );
            
            // if value type - convert to string
            
            if( field.Info->ToString )
            {
                writer.String( field.Info->ToString( ptr, buffer, 256 ) );
            }
            
            // if array
            
            else if( field.Iterator )
            {
                auto itr = field.Iterator( obj );
                
                writer.StartArray();
                
                auto v = itr();
                
                while( v.second )
                {
                    ExampleWrite( writer, v.second, v.first );
                    v = itr();
                }
                
                writer.EndArray();
            }
            
            // recurse on object type ...
            
            else
            {
                ExampleWrite( writer, field.Info, ptr );
            }
        }
    }

    writer.EndObject();
}


////////////////////////////////////////////////////////////////////////////////

void* ExampleRead( void* obj, const Document::GenericValue& node )
{
    // find type
    
    if( node.HasMember( "Type" ) == false )
    {
        return nullptr;
    }
    
    auto& registry = gw::RTTI::Registry::Instance();
    auto type = node[ "Type" ].GetString();
    
    auto typeInfo = registry.Find( type );
    
    if( typeInfo == nullptr )
    {
        return nullptr;
    }
    
    // instantiate
    
    if( obj == nullptr )
    {
        obj = typeInfo->Instantiate();
    }
    
    
    // read fields
    
    if( typeInfo->Fields == nullptr )
    {
        return obj;
    }

    auto field = typeInfo->Fields;
    auto numFields = typeInfo->NumMembers;
    
    while( numFields-- )
    {
        if( node.HasMember( field->Name ) == false )
        {
            field++;
            continue;
        }
        
        auto& fieldNode = node[ field->Name ];
        auto pMember = field->Get( obj );
        
        if( fieldNode.IsString() )
        {
            //field->Info->ToString
            std::cout << field->Name << " = " << fieldNode.GetString() << std::endl;
        }
        else if( fieldNode.IsArray() )
        {
            std::cout << field->Name << " is array" << std::endl;
            
        }
        else if( fieldNode.IsObject() )
        {
            std::cout << field->Name << " is object" << std::endl;
            ExampleRead( pMember, fieldNode );
        }
        else
        {
            std::cerr << "Unknown type " << field->Name << std::endl;
        }
        
        field++;
    
    }
    
    std::cout << std::endl;
    
    return obj;
}

