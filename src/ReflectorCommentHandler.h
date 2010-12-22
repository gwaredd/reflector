#ifndef ReflectorCommentHandler_h
#define ReflectorCommentHandler_h
//////////////////////////////////////////////////////////////////////////
// ReflectorCommentHandler
//
//  Implements the clang::Preprocessor::CommentHandler interface
//
//  Extracts comments containing processing instructions (i.e. has a 
//  %reflect tag) and pushes them to the ReflectorContext for processing at 
//  the appropriate time.
//

#include "Reflector.h"
#include "ReflectorContext.h"

class ReflectorCommentHandler : public CommentHandler
{
    private:

        ReflectorContext&  mReflectorContext;

    public:

        ReflectorCommentHandler( ReflectorContext& context )
            : mReflectorContext( context )
        {
        }

        virtual bool HandleComment( Preprocessor& pp, SourceRange comment );
};

//////////////////////////////////////////////////////////////////////////
#endif // ReflectorCommentHandler_h

