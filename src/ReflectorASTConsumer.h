#ifndef ReflectorASTConsumer_h
#define ReflectorASTConsumer_h
//////////////////////////////////////////////////////////////////////////
// ReflectorASTConsumer
//
//  Implements the clang::ASTConsumer interface to processes the AST
//

#include "Reflector.h"
#include "ReflectorContext.h"
#include "ReflectorWriter.h"

class ReflectorASTConsumer : public ASTConsumer
{
    public:

        ReflectorASTConsumer( ReflectorContext& rc, ReflectorWriter& rw, SourceManager& sm, ASTContext& ast )
            : mSourceManager( sm )
            , mContext( rc )
            , mWriter( rw )
            , mPrintingPolicy( ast.getLangOptions() )
        {
        }

        virtual void HandleTopLevelDecl( DeclGroupRef );

    protected:

        void ReflectDecl( Decl* );
        void ReflectNamedDecl( NamedDecl* );
        void ReflectContents( DeclContext* );
        void ReflectMetaData();
        void ReflectBookmarks();
        void DebugPrint( Decl* );

        SourceManager&      mSourceManager;
        ReflectorContext&   mContext;
        ReflectorWriter&    mWriter;
        PrintingPolicy      mPrintingPolicy;
};

//////////////////////////////////////////////////////////////////////////
#endif // ReflectorASTConsumer_h

