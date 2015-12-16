////////////////////////////////////////////////////////////////////////////////

#include "gwStringRef.h"

#include <ctype.h>
#include <algorithm>

namespace gw
{
    StringRef StringRef::Split( const gwchar ch )
    {
        auto start = mData;

        while( !IsEmpty() && *mData != ch )
        {
            mData++;
            mLength--;
        }

        auto end = mData;

        if( mData && *mData == ch )
        {
            mData++;
            mLength--;
        }

        return StringRef( start, end );
    }

    ////////////////////////////////////////////////////////////////////////////////

    StringRef& StringRef::Trim()
    {
        while( !IsEmpty() && isspace( *mData ) )
        {
            mData++;
            mLength--;
        }

        while( !IsEmpty() && isspace( mData[ mLength - 1 ] ) )
        {
            mLength--;
        }

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    
    int StringRef::Compare( const StringRef& str ) const
    {
        auto len = std::min( mLength, str.mLength );

        if( len > 0 )
        {
            auto cmp = gw_strncmp( mData, str.mData, len );

            if( cmp != 0 )
            {
                return cmp;
            }
        }

        if( mLength == str.mLength )
        {
            return 0;
        }

        return mLength < str.mLength ? -1 : 1;
    }
}

    