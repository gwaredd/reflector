//////////////////////////////////////////////////////////////////////////
// test reflect processing instructions


//------------------------------------------------------------------------
// reflect on/off
//------------------------------------------------------------------------

// reflect turned off by default
int NoReflectVar0;

// explicit reflect

// %reflect
int ReflectVar1;

int NoReflectVar1;

// %%reflect
int ReflectVar2;
int ReflectVar3;
int ReflectVar4;

// %%reflect no
int NoReflectVar2;
int NoReflectVar3;

// %%reflect
class A
{
    public:

        int mA1;
        int mB1;
        int mC0_no; // %reflect ignore
        int mD0_no; // %reflect no
        int mE1;

        // %%reflect no
        int mF0_no;
        int mG0_no;
        int mH0_no;
        int mI1; // %reflect
        int mJ0_no;
};

int ReflectVar5;

//------------------------------------------------------------------------
// reflect access
//------------------------------------------------------------------------

class B
{
    public:     int mPublicA1;
    protected:  int mProtectedA0_no;
    private:    int mPrivateA0_no;

    // %%reflect access protected
    public:     int mPublicB1;
    protected:  int mProtectedB1;
    private:    int mPrivateB0_no;

    // %%reflect access private
    public:     int mPublicC1;
    protected:  int mProtectedC1;
    private:    int mPrivateC1;

    // %%reflect access public
    public:     int mPublicD1;
    protected:  int mProtectedD0_no;
    private:    int mPrivateD0_no;

    // %%reflect access private
    private:    int mPrivateE1;
};

class C
{
    public:     int mPublicA1;
    protected:  int mProtectedA0_no;
    private:    int mPrivateA0_no;

    // %%reflect access private,no write access
    public:     int mPublicB1;
    protected:  int mProtectedB1;
    private:    int mPrivateB1;

    // %%reflect write access
    public:     int mPublicC1;
    protected:  int mProtectedC1;
    private:    int mPrivateC1;
};
    
// %%reflect no

//------------------------------------------------------------------------
// reflect operators
//------------------------------------------------------------------------

// %reflect operators
class COperators
{
    public:
        COperators& operator+=( COperators& );
};

// %reflect
class COperatorsOff
{
    public:
        COperatorsOff& operator+=( COperatorsOff& );
};

//------------------------------------------------------------------------
// reflect write
//------------------------------------------------------------------------

// %reflect write FQN,write namespace,write location
namespace NS1
{
    class A
    {
        public:

            class B
            {
                public: int mMember;
            };
    };
}

// %reflect 
namespace NS0
{
    class A
    {
        public:

            class B
            {
                public: int mMember;
            };
    };
}

//------------------------------------------------------------------------
// reflect bookmarks
//------------------------------------------------------------------------

// %reflect
class ClassBookmarkTest
{
    public:
        // %reflect bm
        int mMemberA;

        int mMemberB;

        // %reflect bm BOOKMARK_NAME

        int mMemberC;

        /*

        %reflect bm CSTYLEREGION

        */
};

//------------------------------------------------------------------------
// reflect attribute
//------------------------------------------------------------------------

// %reflect attribute desc="some test class"
class D
{
    public:

        void SomeFunc();    // %reflect attribute bind=lua;etc=10
        int mSomeVar;       // %reflect attribute min=5;max=10;default=7
};


//------------------------------------------------------------------------
// reflect "comments"
//------------------------------------------------------------------------

// -- %reflect
int NoReflectVar5;


//------------------------------------------------------------------------
// reflect explicit
//------------------------------------------------------------------------

// %reflect explicit
class E
{
    public:

        int mVarA_no;
        int mVarB_no;
        int mVarC; // %reflect
        int mVarD_no;
        int mVarE; // %reflect
};


//------------------------------------------------------------------------
// reflect C style comments
//------------------------------------------------------------------------

/*
%reflect
*/
int ReflectVar6;


/*
    multi-line comments
    %reflect
    etc
*/
int ReflectVar7;

/*
    some comment
    multi-line comments with %reflect in the middle
    some comment
*/
int NoReflectVar6;

 /*
    %reflect 
    %reflect attribute x=y
    %reflect write FQN
*/

int ReflectVar8; /* %reflect */

int ReflectVar9; /*
    %reflect 
*/

int ReflectVar10; /*
    %reflect 
    %reflect attribute x=y
    %reflect write FQN
*/


//------------------------------------------------------------------------
// reflect GNU attributes
//------------------------------------------------------------------------

int ReflectVar11 __attribute__((annotate("%reflect")));
int ReflectVar12 __attribute__((annotate("%reflect attribute x=y")));

class CGNUAttr
{
    public:
        int mMember;
} __attribute__((annotate("%reflect")));

#define R(x) __attribute__((annotate("%reflect "#x)));

int ReflectVar13 R();
int ReflectVar14 R(write FQN);
int ReflectVar15 R(attribute x=y);


//------------------------------------------------------------------------
// reflect inherit
//------------------------------------------------------------------------

// %reflect
class F
{
    public:

        int mParent;
};

class G : public F
{
    public:

        int mChild;
};

// %reflect no inherit
class H
{
    public:

        int mParent;
};

class I : public H
{
    public:

        int mChild;
};


// %reflect write FQN,write namespace,access private
class J
{
    public:

        int mParent;

    private:

        int mParentPrivate;
};

class K : public J
{
    public:

        int mChild;

    private:

        int mChildPrivate;
};

