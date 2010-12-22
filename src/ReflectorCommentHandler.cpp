//////////////////////////////////////////////////////////////////////////

#include "ReflectorCommentHandler.h"

//////////////////////////////////////////////////////////////////////////

bool ReflectorCommentHandler::HandleComment( Preprocessor& pp, SourceRange range )
{
    // convert comment to StringRef

    SourceManager&  sm      = pp.getSourceManager();
    const char*     pStart  = sm.getCharacterData( range.getBegin() );
    const char*     pEnd    = sm.getCharacterData( range.getEnd() );

    // push comment if it potentially contains a PI tag

    StringRef str = StringRef( pStart, pEnd - pStart );

    if( str.find( ReflectorContext::PITagHint ) != StringRef::npos )
    {
        mReflectorContext.PushComment( range );
    }

    return false; // no tokens inserted
}

