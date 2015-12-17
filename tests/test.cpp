////////////////////////////////////////////////////////////////////////////////

// #include <vector>


int a = 0;
int b = 2;


void F()
{
}


////////////////////////////////////////////////////////////////////////////////
// classes

struct Vector3
{
    float x,y,z;
};


///%%

class AClass
{
    public:

        float a;
        const float* b;

        // std::vector< int > v;
        char buffer[ 32 ];

        void SomeFunc( int a )
        {
        }

        Vector3 v3;
        Vector3* pv3;
        Vector3& rv3;

        virtual void AnotherFunc() = 0;

        virtual ~AClass();



    protected:

        float c;

    private:

        float d;

        AClass( Vector3& x )
            : rv3( x )
        {
        }
};

// enums

///%%

enum MyEnum
{
    One,Two,Three
};

///%%

class ClassWithEnum
{
    public:

        /// %%

        enum ClassEnum
        {
            One,Two = 5,Three
        };

        ClassEnum x;

};

////////////////////////////////////////////////////////////////////////////////
// namespaces

namespace NS
{
    /// %%

    class NSClass
    {
        public:
            float a;

    };
}

using namespace NS;


/// %%

class NSRef
{
    public:
        NSClass nsc;
};


////////////////////////////////////////////////////////////////////////////////
// subclasses


/// %%

struct MyStruct
{
    float f;

    class SubClass
    {
        public:

            int a;

    };

    SubClass sc;
};


///%%

template<typename T>
class TemplatedClass
{
    public:

        T t;
};


////////////////////////////////////////////////////////////////////////////////
// reflection

/// %% type=class attr1=a, attr2=b; attr3=c attr4=d attr5="x=y etc"  "attr 6 with space" = " hello world "

class Attrs
{
    public:

        /// %% type=field attr1=a, attr2=b; attr3=c 'attr4-fish !@$'=d attr5="x=y etc"  "attr 6 with space" = " hello world "
        float a;
        float b;

        ///%!
        float notc;
        float butd;

        ///% name=value --ignore=this
        float e;
};

/// %% type=class
/// %% attr1=a
/// attr2=b
/// %% attr3=c

class MultilineAttr
{
};


///%% x=y

enum EnumAttr
{
    EA_One,
    EA_Two,
    ///%!
    EA_NotThree,
    ///%
    EA_Four,
    ///% a=b; c=d
    EA_Five,
};


////////////////////////////////////////////////////////////////////////////////

///% a=b

class A
{
    public: float mA;
};

class B : public A
{
    public: float mB;
};

///% -- clear attrs

class C : public B
{
    public: float mC;
};


///% -- multiple inheritance

class Base
{
};

class C1 : public Base
{
};

class C2 : public Base
{
};

class D : public C1, public C2
{
};

class E : public D
{
};


///% -- virtual

class V1 : public virtual Base
{
};

class V2 : public virtual Base
{
};

class D2 : public V1, public V2
{
};

