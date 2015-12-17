////////////////////////////////////////////////////////////////////////////////
//
// This file has been auto-genertated
//
//  source  : ../../tests/test.json
//  date    :
//
////////////////////////////////////////////////////////////////////////////////

#include "gw/gw.h"
#include "gw/gwRTTI.h"

// TODO: attrs, bases

using namespace gw::RTTI;














////////////////////////////////////////////////////////////////////////////////
// common types

gwRTTI_REGISTER( bool );
gwRTTI_REGISTER( char );
gwRTTI_REGISTER( unsigned char );
gwRTTI_REGISTER( short );
gwRTTI_REGISTER( int );
gwRTTI_REGISTER( unsigned int );
gwRTTI_REGISTER( float );
gwRTTI_REGISTER( double );


////////////////////////////////////////////////////////////////////////////////
// AClass

template<> void TypeInfoImpl< AClass >::Create()
{
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    






    

    static Property p[ 8 ];
        
    






    p[0].Name  = "a";
    p[0].Info  = Type<float>();
    p[0].Get   = []( void* o ) -> void* { return &reinterpret_cast<AClass*>(o)->a; };
        
    






    p[1].Name  = "b";
    p[1].Info  = Type<const float *>();
    p[1].Get   = []( void* o ) -> void* { return &reinterpret_cast<AClass*>(o)->b; };
        
    






    p[2].Name  = "buffer";
    p[2].Info  = Type<char [32]>();
    p[2].Get   = []( void* o ) -> void* { return &reinterpret_cast<AClass*>(o)->buffer; };
        
    






    p[3].Name  = "v3";
    p[3].Info  = Type<Vector3>();
    p[3].Get   = []( void* o ) -> void* { return &reinterpret_cast<AClass*>(o)->v3; };
        
    






    p[4].Name  = "pv3";
    p[4].Info  = Type<Vector3 *>();
    p[4].Get   = []( void* o ) -> void* { return &reinterpret_cast<AClass*>(o)->pv3; };
        
    






    p[5].Name  = "rv3";
    p[5].Info  = Type<Vector3 &>();
    p[5].Get   = []( void* o ) -> void* { return &reinterpret_cast<AClass*>(o)->rv3; };
        
    






    p[6].Name  = "c";
    p[6].Info  = Type<float>();
    p[6].Get   = []( void* o ) -> void* { return &reinterpret_cast<AClass*>(o)->c; };
        
    






    p[7].Name  = "d";
    p[7].Info  = Type<float>();
    p[7].Get   = []( void* o ) -> void* { return &reinterpret_cast<AClass*>(o)->d; };
        
    Properties      = p;
    NumProperties   = 8;



}

gwRTTI_REGISTER( AClass );


////////////////////////////////////////////////////////////////////////////////
// MyEnum

template<> void TypeInfoImpl< MyEnum >::Create()
{
    






    
    
    Constant e[ 3 ];
        
    e[0].Name  = "One";
    e[0].Value = 0;
    // attrs?
        
    e[1].Name  = "Two";
    e[1].Value = 1;
    // attrs?
        
    e[2].Name  = "Three";
    e[2].Value = 2;
    // attrs?
        
    
    Constants       = e;
    NumConstants    = 3;


}

gwRTTI_REGISTER( MyEnum );


////////////////////////////////////////////////////////////////////////////////
// ClassWithEnum

template<> void TypeInfoImpl< ClassWithEnum >::Create()
{
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    






    

    static Property p[ 1 ];
        
    






    p[0].Name  = "x";
    p[0].Info  = Type<ClassWithEnum::ClassEnum>();
    p[0].Get   = []( void* o ) -> void* { return &reinterpret_cast<ClassWithEnum*>(o)->x; };
        
    Properties      = p;
    NumProperties   = 1;



}

gwRTTI_REGISTER( ClassWithEnum );


////////////////////////////////////////////////////////////////////////////////
// ClassWithEnum::ClassEnum

template<> void TypeInfoImpl< ClassWithEnum::ClassEnum >::Create()
{
    






    
    
    Constant e[ 3 ];
        
    e[0].Name  = "One";
    e[0].Value = 0;
    // attrs?
        
    e[1].Name  = "Two";
    e[1].Value = 5;
    // attrs?
        
    e[2].Name  = "Three";
    e[2].Value = 6;
    // attrs?
        
    
    Constants       = e;
    NumConstants    = 3;


}

gwRTTI_REGISTER( ClassWithEnum::ClassEnum );


////////////////////////////////////////////////////////////////////////////////
// NSRef

template<> void TypeInfoImpl< NSRef >::Create()
{
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    






    

    static Property p[ 1 ];
        
    






    p[0].Name  = "nsc";
    p[0].Info  = Type<NS::NSClass>();
    p[0].Get   = []( void* o ) -> void* { return &reinterpret_cast<NSRef*>(o)->nsc; };
        
    Properties      = p;
    NumProperties   = 1;



}

