#ifndef ReflectorContext_h
#define ReflectorContext_h
//////////////////////////////////////////////////////////////////////////
// ReflectorContext is responsible for processing the %reflect instructions
// in the source files and handling the logic appropriately
//

#include "Reflector.h"

#include <queue>
#include <vector>
#include <map>

//////////////////////////////////////////////////////////////////////////

class ReflectorContext
{
    public:

        struct MetaProperty
        {
            StringRef k;
            StringRef v;
        };

        struct Bookmark
        {
            Bookmark( StringRef n, const char* f, unsigned int l )
                : name( n )
                , file( f )
                , line( l )
            {}

            StringRef       name;
            const char*     file;
            unsigned int    line;
        };

    public:

        static const char* const PITag;
        static const char* const ParentPITag;

        ReflectorContext();
        void SetSourceManager( SourceManager* p ) { mpSourceManager = p; }

        // queue processing instructions
        void PushComment( SourceRange& r )  { mPIQueue.push( r ); }
        void PopComments( SourceLocation&, bool process = true );

        // state management
        void FileBegin();
        void FileEnd();
        void DeclBegin( Decl* pDecl );
        void DeclEnd( Decl* pDecl );
        void PropertyBegin( Decl* pDecl );
        void PropertyEnd( Decl* pDecl );

        // query state
        bool CanReflect( NamedDecl* );
        bool IsDebug()              { return CurrentState().debug; }
        bool CanInherit()           { return CurrentState().inherit; }
        bool CanReflectOperators()  { return CurrentState().operators; }
        bool CanWriteFQN()          { return CurrentState().writeFQN; }
        bool CanWriteAccess()       { return CurrentState().writeAccess; }
        bool CanWriteNamespace()    { return CurrentState().writeNamespace; }
        bool CanWriteLocations()    { return CurrentState().writeLocations; }

    protected:

        struct State
        {
            bool reflect;
            bool reflectExplicit;
            bool inherit;
            bool operators;
            bool writeFQN;
            bool writeAccess;
            bool writeNamespace;
            bool writeLocations;
            bool debug;
            AccessSpecifier accessLevel;

            State()
                : reflect( false )
                , inherit( true )
                , reflectExplicit( false )
                , operators( false )
                , writeFQN( false )
                , writeAccess( false )
                , writeNamespace( true )
                , writeLocations( false )
                , debug( false )
                , accessLevel( AS_public )
            {
            }
        };
        void            UpdateCommands( Decl* pDecl );
        void            ProcessCommands( StringRef, SourceLocation& loc );
        void            ProcessBookmark( StringRef name, SourceLocation& loc, unsigned int lineOffset );
        void            SetAttribute( StringRef );

        SourceLocation  GetEndOfLine( SourceLocation& refLoc );

        void            Warning( const char*, SourceLocation&, unsigned int lineOffset );
        void            PushState( State* pState = NULL );
        void            PopState();
        State&          CurrentState()          { return mStateStack.back(); }
        State*          ParentState()           { return HasParent() ? &mStateStack[ mStateStack.size() - 2 ] : NULL; }
        bool            HasParent()             { return mStateStack.size() > 1; }


    protected:

        friend class ReflectorASTConsumer;

        enum { kBufferSize = 128 };

        SourceManager*              mpSourceManager;
        std::queue<SourceRange>     mPIQueue;
        std::vector<State>          mStateStack;
        std::map<std::string,State> mClassesFound;
        char                        mBuffer[ kBufferSize ];
        std::vector<MetaProperty>   mMetaProperties;
        std::vector<Bookmark>       mBookmarks;
};

//////////////////////////////////////////////////////////////////////////
#endif // ReflectorContext_h
