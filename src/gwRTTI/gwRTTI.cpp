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
        
        static const char* EnumTypeToString( const TypeInfo* type, void* obj, char* buffer, int size )
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
        
        static const char* FundamentalTypeToString( const TypeInfo* type, void* obj, char* buffer, int size )
        {
            switch( type->Hash )
            {
                case 0x000000017C94B391: // bool
                    sprintf( buffer, "%s", *((bool*)obj) ? "true" : "false" );
                    break;
                    
                case 0x000000017C952063: // char
                    sprintf( buffer, "%c", *((char*)obj) );
                    break;
                    
                case 0x6A011564F9CBA7A0: // unsigned char
                    sprintf( buffer, "%hhu", *((unsigned char*)obj) );
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
            
            return buffer;
        }
        
        
        const char* ValueTypeToString( const TypeInfo* type, void* obj, char* buffer, int size )
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
                return EnumTypeToString( type, obj, buffer, size );
            }
            else if( type->IsFundamental )
            {
                return FundamentalTypeToString( type, obj, buffer, size );
            }

            return buffer;
        }

        
        ////////////////////////////////////////////////////////////////////////////////
        
        static bool EnumTypeFromString( const TypeInfo* type, void* obj, const char* buffer )
        {
            auto pValue = (int*)obj;
            
            if( type->Constants )
            {
                auto c = type->Constants;
                auto i = type->NumMembers;
                
                while( i-- )
                {
                    if( gw_stricmp( buffer, c->Name ) == 0 )
                    {
                        *pValue = c->Value;
                        return true;
                    }
                    
                    ++c;
                }
            }
            
            return sscanf( buffer, "%u", pValue ) == 1;
        }
        
        static bool FundamentalTypeFromString( const TypeInfo* type, void* obj, const char* buffer )
        {
            switch( type->Hash )
            {
                case 0x000000017C94B391: // bool
                    *((bool*)obj) = gw_stricmp( buffer, "true" ) == 0;
                    return true;
                    
                case 0x000000017C952063: // char
                    return sscanf( buffer, "%c", (char*)obj ) == 1;
                    
                case 0x6A011564F9CBA7A0: // unsigned char
                    return sscanf( buffer, "%hhu", (unsigned char*)obj ) == 1;
                    
                case 0x000000000B888030: // int
                    return sscanf( buffer, "%i", (int*)obj ) == 1;
                    
                case 0xDC6CA38EB23C93CD: // unsigned int
                    return sscanf( buffer, "%u", (int*)obj ) == 1;
                    
                case 0x000000310F71E19B: // float
                    return sscanf( buffer, "%f", (float*)obj ) == 1;
                    
                case 0x00000652F93D5B20: // double
                    return sscanf( buffer, "%lf", (double*)obj ) == 1;
            }
            
            return false;
        }
        
        bool ValueTypeFromString( const TypeInfo* type, void* obj, const char* buffer )
        {
            if( type == nullptr || obj == nullptr || buffer == nullptr )
            {
                return false;
            }
            
            if( type->IsEnum )
            {
                return EnumTypeFromString( type, obj, buffer );
            }
            else if( type->IsFundamental )
            {
                return FundamentalTypeFromString( type, obj, buffer );
            }
            
            return false;
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

