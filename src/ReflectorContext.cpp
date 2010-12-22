//////////////////////////////////////////////////////////////////////////
//
// -- %reflect 
// -- %%reflect 
//
// options - and their "no" equivalents
//
// -- %reflect write FQN
// -- %reflect write namespace
// -- %reflect write access
// -- %reflect write location
//
// -- %reflect inherit
// -- %reflect explicit
// -- %reflect operators
//
// -- %reflect no
// -- %reflect ignore
// -- %reflect attribute [k=v]
// -- %reflect access [public|private|protected]
//
// -- %reflect bm
//
// -- %reflect debug
//
// -- %reflect fqn,nonamespace,inherit


#include "ReflectorContext.h"

//////////////////////////////////////////////////////////////////////////

static StringRef                        _strNull( 0, 0 );
static std::pair<StringRef,StringRef>   _nullPair( _strNull, _strNull );

const char* const ReflectorContext::PITagHint           = "%";
const char* const ReflectorContext::PITag               = "%reflect";
const char* const ReflectorContext::PITagParent         = "%%reflect";
const char* const ReflectorContext::PITagBookmarkStart  = "%<";
const char* const ReflectorContext::PITagBookmarkEnd    = "%>";

//////////////////////////////////////////////////////////////////////////
// pop_word - given a string, return the next word as a <word,tail> pair

static std::pair<StringRef, StringRef> pop_word( StringRef str )
{
    if( str.empty() )
    {
        return _nullPair;
    }

    // find start of word

    size_t pos = str.find_first_not_of( "*/ \t\n\0" );

    if( pos == StringRef::npos )
    {
        // no word found
        return _nullPair;
    }

    // find end of word

    size_t eow = str.find_first_of( "*/ \t\n\0", pos );

    if( eow == StringRef::npos )
    {
        return std::make_pair( str.substr( pos, str.size() - pos ), _strNull );
    }
    else
    {
        size_t son = str.find_first_not_of( "*/ \t\n\0", eow );

        return std::make_pair( str.substr( pos, eow - pos ), son == StringRef::npos ? _strNull : str.substr( son ) );
    }
}


//////////////////////////////////////////////////////////////////////////
// read the next line in the string, skipping comment tags and ws at the
// start

static StringRef _GetNextLine( StringRef& str, unsigned int& lineOffset )
{
    const char* pStartOfLine = NULL;

    StringRef::iterator pStringStart = str.begin();

    // iterate over chars
    StringRef::iterator p = pStringStart;

    while( p != str.end() )
    {
        if( pStartOfLine == NULL )
        {
            if( *p == '\n' )
            {
                lineOffset++;
            }

            // find start of line

            if( *p!='/' && *p!='*' && !isspace(*p) )
            {
                pStartOfLine = p;
            }
        }
        else
        {
            // find end of line

            if( *p == '\0' || *p == '\n' || *p == '\r' )
            {
                // tail current string
                str = str.substr( p - pStringStart );

                // return line
                return StringRef( pStartOfLine, p - pStartOfLine );
            }
        }

        p++;
    }

    // end of string
    str = _strNull;
    return pStartOfLine ? StringRef( pStartOfLine, p - pStartOfLine ) : _strNull;
}

//////////////////////////////////////////////////////////////////////////

ReflectorContext::ReflectorContext()
    : mpSourceManager( NULL )
{
    // add global state
    mStateStack.push_back( State() );
}


//////////////////////////////////////////////////////////////////////////

void ReflectorContext::Warning( const char* msg, SourceLocation& loc, unsigned int lineOffset )
{
    std::pair<FileID, unsigned> locInfo = mpSourceManager->getDecomposedLoc( loc );

    FileID          fileID      = locInfo.first;
    unsigned int    fileOffset  = locInfo.second;
    const FileEntry*pFileEntry  = mpSourceManager->getFileEntryForID( fileID );
    unsigned int    lineNumber  = mpSourceManager->getLineNumber( fileID, fileOffset ) + lineOffset;

    printf( "WARNING: %s in %s:%i\n", msg, pFileEntry->getName(), lineNumber );
}

//////////////////////////////////////////////////////////////////////////
// takes a comment string a processes all %reflect commands contained in it

