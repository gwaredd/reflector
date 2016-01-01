////////////////////////////////////////////////////////////////////////////////

#include "Reflector.h"


////////////////////////////////////////////////////////////////////////////////

class ReflectVisitor : public RecursiveASTVisitor< ReflectVisitor >
{
    public:

        bool VisitEnumDecl( EnumDecl* d )
        {
            mWriter.WriteDecl( d );
            return true;
        }

        bool VisitCXXRecordDecl( CXXRecordDecl* d )
        {
            mWriter.WriteDecl( d );
            return true;
        }

    protected:

        ReflectOutput& mWriter;

    public:

        ReflectVisitor( ReflectOutput& writer )
            : mWriter( writer )
        {
        }
};


////////////////////////////////////////////////////////////////////////////////
// AST consumer - handle interesting declarations in the source

class ReflectASTConsumer : public ASTConsumer
{
    public:

        virtual bool HandleTopLevelDecl( DeclGroupRef group ) override
        {
            for( auto itr = group.begin(); itr != group.end(); ++itr )
            {
                // if this decl is a reflectable type ...

                auto tagDecl = dyn_cast<TagDecl>( *itr );

                if( tagDecl== nullptr )
                {
                    continue;
                }

                // should we reflect it?

                mReflect.SetDefault( false );

                if( mReflect.ParseComment( mContext->getCommentForDecl( tagDecl, nullptr ) ) == false )
                {
                    continue;
                }

                // recursively write it out decl and all contained types

                mReflect.SetDefault( true );
                mVisitor.TraverseDecl( tagDecl );
            }

            return true;
        }


        // keep hold of context as we need it for getCommentForDecl

        virtual void Initialize( ASTContext& Context ) override
        {
            mContext = &Context;
            mWriter.SetContext(  mContext );
        }


    protected:

        ReflectContext  mReflect;
        ReflectOutput   mWriter;
        ReflectVisitor  mVisitor;
        ASTContext*     mContext;


    public:

        ReflectASTConsumer( JSON& json, CompilerInstance& CI )
            : mWriter( json, CI )
            , mVisitor( mWriter )
            , mContext( nullptr )
        {
        }
};


////////////////////////////////////////////////////////////////////////////////
// front end action - invoked for every file processed

class ReflectFrontendAction : public ASTFrontendAction
{
    public:

        virtual std::unique_ptr< ASTConsumer > CreateASTConsumer( CompilerInstance& CI, StringRef file ) override
        {
            llvm::errs() << "Processing '" << file << "'\n";

            mJSON.BeginArray();
            return make_unique< ReflectASTConsumer >( mJSON, CI );
        }

        virtual void EndSourceFileAction() override
        {
            mJSON.EndArray();
            llvm::outs() << "\n";
        }

    protected:

        JSON mJSON;

};
            

////////////////////////////////////////////////////////////////////////////////
            
const char* invocation = nullptr;
            
int Usage()
{
    llvm::errs() << "Usage: " << invocation << " <source file> -- [clang options]\n";
    return 1;
}


////////////////////////////////////////////////////////////////////////////////

int main( int argc, const char* argv[] )
{
    invocation = argv[ 0 ];
    
    if( argc < 3 )
    {
        return Usage();
    }
    
    
    // files
    
    std::vector< std::string > files;
    
    auto arg = &argv[ 1 ];
    
    for( auto count = 1; count < argc && strcmp( "--", *arg ) != 0; ++count, ++arg )
    {
        files.push_back( *arg );
    }
    
    if( files.size() < 1 )
    {
        return Usage();
    }
    
    
    // compilation options

    auto options = FixedCompilationDatabase::loadFromCommandLine( argc, argv );
    
    if( options == nullptr )
    {
        llvm::errs() << "Failed to load compilation database\n";
        return Usage();
    }
    

    // run tool
    
    ClangTool Tool( *options, files );

    return Tool.run( newFrontendActionFactory< ReflectFrontendAction >().get() );
}


