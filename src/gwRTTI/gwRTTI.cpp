////////////////////////////////////////////////////////////////////////////////

#include <map>
#include "gwRTTI.h"

namespace gw
{
    namespace RTTI
    {
        ////////////////////////////////////////////////////////////////////////////////
        // quick string hashing function
        
        static uint64_t DJB2_64( const char* str )
        {
            uint64_t hash = 5381;
            
            if( str )
            {
                while( uint64_t c = (uint64_t) ( *str++ ) )
                {
                    hash = ((hash << 5) + hash) + c; // hash * 33 + c
                }
            }
            
            return hash;
        }
        

        ////////////////////////////////////////////////////////////////////////////////
        
        static const char* __EnumTypeToString( const TypeInfo* type, void* obj, char* buffer, int size )
        {
            // look up value
            
            int value = *((int*)obj);
            
            if( type->Constants )
            {
                auto c = type->Constants;
                auto i = type->NumMembers;
                
                while( i-- )
                {
                    if( c->Value == value )
                    {
                        return c->Name;
                    }
                    
                    ++c;
                }
            }
            
            sprintf( buffer, "%i", value );
            return buffer;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        
        const char* __ValueTypeToString( const TypeInfo* type, void* obj, char* buffer, int size )
        {
            // validate params
            
            if( buffer == nullptr || size <= 0 )
            {
                return nullptr;
            }
            
            *buffer = '\0';
            
            if( obj == nullptr || type == nullptr )
            {
                return buffer;
            }
            
            if( type->IsEnum )
            {
                return __EnumTypeToString( type, obj, buffer, size );
            }
            else if( type->IsFundamental )
            {
                // convert fundamental to string
                
                switch( type->Hash )
                {
                    case 0x000000017C94B391: // bool
                        sprintf( buffer, "%s", *((bool*)obj) ? "true" : "false" );
                        break;
                        
                    case 0x000000017C952063: // char
                        sprintf( buffer, "%c", *((char*)obj) );
                        break;
                        
                    case 0x6A011564F9CBA7A0: // unsigned char
                        sprintf( buffer, "%u", *((unsigned char*)obj) );
                        break;
                        
                    case 0x000000000B888030: // int
                        sprintf( buffer, "%i", *((int*)obj) );
                        break;
                        
                    case 0xDC6CA38EB23C93CD: // unsigned int
                        sprintf( buffer, "%u", *((unsigned int*)obj) );
                        break;
                        
                    case 0x000000310F71E19B: // float
                        sprintf( buffer, "%f", *((float*)obj) );
                        break;
                        
                    case 0x00000652F93D5B20: // double
                        sprintf( buffer, "%lf", *((double*)obj) );
                        break;
                }
            }

            return buffer;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        
        bool TypeInfo::IsA( const TypeInfo* type ) const
        {
            const TypeInfo** info = Inherits;

            do
            {
                if( (*info)->UID == type->UID )
                {
                    return true;
                }
                
                ++info;
            }
            while( *info );
            
            return false;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        
        void* TypeInfo::GetField( void* obj, const char* name ) const
        {
            if( IsEnum )
            {
                return nullptr;
            }
            
            for( auto i=0; i < NumMembers; ++i )
            {
                if( strcmp( Fields[ i ].Name, name ) != 0 )
                {
                    continue;
                }
                
                auto get = Fields[ i ].Get;
                return get ? get( obj ) : nullptr;
            }
            
            return nullptr;
        }


        
        ////////////////////////////////////////////////////////////////////////////////

        const TypeInfo* Registry::Find( const char* name  )
        {
            auto itr = mTypes.find( name );
            return itr != mTypes.end() ? &itr->second : nullptr;
        }
        

        ////////////////////////////////////////////////////////////////////////////////
        
        void* Registry::Instantiate( const char* name  )
        {
            auto info = Find( name );
            return info && info->Instantiate ? info->Instantiate() : nullptr;
        }
        

        ////////////////////////////////////////////////////////////////////////////////
        
        TypeInfo* Registry::FindOrCreate( const char* func )
        {
            // extract type name from function signature
            
            std::string name;
            
            auto start = strchr( func, '<' );
            
            if( start )
            {
                auto end = strchr( ++start, '>' );
                name = end ? std::string( start, end - start ) : func;
            }
            else
            {
                name = func;
            }
            
            // return existing if it exists
            
            auto itr = mTypes.find( name );
            
            if( itr != mTypes.end() )
            {
                return &itr->second;
            }
            
            // otherwise create new entry
            
            auto info = &mTypes[ name ];
            
            info->Name  = name;
            info->UID   = ++mID;
            info->Hash  = DJB2_64( info->Name.c_str() );
            
            return info;
        }
    }
}

