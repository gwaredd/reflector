//////////////////////////////////////////////////////////////////////////
// Reflector
//

#include "Reflector.h"
#include "ReflectorContext.h"
#include "ReflectorCommentHandler.h"
#include "ReflectorASTConsumer.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Basic/Version.h"

//////////////////////////////////////////////////////////////////////////
// custom command line option handler

static bool HandleFrontEndOptions( FrontendOptions& opt )
{
    if( opt.ShowVersion )
    {
        printf( "%s v%s\nbased on %s\n", rfNAME, rfVERSION, getClangFullVersion().c_str() );
        return false;
    }

    if( opt.ShowHelp )
    {
        printf( rfNAME "\n\n" );

        printf( "USAGE: " rfNAME " [options] <inputs>\n\n" );

        printf( "OPTIONS:\n" );        
        printf( "  --help                  show help\n" );
        printf( "  --version               show version\n" );
        printf( "  -o <output>             specify output file\n" );
        printf( "  -std=c++98              specify c++\n" );
        printf( "  -D<macro>[=def]         set preprocessor define\n" );
        printf( "  -I<dir>                 specify include directory\n" );
        printf( "  -@<file>                read options from file\n" );
        printf( "\n" );

        printf( "NB: reflector is based on clang and therefore shares many common\n" );
        printf( "    command line arguments. See that project for more options.\n" );

        return false;
    }

    if( opt.Inputs.empty() || opt.Inputs.at(0).second == "-" )
    {
        printf( "USAGE: " rfNAME " [options] <inputs>\n\n" );
        return false;
    }

    return true;
}


//////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
    ReflectorWriter     reflectorWriter;
    ReflectorContext    reflectorContext;


    // create compiler invocation from command-line args

    CompilerInstance ci;
    ci.createDiagnostics( argc, argv );

    CompilerInvocation in;
    CompilerInvocation::CreateFromArgs( in, &argv[1], argv + argc, ci.getDiagnostics() );

    if( ! HandleFrontEndOptions( in.getFrontendOpts() ) )
    {
        return 0;
    }

    ci.setInvocation( &in );
    ci.setTarget( TargetInfo::CreateTargetInfo( ci.getDiagnostics(), in.getTargetOpts() ) );


    // for each input file ...

    typedef std::vector<std::pair<InputKind, std::string> > Inputs;
    Inputs& inputs = ci.getFrontendOpts().Inputs;

    for( Inputs::iterator itr = inputs.begin(); itr != inputs.end(); itr++ )
    {
        printf( "Processing '%s'\n", itr->second.c_str() );

        // create compiler instance

        ci.createFileManager();
        ci.createSourceManager( ci.getFileManager() );
        ci.createPreprocessor();
        ci.createASTContext();

        // add our custom handlers

        reflectorContext.SetSourceManager( &ci.getSourceManager() );

        ReflectorCommentHandler reflectorCommentHandler( reflectorContext );
        ReflectorASTConsumer    reflectorAstConsumer( reflectorContext, reflectorWriter, ci.getSourceManager(), ci.getASTContext() );

        ci.getPreprocessor().AddCommentHandler( &reflectorCommentHandler );
        ci.setASTConsumer( &reflectorAstConsumer );

        // parse file

        ci.createSema( true, NULL );
        ci.InitializeSourceManager( StringRef( itr->second ) );

        ci.getDiagnosticClient().BeginSourceFile( in.getLangOpts() );
        reflectorContext.FileBegin();

        ParseAST( ci.getSema(), false );

        reflectorContext.FileEnd();
        ci.getDiagnosticClient().EndSourceFile();

        delete ci.takeSema();
    }


    // done - write output

    std::string& outputFile = in.getFrontendOpts().OutputFile;
    reflectorWriter.Save( outputFile.empty() ? "reflector.xml" : outputFile.c_str() );


    // stop CompilerInstance trying to free stuff it shouldn't
    
    ci.takeInvocation();
    ci.takeASTConsumer();

    return 0;
}