gwRTTI_REGISTER( NSRef );


////////////////////////////////////////////////////////////////////////////////
// MyStruct

template<> void TypeInfoImpl< MyStruct >::Create()
{
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    






    

    static Property p[ 2 ];
        
    






    p[0].Name  = "f";
    p[0].Info  = Type<float>();
    p[0].Get   = []( void* o ) -> void* { return &reinterpret_cast<MyStruct*>(o)->f; };
        
    






    p[1].Name  = "sc";
    p[1].Info  = Type<MyStruct::SubClass>();
    p[1].Get   = []( void* o ) -> void* { return &reinterpret_cast<MyStruct*>(o)->sc; };
        
    Properties      = p;
    NumProperties   = 2;



}

gwRTTI_REGISTER( MyStruct );


////////////////////////////////////////////////////////////////////////////////
// MyStruct::SubClass

template<> void TypeInfoImpl< MyStruct::SubClass >::Create()
{
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    






    

    static Property p[ 1 ];
        
    






    p[0].Name  = "a";
    p[0].Info  = Type<int>();
    p[0].Get   = []( void* o ) -> void* { return &reinterpret_cast<MyStruct::SubClass*>(o)->a; };
        
    Properties      = p;
    NumProperties   = 1;



}

gwRTTI_REGISTER( MyStruct::SubClass );


////////////////////////////////////////////////////////////////////////////////
// Attrs

template<> void TypeInfoImpl< Attrs >::Create()
{
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    




    static Attr attrs[ 7 ];
    
    attrs[0].Key     = "type";
    attrs[0].Value   = "class";
    
    attrs[1].Key     = "attr1";
    attrs[1].Value   = "a";
    
    attrs[2].Key     = "attr2";
    attrs[2].Value   = "b";
    
    attrs[3].Key     = "attr3";
    attrs[3].Value   = "c";
    
    attrs[4].Key     = "attr4";
    attrs[4].Value   = "d";
    
    attrs[5].Key     = "attr5";
    attrs[5].Value   = "x=y etc";
    
    attrs[6].Key     = "attr 6 with space";
    attrs[6].Value   = " hello world ";
    
    Attrs       = attrs;
    NumAttrs    = 7;



    

    static Property p[ 4 ];
        
    




    static Attr a[ 7 ];
    
    a[0].Key     = "type";
    a[0].Value   = "field";
    
    a[1].Key     = "attr1";
    a[1].Value   = "a";
    
    a[2].Key     = "attr2";
    a[2].Value   = "b";
    
    a[3].Key     = "attr3";
    a[3].Value   = "c";
    
    a[4].Key     = "attr4-fish ! $ ";
    a[4].Value   = "d";
    
    a[5].Key     = "attr5";
    a[5].Value   = "x=y etc";
    
    a[6].Key     = "attr 6 with space";
    a[6].Value   = " hello world ";
    
    p.Attrs       = a;
    p.NumAttrs    = 7;



    p[0].Name  = "a";
    p[0].Info  = Type<float>();
    p[0].Get   = []( void* o ) -> void* { return &reinterpret_cast<Attrs*>(o)->a; };
        
    






    p[1].Name  = "b";
    p[1].Info  = Type<float>();
    p[1].Get   = []( void* o ) -> void* { return &reinterpret_cast<Attrs*>(o)->b; };
        
    






    p[2].Name  = "butd";
    p[2].Info  = Type<float>();
    p[2].Get   = []( void* o ) -> void* { return &reinterpret_cast<Attrs*>(o)->butd; };
        
    




    static Attr a[ 1 ];
    
    a[0].Key     = "name";
    a[0].Value   = "value";
    
    p.Attrs       = a;
    p.NumAttrs    = 1;



    p[3].Name  = "e";
    p[3].Info  = Type<float>();
    p[3].Get   = []( void* o ) -> void* { return &reinterpret_cast<Attrs*>(o)->e; };
        
    Properties      = p;
    NumProperties   = 4;



}

gwRTTI_REGISTER( Attrs );


////////////////////////////////////////////////////////////////////////////////
// MultilineAttr

template<> void TypeInfoImpl< MultilineAttr >::Create()
{
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    




    static Attr attrs[ 4 ];
    
    attrs[0].Key     = "type";
    attrs[0].Value   = "class";
    
    attrs[1].Key     = "attr1";
    attrs[1].Value   = "a";
    
    attrs[2].Key     = "attr2";
    attrs[2].Value   = "b";
    
    attrs[3].Key     = "attr3";
    attrs[3].Value   = "c";
    
    Attrs       = attrs;
    NumAttrs    = 4;



    



}

gwRTTI_REGISTER( MultilineAttr );


////////////////////////////////////////////////////////////////////////////////
// EnumAttr

