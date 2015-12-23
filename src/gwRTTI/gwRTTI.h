////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <map>


////////////////////////////////////////////////////////////////////////////////
// compiler shenanigans

#if defined( _MSC_VER )

    #include <stdint.h> // for uint64_t

    #define gwDLL_EXPORT        __declspec( dllexport )
    #define gwDLL_IMPORT        __declspec( dllimport )
    #define gwRTTI_FUNCSIG      __FUNCSIG__
    #define gw_stricmp          _stricmp

#elif defined( __GNUC__ )

    #define gwDLL_EXPORT        __attribute__ ((visibility ("default")))
    #define gwDLL_IMPORT        __attribute__ ((visibility ("default")))
    #define gwRTTI_FUNCSIG      __PRETTY_FUNCTION__
    #define gw_stricmp          strcasecmp

#else

    #error unknown compiler

#endif

#ifdef _USRDLL
    #ifdef GWRTTI_EXPORTS
        #define GWRTTI_API gwDLL_EXPORT
    #else
        #define GWRTTI_API gwDLL_IMPORT
    #endif
#else
    #define GWRTTI_API
#endif



////////////////////////////////////////////////////////////////////////////////

namespace gw
{
    namespace RTTI
    {
        struct TypeInfo;
        
        
        ////////////////////////////////////////////////////////////////////////////////
        // "registry"
        
        extern GWRTTI_API const TypeInfo* Find( const char* );
        
        
        ////////////////////////////////////////////////////////////////////////////////
        // custom attribute
        
        struct Attr
        {
            const char* Key;
            const char* Value;
        };
        
        
        ////////////////////////////////////////////////////////////////////////////////
        // enum constant
        
        struct Constant
        {
            const char* Name;
            int         Value;
        };

        
        ////////////////////////////////////////////////////////////////////////////////
        // object field
        
        struct GWRTTI_API Field
        {
            const char*     Name;   // field name
            const TypeInfo* Type;   // underlying type (actual type maybe a pointer or array)
            
            // getter
            
            void*           ( * Get )( void* );
            
            // custom attributes
            
            Attr*           Attrs;
            int             NumAttrs;
            
            // accessors for array types
            
            std::function< std::pair<void*,const TypeInfo*>() > (*Iterator)( void* );
            std::function< bool(void*) > (*Inserter)( void*, int );
            
            // type hints
            
            bool            IsPointer:1;
            bool            IsArray:1;
            
            Field()
                : Name( nullptr )
                , Type( nullptr )
            
                , Get( nullptr )
            
                , Attrs( nullptr )
                , NumAttrs( 0 )
            
                , Iterator( nullptr )
                , Inserter( nullptr )
            
                , IsPointer( false )
                , IsArray( false )
            {
            }
        };


        ////////////////////////////////////////////////////////////////////////////////
        // TypeInfo

        struct GWRTTI_API TypeInfo
        {
            bool IsA( const TypeInfo* ) const;
            
            
            std::string         Name;               // name
            uint32_t            Hash;               // djb2 hash of name
            const TypeInfo**    BaseClasses;        // inherited types ( IsA(...) )
            
            // specialised functions
            
            void*               ( * Instantiate )();
            const char*         ( * ToString    )( void*, char*, int );
            bool                ( * FromString  )( void*, const char* );
            
            // type members
            
            Field*              Fields;             // classes have fields
            Constant*           Constants;          // enums have constants
            int                 NumMembers;         // num fields or num constants
            
            // custom key/value pair attributes
            
            Attr*               Attrs;
            int                 NumAttrs;
            
            // type hints
            
            bool                IsInstantiated:1;   // set to true after registration complete
            bool                IsFundamental:1;
            bool                IsEnum:1;
            bool                IsPOD:1;
            
            TypeInfo()
            
                : Hash( ~0 )
                , BaseClasses( nullptr )
            
                , Instantiate( nullptr )
                , ToString( nullptr )
                , FromString( nullptr )

                , Fields( nullptr )
                , Constants( nullptr )
                , NumMembers( 0 )
            
                , Attrs( nullptr )
                , NumAttrs( 0 )
            
                , IsInstantiated( false )
                , IsFundamental( false )
                , IsEnum( false )
                , IsPOD( false )
            
            {
            }
        };


        ////////////////////////////////////////////////////////////////////////////////
        // TypeInfo implementation
        
