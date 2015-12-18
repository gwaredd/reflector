////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include "gw/gw.h"
#include "gwl/gwStringRef.h"

////////////////////////////////////////////////////////////////////////////////

#ifdef gwDLL
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

        struct GWRTTI_API Property
        {
            const char* Name;
            void* (*Get)( void* );
            void* Attrs; // key,value pairs?

            bool IsPointer;

            const struct TypeInfo* Info;

            Property()
                : Name( nullptr )
                , Get( nullptr )
                , Attrs( nullptr )
                , IsPointer( false )
                , Info( nullptr )
            {
            }
        };


        ////////////////////////////////////////////////////////////////////////////////

        struct GWRTTI_API TypeInfo
        {
            bool    IsA( const TypeInfo* ) const;
            void*   Get( void*, const char* ) const;


            unsigned int    UID;
            std::string     Name;
            uint64_t        Hash;
            void* (*Instantiate)();

            bool            IsFundamental:1;
            bool            IsClass:1;
            bool            IsPOD:1;
            bool            IsEnum:1;
            bool            IsPointer:1;

            Property*       Properties;
            int             NumProperties;
            const TypeInfo** Inherits;
            // contants ...

            TypeInfo()
                : UID( 0 )
                , Hash( ~0 )
                , Properties( nullptr )
                , NumProperties( 0 )
                , Inherits( nullptr )

                , IsFundamental( false )
                , IsClass( false )
                , IsPOD( false )
                , IsEnum( false )
                , IsPointer( false )

                , Instantiate( nullptr )
            {
            }
        };



        ////////////////////////////////////////////////////////////////////////////////

        class GWRTTI_API Registry
        {
            public:

                static Registry& Instance() { static Registry sRegistry; return sRegistry; }

                const TypeInfo* Find( const char* );
                void*           Instantiate( const char* );

                TypeInfo*       FindOrCreate( const char* );

            protected:

                Registry()
                    : mID( 0 )
                {
                }

                unsigned int mID;
                std::map< StringRef, TypeInfo > mTypes; // using StringRef's here to save memory allocation / fast lookup
        };



        ////////////////////////////////////////////////////////////////////////////////
        // templated implementation of specific type info
        // we use template specialisation to provide the

        template< typename T >
        struct TypeInfoImpl : public TypeInfo
        {
            // specialise Create() to provide custom type implementation

            void Create() {}

            // return typeinfo for this class

            static const TypeInfo* Class()
            {
                // first call we fetch from registry - this is because of memory is local to "shared boundaries"
                // (i.e. Windows DLL's). This isn't a problem if the library is statically linked

                #ifdef gwCOMPILER_MSVC
                    static const TypeInfo* info = Initialise( gw::RTTI::Registry::Instance().FindOrCreate( __FUNCSIG__ ) );
                #else
                    static const TypeInfo* info = Initialise( gw::RTTI::Registry::Instance().FindOrCreate( __PRETTY_FUNCTION__ ) );
                #endif

                return info;
            }


            // some default bonus initialisation

            protected:

                static const TypeInfo* Initialise( const TypeInfo* info )
                {
                    auto p = reinterpret_cast< TypeInfoImpl<T>* >( const_cast<TypeInfo*>( info ) );

                    p->Instantiate     = std::is_constructible<T>::value ? []() -> void* { return new T(); } : nullptr;
                    p->IsFundamental   = std::is_fundamental<T>::value;
                    p->IsClass         = std::is_class<T>::value;
                    p->IsPOD           = std::is_pod<T>::value;
                    p->IsEnum          = std::is_enum<T>::value;
                    p->IsPointer       = std::is_pointer<T>::value;

                    p->Create();

                    return p;
                }
        };


        ////////////////////////////////////////////////////////////////////////////////
        // register type to do custom initialisation during global init

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
        // a convenient shorthand - e.g. RTTI::Type<float>(), or RTTI::Type( p )

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

        #define gwRTTI(T) \
            public: \
                virtual const gw::RTTI::TypeInfo* Type() const { return gw::RTTI::Type( this ); } \
                friend void gw::RTTI::TypeInfoImpl<T>::Create();


        template< typename T, typename O >
        T* Cast( O* object )
        {
            // NB: static_cast required to rebase pointer
            return object && object->Type()->IsA( RTTI::Type<T>() ) ? static_cast<T*>( object ) : nullptr;
        }
    }
}

