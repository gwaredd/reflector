////////////////////////////////////////////////////////////////////////////////
// multiple-inheritance
// multiple-inheritance same type
// virtual inheritance
// members that are pointers to type


#pragma once

#include "gw/gw.h"
#include "gwRTTI/gwRTTI.h"


////////////////////////////////////////////////////////////////////////////////

struct Vector3
{
    float x,y,z;
    
    Vector3() : x(1.f), y(2.f), z(3.f) {}
};


////////////////////////////////////////////////////////////////////////////////

class Component
{
    gwRTTI( Component );
    
    public:
    
        float mProperty;
    
        Component()
            : mProperty( 1.141f )
        {
        }
    
};

class AClass
{
    gwRTTI( AClass );
    
    public:
    
        float       mPublicMember;
        Component   mComponent;
        Component*  mpComonent;
        Vector3     mVector;
        Vector3*    mpVector;
        char        mArray[ 32 ];
    
        virtual int DoWork( int x )
        {
            return x + 10;
        }
    
        AClass()
            : mPublicMember( 3.141f )
            , mPrivateMember( 100.f )
    
        {
            mpComonent = new Component();
            mpVector = new Vector3();
            strcpy( mArray, "hello" );
        }
    
        virtual ~AClass()
        {
            delete mpComonent;
            delete mpVector;
        }
    
    protected:
    
        float mPrivateMember;
};


namespace Classes
{
    class NSClass
    {
        public:
            float mProperty;
            
            NSClass()
                : mProperty( 5.f )
            {
                
            }
    };
}



////////////////////////////////////////////////////////////////////////////////
// inheritance

class A
{
    gwRTTI( A );
    
    public:
    
        A() : mA( 1.f ) {}
    
    protected:
        
        float mA;
};

class B : public A
{
    gwRTTI( B );
    
    public:
    
        B() : mB( 2.f ) {}
        
        float mB;
};

class C : public B
{
    gwRTTI( C );
    
    public:
    
        C() : mC( 3.f ) {}
        float mC;
};


////////////////////////////////////////////////////////////////////////////////
// multiple inheritance


