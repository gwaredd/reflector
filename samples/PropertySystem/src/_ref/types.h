//////////////////////////////////////////////////////////////////////////
// DO NOT EDIT - this is generated automatically

namespace Type
{
    enum eType
    {
        kUnknown = -1,
        kTypeShape,
        kTypeSphere,
        kTypePlane,
        kTypeLine,
        kTypeBox,
        kNumTypes
    };

    class Info
    {
        public:

            eType type;

            virtual const char* GetName() = 0;
    }

    Info* Get( eType );
    Info* Get( const char* );
}

