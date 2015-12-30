////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <algorithm>
#include <ctype.h>
#include "gwRTTI.h"

namespace gw
{
    namespace RTTI
    {
        //------------------------------------------------------------------------------
        // util functions
        //------------------------------------------------------------------------------
        
        static uint32_t DJB2( const char* str )
        {
            uint32_t hash = 5381;
            
            while( uint32_t c = uint32_t(*str++) )
            {
                hash = ((hash << 5) + hash) + c; // hash * 33 + c
            }
            
            return hash;
        }


        
#if 0
        
        ////////////////////////////////////////////////////////////////////////////////
        // extract type name from the function signature
        
        static void ExtractTypeFromFuncSig( const char* func, char* buf, size_t size )
        {
            #ifdef _MSC_VER
            
                // for MSVC we need to ignore module info `...'::
                //  e.g.
                //      const struct gw::RTTI::TypeInfo *__cdecl gw::RTTI::TypeInfoImpl<class `int __cdecl main(int,char * * const)'::`2'::ObjectB>::GetType(void)
                
                
                // find start of type

                const char* pos = strchr( func, '<' );
                pos = pos ? ++pos : func;
                
                if( gw_strnicmp( pos, "struct ", 7 ) == 0 )
                {
                    pos += 7;
                }
                else if( gw_strnicmp( pos, "class ", 6 ) == 0 )
                {
                    pos += 6;
                }
                else if( gw_strnicmp( pos, "enum ", 5 ) == 0 )
                {
                    pos += 5;
                }

                bool include = true;

                for( int count = 1; *pos && count > 0 && size > 1; ++pos )
                {
                    switch( *pos )
                    {
                        case '`':
                            include = false;
                            break;

                        case '\'':
                            include = true;
                            if( pos[1] == ':' ) ++pos;
                            if( pos[1] == ':' ) ++pos;
                            break;

                        case '>':
                        case '<':

                            if( *pos == '<' )
                            {
                                ++count;
                            }
                            else if( --count == 0 )
                            {
                                continue;
                            }

                        default:

                            if( include )
                            {
                                *buf++ = *pos;
                            }
                            break;
                    }
                }

                *buf = '\0';
            
            #else
            
                // for GNU/Clang the type appended to the end of the function signature, e.g. "[T = Vector3]"
                
                *buf = '\0';
                
                if( auto pos = strstr( func, "= " ) )
                {
                    pos += 2;
                    auto num = std::min( strlen( pos ), size - 1 ) - 1;
                    
                    if( num >= 1 )
                    {
                        memcpy( buf, pos, num );
                        buf[ num ] = '\0';
                    }
                }
            
            #endif
        }
#endif
        
        
        ////////////////////////////////////////////////////////////////////////////////
        // create a unique hash for each decl type from the given function signature

        static uint32_t TypeHashFromFuncSig( const char* func )
        {
            #ifdef _MSC_VER
            
                //
                // in the case of MSCV the function signature can contain "location" info
                //
                // e.g.
                //      class `int __cdecl main(int,char * * const)'::`2'::ObjectB
                //
                // create a hash ignore the `...'::
                //
                
            
                // find start of type

                const char* pos = strchr( func, '<' );
                pos = pos ? ++pos : func;

                // hash type - ignoring 'location' info

                bool include = true;
                uint32_t hash = 5381;

                for( ; *pos; ++pos )
                {
                    if( *pos == '`' )
                    {
                        include = false;
                    }
                    else if( *pos == '\'' )
                    {
                        include = true;
                        if( pos[1] == ':' ) ++pos;
                        if( pos[1] == ':' ) ++pos;
                    }
                    else if( include )
                    {
                        hash = ((hash << 5) + hash) + uint32_t( *pos ); // hash * 33 + c
                    }
                }

                return hash;
                
            #else
            
                // for GNU/Clang it's OK to just hash the function signature
            
                return DJB2( func );
            
            #endif
        }
        
        
        //------------------------------------------------------------------------------
        // Registry
        //------------------------------------------------------------------------------
        
        // as we can't guarentee the order of the global ctor, place "types" inside a singleton
        // so the maps are initialised on first access
        
        struct Types
        {
            static Types& Get() { static Types types; return types; }
                