void ReflectorContext::ProcessCommands( StringRef strRemaining, SourceLocation& loc )
{
    unsigned int lineOffset = 0;

    // for each line

    while( ! strRemaining.empty() )
    {
        // "pop" next line off of the string
        StringRef curLine = _GetNextLine( strRemaining, lineOffset );

        // does the current line start with a PI tag ([%]%reflect, %< or %>)?

        State* pState  = &CurrentState();
        State* pParent = NULL;

        if( curLine.startswith( PITagParent ) )
        {
            pParent = ParentState();
        }
        else if( curLine.startswith( PITag ) )
        {
            pParent = NULL;
        }
        else if( curLine.startswith( PITagBookmarkStart ) || curLine.startswith( PITagBookmarkEnd ) )
        {
            ProcessBookmark( curLine, loc, lineOffset );
            continue;
        }
        else
        {
            // %reflect not found at start of line so skip it
            continue;
        }

        // pop %reflect tag from current line

        std::pair<StringRef, StringRef> cmd = pop_word( curLine );

        if( CurrentState().debug )
        {
            std::pair<FileID, unsigned> locInfo = mpSourceManager->getDecomposedLoc( loc );

            FileID          fileID      = locInfo.first;
            unsigned int    fileOffset  = locInfo.second;
            const FileEntry*pFileEntry  = mpSourceManager->getFileEntryForID( fileID );
            unsigned int    lineNumber  = mpSourceManager->getLineNumber( fileID, fileOffset ) + lineOffset;

            printf( "DEBUG: %-16s %s, %s(%i)\n", 
                        cmd.first.str().c_str(),
                        cmd.second.empty() ? "''" : cmd.second.str().c_str(),
                        pFileEntry ? pFileEntry->getName() : "???",
                        lineNumber
                    );
        }


        // any %reflect tags automatically turn on exporting

        pState->reflect = true;

        if( pParent )
        {
            pParent->reflect = true;
        }



        // process commands

        while( !cmd.second.empty() )
        {
            // multiple commands can be separated by ,
            cmd = cmd.second.split(',');

            // pop first word of command
            std::pair<StringRef, StringRef> word = pop_word( cmd.first );

            if( word.first.empty() )
            {
                continue;
            }


            // %reflect no

            bool enable = true;

            if( word.first == "no" )
            {
                if( word.second.empty() )
                {
                    pState->reflect = false;
                    if( pParent ) pParent->reflect = false;
                    continue;
                }

                enable = false;
                word = pop_word( word.second );
            }

            // %reflect <cmds>

            if( word.first == "write" )
            {
                word = pop_word( word.second );

                if( word.first == "FQN" )
                {
                    pState->writeFQN = enable;
                    if( pParent ) pParent->writeFQN = enable;

                }
                else if( word.first == "namespace" )
                {
                    pState->writeNamespace = enable;
                    if( pParent ) pParent->writeNamespace = enable;
                }
                else if( word.first == "access" )
                {
                    pState->writeAccess = enable;
                    if( pParent ) pParent->writeAccess = enable;
                }
                else if( word.first == "location" )
                {
                    pState->writeLocations = enable;
                    if( pParent ) pParent->writeLocations = enable;
                }
            }
            else if( word.first == "inherit" )
            {
                pState->inherit = enable;
                if( pParent ) pParent->inherit = enable;
            }
            else if( word.first == "operators" )
            {
                pState->operators = enable;
                if( pParent ) pParent->operators = enable;
            }
            else if( word.first == "explicit" )
            {
                pState->reflectExplicit = enable;
                if( pParent ) pParent->reflectExplicit = enable;
            }
            else if( word.first == "ignore" )
            {
                pState->reflect = !enable;
                if( pParent ) pParent->reflect = !enable;
            }
            else if( word.first == "access" )
            {
                word = pop_word( word.second );

                if( word.first == "public" )
                {
                    pState->accessLevel = AS_public;
                    if( pParent ) pParent->accessLevel = AS_public;
                }
                else if( word.first == "protected" )
                {
                    pState->accessLevel = AS_protected;
                    if( pParent ) pParent->accessLevel = AS_protected;
                }
                else if( word.first == "private" )
                {
                    pState->accessLevel = AS_private;
                    if( pParent ) pParent->accessLevel = AS_private;
                }
            }
            else if( word.first == "attribute" )
            {
                SetAttribute( word.second );
            }
            else if( word.first == "debug" )
            {
                pState->debug = enable;
                if( pParent ) pParent->debug = enable;
            }
            else
            {
                sprintf_s( mBuffer, kBufferSize, "Unknown command '%s'", word.first.str().c_str() );
                Warning( mBuffer, loc, lineOffset );
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// %<[name][>] or %>

void ReflectorContext::ProcessBookmark( StringRef str, SourceLocation& loc, unsigned int lineOffset )
{
    // validate input

    if( str.size() < 2 )
    {
        return;
    }

    // parse bookmark
    Bookmark::eType type = Bookmark::kBookmark;
    StringRef       name;

    if( str[1] == '<' )
    {
        size_t pos = str.find( '>' );

        if( pos == StringRef::npos )
        {
            // %<...
            name = str.substr( 2 );
            type = Bookmark::kRegionStart;
        }
        else
        {
            // %<...>
            type = Bookmark::kBookmark;
            name = str.substr( 2, pos - 2 );
        }
    }
    else if( str[1] == '>' )
    {
        type = Bookmark::kRegionEnd;
    }
    else
    {
        // unknown type
        return;
    }


    // bookmark location
    std::pair<FileID, unsigned> locInfo = mpSourceManager->getDecomposedLoc( loc );

    FileID          fileID      = locInfo.first;
    unsigned int    fileOffset  = locInfo.second;
    unsigned int    lineNumber  = mpSourceManager->getLineNumber( fileID, fileOffset ) + lineOffset;
    const FileEntry*pFileEntry  = mpSourceManager->getFileEntryForID( fileID );

    // add bookmark
    mBookmarks.push_back( Bookmark( name, type, pFileEntry ? pFileEntry->getName() : NULL, lineNumber ) );
}

//////////////////////////////////////////////////////////////////////////
// %reflect attribute k=v;...

void ReflectorContext::SetAttribute( StringRef opt )
{
    if( opt.empty() )
    {
        return;
    }

    const char* p    = opt.data();
    const char* pEnd = &p[ opt.size() ];

    while( p < pEnd )
    {
        MetaProperty prop;

        // skip ws
        while( isspace(*p) && p < pEnd ) p++;

        // read word
        const char* pS = p;
        while( *p && ( isalnum(*p) || *p=='_' ) && p < pEnd ) p++;

        prop.k = StringRef( pS, p - pS );

        // skip =
        if( *p=='=' && p < pEnd ) p++;

        // read value
        pS = p;

        // skip ws
        while( isspace(*p) && p < pEnd ) p++;

        // read quoted "string"?

        const char* pE = p;

        if( *p == '"' )
        {
            // consume "
            if( p < pEnd )
            {
                p++;
                pS++;
            }

            while( *p && *p!='"' && p < pEnd )
            {
                //                 // escaped \" char?
                //                 if( *p=='\\' ) p++;
                // 
                if( p < pEnd ) p++;
            }

            pE = p;

            // consume "
            if( *p=='"' && p < pEnd ) p++;

            // read up to next ";" (or eol)
            while( *p && *p!=';' && p < pEnd ) p++;
        }
        else
        {
            // read up to next ";" (or eol)
            while( *p && *p!=';' && p < pEnd ) p++;
            pE = p;
        }


        // skip the ";"
        if( *p==';' && p < pEnd ) p++;

        //         // skip ws backwards
        //         while( isspace(*pE) && pE!=pS ) pE--;

        // set value
        prop.v = StringRef( pS, pE - pS );

        // add to list
        mMetaProperties.push_back( prop );
    }
}

//////////////////////////////////////////////////////////////////////////
// process all instructions before the given source location

void ReflectorContext::PopComments( SourceLocation& loc, bool process )
{
    while( ! mPIQueue.empty() )
    {
        // get first PI in queue

        SourceRange comment = mPIQueue.front();

        // if PI exists before the reference location

        if( comment.getBegin() < loc )
        {
            if( process )
            {
                const char* pStart  = mpSourceManager->getCharacterData( comment.getBegin() );
                const char* pEnd    = mpSourceManager->getCharacterData( comment.getEnd() );

                ProcessCommands( StringRef( pStart, pEnd - pStart ), comment.getBegin() );
            }

            mPIQueue.pop();
        }
        else
        {
            return;
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// 

void ReflectorContext::FileBegin()
{
    // copy default state for file context
    PushState();
}

void ReflectorContext::FileEnd()
{
    // ensure we've cleaned up

    while( mStateStack.size() > 1 )
    {
        PopState();
    }
}

//////////////////////////////////////////////////////////////////////////
// state management

void ReflectorContext::DeclBegin( Decl* pDecl )
{
    // if this is a class, should we inherit the state from a parent?

    State* pInheritedState = NULL;

    if( CXXRecordDecl* pRecordDecl = dyn_cast<CXXRecordDecl>( pDecl ) )
    {
        if( pRecordDecl->isDefinition() )
        {
            // for each parent class

            CXXRecordDecl::base_class_const_iterator itr;

            for( itr = pRecordDecl->bases_begin(); itr != pRecordDecl->bases_end(); itr++ )
            {
                // find record from CXXBaseSpecifier

                const CXXRecordDecl* pParent = cast<CXXRecordDecl>( itr->getType()->getAs<RecordType>()->getDecl() );

                if( pParent == NULL )
                {
                    continue;
                }

                // look up cached parent state

                std::map<std::string, State>::iterator parentState = mClassesFound.find( pParent->getQualifiedNameAsString() );

                if( parentState != mClassesFound.end() )
                {
                    if( parentState->second.inherit )
                    {
                        pInheritedState = &parentState->second;
                    }
                }
            }
        }
    }


    // push new state
    PushState( pInheritedState );

    // process reflection commands
    UpdateCommands( pDecl );
}

void ReflectorContext::DeclEnd( Decl* pDecl )
{
    // remove any stale comments - this can happen if a class is flagged for not exporting
    // but contains PI's inside it's declaration.

    PopComments( pDecl->getLocEnd(), false );


    // if this was a class, add to list for future reference

    if( CXXRecordDecl* pRecordDecl = dyn_cast<CXXRecordDecl>( pDecl ) )
    {
        if( pRecordDecl->isDefinition() )
        {
            // ... and we don't already have an entry for it

            std::map<std::string, State>::iterator classEntry = mClassesFound.find( pRecordDecl->getQualifiedNameAsString() );

            if( classEntry == mClassesFound.end() )
            {
                mClassesFound[ pRecordDecl->getQualifiedNameAsString() ] = CurrentState();
            }
        }
    }

    // pop state for this context

    PopState();
}

void ReflectorContext::PropertyBegin( Decl* pDecl )
{
    PushState();
    UpdateCommands( pDecl );
}

void ReflectorContext::PropertyEnd( Decl* pDecl )
{
    PopState();
}

void ReflectorContext::PushState( State* pState )
{
    if( pState )
    {
        mStateStack.push_back( *pState );
    }
    else
    {
        // copy top most state
        mStateStack.push_back( mStateStack.back() );
    }

    // if explicit flag is set, reflect must be explicitly set

    if( CurrentState().reflectExplicit )
    {
        CurrentState().reflect = false;
    }
}

void ReflectorContext::PopState()
{
    // always leave the global state on the stack

    if( mStateStack.size() > 1 )
    {
        mStateStack.pop_back();
    }

    mMetaProperties.clear();
    mBookmarks.clear();
}


//////////////////////////////////////////////////////////////////////////
// GetEndOfLine - get the source location for the end of the line referenced
// by the given location

SourceLocation ReflectorContext::GetEndOfLine( SourceLocation& refLoc )
{
    std::pair<FileID, unsigned> locInfo = mpSourceManager->getDecomposedLoc( refLoc );

    FileID          fileID      = locInfo.first;
    unsigned int    fileOffset  = locInfo.second;

    if( ! mpSourceManager->getSLocEntry( fileID ).isFile() )
    {
        return refLoc;
    }

    const FileEntry*pFileEntry = mpSourceManager->getFileEntryForID( fileID );

    if( pFileEntry == NULL )
    {
        return refLoc;
    }

    // NB: this relies on SourceManager::getLocation correctly handling the fact 
    // we are referencing a column beyond the end of the line (and no lines longer
    // that 1024 ;)

    const unsigned int kColumnNum = 1024;
    unsigned int lineNumber = mpSourceManager->getLineNumber( fileID, fileOffset );

    return mpSourceManager->getLocation( pFileEntry, lineNumber, kColumnNum );
}

//////////////////////////////////////////////////////////////////////////

void ReflectorContext::UpdateCommands( Decl* pDecl )
{
    // process pending instructions in the queue up to this decl

    PopComments( GetEndOfLine( pDecl->getLocStart() ) );

    // process annotate attributes for this decl

    for( Decl::attr_iterator at = pDecl->attr_begin(); at != pDecl->attr_end(); at++ )
    {
        if( AnnotateAttr* pAnnotatin = dyn_cast<AnnotateAttr>(*at) )
        {
            ProcessCommands( pAnnotatin->getAnnotation(), pDecl->getLocStart() );
        }
    }
}


//////////////////////////////////////////////////////////////////////////

bool ReflectorContext::CanReflect( NamedDecl* pDecl )
{
    if( pDecl == NULL )
    {
        return false;
    }

    // does the current state allow us to reflect?

    State& s = CurrentState();

    if( ! s.reflect )
    {
        return false;
    }


    // is the access level allowed? (public/protected/private)

    AccessSpecifier as = pDecl->getAccess();

    if( as != AS_none && as > s.accessLevel )
    {
        return false;
    }


    // have we already reflected this class? (relevant for multiple files)

    if( CXXRecordDecl* pRecord = dyn_cast<CXXRecordDecl>(pDecl) )
    {
        std::map<std::string, State>::iterator itr = mClassesFound.find( pRecord->getQualifiedNameAsString() );

        if( itr != mClassesFound.end() )
        {
            return false;
        }
    }

    // ok reflect it then
    return true;
}

