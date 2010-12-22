//////////////////////////////////////////////////////////////////////////

#include "ReflectorASTConsumer.h"

#pragma warning( disable: 4345 ) //  behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized
#include "clang/AST/DeclTemplate.h"


//************************************************************************
// Helper Functions
//************************************************************************

static const char* _GetAccessName( AccessSpecifier as )
{
    switch( as )
    {
        default:
        case AS_none:       return 0;
        case AS_public:     return "public";
        case AS_private:    return "private";
        case AS_protected:  return "protected";
    }
}

//////////////////////////////////////////////////////////////////////////
// _GetKindName
//  convert decl type into node name for export

static const char* _GetKindName( NamedDecl* pDecl )
{
    switch( pDecl->getKind() )
    {
        case Decl::Var:             return "var";
        case Decl::Field:           return "var";
        case Decl::Function:        return "function";
        case Decl::CXXMethod:       return "function";
        case Decl::Enum:            return "enum";
        case Decl::EnumConstant:    return "e";
        case Decl::ClassTemplate:   return "template_class";

        case Decl::CXXRecord:
        {
            if( dyn_cast<CXXRecordDecl>(pDecl)->isUnion() )  return "union";
            if( dyn_cast<CXXRecordDecl>(pDecl)->isStruct() ) return "struct";

            return "class";
        }

        default:
        {
            return "unhandled_type";
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// _GetNamespace
//  recursively walks up the decl context and works out the current
//  namespace for the given decl

static void _GetNamespace( DeclContext* pDeclContext, std::string& str )
{
    // exit if we have topped out

    if( pDeclContext == NULL )
    {
        return;
    }

    // otherwise recurse "upwards"

    _GetNamespace( pDeclContext->getParent(), str );

    // add our lexical context (named space or containing class)

    if( pDeclContext->isNamespace() )
    {
        std::string name = dyn_cast<NamespaceDecl>( pDeclContext )->getNameAsString();

        if( str.empty() )
        {
            str = name;
        }
        else
        {
            str += "::" + name;
        }
    }
};



//************************************************************************
// ReflectorASTConsumer
//************************************************************************


//////////////////////////////////////////////////////////////////////////
// HandleTopLevelDecl
//  callback from the clang Sema when a top level declaration has been
//  parsed. Walk the tree and reflect the contents

void ReflectorASTConsumer::HandleTopLevelDecl( DeclGroupRef declGroup )
{
    for( DeclGroupRef::iterator itr = declGroup.begin(); itr != declGroup.end(); itr++ )
    {
        ReflectDecl( *itr );
    }
}

//////////////////////////////////////////////////////////////////////////
// Debug
//  write decl information for debug
//

void ReflectorASTConsumer::DebugPrint( Decl* pDecl )
{
    std::pair<FileID, unsigned> li = mSourceManager.getDecomposedLoc( pDecl->getLocation() );

    FileID          fileID      = li.first;
    unsigned int    fileOffset  = li.second;

    if( mSourceManager.getSLocEntry( fileID ).isFile() )
    {
        const FileEntry* pFileEntry = mSourceManager.getFileEntryForID( fileID );

        if( pFileEntry )
        {
            // type, name and location

            NamedDecl* pNamedDecl = dyn_cast<NamedDecl>( pDecl );

            if( pNamedDecl )
            {
                printf( "DEBUG: %-16s %s, %s(%i)\n", pNamedDecl->getNameAsString().c_str(), pDecl->getDeclKindName(), pFileEntry->getName(), mSourceManager.getLineNumber( fileID, fileOffset ) );
            }
        }
        else
        {
            printf( "DEBUG: not file entry found for current location\n" );
        }
    }
    else
    {
        printf( "DEBUG: location not file\n" );
    }
}



//////////////////////////////////////////////////////////////////////////
// ReflectDecl
//  determine if and how to reflect a given declaration

void ReflectorASTConsumer::ReflectDecl( Decl* pDecl )
{
    // early out?

    Decl::Kind kind = pDecl->getKind();

    switch( kind )
    {
        case Decl::AccessSpec:
        case Decl::Typedef:
            return;

        default:
            break;
    }


    // notify the ReflectorContext we have a decl so it can process the appropriate
    // processing instructions to get the context up to date

    mContext.DeclBegin( pDecl );

    if( mContext.IsDebug() )
    {
        DebugPrint( pDecl );
    }

    // handle decl by type

    switch( kind )
    {
        case Decl::CXXMethod:
        {
            CXXMethodDecl* pMethod = dyn_cast<CXXMethodDecl>( pDecl );

            if( !pMethod->isOverloadedOperator() || mContext.CanReflectOperators() )
            {
                ReflectNamedDecl( pMethod );
            }
        }
        break;

        case Decl::CXXRecord:
        {
            CXXRecordDecl* pRecord = dyn_cast<CXXRecordDecl>( pDecl );

            // only reflect definitions (not prototypes)

            if( pRecord->isDefinition() )
            {
                ReflectNamedDecl( pRecord );
            }
        }
        break;

        case Decl::Enum:
        case Decl::Record:
        case Decl::Var:
        case Decl::Function:
        case Decl::Field:
        case Decl::EnumConstant:
        case Decl::ClassTemplate:
        {
            ReflectNamedDecl( dyn_cast<NamedDecl>( pDecl ) );
        }
        break;

        case Decl::Namespace:
        case Decl::LinkageSpec:
        {
            ReflectContents( dyn_cast<DeclContext>( pDecl ) );
        }
        break;

        case Decl::CXXConstructor:
        case Decl::CXXDestructor:
        {
            // ignore
        }
        break;

        default:
        {
            printf( "ReflectDecl:: Unhandled decl kind '%s'\n", pDecl->getDeclKindName() );
        }
        break;
    }


    // notify the ReflectorContext we are at the end of the decl so the context can be
    // brought up to date

    mContext.DeclEnd( pDecl );
}


//////////////////////////////////////////////////////////////////////////
// ReflectContents
//  reflect the inner contents of a DeclContext (e.g. the contents of a
//  namespace or class)

void ReflectorASTConsumer::ReflectContents( DeclContext* pDecl )
{
    for( DeclContext::decl_iterator itr = pDecl->decls_begin(); itr != pDecl->decls_end(); itr++ )
    {
        ReflectDecl( *itr );
    }
}


//////////////////////////////////////////////////////////////////////////
// ReflectNamedDecl
//  reflect a named declaration (e.g. function, variable, class, etc)

void ReflectorASTConsumer::ReflectNamedDecl( NamedDecl* pDecl )
{
    const int kBufferSize = 256;
    char buffer[ 256 ];

    // should we reflect this?

    if( pDecl->isImplicit() || !mContext.CanReflect( pDecl ) )
    {
        return;
    }


    // write regions (before we declare node for this decl, so therefore in parent context)
    ReflectBookmarks();

    // create node

    mWriter.BeginNode( _GetKindName( pDecl ) );
    mWriter.SetAttribute( "name", pDecl->getNameAsString().c_str() );

    // attached metadata

    ReflectMetaData();



    //
    // general attributes
    //

    // namespace

    if( mContext.CanWriteNamespace() )
    {
        std::string ns;
        _GetNamespace( pDecl->getLexicalDeclContext(), ns );

        if( ! ns.empty() )
        {
            mWriter.SetAttribute( "namespace", ns.c_str() );
        }
    }

    // fqn

    if( mContext.CanWriteFQN() )
    {
        mWriter.SetAttribute( "fqn", pDecl->getQualifiedNameAsString( mPrintingPolicy ).c_str() );
    }


    // source location

    if( mContext.CanWriteLocations() )
    {
        std::pair<FileID, unsigned> locInfo = mSourceManager.getDecomposedLoc( pDecl->getLocation() );

        FileID          fileID      = locInfo.first;
        unsigned int    fileOffset  = locInfo.second;
        const FileEntry*pFileEntry  = mSourceManager.getFileEntryForID( fileID );
        unsigned int    lineNumber  = mSourceManager.getLineNumber( fileID, fileOffset );

        sprintf_s( buffer, kBufferSize, "%s:%i", pFileEntry ? pFileEntry->getName() : "unknown", lineNumber );

        mWriter.SetAttribute( "location", buffer );
    }


    // custom attributes

    buffer[0] = '\0';
    buffer[1] = '\0';

    switch( pDecl->getKind() )
    {
        case Decl::CXXMethod:
        {
            // qualifiers

            CXXMethodDecl* pMethod = dyn_cast<CXXMethodDecl>( pDecl );

            if( pMethod->isVirtual() )
            {
                strncat_s( buffer, kBufferSize, ",virtual", 10 );
            }

            if( pMethod->isPure() )
            {
                strncat_s( buffer, kBufferSize, ",pure", 10 );
            }

            if( pMethod->getTypeQualifiers() & DeclSpec::TQ_const )
            {
                strncat_s( buffer, kBufferSize, ",const", 10 );
            }

            if( pMethod->isStatic() )
            {
                strncat_s( buffer, kBufferSize, ",static", 10 );
            }

            if( pMethod->isInlineSpecified() )
            {
                strncat_s( buffer, kBufferSize, ",inline", 10 );
            }

            if( mContext.CanWriteAccess() )
            {
                mWriter.SetAttribute( "access", _GetAccessName( pMethod->getAccess() ) );
            }
        }
        // nobreak - FALL THROUGH

        case Decl::Function:
        {
            FunctionDecl* pFunction = dyn_cast<FunctionDecl>( pDecl );

            if( !isa<CXXMethodDecl>(pFunction) )
            {
                if( pFunction->getLinkage() == InternalLinkage )
                {
                    strncat_s( buffer, kBufferSize, ",static", 10 );
                }
            }

            if( buffer[0] )
            {
                mWriter.SetAttribute( "qualifiers", &buffer[1] );
            }


            // return type

            mWriter.TextNode( "return", pFunction->getResultType().getAsString( mPrintingPolicy ).c_str() );


            // params

            for( FunctionDecl::param_const_iterator itr = pFunction->param_begin(); itr != pFunction->param_end(); itr++ )
            {
                mWriter.BeginNode( "param" );
                mWriter.SetAttribute( "name", (*itr)->getNameAsString().c_str() );
                mWriter.SetAttribute( "type", (*itr)->getType().getAsString( mPrintingPolicy ).c_str() );
                mWriter.EndNode();
            }

            if( pFunction->isVariadic() )
            {
                mWriter.BeginNode( "param" );
                mWriter.SetAttribute( "name", "..." );
                mWriter.SetAttribute( "type", "..." );
                mWriter.EndNode();
            }
        }
        break;

        case Decl::Var:
        {
            VarDecl* pVarDecl = dyn_cast<VarDecl>( pDecl );

            if( pVarDecl->getStorageClass() == SC_Static )
            {
                mWriter.SetAttribute( "qualifier", "static" );
            }
        }
        // nobreak - FALL THROUGH

        case Decl::Field:
        {
            // type
            if( mContext.CanWriteAccess() )
            {
                if( const FieldDecl* pField = dyn_cast<FieldDecl>( pDecl ) )
                {
                    mWriter.SetAttribute( "access", _GetAccessName( pField->getAccess() ) );
                }
            }

            QualType type = dyn_cast<ValueDecl>( pDecl )->getType();
            mWriter.SetAttribute( "type", type.getAsString( mPrintingPolicy ).c_str() );
        }
        break;

        case Decl::EnumConstant:
        {
            EnumConstantDecl* pEnum = dyn_cast<EnumConstantDecl>( pDecl );
            mWriter.SetAttribute( "value", pEnum->getInitVal().toString( 10 ).c_str() );
        }
        break;

        case Decl::ClassTemplate:
        {
            ClassTemplateDecl* pClassTemplate = dyn_cast<ClassTemplateDecl>( pDecl );

            TemplateParameterList* pParams = pClassTemplate->getTemplateParameters();

            TemplateParameterList::const_iterator itr = pParams->begin();

            while( itr != pParams->end() )
            {
                mWriter.BeginNode( "template_param" );
                mWriter.SetAttribute( "name", (*itr)->getNameAsString().c_str() );
                mWriter.EndNode();

                itr++;
            }

            ReflectContents( pClassTemplate->getTemplatedDecl() );
        }
        break;

        case Decl::CXXRecord:
        {
            // parent classes

            CXXRecordDecl* pCXXRecordDecl = dyn_cast<CXXRecordDecl>( pDecl );
            CXXRecordDecl::base_class_const_iterator itr = pCXXRecordDecl->bases_begin();

            StringRef name = pCXXRecordDecl->getName();
            TemplateSpecializationKind tsk = pCXXRecordDecl->getTemplateSpecializationKind();
            int n = pCXXRecordDecl->getNumTemplateParameterLists();

            while( itr != pCXXRecordDecl->bases_end() )
            {
                const CXXRecordDecl* pBase = cast<CXXRecordDecl>( itr->getType()->getAs<RecordType>()->getDecl() );

                mWriter.BeginNode( "parent" );
                mWriter.SetAttribute( "name", pBase->getNameAsString().c_str() );

                switch( itr->getAccessSpecifier() )
                {
                    case AS_public:     mWriter.SetAttribute( "access", itr->isVirtual() ? "public,virtual" : "public" ); break;
                    case AS_protected:  mWriter.SetAttribute( "access", itr->isVirtual() ? "protected,virtual" : "protected" ); break;
                    case AS_private:    mWriter.SetAttribute( "access", itr->isVirtual() ? "private,virtual" : "private" ); break;
                }

                // template instantiation?

                if( pBase->getTemplateSpecializationKind() != TSK_Undeclared )
                {
                    const ClassTemplateSpecializationDecl* pSpecialisation = dyn_cast<ClassTemplateSpecializationDecl>( pBase );

                    if( pSpecialisation )
                    {
                        const TemplateArgumentList& args = pSpecialisation->getTemplateInstantiationArgs();

                        for( unsigned int i=0; i < args.size(); i++ )
                        {
                            QualType type = args[i].getAsType();

                            mWriter.BeginNode( "template_argument" );
                            mWriter.SetAttribute( "type", type.getAsString( mPrintingPolicy ).c_str() );
                            mWriter.EndNode();
                        }
                    }
                }

                mWriter.EndNode();

                itr++;
            }
        }
        // nobreak - FALL THROUGH

        case Decl::Record:
        case Decl::Enum:
        case Decl::Namespace:
        {
            ReflectContents( dyn_cast<DeclContext>( pDecl ) );
        }
        break;

        default:
        {
            printf( "Unknown '%s'\n", pDecl->getNameAsString().c_str() );
        }
        break;
    }

    // mop up any straggler PI's
    mContext.PopComments( pDecl->getLocEnd(), true );
    ReflectBookmarks();

    // end node

    mWriter.EndNode();

}


//////////////////////////////////////////////////////////////////////////
// ReflectMetaData
//  write out any special meta data attached to this node

void ReflectorASTConsumer::ReflectMetaData()
{
    std::vector<ReflectorContext::MetaProperty>::iterator itr;

    for( itr = mContext.mMetaProperties.begin(); itr != mContext.mMetaProperties.end(); itr++ )
    {
        mWriter.BeginNode( "attribute" );
        mWriter.SetAttribute( "name",  itr->k.str().c_str() );
        mWriter.SetAttribute( "value", itr->v.str().c_str() );
        mWriter.EndNode();
    }
}

//////////////////////////////////////////////////////////////////////////
// ReflectBookmarks
//  export bookmarks set in code

void ReflectorASTConsumer::ReflectBookmarks()
{
    std::vector<ReflectorContext::Bookmark>::iterator itr;

    for( itr = mContext.mBookmarks.begin(); itr != mContext.mBookmarks.end(); itr++ )
    {
        mWriter.BeginNode( "bookmark" );

        // name
        if( !itr->name.empty() )
        {
            mWriter.SetAttribute( "name", itr->name.str().c_str() );
        }

        // file
        if( itr->file )
        {
            mWriter.SetAttribute( "file", itr->file );
        }

        // line
        const int kBufferSize = 16;
        char buffer[ 256 ];

        _itoa_s( itr->line, buffer, kBufferSize, 10 );
        mWriter.SetAttribute( "line", buffer );

        mWriter.EndNode();
    }

    mContext.mBookmarks.clear();
}

