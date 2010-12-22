//////////////////////////////////////////////////////////////////////////

#include "ReflectorWriter.h"


//////////////////////////////////////////////////////////////////////////

ReflectorWriter::ReflectorWriter()
{
    TiXmlElement* pNode = new TiXmlElement( "reflector" );

    mDoc.LinkEndChild( new TiXmlDeclaration( "1.0", "", "" ) );
    mDoc.LinkEndChild( pNode );

    PushNode( pNode );
}


//////////////////////////////////////////////////////////////////////////

bool ReflectorWriter::Save( const char* filename )
{
    mNodeStack.clear();

    return mDoc.SaveFile( filename );
}


//////////////////////////////////////////////////////////////////////////

void ReflectorWriter::AddNode( TiXmlNode* p )
{
    TiXmlElement* pCur = GetCurrentNode();

    if( pCur )
    {
        pCur->LinkEndChild( p );
    }
}

void ReflectorWriter::PushNode( TiXmlElement* p )
{
    AddNode( p );
    mNodeStack.push_back( p );
}

void ReflectorWriter::PopNode()
{
    if( mNodeStack.size() >= 2 )
    {
        mNodeStack.pop_back();
    }
}


//////////////////////////////////////////////////////////////////////////

void ReflectorWriter::BeginNode( const char* type )
{
    PushNode( new TiXmlElement( type ) );
}

void ReflectorWriter::EndNode( const char* content )
{
    if( content )
    {
        TiXmlElement* pNode = GetCurrentNode();

        if( pNode )
        {
            pNode->LinkEndChild( new TiXmlText( content ) );
         }
    }

    PopNode();
}

void ReflectorWriter::SetAttribute( const char* name, const char* value )
{
    TiXmlElement* pNode = GetCurrentNode();

    if( pNode )
    {
        pNode->SetAttribute( name,  value );
    }
}

void ReflectorWriter::TextNode( const char* name, const char* value )
{
    TiXmlElement* pNode = new TiXmlElement( name );
    pNode->LinkEndChild( new TiXmlText( value ) );
    AddNode( pNode );
}

