////////////////////////////////////////////////////////////////////////////////
// simple AST walker example

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
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;


////////////////////////////////////////////////////////////////////////////////
// globals

ASTContext*         gContext            = nullptr;  // required for getCommentForDecl
CompilerInstance*   gCompilerInstance   = nullptr;  // required for getSourceManager


////////////////////////////////////////////////////////////////////////////////
// walk every decl in given context

class ReflectVisitor : public RecursiveASTVisitor< ReflectVisitor >
{
    public:

        // get special comment above declaration

        void GetComment( TagDecl* decl )
        {
            std::string str;

            auto comment = gContext->getCommentForDecl( decl, nullptr );

            if( comment == nullptr )
            {
                return;
            }

            for( auto commentItr = comment->child_begin(); commentItr != comment->child_end(); ++commentItr )
            {
                auto commentSection = *commentItr;

                if( commentSection->getCommentKind() != comments::BlockContentComment::ParagraphCommentKind )
                {
                    continue;
                }

                for( auto textItr = commentSection->child_begin(); textItr != commentSection->child_end(); textItr++ )
                {
                    if( auto textComment = dyn_cast<comments::TextComment>( *textItr ) )
                    {
                        str += textComment->getText().ltrim();
                        str += " ";
                    }
                }
            }

            //llvm::outs() << str << "\n";
        }


        // walk class or struct declaration

        bool VisitCXXRecordDecl( CXXRecordDecl* decl )
        {
            // parse special comment

            GetComment( decl );

            std::string type = decl->getKindName().str();           // class or struct
            std::string name = decl->getQualifiedNameAsString();    // name

            //llvm::outs() << type << ", " << name << "\n";

            // source file location

            std::string location = decl->getLocStart().printToString( gCompilerInstance->getSourceManager() );


            // remove "class or struct" from text when doing getType().getAsString()

            PrintingPolicy pp( gContext->getLangOpts() );
            pp.SuppressTagKeyword = true;

            // base classes

            for( auto& base : decl->bases() )
            {
                auto type = base.getType().getAsString( pp );

                llvm::outs() << type << "\n";
            }


            // fields

            for( const auto& field : decl->fields() )
            {
                std::string name = field->getName().str();
                std::string type = field->getType().getAsString( pp );
            }

            // done

            return true;
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
                // is decl a struct, class or an enum?

                if( auto tagDecl = dyn_cast<TagDecl>( *itr ) )
                {
                    // recursively walk declaration

                    mVisitor.TraverseDecl( tagDecl );
                }

            }

            return true;
        }


        // keep hold of context as we need it for getCommentForDecl

        virtual void Initialize( ASTContext& Context ) override
        {
            gContext = &Context;
        }


    protected:

        ReflectVisitor  mVisitor;
};


////////////////////////////////////////////////////////////////////////////////
// front end action - invoked for every file processed

class ReflectFrontendAction : public ASTFrontendAction
{
    public:

        virtual std::unique_ptr< ASTConsumer > CreateASTConsumer( CompilerInstance& CI, StringRef file ) override
        {
            gCompilerInstance = &CI;

            llvm::errs() << "Processing '" << file << "'\n";
            return make_unique< ReflectASTConsumer >();
        }
};


////////////////////////////////////////////////////////////////////////////////

int main( int argc, const char* argv[] )
{
    // file to process

    std::vector< std::string > files;
    files.push_back( argv[ 1 ] );

    // compilation options

    auto options = FixedCompilationDatabase::loadFromCommandLine( argc, argv );

    if( options == nullptr )
    {
        llvm::errs() << "Failed to load compilation database\n";
        return -1;
    }


    // run tool

    ClangTool Tool( *options, files );
    return Tool.run( newFrontendActionFactory< ReflectFrontendAction >().get() );
}

