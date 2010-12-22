#ifndef ReflectorWriter_h
#define ReflectorWriter_h
//////////////////////////////////////////////////////////////////////////
// format output

#include "Reflector.h"
#include <vector>
#include "tinyxml.h"

//////////////////////////////////////////////////////////////////////////

class ReflectorWriter
{
    protected:

        TiXmlDocument   mDoc;
        std::vector<TiXmlElement*> mNodeStack;

        TiXmlElement*   GetCurrentNode()            { return mNodeStack.empty() ? NULL : mNodeStack.back(); }

        void            AddNode( TiXmlNode* p );
        void            PushNode( TiXmlElement* p );
        void            PopNode();

    public:

        ReflectorWriter();

        bool Save( const char* );

        void BeginNode( const char* type );
        void EndNode( const char* content = NULL );
        void SetAttribute( const char* name, const char* value );
        void TextNode( const char* name, const char* value );
};

//////////////////////////////////////////////////////////////////////////
#endif // ReflectorWriter_h