            std::map< uint32_t, TypeInfo >  ByFuncSig;
            std::map< uint32_t, TypeInfo* > ByName;
        };


        const TypeInfo* Registry::Find( const char* name )
        {
            if( name )
            {
                auto itr = Types::Get().ByName.find( DJB2( name ) );
                
                if( itr != Types::Get().ByName.end() )
                {
                    return itr->second;
                }
            }

            return nullptr;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        
        const TypeInfo* Registry::FindOrCreate( const char* funcsig )
        {
            auto hash = TypeHashFromFuncSig( funcsig );
            return &Types::Get().ByFuncSig[ hash ];
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        
        void Registry::Register( TypeInfo* info )
        {
            info->Hash          = DJB2( info->Name );
            info->IsRegistered  = true;
            
            Types::Get().ByName[ info->Hash ] = info;
        }
        
        
        ////////////////////////////////////////////////////////////////////////////////
        
        void Registry::Dump()
        {
            printf( "Types by name\n" );

            for( auto itr : Types::Get().ByName )
            {
                auto info = itr.second;

                printf( "  0x%08X: %s\n", itr.first, info->Name );
            }

            printf( "\nTypes by func sig\n" );

            for( auto itr : Types::Get().ByFuncSig )
            {
                printf( "  0x%08X: %s\n", itr.first, itr.second.Name );
            }
        }
        

        
        //------------------------------------------------------------------------------
        // TypeInfo
        //------------------------------------------------------------------------------
        
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
        
        
        //------------------------------------------------------------------------------
        // standard value converters
        //------------------------------------------------------------------------------
        
        
        ////////////////////////////////////////////////////////////////////////////////
        // value types to strings
        
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
            
            gw_sprintf( buffer, size, "%i", value );
            return buffer;
        }
        
        static const char* FundamentalTypeToString( const TypeInfo* type, void* obj, char* buffer, int size )
        {
            switch( type->Hash )
            {
                case 0x7c94b391: // bool
                    gw_sprintf( buffer, size, "%s", *((bool*)obj) ? "true" : "false" );
                    break;
                    
                case 0x7c952063: // char
                {
                    auto v = *((char*)obj);
                    gw_sprintf(buffer, size, "%i", v);
                }
                break;
                    
                case 0xf9cba7a0: // unsigned char
                    gw_sprintf( buffer, size, "%hhu", *((unsigned char*)obj) );
                    break;
                    
                case 0x0b888030: // int
                    gw_sprintf( buffer, size, "%i", *((int*)obj) );
                    break;
                    
                case 0xb23c93cd: // unsigned int
                    gw_sprintf( buffer, size, "%u", *((unsigned int*)obj) );
                    break;
                    
                case 0x0f71e19b: // float
                    gw_sprintf( buffer, size, "%f", *((float*)obj) );
                    break;
                    
                case 0xf93d5b20: // double
                    gw_sprintf( buffer, size, "%lf", *((double*)obj) );
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
        // strings -> value types
        
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
            
            return gw_sscanf( buffer, "%u", pValue ) == 1;
        }
        
        static bool FundamentalTypeFromString( const TypeInfo* type, void* obj, const char* buffer )
        {
            switch( type->Hash )
            {
                case 0x7c94b391: // bool
                    *((bool*)obj) = gw_stricmp( buffer, "true" ) == 0;
                    return true;
                    
                case 0x7c952063: // char
                {
                    int v;

                    if( gw_sscanf(buffer, "%i", &v) == 1 && v >= 0 && v <= 255 )
                    {
                        *((char*)obj) = char(v);
                        return true;
                    }
                }
                break;
                    
                case 0xf9cba7a0: // unsigned char
                    return gw_sscanf( buffer, "%hhu", (unsigned char*)obj ) == 1;
                    
                case 0x0b888030: // int
                    return gw_sscanf( buffer, "%i", (int*)obj ) == 1;
                    
                case 0xb23c93cd: // unsigned int
                    return gw_sscanf( buffer, "%u", (int*)obj ) == 1;
                    
                case 0x0f71e19b: // float
                    return gw_sscanf( buffer, "%f", (float*)obj ) == 1;
                    
                case 0xf93d5b20: // double
                    return gw_sscanf( buffer, "%lf", (double*)obj ) == 1;
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
    }
}

