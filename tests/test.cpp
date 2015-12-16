////////////////////////////////////////////////////////////////////////////////
// tests - data types, reflection stuff

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


///%%!

class AClass
{
    public:

        float a;
        const float* b;

        std::vector< int > v;
        char buffer[ 32 ];

        void SomeFunc( int a )
        {
        }

        Vector3 v3;
        Vector3* pv3;
        Vector3& rv3;

        virtual void AnotherFunc() = 0;

    protected:

        float c;

    private:

        float d;

        AClass()
        {
        }
};

// enums

///%%!

enum MyEnum
{
    One,Two,Three
}

///%%

class ClassWithEnum
{
    public:

        /// %%

        enum ClassEnum
        {
            One,Two = 5,Three
        }

        ClassEnum x;

};

////////////////////////////////////////////////////////////////////////////////
// namespaces

namespace NS
{
    /// %%!

    class NSClass
    {
        public:
            float a;

    };
}

using namespace NS;


/// %%!

class NSRef
{
    public:
        NSClass nsc;
};


////////////////////////////////////////////////////////////////////////////////
// subclasses


///%

struct MyClass
{
    float f;

    class SubClass
    {
        public:

            int a;

    };

    SubClass sc;
};

// inheritance

// enums


////////////////////////////////////////////////////////////////////////////////
// reflection

// %key=value, ...
// %!
// % -- comment

