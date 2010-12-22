//////////////////////////////////////////////////////////////////////////

#define DECLARE_RTTI virtual Type::Info* Type();

struct Vector
{
    float x;
    float y;
    float z;
};

struct Colour
{
    float r;
    float g;
    float b;
    float a;
};


//////////////////////////////////////////////////////////////////////////

// %reflect
struct Shape
{
    char*   name;
    Colour  colour;

    // factory
    static Shape*   Create( const char* name );
    void Set( Shape*, const char* name, const char* value );
};

struct Sphere : public Shape
{
    Vector  pos;
    float   radius;

    // %<auto>
};

struct Plane : public Shape
{
    Vector  normal;
    float   d;

    // %<auto
    // %>
};

struct Line : public Shape
{
    Vector pos;
    Vector dir;

    // %<auto>
};

struct Box : public Shape
{
    Vector max;
    Vector min;

    // %<auto
    void SomeFunction();
    // %>
};

