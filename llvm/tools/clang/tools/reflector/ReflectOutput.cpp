////////////////////////////////////////////////////////////////////////////////

#include "Reflector.h"


////////////////////////////////////////////////////////////////////////////////
// utils

static const char* AccessToString( AccessSpecifier as )
{
    switch( as )
    {
        case AS_public:     return "public";
        case AS_protected:  return "protected";
        case AS_private:    return "private";
        case AS_none:       break;
    }

    return "";
}


////////////////////////////////////////////////////////////////////////////////

void ReflectOutput::WriteDecl( TagDecl* D )
{
    mReflect.ParseComment( mContext->getCommentForDecl( D, nullptr ) );

    mReflect.SetDefault( true );

    // reflect declaration

    mJSON.BeginObject();
    {
        mJSON.Field( "type", D->getKindName().str().c_str() );
        mJSON.Field( "name", D->getQualifiedNameAsString().c_str() );

        auto loc = D->getLocStart().printToString( mCI.getSourceManager() );
        mJSON.Field( "location", loc.c_str() );

        mReflect.OutputAttrs( mJSON );

        if( auto record = dyn_cast<CXXRecordDecl>( D ) )
        {
            WriteClass( record );
        }
        else if( auto enumType = dyn_cast<EnumDecl>( D ) )
        {
            WriteEnum( enumType );
        }
    }
    mJSON.EndObject();
}


////////////////////////////////////////////////////////////////////////////////

void ReflectOutput::WriteClass( CXXRecordDecl* record )
{
    PrintingPolicy pp( mContext->getLangOpts() );
    pp.SuppressTagKeyword = true;

    // base classes

    if( record->getNumBases() > 0 )
    {
        mJSON.BeginArray( "inherits" );

        for( auto& base : record->bases() )
        {
            mJSON.BeginObject();
            {
                mJSON.Field( "type", base.getType().getAsString( pp ).c_str() );
                mJSON.Field( "access", AccessToString( base.getAccessSpecifier() ) );
            }
            mJSON.EndObject();
        }

        mJSON.EndArray();
    }


    // fields

    mJSON.BeginArray( "fields" );
    {
        for( const auto& field : record->fields() )
        {
            if( mReflect.ParseComment( mContext->getCommentForDecl( field, nullptr ) ) )
            {
                mJSON.BeginObject();
                {
                    mJSON.Field( "name", field->getName().str().c_str() );
                    mJSON.Field( "type", field->getType().getAsString( pp ).c_str() );
                    mJSON.Field( "access", AccessToString( field->getAccess() ) );
                    // TODO: qualifiers - e.g. const
                    mReflect.OutputAttrs( mJSON );
                }
                mJSON.EndObject();
            }
        }
    }
    mJSON.EndArray();
}


////////////////////////////////////////////////////////////////////////////////

void ReflectOutput::WriteEnum( EnumDecl* enumType )
{
    // enum constants

    mJSON.BeginArray( "constants" );
    {
        for( const auto& decl : enumType->decls() )
        {
            auto constant = dyn_cast<EnumConstantDecl>( decl );

            if( constant == nullptr )
            {
                continue;
            }

            if( mReflect.ParseComment( mContext->getCommentForDecl( constant, nullptr ) ) )
            {
                mJSON.BeginObject();
                {
                    mJSON.Field( "name", constant->getNameAsString().c_str() );
                    mJSON.Field( "value", constant->getInitVal().toString(10).c_str() );
                    mReflect.OutputAttrs( mJSON );
                }
                mJSON.EndObject();
            }
        }
    }
    mJSON.EndArray();
}

