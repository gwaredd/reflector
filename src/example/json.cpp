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
            
            if( field.Type->ToString )
            {
                writer.String( field.Type->ToString( ptr, buffer, 256 ) );
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
                ExampleWrite( writer, field.Type, ptr );
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
    
    auto type = node[ "Type" ].GetString();
    
    auto typeInfo = gw::RTTI::Find( type );
    
    if( typeInfo == nullptr )
    {
        return nullptr;
    }
    
    
    // instantiate if we weren't already given an object to fill out
    
    if( obj == nullptr )
    {
        obj = typeInfo->Instantiate();
    }
    
    
    // read fields
    
    if( typeInfo->Fields == nullptr )
    {
        return obj;
    }

    
    auto numFields = typeInfo->NumMembers;
    
    for( auto field = typeInfo->Fields; numFields--; ++field )
    {
        if( node.HasMember( field->Name ) == false )
        {
            continue;
        }
        
        auto& fieldNode = node[ field->Name ];
        
        
        // if value type ...
        
        if( fieldNode.IsString() )
        {
            if( field->Type->FromString )
            {
                if( field->Type->FromString( field->Get( obj ), fieldNode.GetString() ) == false )
                {
                    std::cerr << "Failed to convert '" << fieldNode.GetString() << "' to " << field->Name << std::endl;
                }
            }
            else
            {
                std::cerr << "No value converter for " << field->Name << std::endl;
            }
        }
        
        
        // if array of objects ...

        else if( fieldNode.IsArray() )
        {
            // make sure we can insert new objects here ...
            
            if( field->IsArray == false )
            {
                std::cout << field->Name << " is not an array" << std::endl;
                continue;
            }
            
            if( field->Inserter == nullptr )
            {
                std::cout << field->Name << " has no inserter" << std::endl;
                continue;
            }
            
            auto inserter = field->Inserter( obj, fieldNode.Size() );
            
            if( !inserter )
            {
                std::cout << field->Name << " did not return an inserter" << std::endl;
                continue;
            }
            
            // create objects
            
            for( auto itr = fieldNode.Begin(); itr != fieldNode.End(); ++itr )
            {
                if( itr->HasMember( "Type" ) == false )
                {
                    std::cout << "Unknown type of element for " << field->Name << std::endl;
                    continue;
                }
                
                auto& elementNode = *itr;
                auto elementType = gw::RTTI::Find( elementNode[ "Type" ].GetString() );
                
                if( elementType->IsA( field->Type ) == false )
                {
                    std::cout << "Element for " << field->Name << " is incorrect type" << std::endl;
                    continue;
                }
                
                auto element = ExampleRead( nullptr, elementNode );
                
                if( element == nullptr || inserter( element ) == false )
                {
                    std::cout << "Failed to insert object into " << field->Name << std::endl;
                    break;
                }
                
            }
        }
        
        
        // if sub-object
        
        else if( fieldNode.IsObject() )
        {
            ExampleRead( field->Get( obj ), fieldNode );
        }
        
        
        // eek!
        
        else
        {
            std::cerr << "Unknown type " << field->Name << std::endl;
        }
    }
    
    return obj;
}