        template< typename T >
        struct TypeInfoImpl : public TypeInfo
        {
            static const TypeInfo* GetType()
            {
                extern GWRTTI_API const TypeInfo* FindOrCreate( const char* );
                
                static const TypeInfo* info = FindOrCreate( TypeInfoImpl<T>::GetName() );
                return info;
            }
            

            //
            // gwRTTI_REGISTER will specialise this function for use, fall back for
            // unregistered types is to use the function signature
            //
            
            static const char* GetName()
            {
                return gwRTTI_FUNCSIG;
            }
            
        
            // specialise this function for custom TypeInfo implementation

            //
            //  template<> void TypeInfoImpl< Vector3 >::Create()
            //  {
            //  }
            //
            //  gwRTTI_REGISTER( Vector3 );
            //
            
            void Create()
            {
            }
        };
        
        
        //
        // type functions, e.g.
        //
        //  Type< float >()
        //  Type( pSomeValue )
        //
        
        template< typename T > inline const TypeInfo* Type()
        {
            return TypeInfoImpl<T>::GetType();
        }
        
        template< typename T > inline const TypeInfo* Type( const T* )
        {
            return TypeInfoImpl<T>::GetType();
        }


        ////////////////////////////////////////////////////////////////////////////////
        // register type during global ctor initialisation - we do our custom setup here
        //
        //  gwRTTI_REGISTER( Vector3 );
        //

        template<typename T>
        struct Register
        {
            Register()
            {
                // create generic entry
                
                auto info = TypeInfoImpl<T>::GetType();
                
                // now fill out entry
                
                auto impl = static_cast< TypeInfoImpl<T>* >( const_cast<TypeInfo*>( info ) );
                
                
                // set some default values
                
                impl->Instantiate       = std::is_constructible<T>::value ? []() -> void* { return new T(); } : nullptr;
                impl->IsFundamental     = std::is_fundamental<T>::value;
                impl->IsEnum            = std::is_enum<T>::value;
                impl->IsPOD             = std::is_pod<T>::value;
                
                if( std::is_enum<T>::value || std::is_fundamental<T>::value )
                {
                    extern const char* ValueTypeToString( const TypeInfo* type, void* obj, char* buffer, int size );
                    extern bool ValueTypeFromString( const TypeInfo* type, void* obj, const char* buffer );
                    
                    impl->ToString   = []( void* obj, char* buf, int size ) { return ValueTypeToString( TypeInfoImpl<T>::GetType(), obj, buf, size ); };
                    impl->FromString = []( void* obj, const char* buf )     { return ValueTypeFromString( TypeInfoImpl<T>::GetType(), obj, buf ); };
                }
                
                
                // call custom type instantiation
                
                impl->Create();
                
                
                // flag as complete
                
                impl->IsInstantiated = true;
            }
        };

        // use __COUNTER__ here for unique names ( without spaces, e.g. "unsigned int" )

        #define __gwRTTI_REGISTER_CONCAT(a,b) a##b
        #define __gwRTTI_REGISTER_CREATENAME(c) __gwRTTI_REGISTER_CONCAT( __gw_rtti_, c )
        #define gwRTTI_REGISTER(T) \
            template<> const char* gw::RTTI::TypeInfoImpl<T>::GetName() { return #T; } \
            static gw::RTTI::Register<T> __gwRTTI_REGISTER_CREATENAME( __COUNTER__ );


        ////////////////////////////////////////////////////////////////////////////////
        // RTTI for classes
        //
        //  class MyThingy
        //  {
        //      gwRTTI( MyThingy )
        //  };
        //

        #define gwRTTI( T ) \
            public: \
                virtual const gw::RTTI::TypeInfo* GetType() const   { return gw::RTTI::Type( this ); } \
                static  const gw::RTTI::TypeInfo* TypeInfo()        { return gw::RTTI::Type<T>(); } \
                friend struct gw::RTTI::TypeInfoImpl<T>; \
        
        //
        // DynamicCast
        //
        //  SomeObject* obj = gw::RTTI::DynamicCast< SomeObject >( thingy );
        //

        template< typename T, typename O >
        T* DynamicCast( O* object )
        {
            // NB: static_cast required to rebase pointer
            return object && object->GetType()->IsA( gw::RTTI::Type<T>() ) ? static_cast<T*>( object ) : nullptr;
        }
    }
}

