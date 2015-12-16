////////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <string>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Comment.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"


using namespace llvm;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;


////////////////////////////////////////////////////////////////////////////////

class ReflectASTConsumer : public ASTConsumer
{
    public:

        ReflectASTConsumer( CompilerInstance& CI )
            : mCI( CI )
        {
        }

        virtual void Initialize( ASTContext& Context ) override
        {
            mContext = &Context;
        }


        virtual void HandleTagDeclDefinition( TagDecl* D ) override
        {
            auto name   = D->getKindName();
            auto range  = D->getSourceRange();
            auto loc    = range.getBegin();

            llvm::outs() << name << "\n";
            // llvm::outs() << loc.printToString( mCI.getSourceManager() ) << "\n";

            // const char* start = sourceManager.getCharacterData( range.getBegin() );
            // const char* end   = sourceManager.getCharacterData( range.getEnd() );
            // llvm::outs() << std::string( start, end - start ) << "\n";

            auto comment = mContext->getCommentForDecl( D, &mCI.getPreprocessor() );

            if( comment )
            {
                llvm::outs() << "has comment\n";

                for( auto itr = comment->child_begin(); itr != comment->child_end(); ++itr )
                {
                    auto c = *itr;
                    llvm::outs() << c->getCommentKindName() << "\n";
                    c->dump();

                    const char* start = mCI.getSourceManager().getCharacterData( c->getLocStart() );
                    const char* end   = mCI.getSourceManager().getCharacterData( c->getLocEnd() ) + 1;
                    llvm::outs() << std::string( start, end - start ) << "\n";

                    // llvm::outs() << c->getCommentKindName() << "\n";
                    // getText().str();

                }
            }
        }

    protected:

        ASTContext*         mContext;
        CompilerInstance&   mCI;
};


////////////////////////////////////////////////////////////////////////////////

class ReflectFrontendAction : public ASTFrontendAction
{
    public:

        virtual std::unique_ptr< ASTConsumer > CreateASTConsumer( CompilerInstance& CI, StringRef file ) override
        {
            llvm::errs() << "Processing '" << file << "'\n";

            // void Preprocessor::addCommentHandler (   CommentHandler *    Handler )

            // CI.getPreprocessor().SetCommentRetentionState( true, false );

            return make_unique< ReflectASTConsumer >( CI );
        }
};


////////////////////////////////////////////////////////////////////////////////

int main( int argc, const char* argv[] )
{
    cl::OptionCategory Options( "Extras" );
    cl::extrahelp CommonHelp( CommonOptionsParser::HelpMessage );

    CommonOptionsParser op( argc, argv, Options );

    ClangTool Tool( op.getCompilations(), op.getSourcePathList() );

    // clang::CommentOptions::ParseAllComments = true;

    // turn off errors
    IgnoringDiagConsumer diagConsumer;
    Tool.setDiagnosticConsumer( &diagConsumer );

    return Tool.run( newFrontendActionFactory< ReflectFrontendAction >().get() );
}


