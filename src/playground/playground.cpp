////////////////////////////////////////////////////////////////////////////////
//
//
// http://www.axelmenzel.de/articles/rtti
// http://stackoverflow.com/questions/1129894/why-cant-you-use-offsetof-on-non-pod-structures-in-c
//
// http://www.gamasutra.com/view/news/128978/Reflection_in_C_The_simple_implementation_of_Splinter_Cell.php
// http://www.gamasutra.com/view/feature/6379/sponsored_feature_behind_the_.php?print=1
//
//

#include "classes.h"

using namespace gw;




////////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
//    rtti = RTTIImpl<MyClass>::Class();
//    rtti = MyClass::Class();
//    rtti = Registry::Find( "MyClassRTTI" );
//    rtti = Type<MyClass>();
    
    //typeid(A).name();


    auto t = RTTI::Type<float>();
    t = RTTI::Type<int>();
    t = RTTI::Type<Vector3>();

    if( RTTI::Type<C>()->IsA( RTTI::Type<B>() ) )
    {
        gwTrace( "ok" );
    }

    if( RTTI::Type<A>()->IsA( RTTI::Type<C>() ) )
    {
        gwTrace( "not ok" );
    }
    
    Vector3 v;
    t = RTTI::Type( &v );
    
    float* pV = (float*) t->Properties[1].Get( &v );
    
    gwTrace( "v is %f", *pV );
    
    pV = (float*) t->Get( &v, "z" );
    gwTrace( "v is %f", *pV );
    

    A a;
    C c;

//    c.mA = 15.f;
    c.mB = 25.f;
    c.mC = 45.f;

    if( c.Type()->IsA( RTTI::Type( &a ) ) )
    {
        gwTrace( "ok" );
    }

    auto cp = RTTI::Cast<C>( &c );

    t = RTTI::Type( cp );

//    auto v = (float*) t->Properties[0].Get( cp );
//    gwTrace( "v: %f", *v );


//    Vector3* v = (Vector3*) t->Instantiate();

    gwTrace( "test" );

    return 0;
}



////////////////////////////////////////////////////////////////////////////////
#if 0



// constexpr std::size_t offset_of()
// {
//     return reinterpret_cast<std::size_t>(&(((T*)0)->*M));
// }

// template< typename C, typename T > size_t offset_of( T C::* p )
// {
// //     return reinterpret_cast<std::size_t>( &(((C*)nullptr)->value) );
//     return reinterpret_cast<std::size_t>( &(((C*)nullptr)->T) );
// }

#define offset_of( C, M ) reinterpret_cast<std::size_t>( &(((C*)nullptr)->M) )

// template< typename C, typename T > size_t read( C* o, T C::* p )
// {
//     return reinterpret_cast<std::size_t>( &((o)->value) );
// }
#endif

