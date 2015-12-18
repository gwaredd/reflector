////////////////////////////////////////////////////////////////////////////////

#include <map>
#include "gwRTTI.h"

namespace gw
{
    namespace RTTI
    {
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
        
        void* TypeInfo::Get( void* obj, const char* name ) const
        {
            for( auto i=0; i < NumProperties; ++i )
            {
                if( strcmp( Properties[ i ].Name, name ) != 0 )
                {
                    continue;
                }
                
                auto get = Properties[ i ].Get;
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
            
            StringRef name;
            
            auto start = strchr( func, '<' );
            
            if( start )
            {
                auto end = strchr( ++start, '>' );
                name = end ? StringRef( start, end ) : func;
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
            
            info->Name  = std::string( name );
            info->UID   = ++mID;
            info->Hash  = DJB2_64( info->Name.c_str() );
            
            return info;
        }
    }
}

//gwRTTI_REGISTER( bool );
//gwRTTI_REGISTER( char );
//gwRTTI_REGISTER( unsigned char );
//gwRTTI_REGISTER( short );
//gwRTTI_REGISTER( int );
//gwRTTI_REGISTER( unsigned int );
//gwRTTI_REGISTER( float );
//gwRTTI_REGISTER( double );


//        int32_t Object::* pointer_to_member_variable = &Object::m_Member;
//        // These are typically dereferenced with an instance of the object
//        // type (just like member function pointers):
//        Object object, *pointer = new Object;
//        int32_t value1 = object.*pointer_to_member_variable;
//        int32_t value2 = pointer->*pointer_to_member_variable;
//
//        // To compute the offset from a pointer to a member variable
//        template< class ObjectT, class DataT >
//        uint32_t GetFieldOffset( ObjectT DataT::* field )
//        {
//            // a pointer-to-member is really just an offset value
//            // disguised by the compiler
//            return (uint32_t) (uintptr_t) &( ((ObjectT*)NULL)->*field );
//        }

