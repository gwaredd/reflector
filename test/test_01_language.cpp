//////////////////////////////////////////////////////////////////////////
// test language features

// %%reflect

// global variables

int gGlobalVariable;
const int kGlobalConst = 100;
const float kGlobalConstFloat = 3.141f;
static int gGlobalStatic = 99;

// prototypes

typedef int TypedefDecl;
class ClassPrototype;

// global functions

void GlobalFunction()
{
}

static void GlobalStaticFunction( float& a )
{
}

static __cdecl void GlobalCDeclStaticFunction( int a )
{
}

void VariadicFunction( int a, float b, const char* c, ... )
{
}

extern "C"
{
    void GlobalExternCFunction()
    {
    }
};

// class members

class ClassDeclaration
{
    public:

        int mMemberVariable;
        static int mStaticMemberVariable;

        ClassDeclaration();

        void Method();
        void MethodWithParams( int a, int b, int c );
        int  MethodReturnInt( int a, int b, int c );
        const int MethodVariousParams( int a, float b, const bool c, char* d, char& e ) const;

        virtual int VirtualMethodReturnInt( int a, int b, int c );
        virtual int PureVirtualMethodReturnInt( int a, int b, int c ) = 0;
        virtual int ConstVirtualMethod( int a, int b, int c ) const;

        int ImplicitInlineMethod( int a, int b, int c ) { return a + b * c; }
        inline int ExplicitInlineMethod( int a, int b, int c ) { return a * b + c; }

        static int StaticFunction( int a, int b, int c );

        ClassPrototype& MethodClassParameter( ClassPrototype& );
        TypedefDecl MethodWithTypeDef( TypedefDecl );

        ClassDeclaration& operator+=( ClassPrototype& );

    protected:

        int mProtectedMemberVariable;
        void ProtectedMethodWithParams( int a, int b, int c );

    private:

        int mPrivateMemberVariable;
        void PrivateMethodWithParams( int a, int b, int c );
};


// class variations

class AbstractClass
{
    public:

        int mMemberVariable;

        virtual void Pure() = 0;
};

class InnerClassDecl
{
    public:

        class MyInnerClass
        {
            public:

                int mInsideMember;

        };

        int mOutsideMember;
        MyInnerClass mInnerClass;
};

// multiple inheritance

class ParentClass
{
    public:

        int mParentMember;
};

class InterfaceClass
{
    public:

        void SomeFunction();
        int mInterfaceMember;
};

class MultipleInheritClass : public ParentClass, public InterfaceClass
{
    public:

        int mChildMember;
};


// virtual inheritance

class A {};
class B : public virtual A {};
class C : public virtual A {};

class VirtualInheritance : public B, public C
{
};


// structs and unions

struct StructDecl
{
    int a;
    float b;

    int  MethodReturnInt( int a, int b, int c );
};

union UnionDecl
{
    int a;
    float b;
};


// enums

enum // anonymous
{
    kEnum1,
    kEnum2,
    kEnum3,
};

enum NamedEnum
{
    kNamedEnum1,
    kNamedEnum2,
    kNamedEnum3,
};

enum EnumWithValues
{
    kNamedValueEnum1 = 10,
    kNamedValueEnum2,
    kNamedValueEnum3 = 0x80,
};

// templates

template<typename T> T TemplatedFunction( T t )
{
    return t + 1;
}

void UseTemplateFunction()
{
    TemplatedFunction( 2 );
}

template<typename T> class TemplatedClass
{
    public:

        T mMemberVariableTemplate;
};

class TemplateInheritedClass : public TemplatedClass<int>
{
    public:

        int mSomeVar;

};

// namespaces

namespace // anonymous
{
    class ClassAnonymousNS
    {
        public: int mMemberVariable;
    };
}

namespace NamespaceDecl
{
    class ClassInNamespace
    {
        public: int mMemberVariable;
    };
}

namespace NamespaceDecl
{
    namespace NestedNS
    {
        class ClassInNestedNamespace
        {
            public: int mMemberVariable;
        };
    }
}


// macros

#define CREATE_TAG(x) Func##x

class MacroClass
{
    public:

        void CREATE_TAG( MyFunc )();
};

