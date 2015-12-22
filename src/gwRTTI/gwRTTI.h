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
        
        struct TypeInfo;

        struct GWRTTI_API Field
        {
            const char*     Name;   // field name
            const TypeInfo* Type;   // underlying type (actual type maybe a pointer or array)
            
            // getter
            
            void*           ( * Get )( void* );
            
            // custom attributes
            
            Attr*           Attrs;
            int             NumAttrs;
            
            // array accessors
            
            std::function< std::pair<void*,const TypeInfo*>() > (*Iterator)( void* );
            std::function< bool(void*) > (*Inserter)( void*, int );
            
            // pointer types
            
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
        // Base TypeInfo

        struct GWRTTI_API TypeInfo
        {
            bool IsA( const TypeInfo* ) const;
            
            
            std::string         Name;               // name
            uint64_t            Hash;               // djb2_64 hash of name
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
            
            bool                IsInstantiated:1;   // set to true after registration
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
        // registry should be safe accross shared boundaries (windows DLL's)
        
        extern GWRTTI_API const TypeInfo* Find( const char* );
        extern GWRTTI_API const TypeInfo* FindOrCreate( const char* );


        ////////////////////////////////////////////////////////////////////////////////
        // TypeInfo specialisation

        template< typename T >
        struct TypeInfoImpl : public TypeInfo
        {
            public:
            
                // specialise Create() to provide custom type implementation
                void Create() {}

                static const TypeInfo* GetType()
                {
                    static const TypeInfo* info = FindOrCreate( gwRTTI_FUNCSIG );
                    return info;
                }
        };


        ////////////////////////////////////////////////////////////////////////////////
        // register type during global ctor initialisation - we do our custom setup here

        template<typename T>
        struct Register
        {
            Register()
            {
                // create generic entry
                
                auto generic_info = TypeInfoImpl<T>::GetType();
                
                // now fill in with custom value
                
                auto info = reinterpret_cast< TypeInfoImpl<T>* >( const_cast<TypeInfo*>( generic_info ) );
                
                // set some default values we can infer from type
                
                info->Instantiate       = std::is_constructible<T>::value ? []() -> void* { return new T(); } : nullptr;
                info->IsFundamental     = std::is_fundamental<T>::value;
                info->IsEnum            = std::is_enum<T>::value;
                info->IsPOD             = std::is_pod<T>::value;
                
                if( std::is_enum<T>::value || std::is_fundamental<T>::value )
                {
                    extern const char* ValueTypeToString( const TypeInfo* type, void* obj, char* buffer, int size );
                    extern bool ValueTypeFromString( const TypeInfo* type, void* obj, const char* buffer );
                    
                    info->ToString   = []( void* obj, char* buf, int size ) { return ValueTypeToString( TypeInfoImpl<T>::GetType(), obj, buf, size ); };
                    info->FromString = []( void* obj, const char* buf )     { return ValueTypeFromString( TypeInfoImpl<T>::GetType(), obj, buf ); };
                }
                
                // now call custom type instantiation
                
                info->Create();
                
                
                // flag as complete (useful if we're dynamically loading type info)
                
                info->IsInstantiated = true;
            }
        };

        // use __COUNTER__ as some names have spaces in them (e.g. "unsigned int")

        #define __gwRTTI_REGISTER_CONCAT(a,b) a##b
        #define __gwRTTI_REGISTER_CREATENAME(c) __gwRTTI_REGISTER_CONCAT( __gw_rtti_, c )
        #define gwRTTI_REGISTER(t) static gw::RTTI::Register<t> __gwRTTI_REGISTER_CREATENAME( __COUNTER__ );


        ////////////////////////////////////////////////////////////////////////////////
        // useful shorthand functions

        template< typename T >
        inline const TypeInfo* Type()
        {
            return TypeInfoImpl<T>::GetType();
        }

        template< typename T >
        inline const TypeInfo* Type( const T* )
        {
            return TypeInfoImpl<T>::GetType();
        }


        ////////////////////////////////////////////////////////////////////////////////
        // RTTI for classes

        #define gwRTTI( T ) \
            public: \
                virtual const gw::RTTI::TypeInfo* GetType() const { return gw::RTTI::Type( this ); } \
                static const gw::RTTI::TypeInfo* TypeInfo() { return gw::RTTI::Type<T>(); } \
                friend struct gw::RTTI::TypeInfoImpl<T>; \
        


        template< typename T, typename O >
        T* DynamicCast( O* object )
        {
            // NB: static_cast required to rebase pointer
            return object && object->GetType()->IsA( RTTI::Type<T>() ) ? static_cast<T*>( object ) : nullptr;
        }
    }
}

