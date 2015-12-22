////////////////////////////////////////////////////////////////////////////////

#include <map>
#include "gwRTTI.h"

namespace gw
{
    namespace RTTI
    {
        ////////////////////////////////////////////////////////////////////////////////
        
        class Registry
        {
            public:
                
                // as we can't guarentee the global ctor order, new up the Registry on demand
            
                static Registry* Instance()
                {
                    if( sRegistry == nullptr )
                    {
                        sRegistry=  new Registry();
                    }
                    
                    return sRegistry;
                }
                
                const TypeInfo*     Find( const char* );
                TypeInfo*           Create( const char* );
            
                
            protected:
            
                static Registry* sRegistry;
                std::map< std::string, TypeInfo > mTypes;
        };
        
        Registry* Registry::sRegistry = nullptr;
        
        const TypeInfo* Find( const char* name )
        {
            return Registry::Instance()->Find( name );
        }
        
        const TypeInfo* Create( const char* name )
        {
            return Registry::Instance()->Create( name );
        }
        
        
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
        // for convenience - standard conversion of value types to strings
        
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
        // for convenience - standard conversion of strings -> value types
        
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
            const TypeInfo** info = BaseClasses;

            do
            {
                if( *info == type )
                {
                    return true;
                }
                
                ++info;
            }
            while( *info );
            
            return false;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////

        const TypeInfo* Registry::Find( const char* name )
        {
            auto itr = mTypes.find( name );
            return itr != mTypes.end() ? &itr->second : nullptr;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        // extract type name from function signature

        std::string ExtractName( const char* func )
        {
            // function signatures look something like this ...
            //  static const gw::RTTI::TypeInfo *gw::RTTI::TypeInfoImpl< std::string<char> >::Initialise()
            
            std::string name;
            
            // extract the type from within the <>
            
            auto start = strchr( func, '<' );
            
            if( start )
            {
                // balance the <>
                
                auto end   = ++start;
                auto count = 1;
                
                while( *end )
                {
                    if( *end == '<' )
                    {
                        ++count;
                    }
                    else if( *end == '>' )
                    {
                        if( --count == 0 )
                        {
                            break;
                        }
                    }
                    
                    ++end;
                }
                
                name = *end ? std::string( start, end - start ) : func;
            }
            else
            {
                name = func;
            }
            
            return name;
        }


        ////////////////////////////////////////////////////////////////////////////////
        
        TypeInfo* Registry::Create( const char* name )
        {
            auto info = &mTypes[ name ];
            
            info->Name  = name;
            info->Hash  = DJB2_64( name );
            
            return info;
        }
    }
}