template<> void TypeInfoImpl< EnumAttr >::Create()
{
    




    static Attr attrs[ 1 ];
    
    attrs[0].Key     = "x";
    attrs[0].Value   = "y";
    
    Attrs       = attrs;
    NumAttrs    = 1;



    
    
    Constant e[ 4 ];
        
    e[0].Name  = "EA_One";
    e[0].Value = 0;
    // attrs?
        
    e[1].Name  = "EA_Two";
    e[1].Value = 1;
    // attrs?
        
    e[2].Name  = "EA_Four";
    e[2].Value = 3;
    // attrs?
        
    e[3].Name  = "EA_Five";
    e[3].Value = 4;
    // attrs?
        
    
    Constants       = e;
    NumConstants    = 4;


}

gwRTTI_REGISTER( EnumAttr );


////////////////////////////////////////////////////////////////////////////////
// A

template<> void TypeInfoImpl< A >::Create()
{
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    




    static Attr attrs[ 1 ];
    
    attrs[0].Key     = "a";
    attrs[0].Value   = "b";
    
    Attrs       = attrs;
    NumAttrs    = 1;



    

    static Property p[ 1 ];
        
    






    p[0].Name  = "mA";
    p[0].Info  = Type<float>();
    p[0].Get   = []( void* o ) -> void* { return &reinterpret_cast<A*>(o)->mA; };
        
    Properties      = p;
    NumProperties   = 1;



}

gwRTTI_REGISTER( A );


////////////////////////////////////////////////////////////////////////////////
// B

template<> void TypeInfoImpl< B >::Create()
{
    
    static const TypeInfo* bases[] = { this, A, nullptr };
    Inherits = bases;

    




    static Attr attrs[ 1 ];
    
    attrs[0].Key     = "a";
    attrs[0].Value   = "b";
    
    Attrs       = attrs;
    NumAttrs    = 1;



    

    static Property p[ 1 ];
        
    






    p[0].Name  = "mB";
    p[0].Info  = Type<float>();
    p[0].Get   = []( void* o ) -> void* { return &reinterpret_cast<B*>(o)->mB; };
        
    Properties      = p;
    NumProperties   = 1;



}

gwRTTI_REGISTER( B );


////////////////////////////////////////////////////////////////////////////////
// C

template<> void TypeInfoImpl< C >::Create()
{
    
    static const TypeInfo* bases[] = { this, B, nullptr };
    Inherits = bases;

    






    

    static Property p[ 1 ];
        
    






    p[0].Name  = "mC";
    p[0].Info  = Type<float>();
    p[0].Get   = []( void* o ) -> void* { return &reinterpret_cast<C*>(o)->mC; };
        
    Properties      = p;
    NumProperties   = 1;



}

gwRTTI_REGISTER( C );


////////////////////////////////////////////////////////////////////////////////
// Base

template<> void TypeInfoImpl< Base >::Create()
{
    
    static const TypeInfo* bases[] = { this, nullptr };
    Inherits = bases;

    






    



}

gwRTTI_REGISTER( Base );


////////////////////////////////////////////////////////////////////////////////
// C1

template<> void TypeInfoImpl< C1 >::Create()
{
    
    static const TypeInfo* bases[] = { this, Base, nullptr };
    Inherits = bases;

    






    



}

gwRTTI_REGISTER( C1 );


////////////////////////////////////////////////////////////////////////////////
// C2

template<> void TypeInfoImpl< C2 >::Create()
{
    
    static const TypeInfo* bases[] = { this, Base, nullptr };
    Inherits = bases;

    






    



}

gwRTTI_REGISTER( C2 );


////////////////////////////////////////////////////////////////////////////////
// D

template<> void TypeInfoImpl< D >::Create()
{
    
    static const TypeInfo* bases[] = { this, C1, C2, nullptr };
    Inherits = bases;

    






    



}

gwRTTI_REGISTER( D );


////////////////////////////////////////////////////////////////////////////////
// E

template<> void TypeInfoImpl< E >::Create()
{
    
    static const TypeInfo* bases[] = { this, D, nullptr };
    Inherits = bases;

    






    



}

gwRTTI_REGISTER( E );


////////////////////////////////////////////////////////////////////////////////
// V1

template<> void TypeInfoImpl< V1 >::Create()
{
    
    static const TypeInfo* bases[] = { this, Base, nullptr };
    Inherits = bases;

    






    



}

gwRTTI_REGISTER( V1 );


////////////////////////////////////////////////////////////////////////////////
// V2

template<> void TypeInfoImpl< V2 >::Create()
{
    
    static const TypeInfo* bases[] = { this, Base, nullptr };
    Inherits = bases;

    






    



}

gwRTTI_REGISTER( V2 );


////////////////////////////////////////////////////////////////////////////////
// D2

template<> void TypeInfoImpl< D2 >::Create()
{
    
    static const TypeInfo* bases[] = { this, V1, V2, nullptr };
    Inherits = bases;

    






    



}

gwRTTI_REGISTER( D2 );


