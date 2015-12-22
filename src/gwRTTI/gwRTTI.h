////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <map>


////////////////////////////////////////////////////////////////////////////////
// compiler shenanigans

#if defined( _MSC_VER )

    #include <stdint.h> // for uint64_t

    #define gwDLL_EXPORT __declspec( dllexport )
    #define gwDLL_IMPORT __declspec( dllimport )

    #define gw_stricmp _stricmp

#elif defined( __GNUC__ )

    #define gwDLL_EXPORT __attribute__ ((visibility ("default")))
    #define gwDLL_IMPORT __attribute__ ((visibility ("default")))

    #define gw_stricmp strcasecmp

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
            
                , IsFundamental( false )
                , IsEnum( false )
                , IsPOD( false )
            
            {
            }
        };


        ////////////////////////////////////////////////////////////////////////////////
        // registry should be safe accross shared boundaries (windows DLL's)
        
        extern GWRTTI_API const TypeInfo* Find( const char* );
        extern GWRTTI_API const TypeInfo* Create( const char* );


        ////////////////////////////////////////////////////////////////////////////////
        // TypeInfo specialisation
        
        template< typename T >
        struct TypeInfoImpl : public TypeInfo
        {
            public:
            
                // specialise Create() to provide custom type implementation
                void Create() {}

                static const TypeInfo* Class()
                {
                    // first call instantiates the type (which should be from a gwRTTI_REGISTER)
                    static const TypeInfo* info = CreateTypeInfo();
                    return info;
                }


            protected:
            
                // instantiate type
            
                static const TypeInfo* CreateTypeInfo()
                {
                    // get type name
                    
                    extern std::string ExtractName( const char* );
                    
                    #ifdef _MSC_VER
                        auto name = ExtractName( __FUNCSIG__ );
                    #else
                        auto name = ExtractName( __PRETTY_FUNCTION__ );
                    #endif
                    
                    // return existing entry if it exists
                    
                    auto info = gw::RTTI::Find( name.c_str() );
                    
                    if( info == nullptr )
                    {
                        // create it if it doesn't
                        
                        info = gw::RTTI::Create( name.c_str() );
                        
                        // set some default values
                        
                        auto p = reinterpret_cast< TypeInfoImpl<T>* >( const_cast<TypeInfo*>( info ) );

                        p->Instantiate      = std::is_constructible<T>::value ? []() -> void* { return new T(); } : nullptr;
                        p->IsFundamental    = std::is_fundamental<T>::value;
                        p->IsEnum           = std::is_enum<T>::value;
                        p->IsPOD            = std::is_pod<T>::value;
                        
                        if( std::is_enum<T>::value || std::is_fundamental<T>::value )
                        {
                            extern const char* ValueTypeToString( const TypeInfo* type, void* obj, char* buffer, int size );
                            extern bool ValueTypeFromString( const TypeInfo* type, void* obj, const char* buffer );
                            
                            p->ToString   = []( void* obj, char* buf, int size ) { return ValueTypeToString( TypeInfoImpl<T>::Class(), obj, buf, size ); };
                            p->FromString = []( void* obj, const char* buf )     { return ValueTypeFromString( TypeInfoImpl<T>::Class(), obj, buf ); };
                        }
                        
                        // now call custom type instantiation

                        p->Create();
                    }

                    return info;
                }
        };


        ////////////////////////////////////////////////////////////////////////////////
        // register type during global ctor initialisation

        template<typename T>
        struct Register
        {
            Register()
            {
                TypeInfoImpl<T>::Class();
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
            return TypeInfoImpl<T>::Class();
        }

        template< typename T >
        inline const TypeInfo* Type( const T* )
        {
            return TypeInfoImpl<T>::Class();
        }


        ////////////////////////////////////////////////////////////////////////////////
        // RTTI for classes

        #define gwRTTI( T ) \
            public: \
                virtual const gw::RTTI::TypeInfo* GetType() const { return gw::RTTI::Type( this ); } \
                static const gw::RTTI::TypeInfo* Class() { return gw::RTTI::Type<T>(); } \
                friend struct gw::RTTI::TypeInfoImpl<T>; \
        


        template< typename T, typename O >
        T* DynamicCast( O* object )
        {
            // NB: static_cast required to rebase pointer
            return object && object->GetType()->IsA( RTTI::Type<T>() ) ? static_cast<T*>( object ) : nullptr;
        }
    }
}

