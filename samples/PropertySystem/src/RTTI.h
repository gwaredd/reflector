#ifndef RTTI_h
#define RTTI_h
////////////////////////////////////////////////////////////////////////////////

namespace RTTI
{
    class Type
    {
        public:

            virtual const char*     Name() = 0;
            virtual unsigned int    ClassID() = 0;

            // TODO:
            // enum properties
            // IsA, get parent type
            // instantiate/factory

            // class id
            // name
            // base rtti
            // factory fn
            // property list
    };


    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // RTTIImpl is the "sandwich class" that augments a class with RTTI support. Classes supporting RTTI 
    // need to derive from this class, with their parent specified as the BaseClass template parameter.

    template <class T, class BaseClass> class Impl : public BaseClass
    {
        public:

            static  T*      Create()    { return new T(); }
            static  Type*   ClassType() { return spType; }
            virtual Type*   GetType()   { return spType; }

        protected:

            static Type*    spType;
    };
}

////////////////////////////////////////////////////////////////////////////////
#endif // RTTI_h

