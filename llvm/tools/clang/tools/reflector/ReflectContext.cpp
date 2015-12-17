////////////////////////////////////////////////////////////////////////////////

#include "Reflector.h"


////////////////////////////////////////////////////////////////////////////////
// write out last set of attrs from comment to json

void ReflectContext::OutputAttrs( JSON& json )
{
    if( mAttrs.size() == 0 )
    {
        return;
    }

    json.BeginObject( "attrs" );

    for( auto& attr : mAttrs )
    {
        json.Field( attr.first.c_str(), attr.second.c_str() );
    }

    json.EndObject();
}


////////////////////////////////////////////////////////////////////////////////
// parsing functions

void ReflectContext::SkipWhitespace()
{
    while( *mP && isspace( *mP ) )
    {
        ++mP;
    }
}

void ReflectContext::SkipPunct()
{
    while( *mP && ( *mP != '\"' && *mP != '\'' && *mP != '-' ) && ( ispunct( *mP ) || isspace(*mP) ) )
    {
        ++mP;
    }
}

void ReflectContext::SkipIf( char ch )
{
    if( *mP && *mP == ch )
    {
        ++mP;
    }
}

////////////////////////////////////////////////////////////////////////////////

StringRef ReflectContext::NextWord()
{
    SkipWhitespace();

    if( *mP == '"' || *mP == '\'' )
    {
        char stop  = *mP++;
        auto start = mP;

        while( *mP && *mP != stop )
        {
            if( *mP == '\\' )
            {
                ++mP;
                if( *mP ) ++mP;
            }
            else
            {
                ++mP;
            }
        }

        StringRef word( start, mP - start );

        if( *mP ) ++mP;
        SkipWhitespace();

        return word;
    }
    else
    {
        auto start = mP;

        while( *mP && ( isalnum( *mP ) || *mP == '_' ) )
        {
            mP++;
        }

        StringRef word( start, mP - start );

        SkipWhitespace();

        return word;
    }
}



////////////////////////////////////////////////////////////////////////////////
// parse a %%reflect comment

// %reflect all|explicit
// %key=value, ...
// %!
// % -- comment

bool ReflectContext::ParseComment( comments::FullComment* comment )
{
    mAttrs.clear();

    if( comment == nullptr )
    {
        return mReflect;
    }

    // amalgamate comments

    std::string str;

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

                // reflect comments start with % - otherwise ignore them

                if( str[0] != '%' )
                {
                    return mReflect;
                }
            }
        }
    }


    // %%

    mP = str.c_str();

    SkipIf( '%' );
    SkipIf( '%' );


    // %! - don't reflect

    if( *mP == '!' )
    {
        mReflect = mP[1] != '!'; // %!! turn rest of fields off (unless we reflect again)
        return false;
    }

    // toggle back on

    mReflect = true;


    // %% key=value, a="hello world", ... -- comment

    SkipWhitespace();

    while( *mP )
    {
        // comment

        if( mP[0] == '-' && mP[1] == '-' )
        {
            break;
        }

        // key=value

        auto key = NextWord();

        if( *mP != '=' || key.empty() )
        {
            break;
        }

        ++mP;
        auto value = NextWord();

        SkipPunct();

        // add to list

        mAttrs.push_back( std::make_pair( key.str(), value.str() ) );
    }

    return mReflect;
}

